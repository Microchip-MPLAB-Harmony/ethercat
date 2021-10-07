/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9255_ecat_util.c

  Summary
    ETherCAT source file which interface between EtherCAT driver and the 
    Micorchip H3 Peripheral libraries.

  Description
    This file defines the interface between EtherCAT driver and Microchip H3 
    Peripheral libraries.

  Remarks:
    None.
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#include "drv_lan9255.h"
#include "drv_lan9255_ecat_util.h"
#include "drv_lan9255_definitions.h"
#include "definitions.h"
#include "ecatslv.h"

static void ECAT_PDI_TimerInterruptInitialization(void);
/* This is the driver instance object array. */
static DRV_LAN9255_UTIL_OBJ gDrvLan9255UtilObj;
     
char    gEtherCATQSPITransmission = false;

/*Global interrupt enable and disable  */
static void ECAT_QSPI_TransmissionFlagClear(void);
static void ECAT_QSPI_EventHandlerSet(void);
static bool ECAT_QSPI_IsTransmissionBusy(void);
static volatile uint32_t processorStatus;
static void _ECAT_QSPI_SyncWait(void);

uint8_t gau8DummyCntArr[SETCFG_MAX_DATA_BYTES] = {0,0,0,1,0,0,1,0,0,2,0,0,1,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1,0,0};

static uint16_t RoundUpMultipleOf (uint16_t u16X, uint16_t u16Type);
/*
 * Make the u16X as multiple of 2 or 4 or 8, based on u16Type
 */
static uint16_t RoundUpMultipleOf (uint16_t u16X, uint16_t u16Type)
{
    uint16_t u16Val = 0;
    
    switch (u16Type)
    {
        case SINGLE_SPI:
            /* Single Bit SPI, Round up multiple of 8 */
            u16Val = (u16X % 8) ? (u16X + (8 - (u16X % 8))) : u16X; 
            break;
        case DUAL_SPI:
            /* Dual SPI, Round up multiple of 4 */
            u16Val = (u16X % 4) ? (u16X + (4 - (u16X % 4))) : u16X;
            break;
        case QUAD_SPI:
            /* QUAD SPI, Round up multiple of 2 */
            u16Val = (u16X % 2) ? (u16X + (2 - (u16X % 2))) : u16X;
            break;
        default:
            /* Invalid SPI type */
            u16Val = 1;
            break;
    }
	return u16Val;
}

// This function returns the number of Dummy Bytes required based on the 
// internal access time, no. of SPI clocks involved in processing the instruction and width of SPI clock period

uint8_t GetDummyBytesRequired (uint8_t u8SPIType, uint16_t u16IAT, uint16_t u16SPIClkCount, uint16_t u16SPIClkPeriodns)
{
	uint16_t u16DummyClkCount = 0;
    uint8_t u8DummyByte = 0;
	uint16_t u16SPIClkTime = 0, u16DummyTime = 0;
    
    /* SPI Clock time */
    u16SPIClkTime = u16SPIClkCount * u16SPIClkPeriodns;
	if (u16SPIClkTime >= u16IAT)
	{
		return 0;
	}
	/* Dummy Time */
	u16DummyTime = (u16IAT - u16SPIClkTime);

	// Express the Dummy time in terms of number of SPI clocks
    if (u16DummyTime <= 0) {
        u16DummyClkCount = 0;
    } else {
        u16DummyClkCount = u16DummyTime / u16SPIClkPeriodns;
        /* Getting the fractional part */
        if (u16DummyTime % u16SPIClkPeriodns) {
            /* Adding one clock count
             * So that fractional part will be considered
             * round of multiple value gets proper value
             */
            u16DummyClkCount++;
        }
    }

    /*
     * Make dummy clock count as Byte/WORD aligned
     * if (SPIType == QUAD_SPI), then (round to next multiple of 2)
     * if (SPIType == DUAL_SPI), then (round to next multiple of 4)
     * if (SPIType == SINGLE_SPI), then (round to next multiple of 8) 
     */
	u16DummyClkCount = RoundUpMultipleOf (u16DummyClkCount, u8SPIType);
    
    /* Convert the clock count to dummy byte */
    if (u8SPIType == QUAD_SPI) {
        u8DummyByte = u16DummyClkCount >> 1;
    } else if (u8SPIType == DUAL_SPI) {
        u8DummyByte = u16DummyClkCount >> 2;
    } else if (u8SPIType == SINGLE_SPI) {
        u8DummyByte = u16DummyClkCount >> 3;
    }
    return u8DummyByte;
}




void CRITICAL_SECTION_ENTER(void)
{
   /* block interrupts with priority number higher or equal to ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY */
   __set_BASEPRI(ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY << (8 - ETHERCAT_INT_PRIO_BITS));
}

void CRITICAL_SECTION_LEAVE(void)
{
    /*remove the BASEPRI masking */
    __set_BASEPRI( 0U );
}

