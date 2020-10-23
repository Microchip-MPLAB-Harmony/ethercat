/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9252_ecat_util.c

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
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

#include "drv_lan9252.h"
#include "drv_lan9252_ecat_util.h"
#include "drv_lan9252_definitions.h"
#include "definitions.h"

static void ECAT_PDI_TimerInterruptInitialization(void);



volatile char    gEtherCATQSPITransmission = false;

/* This is the driver instance object array. */
static DRV_LAN9252_UTIL_OBJ gDrvLan9252UtilObj;
/*Global interrupt enable and disable  */
static void ECAT_QSPI_TransmissionFlagClear(void);
static void ECAT_QSPI_EventHandlerSet(void);
static bool ECAT_QSPI_IsTransmissionBusy(void);
static volatile uint32_t processorStatus;


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
    void _ECAT_Sync0Callback()

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
    void _ECAT_Sync1Callback()

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
    void ECAT_SyncInterruptsInitialization()

Summary:
    Register Callback function for PDI SYNC0 and SYNC1 interrupts
*******************************************************************************/
void ECAT_SyncInterruptsInitialization()
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
    void ECAT_ESCIRQInitialization()

Summary:
    Register Callback function for PDI ESC(EtherCAT Slave Controller) interrupts
*******************************************************************************/
void ECAT_ESCIRQInitialization()
{
	EIC_CallbackRegister(EIC_PIN_7,_ECAT_EscInterruptRequestCallback, 0);
}

/*******************************************************************************
Function:
    void _ECAT_ChipSelectDisable()

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
    void _ECAT_ChipSelectEnable()

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
            HW_EscWrite((MEM_ADDR*)countArr,adr+addrInc,count);
            HW_EscRead((MEM_ADDR*)countArr1,adr+addrInc,count);
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
    void SPIWrite()

Summary:
    API for SPI one byte write.
*******************************************************************************/

void ECAT_Lan9252_SPIWrite(uint16_t adr, uint8_t *data)
{
	uint8_t len = 4;
    uint8_t  txData[4]={0,0,0,0};

    txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)(adr >> 8);
    txData[2] = (uint8_t)adr;
    
    _ECAT_ChipSelectEnable();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData,3);
    while(ECAT_QSPI_IsTransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    gDrvLan9252UtilObj.spiPlib->spiWrite(data, len);
    while(ECAT_QSPI_IsTransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }  
    _ECAT_ChipSelectDisable();

}

/*******************************************************************************
Function:
    void ECAT_Lan9252_SPIRead(uint16_t adr, uint8_t *data)

Summary:
    This function does Read Access to Non-EtherCAT and EtherCAT Core CSR 
	using 'Serial Read(0x03)' command supported by LAN9252 Compatible SPI. 
    This function shall be used only when PDI is selected as 
    LAN9252 Compatible SPI (0x80).
*******************************************************************************/
void ECAT_Lan9252_SPIRead(uint16_t adr, uint8_t *data)
{
	uint8_t len = DWORD_LENGTH;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
    
	txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)(adr >> 8);
    txData[2] = (uint8_t)adr;
    
    _ECAT_ChipSelectEnable();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData, 3);
    while(ECAT_QSPI_IsTransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }	
    gDrvLan9252UtilObj.spiPlib->spiRead(rxData, len);
    while(ECAT_QSPI_IsTransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }    
    memcpy(data,rxData,len);
    
	_ECAT_ChipSelectDisable();
 }

