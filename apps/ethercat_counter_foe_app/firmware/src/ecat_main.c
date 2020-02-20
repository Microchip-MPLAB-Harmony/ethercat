/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    ecat_main.c

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
#include "ecat_main.h"

void PDI_Init_SYSTick_Interrupt();
     

volatile char    gEtherCATQSPITransmission = false;


void EtherCAT_TransmissionFlagClear(void);
void EtherCAT_QSPI_CallbackRegistration(void);
bool EtherCAT_QSPITransmissionBusy(void);


void ECAT_SPI_Write_EnterAndLeave_Critical(void* pTransmitData, size_t txSize)
{
    int sysIntStatus=0;
    sysIntStatus = SYS_INT_Disable();
    QSPI_Write(pTransmitData, txSize);
    SYS_INT_Restore(sysIntStatus);
}

void ECAT_SPI_Read_EnterAndLeave_Critical(void* pReceiveData, size_t rxSize)
{
    int sysIntStatus=0;
    sysIntStatus = SYS_INT_Disable();
    QSPI_Write(pReceiveData, rxSize);
    SYS_INT_Restore(sysIntStatus);
}

void CRITICAL_SECTION_ENTER(void)
{
    __set_BASEPRI(4 << (8 - __NVIC_PRIO_BITS));
}

void CRITICAL_SECTION_LEAVE(void)
{
    __set_BASEPRI(0U); // remove the BASEPRI masking
}

#ifdef DC_SUPPORTED
/*******************************************************************************
    Function:
        void ether_cat_sync0_cb()

    Summary:
        Interrupt service routine for the interrupt from SYNC0
*******************************************************************************/
void ether_cat_sync0_cb()
{
	CRITICAL_SECTION_ENTER();
	Sync0_Isr();
	CRITICAL_SECTION_LEAVE();
}

/*******************************************************************************
    Function:
        void ether_cat_sync1_cb()

    Summary:
        Interrupt service routine for the interrupt from SYNC1
*******************************************************************************/
void ether_cat_sync1_cb()
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
    EIC_CallbackRegister(EIC_PIN_0,ether_cat_sync0_cb, 0);
    // SYNC1 interrupt callback 
    EIC_CallbackRegister(EIC_PIN_1,ether_cat_sync1_cb, 0);
}
#endif // DC_SUPPORTED

/*******************************************************************************
    Function:
        void ether_cat_escirq_cb()

    Summary:
        Interrupt service routine for the interrupt from ESC
*******************************************************************************/
void ether_cat_escirq_cb()
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
    EIC_CallbackRegister(EIC_PIN_7,ether_cat_escirq_cb, 0);
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
    
    while(QSPI_IsBusy());
    QSPI_Write(txData,3);
    while(EtherCAT_QSPITransmissionBusy())
    {        
    }
    EtherCAT_TransmissionFlagClear();
    
    while(QSPI_IsBusy());
    QSPI_Write(data, len);
    while(EtherCAT_QSPITransmissionBusy())
    {        
    }
    EtherCAT_TransmissionFlagClear();
    
    while(QSPI_IsBusy());    
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
    while(QSPI_IsBusy());
    QSPI_Write(txData, 3);
    while(EtherCAT_QSPITransmissionBusy())
    {        
    }
    EtherCAT_TransmissionFlagClear();
    
    while(QSPI_IsBusy());	
    QSPI_Read(rxData, len);
    while(EtherCAT_QSPITransmissionBusy())
    {        
    }
    EtherCAT_TransmissionFlagClear();
    
    while(QSPI_IsBusy());
    
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
    while(QSPI_IsBusy());    
    QSPI_Write(txData, 3);
    while(EtherCAT_QSPITransmissionBusy())
    {        
    }
    EtherCAT_TransmissionFlagClear();
    
    while(QSPI_IsBusy());    
    while (startAlignSize--)
	{
		QSPI_Read(&dummy,1);
        while(EtherCAT_QSPITransmissionBusy())
        {        
        }
        EtherCAT_TransmissionFlagClear();
        while(QSPI_IsBusy());
	}
    
    while (Len--)
	{
		QSPI_Read(rxData,1);
        while(EtherCAT_QSPITransmissionBusy())
        {        
        }
        EtherCAT_TransmissionFlagClear();
        while(QSPI_IsBusy());
        *pData++ = rxData[0];
	}
    

    while (EndAlignSize--)
	{
    	QSPI_Read(&dummy,1);
        while(EtherCAT_QSPITransmissionBusy())
        {        
        }
        EtherCAT_TransmissionFlagClear();
        while(QSPI_IsBusy());
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
    while(QSPI_IsBusy());
    
    QSPI_Write(txData, 3);
    while(EtherCAT_QSPITransmissionBusy())
    {        
    }
    EtherCAT_TransmissionFlagClear();
    
    while(QSPI_IsBusy());
    while (startAlignSize--)
	{
        QSPI_Write(&dummy,1);
        while(EtherCAT_QSPITransmissionBusy())
        {        
        }
        EtherCAT_TransmissionFlagClear();
        while((QSPI_IsBusy()));
	}
    
    while (Len--)
	{        
        txData[0] = *pData;
		QSPI_Write(txData,1);
        while(EtherCAT_QSPITransmissionBusy())
        {        
        }
        EtherCAT_TransmissionFlagClear();
        while(QSPI_IsBusy());
        pData++;
	}

    while (EndAlignSize--)
	{        
        txData[0] = dummy;
		QSPI_Write(txData,1);
        while(EtherCAT_QSPITransmissionBusy())
        {        
        }
        EtherCAT_TransmissionFlagClear();
        while(QSPI_IsBusy());
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
    SYSTICK_TimerCallbackSet(ECAT_SysTick_Handler,(uintptr_t) NULL);
    SYSTICK_TimerStart();
}
/*******************************************************************************
    Function:
        void stop_timer(void)

    Summary:
        Disable SysTick ISR
    Description:
        This routine disable SysTick Interrupt.
*******************************************************************************/
void stop_timer(void)
{
	NVIC_DisableIRQ(SysTick_IRQn);
}

/*******************************************************************************
    Function:
        void start_timer(void)

    Summary:
        Enable SysTick ISR
    Description:
        This routine enable SysTick Interrupt.
*******************************************************************************/
void start_timer(void)
{
	NVIC_EnableIRQ(SysTick_IRQn);
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
    EtherCAT_QSPI_CallbackRegistration();
	LAN9252_Init();
}

void ECAT_SPI_Callback(uintptr_t context)
{
    // transmission completed
    gEtherCATQSPITransmission = true;
}

void EtherCAT_TransmissionFlagClear(void)
{
    // clear transmission flag to false
    gEtherCATQSPITransmission = false;
}

bool EtherCAT_QSPITransmissionBusy(void)
{
    return (gEtherCATQSPITransmission==false);
}

void EtherCAT_QSPI_CallbackRegistration(void)
{
    QSPI_CallbackRegister(ECAT_SPI_Callback,0);
}