#ifdef DC_SUPPORTED
/*******************************************************************************
Function:
    void _ECAT_Sync0Callback(PIO_PIN pin, uintptr_t context)

Summary:
    Interrupt service routine for the interrupt from SYNC0
*******************************************************************************/
void _ECAT_Sync0Callback(uintptr_t context)
{
	CRITICAL_SECTION_ENTER();
	Sync0_Isr();
	CRITICAL_SECTION_LEAVE();
}

/*******************************************************************************
Function:
    void _ECAT_Sync1Callback(PIO_PIN pin, uintptr_t context)

Summary:
    Interrupt service routine for the interrupt from SYNC1
*******************************************************************************/
void _ECAT_Sync1Callback(uintptr_t context)
{
	CRITICAL_SECTION_ENTER();
	Sync1_Isr();
  	CRITICAL_SECTION_LEAVE();   
}

/*******************************************************************************
Function:
    void ECAT_SyncInterruptsInitialization(void)

Summary:
    Register Callback function for PDI SYNC0 and SYNC1 interrupts
*******************************************************************************/
void ECAT_SyncInterruptsInitialization(void)
{
// SYNC0 and SYNC1 interrupt callback 
    EIC_CallbackRegister(EIC_PIN_0,_ECAT_Sync0Callback, 0);
    EIC_CallbackRegister(EIC_PIN_1,_ECAT_Sync1Callback, 0);
	
}
#endif // DC_SUPPORTED

/*******************************************************************************
Function:
    void _ECAT_EscInterruptRequestCallback()

Summary:
    Interrupt service routine for the interrupt from ESC
*******************************************************************************/
void _ECAT_EscInterruptRequestCallback(uintptr_t context)
{
	CRITICAL_SECTION_ENTER();
	PDI_Isr();    
	CRITICAL_SECTION_LEAVE();
}

/*******************************************************************************
Function:
    void ECAT_ESCIRQInitialization(void)

Summary:
    Register Callback function for PDI ESC(EtherCAT Slave Controller) interrupts
*******************************************************************************/
void ECAT_ESCIRQInitialization(void)
{
	EIC_CallbackRegister(EIC_PIN_7,_ECAT_EscInterruptRequestCallback, 0);
}
/*******************************************************************************
Function:
    void _ECAT_ChipSelectDisable(void)

Summary:
    Disable EtherCAT slave
*******************************************************************************/
void _ECAT_ChipSelectDisable(void)
{
/* SPI Chip Select PIN set */
    PORT_PinSet((PORT_PIN)PORT_PIN_PB11);

}

/*******************************************************************************
Function:
    void _ECAT_ChipSelectEnable(void)

Summary:
    Enable EtherCAT slave
*******************************************************************************/
void _ECAT_ChipSelectEnable(void)
{
    /* SPI Chip Select PIN Clear */
    PORT_PinClear((PORT_PIN)PORT_PIN_PB11);
}

