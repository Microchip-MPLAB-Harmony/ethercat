/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9252_ecat_adpt.c

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

void PDI_Init_SYSTick_Interrupt();
     

volatile char    gEtherCATQSPITransmission = false;

/* This is the driver instance object array. */
static DRV_LAN9252_UTIL_OBJ gDrvLan9252UtilObj;
/*Global interrupt enable and disable  */
void ECAT_QSPI_TransmissionFlagClear(void);
void ECAT_QSPI_CallbackRegistration(void);
bool ECAT_QSPI_TransmissionBusy(void);
volatile uint32_t processorStatus;


void CRITICAL_SECTION_ENTER(void)
{
//<#if __PROCESSOR?matches("PIC32M.*") == true>
//    processorStatus = __builtin_disable_interrupts();
//<else>
//    processorStatus = __get_PRIMASK();
//    __disable_irq();
//</#if>
    processorStatus = __get_PRIMASK();
    __disable_irq();    
    //__set_BASEPRI(4 << (8 - __NVIC_PRIO_BITS));
}

void CRITICAL_SECTION_LEAVE(void)
{
//<#if __PROCESSOR?matches("PIC32M.*") == true>
//    __builtin_mtc0(12, 0, processorStatus);
//<else>
//    __set_PRIMASK( processorStatus );
//</#if>
    __set_PRIMASK( processorStatus );
   // __set_BASEPRI(0U); // remove the BASEPRI masking
}

#ifdef DC_SUPPORTED
/*******************************************************************************
    Function:
        void ethercat_sync0_cb()

    Summary:
        Interrupt service routine for the interrupt from SYNC0
*******************************************************************************/
void ethercat_sync0_cb()
{
	CRITICAL_SECTION_ENTER();
	Sync0_Isr();
	CRITICAL_SECTION_LEAVE();
}

/*******************************************************************************
    Function:
        void ethercat_sync1_cb()

    Summary:
        Interrupt service routine for the interrupt from SYNC1
*******************************************************************************/
void ethercat_sync1_cb()
{
	CRITICAL_SECTION_ENTER();
	Sync1_Isr();
  	CRITICAL_SECTION_LEAVE();   
}

/*******************************************************************************
    Function:
        void PDI_Init_SYNC_Interrupts()

    Summary:
        Register Callback function for PDI SYNC0 and SYNC1 interrupts
*******************************************************************************/
void PDI_Init_SYNC_Interrupts()
{
    // SYNC0 interrupt callback 
    EIC_CallbackRegister(EIC_PIN_0,ethercat_sync0_cb, 0);
    // SYNC1 interrupt callback 
    EIC_CallbackRegister(EIC_PIN_1,ethercat_sync1_cb, 0);
}
#endif // DC_SUPPORTED

/*******************************************************************************
    Function:
        void ether_cat_escirq_cb()

    Summary:
        Interrupt service routine for the interrupt from ESC
*******************************************************************************/
void ethercat_escirq_cb()
{
	CRITICAL_SECTION_ENTER();
	PDI_Isr();    
	CRITICAL_SECTION_LEAVE();
}

/*******************************************************************************
    Function:
        void PDI_Init_SYNC_Interrupts()

    Summary:
        Register Callback function for PDI ESC interrupts
*******************************************************************************/
void PDI_IRQ_Interrupt()
{
    // External interrupt callback 
    EIC_CallbackRegister(EIC_PIN_7,ethercat_escirq_cb, 0);
}

/*******************************************************************************
    Function:
        void SPIChipSelectSet()

    Summary:
        Disable EtherCAT slave
*******************************************************************************/
void SPIChipSelectSet(void)
{
   SPI_CS_Set();
}

/*******************************************************************************
    Function:
        void SPIChipSelectClr()

    Summary:
        Enable EtherCAT slave
*******************************************************************************/
void SPIChipSelectClr(void)
{
    SPI_CS_Clear();
}

/*******************************************************************************
    Function:
        void EtherCATTestPinSet()

    Summary:
        Set EtherCAT test pin.
*******************************************************************************/
void EtherCATTestPinSet(void)
{
    Ethercat_Test_Pin_Set();
}

