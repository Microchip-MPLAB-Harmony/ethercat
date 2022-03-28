/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9253_ecat_util.c

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

#include "drv_lan9253.h"
#include "drv_lan9253_ecat_util.h"
#include "drv_lan9253_definitions.h"
#include "definitions.h"
#include "ecatslv.h"

static void ECAT_PDI_TimerInterruptInitialization(void);
/* This is the driver instance object array. */
static DRV_LAN9253_UTIL_OBJ gDrvLan9253UtilObj;
     
static uint8_t gau8rx_data[128] = {0};

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
	Sync0_Isr();
}

/*******************************************************************************
Function:
    void _ECAT_Sync1Callback(PIO_PIN pin, uintptr_t context)

Summary:
    Interrupt service routine for the interrupt from SYNC1
*******************************************************************************/
void _ECAT_Sync1Callback(uintptr_t context)
{
	Sync1_Isr();
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
        void ECAT_SQI_SetCfg_dataInit()

    Summary:
        This function configures the number of initial, per byte and per DWORD
        dummy bytes needed as per the current PDI frequency. 
*******************************************************************************/

void ECAT_SQI_SetCfg_dataInit(void)
{
    uint16_t u16SQIClkPeriod = 0, u16SQIClkfreq = 0;
    uint8_t  u8DummyBytes =0;
	
    u16SQIClkfreq = DRV_LAN9253_BAUDRATE_PDI_FREQ;
    /* Get clock period in Nano seconds */
    u16SQIClkPeriod = CLK_PERIOD_1MHZ/u16SQIClkfreq;
    
    /* Fill the SQI related part in gau8DummyCntArr */
    /* SQI Fast read, index from 33, 34, 35 
     * as initial, intra DWORD  inter DWORD
     */
	u8DummyBytes = GetDummyBytesRequired (QUAD_SPI, IAT_DWRD, 0, u16SQIClkPeriod);
	//Fix is added to provide PDI counter error test
    //making dummy bytes calculated as DWORD aligned
#ifdef ECAT_DUMMY_READ_EN
	if((u8DummyBytes % DWORD_LENGTH) != 0)
	{
		/* As SQI , we make sure the dummy bytes required should be multiple of 4*/
        /* making round off to the next DWORD aligned value */
        u8DummyBytes += (DWORD_LENGTH - (u8DummyBytes % DWORD_LENGTH));
        gau8DummyCntArr[SQI_FASTREAD_INITIAL_OFFSET] = u8DummyBytes;
	}
#else
    gau8DummyCntArr[SQI_FASTREAD_INITIAL_OFFSET] = u8DummyBytes;
#endif	

    /* SQI Write, index starts from 36, 37  38 
     * as Initial, Intra DWORD  Inter DWORD
     */
    u8DummyBytes = GetDummyBytesRequired (QUAD_SPI, IAT_NULL, 0, u16SQIClkPeriod);
    if (u8DummyBytes == 0x0) 
	{
        /* Set minimum one dummy byte */
        /* SPECIAL CASE - Add 1 byte clock cycle count to byDummy
        * SAMD51 and SAME53 supports 24 bit and 32 bit addressing format
        * LAN925x expects 16bit addressing format
        * So In order to support SAMD51, converting the 16bit address to 24bit
        * treating the extra address byte as dummy cycle, 
        * so add the 1 byte dummy cycle from the requested.
        */
        gau8DummyCntArr[SQI_WRITE_INITIAL_OFFSET] = 0x1;
    }
	else
	{
		gau8DummyCntArr[SQI_WRITE_INITIAL_OFFSET] = u8DummyBytes;
	}
    
}

/*******************************************************************************
    Function:
        void ECAT_SQI_SetConfiguration(uint8_t *dummyArr)

    Summary:
        This function configures the number of initial, per byte and per DWORD dummy
		bytes needed. 
*******************************************************************************/

void ECAT_SQI_SetConfiguration(uint8_t *pu8DummyByteCnt)
{
    qspi_register_xfer_t qspi_xfer;
    uint16_t u16XfrLen;
    
    /* CMD_SPI_SETCFG does not require dummy Byte */
    memset((void *)&qspi_xfer, 0, sizeof(qspi_register_xfer_t));
    /* Load the instruction */
	qspi_xfer.instruction = CMD_SQI_SETCFG;
    /* Setting single bit SPI */
    qspi_xfer.width = SINGLE_BIT_SPI;
    
    u16XfrLen = SETCFG_MAX_DATA_BYTES;
 
    gDrvLan9253UtilObj.sqiPlib->sqiRegisterWrite(&qspi_xfer, (uint32_t *)pu8DummyByteCnt, u16XfrLen);
}

/*******************************************************************************
    Function:
        void ECAT_SQI_EnableQuadMode(void)

    Summary:
        This function Enable the SQI mode in slave, send ENABLE_SQI
*******************************************************************************/

void ECAT_SQI_EnableQuadMode(void)
{
    qspi_command_xfer_t qspi_xfer;
    
    memset((void *)&qspi_xfer, 0, sizeof(qspi_command_xfer_t));
    /* Load the instruction */
	qspi_xfer.instruction = CMD_ENABLE_SQI;
    /* Setting single bit SPI */
    qspi_xfer.width = SINGLE_BIT_SPI;
    
    /* Sending address as 0, as we don't need to send address */
   gDrvLan9253UtilObj.sqiPlib->sqiCommandWrite(&qspi_xfer, 0x0);
}

/*******************************************************************************
    Function:
        void ECAT_SQI_DisableQuadMode(void)

    Summary:
        This function Disable the SQI mode in slave, send RST_QIO
*******************************************************************************/

void ECAT_SQI_DisableQuadMode(void)
{
    qspi_command_xfer_t qspi_xfer;
    
    memset((void *)&qspi_xfer, 0, sizeof(qspi_command_xfer_t));
    /* Load the instruction */
	qspi_xfer.instruction = CMD_RESET_SQI;
    /* Setting single bit SPI */
    qspi_xfer.width = SINGLE_BIT_SPI;
    
    /* Sending address as 0, as we don't need to send address */
    gDrvLan9253UtilObj.sqiPlib->sqiCommandWrite (&qspi_xfer, 0x0);
}

/*******************************************************************************
Function:
    void ECAT_Lan9253_IsPDIFunctional(uint8_t *data)

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
void ECAT_Lan9253_IsPDIFunctional(uint8_t *pu8Data)
{
	qspi_memory_xfer_t qspi_xfer;
    uint8_t		u8dummy_cycle = 0;
    uint32_t 	u16XfrLen = 0;	
	uint32_t 	u32InstrAddr = 0;
    
    /* FAST READ require one dummy Byte */
    u8dummy_cycle = CMD_SPI_FAST_READ_DUMMY * QSPI_SPI_ONE_BYTE_CLK_COUNT;
    memset((void *)&qspi_xfer, 0, sizeof(qspi_memory_xfer_t));
    /* Load the instruction */
	qspi_xfer.instruction = CMD_FAST_READ;
    /* Setting single bit SPI, as Byte test register supports SPI */
    qspi_xfer.width = SINGLE_BIT_SPI;
    /* Configure the dummy cycles */
    qspi_xfer.dummy_cycles = u8dummy_cycle;
    
    u16XfrLen = DWORD_LENGTH;
    u32InstrAddr = LAN925x_BYTE_ORDER_REG;
    u32InstrAddr = (u32InstrAddr << 8) | u16XfrLen;
 
    gDrvLan9253UtilObj.sqiPlib->sqiMemoryRead(&qspi_xfer, (uint32_t *)pu8Data, (uint32_t)u16XfrLen, u32InstrAddr);	

}