#ifdef ETHERCAT_DEBUG
void SPIreadWriteTest(void)
{
    uint16_t adr=0x1500;
    uint32_t count =0;
    uint8_t countArr[16]= {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
    uint8_t countArr1[16];
    uint8_t addrInc=0;
    
    for(addrInc=0;addrInc<8;addrInc++)
    {
        for(count =1;count<16;)
        {
            //HW_EscWrite((MEM_ADDR*)countArr,adr+addrInc,count);
            //HW_EscRead((MEM_ADDR*)countArr1,adr+addrInc,count);
            if(memcmp(countArr,countArr1,count) != 0)
            {
                break;
            }
            count= count+1;
        }
    }
}
#endif

/*******************************************************************************
    Function:
        void ECAT_SPI_SetCfg_dataInit(void)

    Summary:
        This function configures the number of initial, per byte and per DWORD
        dummy bytes needed as per the current PDI frequency. 
*******************************************************************************/

void ECAT_SPI_SetCfg_dataInit(void)
{
    uint16_t u16SPIClkPeriod = 0, u16SPIClkfreq = 0;
    
    u16SPIClkfreq = DRV_LAN9255_BAUDRATE_PDI_FREQ;
    /* Get clock period in Nano seconds */
    u16SPIClkPeriod = CLK_PERIOD_1MHZ/u16SPIClkfreq;
    
    /* Fill the SPI related part in gau8DummyCntArr */
    /* SPI read, index from 0, 1, 2 
     * as initial, intra DWORD  inter DWORD
     */
    gau8DummyCntArr[SPI_READ_INITIAL_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_BYRD, 0, u16SPIClkPeriod);
    gau8DummyCntArr[SPI_READ_INTRA_DWORD_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_BYRD, 6, u16SPIClkPeriod);
    gau8DummyCntArr[SPI_READ_INTER_DWORD_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_BYRD, 6, u16SPIClkPeriod);
    
    /* SPI Fast read, index from 3, 4, 5 
     * as initial, intra DWORD  inter DWORD
     */
    gau8DummyCntArr[SPI_FASTREAD_INITIAL_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_DWRD, 0, u16SPIClkPeriod);
    gau8DummyCntArr[SPI_FASTREAD_INTRA_DWORD_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_DWRD, 30, u16SPIClkPeriod);
    gau8DummyCntArr[SPI_FASTREAD_INTER_DWORD_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_DWRD, 30, u16SPIClkPeriod);
    /* SPI Write, index starts from 18, 19, 20  
     * as Initial, Intra DWORD  Inter DWORD
     */
    gau8DummyCntArr[SPI_WRITE_INITIAL_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_NULL, 0, u16SPIClkPeriod);
    gau8DummyCntArr[SPI_WRITE_INTRA_DWORD_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_BYWR, 8, u16SPIClkPeriod);
    gau8DummyCntArr[SPI_WRITE_INTER_DWORD_OFFSET] = GetDummyBytesRequired (SINGLE_SPI, IAT_DWDWR, 32, u16SPIClkPeriod);
    
}

/*******************************************************************************
    Function:
        void ECAT_SPI_SetConfiguration(uint8_t *dummyArr)

    Summary:
        This function configures the number of initial, per byte and per DWORD dummy
		bytes needed. 
*******************************************************************************/

void ECAT_SPI_SetConfiguration(uint8_t *pu8DummyByteCnt)
{
    uint8_t u8Len = 0, u8txData = 0, u8txLen = 1;
    
    u8Len = SETCFG_MAX_DATA_BYTES;
    u8txData = CMD_SPI_SETCFG;

    _ECAT_ChipSelectEnable();
    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    { 
    }
	
    /* Send command */
    gDrvLan9255UtilObj.spiPlib->spiWrite(&u8txData, u8txLen);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    { 
    }
    
    /* Now write set cfg data */
	do
	{
        gDrvLan9255UtilObj.spiPlib->spiWrite(pu8DummyByteCnt, u8txLen);
        _ECAT_QSPI_SyncWait();
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{ 
		}
        pu8DummyByteCnt++;
	} while (--u8Len);
    
	_ECAT_ChipSelectDisable();
}

/*******************************************************************************
    Function:
        void ECAT_SPI_DisableQuadMode(void)

    Summary:
        This function Disable the SQI mode in slave, send RST_QIO
*******************************************************************************/

void ECAT_SPI_DisableQuadMode(void)
{
    uint8_t u8txData = 0, u8txLen = 1;
    
    u8txData = CMD_RESET_SQI;
    
    _ECAT_ChipSelectEnable();
    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    { 
    }
	
    /* Send command */
    gDrvLan9255UtilObj.spiPlib->spiWrite(&u8txData, u8txLen);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    { 
    }
    _ECAT_ChipSelectDisable();
}

/*******************************************************************************
Function:
    void ECAT_Lan9255_IsPDIFunctional(uint8_t *data)

Summary:
    Number of dummy bytes that the S/W uses is set dynamically using SETCFG. 
    Design will decode the SETCFG instruction and respond to the SPI Read/Write 
    commands from the master accordingly. If we use SPIRead() API which uses 
    dummy bytes (initial/Per Byte/Per Dword) to poll for Byte Order register, 
    the design would not be aware of dummy byte count at this point and reading 
    byte order register will fail(Since, issuing of SETCFG 
    from the S/W itself is possible only after polling for byte order register) 
    Hence, a separate API is maintained.
*******************************************************************************/
void ECAT_Lan9255_IsPDIFunctional(uint8_t *pu8Data)
{
	/* Before device initialization, the SPI/SQI interface will not return valid data.
    To determine when the SPI/SQI interface is functional, the Byte Order Test 
    Register (BYTE_TEST) should be polled. Once the correct pattern is read, the interface
	can be considered functional */
    uint8_t  u8Len = 4;
    uint8_t  u8TxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  u8RxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  u8TxLen=0,u8RxLen=0;
    
    u8TxLen=u8RxLen=3;
    
    _ECAT_ChipSelectEnable();

	u8TxData[0] = CMD_SERIAL_READ;
    u8TxData[1] = (uint8_t)HIBYTE(LAN925x_BYTE_ORDER_REG);
    u8TxData[2] = (uint8_t)LOBYTE(LAN925x_BYTE_ORDER_REG);
    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    { 
    }
    gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData,u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
      // Update the EscALEvent value which is required for the state change
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
    
    gDrvLan9255UtilObj.spiPlib->spiRead(u8RxData, u8Len);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {
    }
	memcpy(pu8Data,u8RxData,u8Len);	
	_ECAT_ChipSelectDisable();


}


/* 
    Function: ECAT_Lan925x_SPIWrite

    This function does Write Access to Non-Ether CAT and Ether CAT Core CSR 
	using 'Serial Write(0x02)' command supported by LAN9252/LAN9255 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9252/LAN9255 Compatible SPI (0x80)
     
    Input : u16Adddr    -> Address of the register to be written
            *pu8Data  -> Pointer to the data that is to be written

    Output : None
	
	Note   : In LAN9252 Compatible SPI, all registers are DWORD aligned. Length will be fixed to 4. Hence,
			 there is no separate length argument.
*/

void ECAT_Lan925x_SPIWrite(uint16_t u16Adddr, uint8_t *pu8Data, uint8_t u8Len)
{
	uint8_t u8dummy_clk_cnt = 0;
    uint8_t u8TxData[DWORD_LENGTH] = {0,0,0,0};
    uint8_t	u8RxData[DWORD_LENGTH] = {0,0,0,0};
    uint8_t u8TxLen = 1, u8RxLen = 1;
	uint8_t u8RxOneByteData=0;
	
    u8TxData[0] = CMD_SERIAL_WRITE;
    u8TxData[1] = (uint8_t)(u16Adddr >> 8);
    u8TxData[2] = (uint8_t)u16Adddr;
    
    _ECAT_ChipSelectEnable();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
	u8TxLen = u8RxLen = 3;
	gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData,u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
  
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
    
	u8TxLen = u8RxLen = 1;
	
	 /* Initial Dummy cycle added by dummy write */
    u8dummy_clk_cnt = gau8DummyCntArr[SPI_WRITE_INITIAL_OFFSET];
	while (u8dummy_clk_cnt--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{        
		}
	}
    gDrvLan9255UtilObj.spiPlib->spiWrite(pu8Data, u8Len);
    _ECAT_QSPI_SyncWait();
    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
	
	/* Get the Intra DWORD dummy clock count */
	u8dummy_clk_cnt = gau8DummyCntArr[SPI_WRITE_INTRA_DWORD_OFFSET];
	/* Add Intra DWORD dummy clocks, avoid for last byte */
	if (1 != u8Len) 
	{
		while (u8dummy_clk_cnt--)
		{
			gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
			_ECAT_QSPI_SyncWait();
			while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
			{        
			}
		}
	}
    _ECAT_ChipSelectDisable();

}

/* 
    Function: ECAT_Lan925x_SPIRead

    This function does Read Access to Non-EtherCAT and EtherCAT Core CSR 
	using 'Serial Read(0x03)' command supported by LAN9252/LAN9255 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9252/LAN9255 Compatible SPI (0x80)
     
    Input : u16Addr    -> Address of the register to be read
            *pu8Data -> Pointer to the data that is to be read

    Output : None
	
	Note   : In LAN9252 Compatible SPI, all registers are DWORD aligned. Length will be fixed to 4. Hence,
			 there is no separate length argument.
*/

void ECAT_Lan925x_SPIRead(uint16_t u16Addr, uint8_t *pu8Data, uint8_t u8Len)
{
	uint8_t  u8dummy_clk_cnt = 0;
    uint8_t  u8TxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  u8RxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  u8TxLen = 1, u8RxLen = 1;
	uint8_t u8TxOneByteData=0, u8RxOneByteData=0;
	
	u8TxData[0] = CMD_SERIAL_READ;
    u8TxData[1] = (uint8_t)(u16Addr >> 8);
    u8TxData[2] = (uint8_t)u16Addr;
    
    _ECAT_ChipSelectEnable();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
	/* SPI read and write with 3 bytes of TX and RX length */
	u8TxLen = u8RxLen = 3;
	
    gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData, u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
	
	/* SPI read and write with 1 bytes of TX and RX length */
	u8TxLen = u8RxLen = 1;
	
    /* Initial Dummy cycle added by dummy read */
    u8dummy_clk_cnt = gau8DummyCntArr[SPI_READ_INITIAL_OFFSET];
	while(u8dummy_clk_cnt--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{
		}
	}
	
    do
	{
		if(1 == u8Len)
		{
			u8TxOneByteData = READ_TERMINATION_BYTE;
		}
		else
		{
			u8TxOneByteData = 0;
		}
		
		gDrvLan9255UtilObj.spiPlib->spiWriteRead(&u8TxOneByteData, u8TxLen, &u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{
		}
		*pu8Data++ = u8RxOneByteData;
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_READ_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (1 != u8Len) 
		{
            while(u8dummy_clk_cnt--)
            {
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{
				}
            }
        }
		
	} while (--u8Len);
    
    _ECAT_ChipSelectDisable();
}

/* 
    Function: ECAT_Lan925x_SPIFastRead

    This function does Read Access to Non-Ether CAT and Ether CAT Core CSR 
	using 'Fast Read(0x0B)' command supported by LAN9252/LAN9255 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9252/LAN9255 Compatible SPI (0x80)
     
    Input : u16Addr    -> Address of the register to be read
            *pu8Data -> Pointer to the data that is to be read

    Output : None

*/

void ECAT_Lan925x_SPIFastRead(uint16_t u16Addr, uint8_t *pu8Data, uint8_t u8Len)
{
    uint8_t  u8TxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  u8RxData[DWORD_LENGTH]={0,0,0,0};
	uint8_t u8TxLen = 1, u8RxLen = 1,u8dummy_clk_cnt = 0;
	uint8_t u8TxOneByteData=0, u8RxOneByteData=0;
	
	u8TxData[0] = CMD_FAST_READ;
    u8TxData[1] = (uint8_t)(u16Addr >> 8);
    u8TxData[2] = (uint8_t)u16Addr;	
    
	/* SPI read and write with 3 bytes of TX and RX length */
	u8TxLen = u8RxLen = 3;
	
	_ECAT_ChipSelectEnable();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
    gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData, u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
	
	/* SPI read and write with 1 bytes of TX and RX length */
	u8TxLen = u8RxLen = 1;
    
	/* Send Transfer length */
    gDrvLan9255UtilObj.spiPlib->spiWrite(&u8Len, u8TxLen);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {
    }
    
	/* Initial Dummy cycle added by dummy read */
	u8dummy_clk_cnt = gau8DummyCntArr[SPI_FASTREAD_INITIAL_OFFSET];
	while (u8dummy_clk_cnt--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{        
		}
	}
	    
    do
	{
		if (1 == u8Len)
		{
			u8TxOneByteData = READ_TERMINATION_BYTE;
		}
		else
		{
			u8TxOneByteData = 0;
		}
        
		gDrvLan9255UtilObj.spiPlib->spiWriteRead(&u8TxOneByteData, u8TxLen, &u8RxOneByteData, u8RxLen);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		*pu8Data++ = u8RxOneByteData;
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_FASTREAD_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (1 != u8Len) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	} while(--u8Len);

	_ECAT_ChipSelectDisable();
}

/* 
    Function: ECAT_Lan925x_SPIReadPDRAM

    This function does Read Access to Ether CAT Core Process RAM  using 'Serial Read(0x03)' command 
	supported by LAN9252/LAN9255 Compatible SPI. This function shall be used only when PDI is selected as
	LAN9252 Compatible SPI (0x80)
     
    Input : u16Addr    -> Address of the RAM location to be read
            *pu8Data -> Pointer to the data that is to be read
			u8Len	 -> Number of bytes to be read from PDRAM location

    Output : None

*/

void ECAT_Lan925x_SPIReadPDRAM(uint8_t *pu8Data, uint16_t u16Addr, uint16_t u16Len)
{
	UINT32_VAL 	u32Val;
	uint8_t		u8StartAlignSize, u8EndAlignSize;
    uint8_t  	u8TxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  	u8RxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t 	u8TxLen = 1, u8RxLen = 1,u8dummy_clk_cnt = 0;
	uint8_t 	u8TxOneByteData=0, u8RxOneByteData=0;
	
	/* Address and length */
	u32Val.w[0] = u16Addr;
	u32Val.w[1] = u16Len;
	MCHP_ESF_PDI_WRITE(ECAT_PRAM_RD_ADDR_LENGTH_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

	/* Read command */
	u32Val.Val = 0x80000000;
	MCHP_ESF_PDI_WRITE(ECAT_PRAM_RD_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

	u8StartAlignSize = (u16Addr & 3);
	u8EndAlignSize = (u16Len + u8StartAlignSize) & 3;
	if (u8EndAlignSize & 3)
	{
		u8EndAlignSize = (((u8EndAlignSize + 4) & 0xC) - u8EndAlignSize);
	}

	/* SPI read and write with 3 bytes of TX and RX length */
	u8TxLen = u8RxLen = 3;
	
	/* Read SPI FIFO */
	_ECAT_ChipSelectEnable();
    
    u8TxData[0] = CMD_SERIAL_READ;
    u8TxData[1] = (uint8_t)0;
    u8TxData[2] = (uint8_t)0x04;
	
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData, u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
    
	/* SPI read and write with 1 bytes of TX and RX length */
	u8TxLen = u8RxLen = 1;
	
	/* Initial Dummy cycle added by dummy read */
	u8dummy_clk_cnt = gau8DummyCntArr[SPI_READ_INITIAL_OFFSET];
	while (u8dummy_clk_cnt--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{        
		}		
	}
	
    while(u8StartAlignSize--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_READ_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (1 != u8StartAlignSize) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}
    
    while(u16Len--)
	{
		if (0 == u16Len)
		{
			u8TxOneByteData = READ_TERMINATION_BYTE;
		}
		else
		{
			u8TxOneByteData = 0;
		}
        
		gDrvLan9255UtilObj.spiPlib->spiWriteRead(&u8TxOneByteData, u8TxLen,&u8RxOneByteData,u8RxLen);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
        *pu8Data++ = u8RxOneByteData;
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_READ_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u16Len) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}

    while(u8EndAlignSize--)
	{
    	gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData,1);  
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_READ_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u8EndAlignSize) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}
    
	_ECAT_ChipSelectDisable();
        
}

/* 
    Function: ECAT_Lan925x_SPIFastReadPDRAM

    This function does Read Access to Ether CAT Core Process RAM  using 'Fast Read(0x0B)' command 
	supported by LAN9252/LAN9255 Compatible SPI. This function shall be used only when PDI is selected as
	LAN9252/LAN9255 Compatible SPI (0x80)
     
    Input : u16Addr    -> Address of the RAM location to be read
            *pu8Data -> Pointer to the data that is to be read
			u16Len	 -> Number of bytes to be read from PDRAM location

    Output : None
*/

void ECAT_Lan925x_SPIFastReadPDRAM(uint8_t *pu8Data, uint16_t u16Addr, uint16_t u16Len)
{
	UINT32_VAL 	u32Val;
	uint8_t 	u8StartAlignSize, u8EndAlignSize;
	uint16_t 	wXfrLen = 0, wTotalLen = 0;
    uint8_t  	u8TxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  	u8RxData[DWORD_LENGTH]={0,0,0,0};
	uint8_t 	u8TxLen = 1, u8RxLen = 1,u8dummy_clk_cnt = 0;
	uint8_t 	u8TxOneByteData=0, u8RxOneByteData=0;
	
	u8StartAlignSize = (u16Addr & 3);
	u8EndAlignSize = (u16Len & 3) + u8StartAlignSize;
	if (u8EndAlignSize & 3)
    {
		u8EndAlignSize = (((u8EndAlignSize + 4) & 0xC) - u8EndAlignSize);
	}
	
	wTotalLen = u16Len + u8StartAlignSize + u8EndAlignSize; 
	
	/* From DOS, "For the one byte transfer length format,	bit 7 is low and bits 6-0 specify the 
	length up to 127 bytes. For the two byte transfer length format, bit 7 of the first byte
	is high and bits 6-0 specify the lower 7 bits of the length. Bits 6-0 of the of the second byte 
	field specify the upper 7 bits of the length with a maximum transfer length of 16,383 bytes (16K-1)" */ 
	if (wTotalLen <= ONE_BYTE_MAX_XFR_LEN)
	{
		wXfrLen = wTotalLen; 
	}
	else  
	{
		wXfrLen = (wTotalLen & 0x7F) | 0x80;
		wXfrLen |= ((wTotalLen & 0x3F80) << 1);
	}
	
	/* Address and length */
	u32Val.w[0] = u16Addr;
	u32Val.w[1] = u16Len;
	MCHP_ESF_PDI_WRITE(ECAT_PRAM_RD_ADDR_LENGTH_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

	/* Read command */
	u32Val.Val = 0x80000000;
	MCHP_ESF_PDI_WRITE(ECAT_PRAM_RD_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

	/* SPI read and write with 3 bytes of TX and RX length */
	u8TxLen = u8RxLen = 3;
	
	/* Read SPI FIFO */
	_ECAT_ChipSelectEnable();
	
    u8TxData[0] = CMD_FAST_READ;
    u8TxData[1] = (uint8_t)0;
    u8TxData[2] = (uint8_t)0x04;
	
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData, u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
    
	/* SPI read and write with 1 bytes of TX and RX length */
	u8TxLen = u8RxLen = 1;
	
	/* Send Transfer length */
    u8TxOneByteData = LOBYTE(wXfrLen);
    gDrvLan9255UtilObj.spiPlib->spiWrite(&u8TxOneByteData, 1);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }
    
	if (u16Len > ONE_BYTE_MAX_XFR_LEN)    /* Two byte Xfr length */
	{
        u8TxOneByteData = HIBYTE(wXfrLen);
        gDrvLan9255UtilObj.spiPlib->spiWrite(&u8TxOneByteData, 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}

    /* Initial Dummy cycle added by dummy read */
	u8dummy_clk_cnt = gau8DummyCntArr[SPI_FASTREAD_INITIAL_OFFSET];
	while (u8dummy_clk_cnt--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{        
		}		
	}
  	
	while(u8StartAlignSize--)
	{
        gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_FASTREAD_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u8StartAlignSize) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}

	while(u16Len--)
	{
		if (0 == u16Len)
		{
			u8TxOneByteData = READ_TERMINATION_BYTE;
		}
		else
		{
			u8TxOneByteData = 0;
		}
        
        gDrvLan9255UtilObj.spiPlib->spiWriteRead(&u8TxOneByteData,1,&u8RxOneByteData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
        *pu8Data++ = u8RxOneByteData;
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_FASTREAD_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u16Len) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}

	while(u8EndAlignSize--)
	{
        gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData,1);  
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_FASTREAD_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u8EndAlignSize) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}

	_ECAT_ChipSelectDisable();    
    
}