/*******************************************************************************
    Function:
        void EtherCATTestPinClr()

    Summary:
        Clear EtherCAT test pin.
*******************************************************************************/
void EtherCATTestPinClr(void)
{
    Ethercat_Test_Pin_Clear();
}

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

/*******************************************************************************
    Function:
        void SPIWrite()

    Summary:
        API for SPI one byte write.
*******************************************************************************/

void SPIWrite(uint16_t adr, uint8_t *data)
{
	uint8_t len = 4;
    uint8_t  txData[4]={0,0,0,0};

    txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)(adr >> 8);
    txData[2] = (uint8_t)adr;
    
    SPIChipSelectClr();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData,3);
    while(ECAT_QSPI_TransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    gDrvLan9252UtilObj.spiPlib->spiWrite(data, len);
    while(ECAT_QSPI_TransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());   
    SPIChipSelectSet();

}

/*******************************************************************************
    Function:
        void SPIRead(uint16_t adr, uint8_t *data)

    Summary:
        API for SPI one byte read .
*******************************************************************************/

void SPIRead(uint16_t adr, uint8_t *data)
{
	uint8_t len = 4;
    uint8_t  txData[4]={0,0,0,0};
    uint8_t  rxData[4]={0,0,0,0};
    
	txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)(adr >> 8);
    txData[2] = (uint8_t)adr;
    
    SPIChipSelectClr();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData, 3);
    while(ECAT_QSPI_TransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());	
    gDrvLan9252UtilObj.spiPlib->spiRead(rxData, len);
    while(ECAT_QSPI_TransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    
    memcpy(data,rxData,len);
    
	SPIChipSelectSet();

 }