/* LAN9253 */
/* 
    Function: ECAT_LAN925x_SQIWrite

    This function does Write Access to Non-Ether CAT core CSR, Ether CAT Core CSR and Process RAM
	using 'Serial Write(0x02)' command supported by LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9253 Compatible SPI (0x82)
     
    Input : u16Adddr    -> Address of the register to be written
            *pu8Data  -> Pointer to the data that is to be written
			u32Length  -> Number of bytes to be written 

    Output : None
	
	Note   : Since now SPI is running at 5MHz, Serial Write is successful without any dummy bytes or wait
			 signal. But, as the SPI clock speed increases, we have to follow either of these. 

*/

void ECAT_LAN925x_SQIWrite(uint16_t u16Addr, uint8_t *pu8Data, uint32_t u32Length)
{	
	uint32_t u32ModLen = 0;
    qspi_memory_xfer_t qspi_xfer;
	uint32_t u32InstrAddr = 0;
	uint8_t u8Dummy = 0;    /* Disabled as write does not require dummy */

	/* Core CSR and Process RAM accesses can have any alignment and length */
	if (u16Addr < 0x3000)
	{
		/* DWORD Buffering - Applicable for Write only */
		if (u32Length > 1)
		{
			u16Addr |= 0xC000; 	/* addr[15:14]=11 */
		}
		else
		{
			/* Do Nothing */
		}
	}
	else
	{
		/* Non Core CSR length will be adjusted if it is not DWORD aligned */
		u32ModLen = u32Length % 4;
		if (1 == u32ModLen)
		{
			u32Length = u32Length + 3;
		}
		else if (2 == u32ModLen)
		{
			u32Length = u32Length + 2;
		}
		else if (3 == u32ModLen)
		{
			u32Length = u32Length + 1;
		}
		else
		{
			/* Do nothing if length is 0 since it is DWORD access */
		}
	}

	memset((void *)&qspi_xfer, 0, sizeof(qspi_memory_xfer_t));
	qspi_xfer.instruction = CMD_SERIAL_WRITE;
	qspi_xfer.width = QUAD_CMD;
	qspi_xfer.dummy_cycles = 0 /*u8Dummy*/;

	/* Get the dummy Byte count */
	/* SPECIAL CASE - Reduce 1 byte clock cycle count from byDummy
     * SAMD51 supports 24 bit and 32 bit addressing format
     * LAN925x expects 16bit addressing format
     * So In order to support SAMD51, converting the 16bit address to 24bit
     * treating the extra address byte as dummy cycle, 
     * so reduce the 1 byte dummy cycle from the requested.
     */
	u8Dummy = (gau8DummyCntArr[SQI_WRITE_INITIAL_OFFSET] - 1);
    qspi_xfer.dummy_cycles = u8Dummy;
    u32InstrAddr = u16Addr;
    u32InstrAddr = u32InstrAddr << 8;

    gDrvLan9253UtilObj.sqiPlib->sqiMemoryWrite(&qspi_xfer, (uint32_t *)&pu8Data[0], u32Length, u32InstrAddr);

}