/* 
    Function: ECAT_Lan925x_SPIWritePDRAM

    This function does Write Access to Ether CAT Core Process RAM  using 'Serial Write(0x02)' command 
	supported by LAN9252/LAN9255 Compatible SPI. This function shall be used only when PDI is selected as
	LAN9252/LAN9255 Compatible SPI (0x80)
     
    Input : u16Addr    -> Address of the RAM location to be written
            *pu8Data -> Pointer to the data that is to be written
			u16Len	 -> Number of bytes to be written to PDRAM location

    Output : None

*/

void ECAT_Lan925x_SPIWritePDRAM(uint8_t *pu8Data, uint16_t u16Addr, uint16_t u16Len)
{
	UINT32_VAL 	u32Val;
	uint8_t 	u8StartAlignSize, u8EndAlignSize; 
    uint8_t  	u8TxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  	u8RxData[DWORD_LENGTH]={0,0,0,0};
    uint8_t 	u8TxLen = 1, u8RxLen = 1,u8dummy_clk_cnt = 0;
	uint8_t 	u8TxOneByteData=0, u8RxOneByteData=0;
	
	/* Address and length */
	u32Val.w[0] = u16Addr;
	u32Val.w[1] = u16Len;
	MCHP_ESF_PDI_WRITE(ECAT_PRAM_WR_ADDR_LENGTH_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

	/* write command */
	u32Val.Val = 0x80000000;
	MCHP_ESF_PDI_WRITE(ECAT_PRAM_WR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
	u8StartAlignSize = (u16Addr & 3);
//	u8EndAlignSize = (u16Len & 3) + u8StartAlignSize;
	// Changed for a bug identified with HBI - Demux BSP
	u8EndAlignSize = (u16Len + u8StartAlignSize) & 3;
	if (u8EndAlignSize & 3)
    {
		u8EndAlignSize = (((u8EndAlignSize + 4) & 0xC) - u8EndAlignSize);
	}
	
	/* SPI read and write with 3 bytes of TX and RX length */
	u8TxLen = u8RxLen = 3;
	
	/* Writing to FIFO */
	_ECAT_ChipSelectEnable();
	
	u8TxData[0] = CMD_SERIAL_WRITE;
    u8TxData[1] = (uint8_t)0;
    u8TxData[2] = (uint8_t)0x20;

    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9255UtilObj.spiPlib->spiWriteRead(u8TxData, u8TxLen,u8RxData,u8RxLen);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
    {    
    }
    gEscALEvent.Byte[0] = u8RxData[1];
    gEscALEvent.Byte[1] = u8RxData[2];
    
	/* SPI read and write with 1 bytes of TX and RX length */
	u8TxLen = u8RxLen = 1;
	
	
	/* Get the Intra DWORD dummy clock count */
	u8dummy_clk_cnt = gau8DummyCntArr[SPI_WRITE_INITIAL_OFFSET];
	/* Add Intra DWORD dummy clocks, avoid for last byte */
	
	while (u8dummy_clk_cnt--)
	{
		gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
		{       
		}
	}
	
	
	while(u8StartAlignSize--)
	{
        gDrvLan9255UtilObj.spiPlib->spiWrite(&u8TxOneByteData,u8TxLen);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_WRITE_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u8StartAlignSize) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}
    
    while (u16Len--)
	{        
		u8TxOneByteData = *pu8Data;
		gDrvLan9255UtilObj.spiPlib->spiWrite(&u8TxOneByteData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
        pu8Data++;
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_WRITE_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u16Len) 
		{
            while(u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}

    while(u8EndAlignSize--)
	{        
        u8TxOneByteData = 0;
		gDrvLan9255UtilObj.spiPlib->spiWrite(&u8TxOneByteData,u8TxLen);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
        {            
        }
		
		/* Get the Intra DWORD dummy clock count */
        u8dummy_clk_cnt = gau8DummyCntArr[SPI_WRITE_INTRA_DWORD_OFFSET];
        /* Add Intra DWORD dummy clocks, avoid for last byte */
        if (0 != u8EndAlignSize) 
		{
            while (u8dummy_clk_cnt--)
			{
				gDrvLan9255UtilObj.spiPlib->spiRead(&u8RxOneByteData, u8RxLen);
                _ECAT_QSPI_SyncWait();
                while(gDrvLan9255UtilObj.spiPlib->spiIsBusy())
				{       
				}
            }
        }
	}
	
	_ECAT_ChipSelectDisable();    
}


/*******************************************************************************
Function:
    uint16_t ECAT_PDI_TimerGet(void)

Summary:
    Get the 1ms current timer value
Description:
    This routine gets the 1ms current timer value.
*******************************************************************************/
uint16_t ECAT_PDI_TimerGet()
{
	return (0);
}

/*******************************************************************************
Function:
    void ECAT_PDI_TimerClear(void)

Summary:
    Clear the 1ms current timer value
Description:
    This routine clears the 1ms current timer value.
*******************************************************************************/
void ECAT_PDI_TimerClear(void)
{
}

/*******************************************************************************
Function:
    void ECAT_PDI_TimerInterrupt(void)

Summary:
    This function configure and enable the TIMER interrupt for 1ms

Description:
    This function configure and enable the TIMER interrupt for 1ms
*******************************************************************************/
void ECAT_PDI_TimerInterrupt(void)
{
    ECAT_PDI_TimerInterruptInitialization();
}

/*******************************************************************************
Function:
    static void _ECAT_TimerInterruptCallback(uintptr_t context)

Summary:
    Ethercat Timer interrupt Handler
Description:
    This routine load the Timer register and perform EtherCAT check operation.
*******************************************************************************/
static void _ECAT_TimerInterruptCallback(uintptr_t context)
{
    CRITICAL_SECTION_ENTER();
	ECAT_CheckTimer();
    CRITICAL_SECTION_LEAVE();
}

/*******************************************************************************
Function:
    static void ECAT_PDI_TimerInterruptInitialization()

Summary:
    Register Callback function for PDI SYS_Tick interrupt
*******************************************************************************/
static void ECAT_PDI_TimerInterruptInitialization(void)
{
    gDrvLan9255UtilObj.timerPlib->timerCallbackSet(_ECAT_TimerInterruptCallback,(uintptr_t) NULL);
    gDrvLan9255UtilObj.timerPlib->timerStart();
}
/*******************************************************************************
Function:
    static void __ECAT_QSPI_SyncWait(void)

Summary:
    Ethercat QSPI synchronization process
*******************************************************************************/
static void _ECAT_QSPI_SyncWait(void)
{
    while(ECAT_QSPI_IsTransmissionBusy()) 
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();                            
}
/*******************************************************************************
Function:
	void ECAT_HWSetlED(uint8_t RunLed,uint8_t ErrLed)

Summary:
	This function set the Error LED if it is required.

Description:
	LAN9255 doesn't support Run LED. So this feature has to be enabled by PDI SOC if it is needed.
	LAN9255 supports error LED.

Parameters:
    RunLed	- It is not used. This will be set by LAN9255.
    ErrLed	- 1- ON, 0-0FF.
*******************************************************************************/
void ECAT_HWSetlED(uint8_t RunLed, uint8_t ErrLed)
{
	if(ErrLed == false)
	{
/* Error Select PIN set. LED status is OFF */
		PORT_PinSet((PORT_PIN)PORT_PIN_PB31);
	}
	else
	{
/* Error Select PIN Clear . LED status is ON*/
		PORT_PinClear((PORT_PIN)PORT_PIN_PB31);
	}
}

/*******************************************************************************
Function:
    void ECAT_Initialization(void)

Summary:
    Initialize EtherCAT
Description:
    This routine enable SPI module and initialize LAN9255
*******************************************************************************/
void ECAT_Initialization()
{
	
    // Ethercat QSPI Callback registration 
    ECAT_QSPI_EventHandlerSet();
	
	uint32_t u32data;
	do
	{
		MCHP_ESF_IS_PDI_FUNCTIONAL((uint8_t *)&u32data);
        if (u32data == 0xFFFFFFFF)
        {
            /* Reset SQIO mode */
            ECAT_SPI_DisableQuadMode();
        }
	} while (0x87654321 != u32data);
    
    /* Update the SQI related config data as per the frequency */
	ECAT_SPI_SetCfg_dataInit();
    
    /* SQI Set configuration for dummy cycle */
    ECAT_SPI_SetConfiguration(gau8DummyCntArr);
	LAN9255_Init();
}

void ECAT_Util_Initialize(
    const unsigned short int drvIndex,
    const void * const init
)
{
    DRV_LAN9255_UTIL_INIT* lan9255UtilInit  = (DRV_LAN9255_UTIL_INIT *)init;

    gDrvLan9255UtilObj.spiTransferStatus    = DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
    gDrvLan9255UtilObj.spiPlib              = lan9255UtilInit->spiPlib;
	// Timer PLIB initialization for LAN9255 driver 
    gDrvLan9255UtilObj.timerPlib            = lan9255UtilInit->timerPlib;
   
}

static void ECAT_QSPI_Callback(uintptr_t context)
{
    // transmission completed
    gDrvLan9255UtilObj.spiTransferStatus = DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
}

static void ECAT_QSPI_TransmissionFlagClear(void)
{
    // clear transmission flag to false
    gDrvLan9255UtilObj.spiTransferStatus = DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_BUSY;
}

static bool ECAT_QSPI_IsTransmissionBusy(void)
{
    return (gDrvLan9255UtilObj.spiTransferStatus == DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_BUSY);
}

static void ECAT_QSPI_EventHandlerSet(void)
{
    gDrvLan9255UtilObj.spiPlib->spiCallbackRegister(ECAT_QSPI_Callback,0);
}