void ReadPdRam(UINT8 *pData, UINT16 Address, UINT16 Len) 
{
	UINT32_VAL param32_1;
	UINT8 startAlignSize, EndAlignSize;
	UINT8 dummy = 0;
    uint8_t  txData[4]={0,0,0,0};
    uint8_t  rxData[4]={0,0,0,0};
	/* Address and length */
	param32_1.w[0] = Address;
	param32_1.w[1] = Len;
	SPIWrite(0x0308, (uint8_t*)&param32_1.Val);

	/* Read command */
	param32_1.Val = 0x80000000;
	SPIWrite(0x030c, (uint8_t*)&param32_1.Val);

	startAlignSize = (Address & 3);
	EndAlignSize = (Len & 3) + startAlignSize;
	if (EndAlignSize & 3){
		EndAlignSize = (((EndAlignSize + 4) & 0xC) - EndAlignSize);
	}


        
    txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x04;
	
    SPIChipSelectClr();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData, 3);
    while(ECAT_QSPI_TransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    while (startAlignSize--)
	{
		gDrvLan9252UtilObj.spiPlib->spiRead(&dummy,1);
        while(ECAT_QSPI_TransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
	}
    
    while (Len--)
	{
		gDrvLan9252UtilObj.spiPlib->spiRead(rxData,1);
        while(ECAT_QSPI_TransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
        *pData++ = rxData[0];
	}
    

    while (EndAlignSize--)
	{
    	gDrvLan9252UtilObj.spiPlib->spiRead(&dummy,1);
        while(ECAT_QSPI_TransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
	}
    
	SPIChipSelectSet();

}

void WritePdRam(UINT8 *pData, UINT16 Address, UINT16 Len) 
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
	if (EndAlignSize & 3){
		EndAlignSize = (((EndAlignSize + 4) & 0xC) - EndAlignSize);
	}

    /* Writing to FIFO */    
	txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x20;

    SPIChipSelectClr();
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    
    gDrvLan9252UtilObj.spiPlib->spiWrite(txData, 3);
    while(ECAT_QSPI_TransmissionBusy())
    {        
    }
    ECAT_QSPI_TransmissionFlagClear();
    
    while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
    while (startAlignSize--)
	{
        gDrvLan9252UtilObj.spiPlib->spiWrite(&dummy,1);
        while(ECAT_QSPI_TransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
	}
    
    while (Len--)
	{        
        txData[0] = *pData;
		gDrvLan9252UtilObj.spiPlib->spiWrite(txData,1);
        while(ECAT_QSPI_TransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
        pData++;
	}

    while (EndAlignSize--)
	{        
        txData[0] = dummy;
		gDrvLan9252UtilObj.spiPlib->spiWrite(txData,1);
        while(ECAT_QSPI_TransmissionBusy())
        {        
        }
        ECAT_QSPI_TransmissionFlagClear();
        while(gDrvLan9252UtilObj.spiPlib->spiIsBusy());
	}
    
    SPIChipSelectSet();
}
/*******************************************************************************
    Function:
        UINT16 PDI_GetTimer(void)

    Summary:
        Get the 1ms current timer value
    Description:
        This routine gets the 1ms current timer value.
*******************************************************************************/
UINT16 PDI_GetTimer()
{
	return (0);
}

/*******************************************************************************
    Function:
        void PDI_ClearTimer(void)

    Summary:
        Clear the 1ms current timer value
    Description:
        This routine clears the 1ms current timer value.
*******************************************************************************/
void PDI_ClearTimer(void)
{
}

/*******************************************************************************
    Function:
    void PDI_Timer_Interrupt(void)

    Summary:
     This function configure and enable the TIMER interrupt for 1ms

    Description:
    This function configure and enable the TIMER interrupt for 1ms
*******************************************************************************/
void PDI_Timer_Interrupt(void)
{
    PDI_Init_SYSTick_Interrupt();
}

/*******************************************************************************
    Function:
        void SysTick_Handler(void)

    Summary:
        SysTick ISR Handler
    Description:
        This routine load the SysTick LOAD register and perform EtherCAT check opration.
*******************************************************************************/
void ECAT_SysTick_Handler(uintptr_t context)
{
	EtherCATTestPinSet();
    CRITICAL_SECTION_ENTER();
	ECAT_CheckTimer();
    CRITICAL_SECTION_LEAVE();
	EtherCATTestPinClr();
}

/*******************************************************************************
    Function:
        void PDI_Init_SYSTick_Interrupt()

    Summary:
        Register Callback function for PDI SYS_Tick interrupt
*******************************************************************************/
void PDI_Init_SYSTick_Interrupt()
{
    /* SYSTICK_TimerCallbackSet(ECAT_SysTick_Handler,(uintptr_t) NULL);
    SYSTICK_TimerStart(); */
    gDrvLan9252UtilObj.timerPlib->timerCallbackSet(ECAT_SysTick_Handler,(uintptr_t) NULL);
    gDrvLan9252UtilObj.timerPlib->timerStart();
}

/*******************************************************************************
    Function:
    void HW_SetLed(UINT8 RunLed,UINT8 ErrLed)

    Summary:
     This function set the Error LED if it is required.

    Description:
    LAN9252 does not support error LED. So this feature has to be enabled by PDI SOC if it is needed.

    Parameters:
        RunLed	- It is not used. This will be set by LAN9252.
        ErrLed	- 1- ON, 0-0FF.
*******************************************************************************/
void HW_SetLed(UINT8 RunLed, UINT8 ErrLed)
{
	/* Here RunLed is not used. Because on chip supported RUN Led is available*/
}

/*******************************************************************************
    Function:
        void ether_cat_init(void)

    Summary:
        Initialize EtherCAT
    Description:
        This routine enable SPI module and initialize LAN9252
*******************************************************************************/
void EtherCATInit()
{
    // Ethercat QSPI Callback registration 
    ECAT_QSPI_CallbackRegistration();
	LAN9252_Init();
}

// *****************************************************************************
// *****************************************************************************
// Section: DRV_LAN9252 Driver Global Functions
// *****************************************************************************
// *****************************************************************************

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

void ECAT_QSPI_Callback(uintptr_t context)
{
    // transmission completed
    gDrvLan9252UtilObj.spiTransferStatus = DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
}

void ECAT_QSPI_TransmissionFlagClear(void)
{
    // clear transmission flag to false
    gDrvLan9252UtilObj.spiTransferStatus = DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_BUSY;
}

bool ECAT_QSPI_TransmissionBusy(void)
{
    return (gDrvLan9252UtilObj.spiTransferStatus == DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_BUSY);
}

void ECAT_QSPI_CallbackRegistration(void)
{
    gDrvLan9252UtilObj.spiPlib->spiCallbackRegister(ECAT_QSPI_Callback,0);
}