void ECAT_Lan9252_SPIReadPDRAM(UINT8 *pData, UINT16 Address, UINT16 Len) 
{
	UINT32_VAL param32_1;
	UINT8 startAlignSize, EndAlignSize;
	UINT8 dummy = 0;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
	/* Address and length */
	param32_1.w[0] = Address;
	param32_1.w[1] = Len;
	SPIWrite(0x0308, (uint8_t*)&param32_1.Val);

	/* Read command */
	param32_1.Val = 0x80000000;
	SPIWrite(0x030c, (uint8_t*)&param32_1.Val);

	startAlignSize = (Address & 3);
	EndAlignSize = (Len & 3) + startAlignSize;
	if (EndAlignSize & 3)
	{
		EndAlignSize = (((EndAlignSize + 4) & 0xC) - EndAlignSize);
	}


        
    txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x04;
	
    _ECAT_ChipSelectEnable();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData, 3);
    while(ECAT_QSPI_IsTransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    while (startAlignSize--)
	{
		gDrvLan9252UtilObj.spiPlib->spiRead(&dummy,1);
        while(ECAT_QSPI_IsTransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
        {
        }
	}
    
    while (Len--)
	{
		gDrvLan9252UtilObj.spiPlib->spiRead(rxData,1);
        while(ECAT_QSPI_IsTransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
        {
        }
        *pData++ = rxData[0];
	}
    

    while (EndAlignSize--)
	{
    	gDrvLan9252UtilObj.spiPlib->spiRead(&dummy,1);
        while(ECAT_QSPI_IsTransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
        {
        }
	}
    
	_ECAT_ChipSelectDisable();

}

void ECAT_Lan9252_SPIWritePDRAM(UINT8 *pData, UINT16 Address, UINT16 Len) 
{
	UINT32_VAL param32_1;
	UINT8 startAlignSize, EndAlignSize;
	UINT8 dummy = 0;
    uint8_t  txData[4]={0,0,0,0};
    
	/* Address and length */
	param32_1.w[0] = Address;
	param32_1.w[1] = Len;
	SPIWrite(0x0310, (uint8_t*)&param32_1.Val);

	/* write command */
	param32_1.Val = 0x80000000;
	SPIWrite(0x0314, (uint8_t*)&param32_1.Val );
	
	startAlignSize = (Address & 3);
	EndAlignSize = (Len & 3) + startAlignSize;
	if (EndAlignSize & 3)
	{
		EndAlignSize = (((EndAlignSize + 4) & 0xC) - EndAlignSize);
	}

    /* Writing to FIFO */    
	txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x20;

    _ECAT_ChipSelectEnable();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData, 3);
    while(ECAT_QSPI_IsTransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
    {
    }
    while (startAlignSize--)
	{
        gDrvLan9252UtilObj.spiPlib->spiWrite(&dummy,1);
        while(ECAT_QSPI_IsTransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
        {
        }
	}
    
    while (Len--)
	{        
        txData[0] = *pData;
		gDrvLan9252UtilObj.spiPlib->spiWrite(txData,1);
        while(ECAT_QSPI_IsTransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
        {
        }
        
        pData++;
	}

    while (EndAlignSize--)
	{        
        txData[0] = dummy;
		gDrvLan9252UtilObj.spiPlib->spiWrite(txData,1);
        while(ECAT_QSPI_IsTransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy())
        {
        }
	}
    
    _ECAT_ChipSelectDisable();
}
/*******************************************************************************
Function:
    UINT16 ECAT_PDI_TimerGet(void)

Summary:
    Get the 1ms current timer value
Description:
    This routine gets the 1ms current timer value.
*******************************************************************************/
UINT16 ECAT_PDI_TimerGet()
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
    gDrvLan9252UtilObj.timerPlib->timerCallbackSet(_ECAT_TimerInterruptCallback,(uintptr_t) NULL);
    gDrvLan9252UtilObj.timerPlib->timerStart();
}

/*******************************************************************************
Function:
	void ECAT_HWSetlED(UINT8 RunLed,UINT8 ErrLed)

Summary:
	This function set the Error LED if it is required.

Description:
	LAN9253 doesn't support Run LED. So this feature has to be enabled by PDI SOC if it is needed.
	LAN9253 supports error LED.

Parameters:
    RunLed	- It is not used. This will be set by LAN9253.
    ErrLed	- 1- ON, 0-0FF.
*******************************************************************************/
void ECAT_HWSetlED(UINT8 RunLed, UINT8 ErrLed)
{
	if(ErrLed == false)
	{
/* Error Select PIN set, LED status is OFF*/
		PORT_PinSet((PORT_PIN)PORT_PIN_PB31);
	}
	else
	{
/* Error Select PIN Clear, LED status is ON*/
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
    // Ethercat QSPI Callback registration 
    ECAT_QSPI_EventHandlerSet();
	LAN9252_Init();
}

void ECAT_Util_Initialize(
    const unsigned short int drvIndex,
    const void * const init
)
{
    DRV_LAN9252_UTIL_INIT* lan9252UtilInit  = (DRV_LAN9252_UTIL_INIT *)init;

   
    gDrvLan9252UtilObj.spiTransferStatus    = DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
    gDrvLan9252UtilObj.spiPlib              = lan9252UtilInit->spiPlib;
	
	// Timer PLIB initialization for LAN9252 driver 
    gDrvLan9252UtilObj.timerPlib            = lan9252UtilInit->timerPlib;
    
}

static void ECAT_QSPI_Callback(uintptr_t context)
{
    // transmission completed
    gDrvLan9252UtilObj.spiTransferStatus = DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
}

static void ECAT_QSPI_TransmissionFlagClear(void)
{
    // clear transmission flag to false
    gDrvLan9252UtilObj.spiTransferStatus = DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_BUSY;
}

static bool ECAT_QSPI_IsTransmissionBusy(void)
{
    return (gDrvLan9252UtilObj.spiTransferStatus == DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_BUSY);
}

static void ECAT_QSPI_EventHandlerSet(void)
{
    gDrvLan9252UtilObj.spiPlib->spiCallbackRegister(ECAT_QSPI_Callback,0);
}