/* 
    Function: ECAT_LAN925x_SQIRead

    This function does Read Access to Non-Ether CAT core CSR, Ether CAT Core CSR and Process RAM
	using 'Fast Read(0x0B)' command supported by LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9253 Compatible SPI (0x82)
     
    Input : u16Addr     -> Address of the register to be read
            *pu8Data    -> Pointer to the data that is to be read
 *          u32Length   -> Length of the data to be read

    Output : None

*/

void ECAT_LAN925x_SQIRead(uint16_t u16Addr, uint8_t *pu8data, uint32_t u32Length)
{	
	uint32_t u32ModLen = 0;
    uint8_t u8Dummy = 0;
    qspi_memory_xfer_t qspi_xfer;
	uint32_t u32InstrAddr = 0;
	
	/* Core CSR and Process RAM accesses can have any alignment and length */
	if (u16Addr < 0x3000)
	{
		if (u32Length>1)
		{
			/* Use Auto-increment if number of bytes to read is more than 1 */
			u16Addr |= 0x4000;			
		}

	}
	else
	{
		/* Non Core CSR length will be adjusted if it is not DWORD aligned */
		u32ModLen = u32Length % 4; 
		if (1 == u32ModLen)
		{
			u32Length = u32Length + 3; 
		}
		else if (2 == u32ModLen)
		{
			u32Length = u32Length + 2; 
		}
		else if (3 == u32ModLen)
		{
			u32Length = u32Length + 1; 
		}
		else 
		{
			/* Do nothing if length is 0 since it is DWORD access */
		}
	}

	memset((void *)&gau8rx_data[0], 0, sizeof(gau8rx_data));
    memset((void *)&qspi_xfer, 0, sizeof(qspi_memory_xfer_t));
	qspi_xfer.instruction = CMD_FAST_READ;
    qspi_xfer.width = QUAD_CMD;
	//Fix is added to provide PDI error counter test
    //the transfer length should not contain dummy bytes length
#ifdef ETHERCAT_DUMMY_READ_EN
    qspi_xfer.dummy_cycles = u8Dummy;
    /* Get the number of dummy cycle required */
    u8Dummy = gau8DummyCntArr[SQI_FASTREAD_INITIAL_OFFSET];

    u32InstrAddr = u16Addr;
    u32InstrAddr = (u32InstrAddr << 8) | (u32Length);

    gDrvLan9253UtilObj.sqiPlib->sqiMemoryRead(&qspi_xfer, (uint32_t *)&gau8rx_data[0], u32Length + u8Dummy, u32InstrAddr);
    
   //Fix is added for odd address failure
    u8Dummy += (u16Addr & 0x3);

    memcpy (pu8data, gau8rx_data + u8Dummy, u32Length);
#else
	 /* Get the number of dummy cycle required */
    u8Dummy = gau8DummyCntArr[SQI_FASTREAD_INITIAL_OFFSET];
    qspi_xfer.dummy_cycles = u8Dummy * QSPI_SQI_ONE_BYTE_CLK_COUNT;

    u32InstrAddr = u16Addr;
    u32InstrAddr = (u32InstrAddr << 8) | u32Length;

    gDrvLan9253UtilObj.sqiPlib->sqiMemoryRead(&qspi_xfer, (uint32_t *)&gau8rx_data[0], u32Length, u32InstrAddr);
    
   //Fix is added for odd address failure
   // u8Dummy += (u16Addr & 0x3);
    
    memcpy (pu8data, gau8rx_data, u32Length);
#endif 
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
    gDrvLan9253UtilObj.timerPlib->timerCallbackSet(_ECAT_TimerInterruptCallback,(uintptr_t) NULL);
    gDrvLan9253UtilObj.timerPlib->timerStart();
}
/*******************************************************************************
Function:
	void ECAT_HWSetlED(uint8_t RunLed,uint8_t ErrLed)

Summary:
	This function set the Error LED if it is required.

Description:
	LAN9253 doesn't support Run LED. So this feature has to be enabled by PDI SOC if it is needed.
	LAN9253 supports error LED.

Parameters:
    RunLed	- It is not used. This will be set by LAN9253.
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
    This routine enable SPI module and initialize LAN9253
*******************************************************************************/
void ECAT_Initialization()
{
	
	uint32_t u32data;
	do
	{
		MCHP_ESF_IS_PDI_FUNCTIONAL((uint8_t *)&u32data);
        if (u32data == 0xFFFFFFFF)
        {
            /* Reset SQIO mode */
            ECAT_SQI_DisableQuadMode();
        }
	} while (0x87654321 != u32data);
    
	/* Update the SQI related config data as per the frequency */
	ECAT_SQI_SetCfg_dataInit();
#ifdef ETHERCAT_DUMMY_READ_EN
    /* SQI Set configuration for dummy cycle */
    ECAT_SQI_SetConfiguration (gau8DummyCntArr);
                
    /* Send "Enable SQI" command for slave to switch to SQI mode */
    ECAT_SQI_EnableQuadMode();
#endif	
	LAN9253_Init();
}

void ECAT_Util_Initialize(
    const unsigned short int drvIndex,
    const void * const init
)
{
    DRV_LAN9253_UTIL_INIT* lan9253UtilInit  = (DRV_LAN9253_UTIL_INIT *)init;

	gDrvLan9253UtilObj.sqiPlib              = lan9253UtilInit->sqiPlib;
	// Timer PLIB initialization for LAN9253 driver 
    gDrvLan9253UtilObj.timerPlib            = lan9253UtilInit->timerPlib;
   
}

