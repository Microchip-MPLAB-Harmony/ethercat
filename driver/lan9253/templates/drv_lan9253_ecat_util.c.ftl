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

#include "drv_lan9253.h"
#include "drv_lan9253_ecat_util.h"
#include "drv_lan9253_definitions.h"
#include "definitions.h"
#include "ecatslv.h"

static void ECAT_PDI_TimerInterruptInitialization(void);
     

char    gEtherCATQSPITransmission = false;

/* This is the driver instance object array. */
static DRV_LAN9253_UTIL_OBJ gDrvLan9253UtilObj;
/*Global interrupt enable and disable  */
static void ECAT_QSPI_TransmissionFlagClear(void);
static void ECAT_QSPI_EventHandlerSet(void);
static bool ECAT_QSPI_IsTransmissionBusy(void);
static volatile uint32_t processorStatus;
static void _ECAT_QSPI_SyncWait(void);

void CRITICAL_SECTION_ENTER(void)
{
<#if __PROCESSOR?matches("PIC32M.*") == true>
 /* Mask interrupts at and below the kernel interrupt priority. */
   /* Read current status */
    processorStatus = _CP0_GET_STATUS();
   /* Clear Current IPL bits */
    processorStatus &= ETHERCAT_INTPRIORITYLEVEL_MASK;
   /* Update status register with new value after shifting a value equal to ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY */
    _CP0_SET_STATUS( ( processorStatus | ( ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY << ETHERCAT_PRIORITY_SHIFT_BITS ) ) );
<#else>
   /* block interrupts with priority number higher or equal to ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY */
   __set_BASEPRI(ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY << (8 - ETHERCAT_INT_PRIO_BITS));
</#if>
}

void CRITICAL_SECTION_LEAVE(void)
{
<#if __PROCESSOR?matches("PIC32M.*") == true>
    /* Unmask all the interrupts */
    __builtin_mtc0(12, 0, processorStatus);
<#else>
    /*remove the BASEPRI masking */
    __set_BASEPRI( 0U );
</#if>
}

#ifdef DC_SUPPORTED
/*******************************************************************************
Function:
    void _ECAT_Sync0Callback()

Summary:
    Interrupt service routine for the interrupt from SYNC0
*******************************************************************************/
<#if PORT_PLIB == "EIC">
void _ECAT_Sync0Callback(uintptr_t context)
<#elseif PORT_PLIB == "GPIO">
void _ECAT_Sync0Callback(GPIO_PIN pin, uintptr_t context)
<#elseif PORT_PLIB == "PIO">
void _ECAT_Sync0Callback(PIO_PIN pin, uintptr_t context)
</#if>
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
<#if PORT_PLIB == "EIC">
void _ECAT_Sync1Callback(uintptr_t context)
<#elseif PORT_PLIB == "GPIO">
void _ECAT_Sync1Callback(GPIO_PIN pin, uintptr_t context)
<#elseif PORT_PLIB == "PIO">
void _ECAT_Sync1Callback(PIO_PIN pin, uintptr_t context)
</#if>
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
void PDI_Init_SYNC_Interrupts()
{
// SYNC0 and SYNC1 interrupt callback 
<#if PORT_PLIB == "EIC">
    EIC_CallbackRegister(${DRV_LAN9253_SYNC0_INT},_ECAT_Sync0Callback, 0);
    EIC_CallbackRegister(${DRV_LAN9253_SYNC1_INT},_ECAT_Sync1Callback, 0);
<#elseif PORT_PLIB == "GPIO">
    GPIO_PinInterruptCallbackRegister(${DRV_LAN9253_SYNC0_INT}, _ECAT_Sync0Callback, 0);
    GPIO_PinInterruptCallbackRegister(${DRV_LAN9253_SYNC1_INT}, _ECAT_Sync1Callback, 0);
<#elseif PORT_PLIB == "PIO">
    PIO_PinInterruptCallbackRegister(${DRV_LAN9253_SYNC0_INT}, _ECAT_Sync0Callback, 0);
    PIO_PinInterruptCallbackRegister(${DRV_LAN9253_SYNC1_INT}, _ECAT_Sync1Callback, 0);	
</#if>
	
}
#endif // DC_SUPPORTED

/*******************************************************************************
Function:
    void _ECAT_EscInterruptRequestCallback()

Summary:
    Interrupt service routine for the interrupt from ESC
*******************************************************************************/
<#if PORT_PLIB == "EIC">
void _ECAT_EscInterruptRequestCallback(uintptr_t context)
<#elseif PORT_PLIB == "GPIO">
void _ECAT_EscInterruptRequestCallback(GPIO_PIN pin, uintptr_t context)
<#elseif PORT_PLIB == "PIO">
void _ECAT_EscInterruptRequestCallback(PIO_PIN pin, uintptr_t context)
</#if>
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
void PDI_IRQ_Interrupt()
{
<#if PORT_PLIB == "EIC">
	EIC_CallbackRegister(${DRV_LAN9253_IRQ_INT},ECAT_ESCIRQInitialization, 0);
<#elseif PORT_PLIB == "GPIO">
    GPIO_PinInterruptCallbackRegister(${DRV_LAN9253_IRQ_INT}, ECAT_ESCIRQInitialization, 0);
<#elseif PORT_PLIB == "PIO">
    PIO_PinInterruptCallbackRegister(${DRV_LAN9253_IRQ_INT}, ECAT_ESCIRQInitialization, 0);
</#if>
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
<#if DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB == "GPIO">
    GPIO_PinSet((GPIO_PIN)${DRV_LAN9253_CHIP_SELECT_PIN});
<#elseif DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB == "PIO">
    PIO_PinSet((PIO_PIN)${DRV_LAN9253_CHIP_SELECT_PIN});
<#elseif DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB == "PORT">
    PORT_PinSet((PORT_PIN)${DRV_LAN9253_CHIP_SELECT_PIN});
</#if>

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
<#if DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB == "GPIO">
    GPIO_PinClear((GPIO_PIN)${DRV_LAN9253_CHIP_SELECT_PIN});
<#elseif DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB == "PIO">
    PIO_PinClear((PIO_PIN)${DRV_LAN9253_CHIP_SELECT_PIN});
<#elseif DRV_LAN9253_SPI_CHIP_SELECT_PORT_PLIB == "PORT">
    PORT_PinClear((PORT_PIN)${DRV_LAN9253_CHIP_SELECT_PIN});
</#if>
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
void ECAT_Lan9253_IsPDIFunctional(uint8_t *pbyData)
{
	/* Before device initialization, the SPI/SQI interface will not return valid data.
    To determine when the SPI/SQI interface is functional, the Byte Order Test 
    Register (BYTE_TEST) should be polled. Once the correct pattern is read, the interface
	can be considered functional */
	uint8_t byLen = 4;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
    
    _ECAT_ChipSelectEnable();

	txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)HIBYTE(LAN925x_BYTE_ORDER_REG);
    txData[2] = (uint8_t)LOBYTE(LAN925x_BYTE_ORDER_REG);
    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    { 
    }
    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData,3,rxData,3);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
      // Update the EscALEvent value which is required for the state change
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
    gDrvLan9253UtilObj.spiPlib->spiRead(rxData, byLen);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {
    }
	memcpy(pbyData,rxData,byLen);	
	_ECAT_ChipSelectDisable();

}

#ifdef ETHERCAT_SPI_INDIRECT_MODE_ACCESS

/* 
    Function: ECAT_Lan925x_SPIWrite

    This function does Write Access to Non-Ether CAT and Ether CAT Core CSR 
	using 'Serial Write(0x02)' command supported by LAN9252/LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9252/LAN9253 Compatible SPI (0x80)
     
    Input : wAdddr    -> Address of the register to be written
            *pbyData  -> Pointer to the data that is to be written

    Output : None
	
	Note   : In LAN9252 Compatible SPI, all registers are DWORD aligned. Length will be fixed to 4. Hence,
			 there is no separate length argument.
*/

void ECAT_Lan925x_SPIWrite(uint16_t wAdddr, uint8_t *pbyData, uint8_t wLen)
{
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH] = {0,0,0,0};
    
    txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)(wAdddr >> 8);
    txData[2] = (uint8_t)wAdddr;
    
    _ECAT_ChipSelectEnable();
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
	gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData,3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
  
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
    gDrvLan9253UtilObj.spiPlib->spiWrite(pbyData, wLen);
    _ECAT_QSPI_SyncWait();
    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    _ECAT_ChipSelectDisable();

}

/* 
    Function: ECAT_Lan925x_SPIRead

    This function does Read Access to Non-EtherCAT and EtherCAT Core CSR 
	using 'Serial Read(0x03)' command supported by LAN9252/LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9252/LAN9253 Compatible SPI (0x80)
     
    Input : wAddr    -> Address of the register to be read
            *pbyData -> Pointer to the data that is to be read

    Output : None
	
	Note   : In LAN9252 Compatible SPI, all registers are DWORD aligned. Length will be fixed to 4. Hence,
			 there is no separate length argument.
*/

void ECAT_Lan925x_SPIRead(uint16_t wAddr, uint8_t *pbyData, uint8_t wLen)
{
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
    UINT8 u8Tmp = 0;
    
	txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)(wAddr >> 8);
    txData[2] = (uint8_t)wAddr;
    
    _ECAT_ChipSelectEnable();
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy());
    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy());
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
    do
	{
		if (1 == wLen)
		{
			u8Tmp = READ_TERMINATION_BYTE;
		}
		gDrvLan9253UtilObj.spiPlib->spiWriteRead(&u8Tmp, 1, &rxData[0], 1);
		_ECAT_QSPI_SyncWait();
		while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
		{
		}
		*pbyData++ = rxData[0];
	} while (--wLen);
    
    _ECAT_ChipSelectDisable();
}

/* 
    Function: ECAT_Lan925x_SPIFastRead

    This function does Read Access to Non-Ether CAT and Ether CAT Core CSR 
	using 'Fast Read(0x0B)' command supported by LAN9252/LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9252/LAN9253 Compatible SPI (0x80)
     
    Input : wAddr    -> Address of the register to be read
            *pbyData -> Pointer to the data that is to be read

    Output : None

*/

void ECAT_Lan925x_SPIFastRead(uint16_t wAddr, uint8_t *pbyData, uint8_t wLen)
{
	UINT8 byDummy = 0;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
		
	txData[0] = CMD_FAST_READ;
    txData[1] = (uint8_t)(wAddr >> 8);
    txData[2] = (uint8_t)wAddr;	
    
	_ECAT_ChipSelectEnable();
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
	/* Send Transfer length */
    gDrvLan9253UtilObj.spiPlib->spiWrite(&wLen, 1);
    _ECAT_QSPI_SyncWait();
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {
    }
    
	/* For LAN9252 compatible SPI, default number of initial dummy bytes is 1. 
     * No per Byte and per Dword dummies are needed. */
    gDrvLan9253UtilObj.spiPlib->spiWrite(&byDummy, 1);
    _ECAT_QSPI_SyncWait();
	while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    
    do
	{
        gDrvLan9253UtilObj.spiPlib->spiWriteRead(&byDummy, 1, &rxData[0], 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
		*pbyData++ = rxData[0];		
	} while(--wLen);

	_ECAT_ChipSelectDisable();
}

/* 
    Function: ECAT_Lan925x_SPIReadPDRAM

    This function does Read Access to Ether CAT Core Process RAM  using 'Serial Read(0x03)' command 
	supported by LAN9252/LAN9253 Compatible SPI. This function shall be used only when PDI is selected as
	LAN9252 Compatible SPI (0x80)
     
    Input : wAddr    -> Address of the RAM location to be read
            *pbyData -> Pointer to the data that is to be read
			wLen	 -> Number of bytes to be read from PDRAM location

    Output : None

*/

void ECAT_Lan925x_SPIReadPDRAM(UINT8 *pbyData, UINT16 wAddr, UINT16 wLen)
{
	UINT32_VAL uParam32_1;
	UINT8 byStartAlignSize, byEndAlignSize, byDummy = 0;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
    
	/* Address and length */
	uParam32_1.w[0] = wAddr;
	uParam32_1.w[1] = wLen;
	MCHP_ESF_PDI_WRITE(0x0308, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

	/* Read command */
	uParam32_1.Val = 0x80000000;
	MCHP_ESF_PDI_WRITE(0x030c, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

	byStartAlignSize = (wAddr & 3);
	byEndAlignSize = (wLen + byStartAlignSize) & 3;
	if (byEndAlignSize & 3){
		byEndAlignSize = (((byEndAlignSize + 4) & 0xC) - byEndAlignSize);
	}

	/* Read SPI FIFO */
	_ECAT_ChipSelectEnable();
    
    txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x04;
	
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
    while(byStartAlignSize--)
	{
		gDrvLan9253UtilObj.spiPlib->spiRead(&byDummy,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}
    
    while(wLen--)
	{
		gDrvLan9253UtilObj.spiPlib->spiRead(rxData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
        *pbyData++ = rxData[0];
	}

    while(byEndAlignSize--)
	{
    	gDrvLan9253UtilObj.spiPlib->spiRead(&byDummy,1);  
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}
    
	_ECAT_ChipSelectDisable();
        
}

/* 
    Function: ECAT_Lan925x_SPIFastReadPDRAM

    This function does Read Access to Ether CAT Core Process RAM  using 'Fast Read(0x0B)' command 
	supported by LAN9252/LAN9253 Compatible SPI. This function shall be used only when PDI is selected as
	LAN9252/LAN9253 Compatible SPI (0x80)
     
    Input : wAddr    -> Address of the RAM location to be read
            *pbyData -> Pointer to the data that is to be read
			wLen	 -> Number of bytes to be read from PDRAM location

    Output : None
*/

void ECAT_Lan925x_SPIFastReadPDRAM(UINT8 *pbyData, UINT16 wAddr, UINT16 wLen)
{
	UINT32_VAL uParam32_1;
	UINT8 byStartAlignSize, byEndAlignSize, byDummy = 0, byTmp;
	UINT16 wXfrLen = 0, wTotalLen = 0;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};

	byStartAlignSize = (wAddr & 3);
	byEndAlignSize = (wLen & 3) + byStartAlignSize;
	if (byEndAlignSize & 3)
    {
		byEndAlignSize = (((byEndAlignSize + 4) & 0xC) - byEndAlignSize);
	}
	
	wTotalLen = wLen + byStartAlignSize + byEndAlignSize; 
	
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
	uParam32_1.w[0] = wAddr;
	uParam32_1.w[1] = wLen;
	MCHP_ESF_PDI_WRITE(0x0308, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

	/* Read command */
	uParam32_1.Val = 0x80000000;
	MCHP_ESF_PDI_WRITE(0x030c, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

	/* Read SPI FIFO */
	_ECAT_ChipSelectEnable();
	
    txData[0] = CMD_FAST_READ;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x04;
	
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
	/* Send Transfer length */
    byTmp = LOBYTE(wXfrLen);
    gDrvLan9253UtilObj.spiPlib->spiWrite(&byTmp, 1);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    
	if (wLen > ONE_BYTE_MAX_XFR_LEN)    /* Two byte Xfr length */
	{
        byTmp = HIBYTE(wXfrLen);
        gDrvLan9253UtilObj.spiPlib->spiWrite(&byTmp, 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}

    gDrvLan9253UtilObj.spiPlib->spiWrite(&byDummy, 1);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
  	
	while(byStartAlignSize--)
	{
        gDrvLan9253UtilObj.spiPlib->spiRead(&byDummy,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}

	while(wLen--)
	{
        gDrvLan9253UtilObj.spiPlib->spiRead(rxData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
        *pbyData++ = rxData[0];
	}

	while(byEndAlignSize--)
	{
        gDrvLan9253UtilObj.spiPlib->spiRead(&byDummy,1);  
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}

	_ECAT_ChipSelectDisable();    
    
}

/* 
    Function: ECAT_Lan925x_SPIWritePDRAM

    This function does Write Access to Ether CAT Core Process RAM  using 'Serial Write(0x02)' command 
	supported by LAN9252/LAN9253 Compatible SPI. This function shall be used only when PDI is selected as
	LAN9252/LAN9253 Compatible SPI (0x80)
     
    Input : wAddr    -> Address of the RAM location to be written
            *pbyData -> Pointer to the data that is to be written
			wLen	 -> Number of bytes to be written to PDRAM location

    Output : None

*/

void ECAT_Lan925x_SPIWritePDRAM(UINT8 *pbyData, UINT16 wAddr, UINT16 wLen)
{
	UINT32_VAL uParam32_1;
	UINT8 byStartAlignSize, byEndAlignSize, byDummy = 0; 
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
    
	/* Address and length */
	uParam32_1.w[0] = wAddr;
	uParam32_1.w[1] = wLen;
	MCHP_ESF_PDI_WRITE(0x0310, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

	/* write command */
	uParam32_1.Val = 0x80000000;
	MCHP_ESF_PDI_WRITE(0x0314, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
	
	byStartAlignSize = (wAddr & 3);
//	byEndAlignSize = (wLen & 3) + byStartAlignSize;
	// Changed for a bug identified with HBI - Demux BSP
	byEndAlignSize = (wLen + byStartAlignSize) & 3;
	if (byEndAlignSize & 3)
    {
		byEndAlignSize = (((byEndAlignSize + 4) & 0xC) - byEndAlignSize);
	}
	
	/* Writing to FIFO */
	_ECAT_ChipSelectEnable();
	
	txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)0;
    txData[2] = (uint8_t)0x20;

    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {    
    }
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
	while(byStartAlignSize--)
	{
        gDrvLan9253UtilObj.spiPlib->spiWrite(&byDummy,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}
    
    while (wLen--)
	{        
        txData[0] = *pbyData;
		gDrvLan9253UtilObj.spiPlib->spiWrite(txData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
        pbyData++;
	}

    while (byEndAlignSize--)
	{        
        txData[0] = byDummy;
		gDrvLan9253UtilObj.spiPlib->spiWrite(txData,1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}
	
	_ECAT_ChipSelectDisable();    
}
#endif

#ifdef ETHERCAT_SPI_DIRECT_MODE_ACCESS
/* LAN9253 */
/* 
    Function: ECAT_Lan9253_SPIWrite

    This function does Write Access to Non-Ether CAT core CSR, Ether CAT Core CSR and Process RAM
	using 'Serial Write(0x02)' command supported by LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9253 Compatible SPI (0x82)
     
    Input : wAdddr    -> Address of the register to be written
            *pbyData  -> Pointer to the data that is to be written
			dwLength  -> Number of bytes to be written 

    Output : None
	
	Note   : Since now SPI is running at 5MHz, Serial Write is successful without any dummy bytes or wait
			 signal. But, as the SPI clock speed increases, we have to follow either of these. 

*/

void ECAT_Lan9253_SPIWrite(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength)
{	
	uint32_t    dwModLen = 0;
    
#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	BOOL bWaitAck;
#endif
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0}; 
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
        
	/* Core CSR and Process RAM accesses can have any alignment and length */
	if (wAddr < 0x3000)
	{
		/* DWORD Buffering - Applicable for Write only */
		if (dwLength > 1)
		{
			wAddr |= 0xC000; 	/* addr[15:14]=11 */
		}
		else
		{
			/* Do Nothing */
		}
	}
	else
	{
		/* Non Core CSR length will be adjusted if it is not DWORD aligned */
		dwModLen = dwLength % 4;
		if (1 == dwModLen)
		{
			dwLength = dwLength + 3;
		}
		else if (2 == dwModLen)
		{
			dwLength = dwLength + 2;
		}
		else if (3 == dwModLen)
		{
			dwLength = dwLength + 1;
		}
		else
		{
			/* Do nothing if length is 0 since it is DWORD access */
		}
	}

	_ECAT_ChipSelectEnable();
	
#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	/* "Even with a pending write, the host should be
	allowed to assert SCS# and look at WAIT_ACK. The host will then either de-assert SCS# 
	or it will wait for WAIT_ACK inactive before starting the instruction shift." 
	This is to wait for the internal pending write to complete, if any */
	do
	{
 
		bWaitAck = gpio_get_pin_level(SPI_WAITACK);
	} while (bWaitAck != ACK);
#endif 	
	
    txData[0] = CMD_SERIAL_WRITE;
    txData[1] = (uint8_t)(wAddr >> 8);
    txData[2] = (uint8_t)wAddr;
	
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    // Update the EscALEvent value which is required for the state change
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    do
	{
        txData[0] = *pbyData++;
        gDrvLan9253UtilObj.spiPlib->spiWrite(&txData[0],1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	} while (--dwLength);

	_ECAT_ChipSelectDisable();
}

/* 
    Function: ECAT_Lan9253_SPIRead

    This function does Read Access to Non-Ether CAT core CSR, Ether CAT Core CSR and Process RAM
	using 'Serial Read(0x03)' command supported by LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9253 Compatible SPI (0x82)
     
    Input : wAddr    -> Address of the register to be read
            *pbyData -> Pointer to the data that is to be read

    Output : None

*/

void ECAT_Lan9253_SPIRead(uint16_t wAddr, uint8_t *pbydata, uint32_t dwLength)
{	
	uint8_t byTmp = 0;
#if DUMMY_CYCLES_BASED_SPI_DIRECT_MODE
    uint8_t byDummy = 0;
#endif
	UINT32 dwModLen = 0;
#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	BOOL bWaitAck;
#endif
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};

	/* Core CSR and Process RAM accesses can have any alignment and length */
	if (wAddr < 0x3000)
	{
		if (dwLength>1)
		{
			/* Use Auto-increment if number of bytes to read is more than 1 */
			wAddr |= 0x4000;			
		}
	}
	else
	{
		/* Non Core CSR length will be adjusted if it is not DWORD aligned */
		dwModLen = dwLength % 4; 
		if (1 == dwModLen)
		{
			dwLength = dwLength + 3; 
		}
		else if (2 == dwModLen)
		{
			dwLength = dwLength + 2; 
		}
		else if (3 == dwModLen)
		{
			dwLength = dwLength + 1; 
		}
		else 
		{
			/* Do nothing if length is 0 since it is DWORD access */
		}
	}
	_ECAT_ChipSelectEnable();

#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	/* "Even with a pending write, the host should be
	allowed to assert SCS# and look at WAIT_ACK. The host will then either de-assert SCS# 
	or it will wait for WAIT_ACK inactive before starting the instruction shift." 
	This is to wait for the internal pending write to complete, if any */
	do
	{ 
		bWaitAck = gpio_get_pin_level(SPI_WAITACK);
	} while (bWaitAck != ACK);
#endif
	
	txData[0] = CMD_SERIAL_READ;
    txData[1] = (uint8_t)(wAddr >> 8);
    txData[2] = (uint8_t)wAddr;
    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9253UtilObj.spiPlib->spiWrite(txData, 3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {    
    }
    
	do
	{
#ifdef DUMMY_CYCLES_BASED_SPI_DIRECT_MODE
		/* Dummy before each byte */ 
		{
            gDrvLan9253UtilObj.spiPlib->spiWriteRead(&byDummy, 1, &byTmp, 1);
            _ECAT_QSPI_SyncWait();    
            while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
            {
            }
		}
#elif WAIT_ACK_BASED_SPI_DIRECT_MODE
		do
		{/* After address is given, wait_ack will become low. Wait till WAIT_ACK signal becomes high */
			bWaitAck = gpio_get_pin_level(SPI_WAITACK);
		} while (bWaitAck != ACK);		
#else 
		/* Wait for specified time. No dummy or WAIT_ACK needed in this case */
#endif 				
		if (1 == dwLength)
		{
			byTmp = READ_TERMINATION_BYTE;
		}
        else
        {
            byTmp = 0;
        }
        gDrvLan9253UtilObj.spiPlib->spiWriteRead(&byTmp, 1, &rxData[0], 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {
        }
		*pbydata++ = rxData[0];
	} while (--dwLength);

	_ECAT_ChipSelectDisable();	
}

/* 
    Function: ECAT_Lan9253_SPIFastRead

    This function does Read Access to Non-Ether CAT core CSR, Ether CAT Core CSR and Process RAM
	using 'Fast Read(0x0B)' command supported by LAN9253 Compatible SPI. This function shall be used
	only when PDI is selected as LAN9253 Compatible SPI (0x82)
     
    Input : wAddr    -> Address of the register to be read
            *pbyData -> Pointer to the data that is to be read
			dwLength -> Number of bytes to be read 

	Note   : Since now SPI is running at 5MHz, Fast Read is successful without any dummy bytes or wait
	signal. But, as the SPI clock speed increases, we have to follow either of these.
		
*/

void ECAT_Lan9253_SPIFastRead(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength)
{
 		
	UINT8 byDummy = 0, byStartAlignSize = 0, byItr, byTmp;
	UINT16 wXfrLen = 0;
	UINT32 dwModLen = 0;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	BOOL bWaitAck; 
#endif
	
	/* Core CSR and Process RAM accesses can have any alignment and length */
	if (wAddr < 0x3000)
	{
		/* Use Auto-increment for incrementing byte address*/
		wAddr |= 0x4000;			
		
		/* To calculate initial number of dummy bytes which is based on starting address */
		byStartAlignSize = (wAddr & 3); 
	}
	else 
	{  	/* System CSRs are DWORD aligned and are a DWORD in length. Non- DWORD aligned / non-DWORD length access 
	is not supported. */
		dwModLen = dwLength % 4;
		if (1 == dwModLen)
		{
			dwLength = dwLength + 3;
		}
		else if (2 == dwModLen)
		{
			dwLength = dwLength + 2;
		}
		else if (3 == dwModLen)
		{
			dwLength = dwLength + 1;
		}
		else
		{
			/* Do nothing is length is 0 */
		}		
	}

	/* From DOS, "For the one byte transfer length format,	bit 7 is low and bits 6-0 specify the 
	length up to 127 bytes. For the two byte transfer length format, bit 7 of the first byte
	is high and bits 6-0 specify the lower 7 bits of the length. Bits 6-0 of the of the second byte 
	field specify the upper 7 bits of the length with a maximum transfer length of 16,383 bytes (16K-1)" */ 
	if (dwLength <= ONE_BYTE_MAX_XFR_LEN)
	{
		wXfrLen = dwLength; 
	}
	else  
	{
		wXfrLen = (dwLength & 0x7F) | 0x80;
		wXfrLen |= ((dwLength & 0x3F80) << 1);
	}	
	_ECAT_ChipSelectEnable();
	
#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	/* "Even with a pending write, the host should be allowed to assert SCS#
    and look at WAIT_ACK. The host will then either deassert SCS# 
	or it will wait for WAIT_ACK inactive before starting the instruction shift." 
	This is to wait for the internal pending write to complete, if any */
	do
	{ 
		bWaitAck = gpio_get_pin_level(SPI_WAITACK);
	} while (bWaitAck != ACK);
#elif DUMMY_CYCLES_BASED_SPI_DIRECT_MODE
	/* Do Nothing */ 
#else 
	/* Add delay if needed */	
#endif
    
    txData[0] = CMD_FAST_READ;
    txData[1] = (uint8_t)(wAddr >> 8);
    txData[2] = (uint8_t)wAddr;
    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }

    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
	
    EscALEvent.Byte[0] = rxData[1];
    EscALEvent.Byte[1] = rxData[2];
    
    /* Send Transfer length */
    txData[0] = (uint8_t)(LOBYTE(wXfrLen));
    gDrvLan9253UtilObj.spiPlib->spiWriteRead(&txData[0], 1, &byTmp, 1);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    
    if (dwLength > ONE_BYTE_MAX_XFR_LEN)    /* Two byte Xfr length */
    {
        txData[0] = (uint8_t)(HIBYTE(wXfrLen));
        gDrvLan9253UtilObj.spiPlib->spiWriteRead(&txData[0], 1, &byTmp, 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}
#ifdef WAIT_ACK_BASED_SPI_DIRECT_MODE
	do
	{		
		bWaitAck = gpio_get_pin_level(SPI_WAITACK);
	} while (bWaitAck != ACK);
#endif 
	
	/* 1 default dummy + extra dummies based on address that needs to be accessed. 
	   "For Fast reads with Non DWORD aligned address, Dummy data will be sent 
       before the actual data. 
	   So to read 2001 you will get a dummy byte and then data in address 2001. 
	   SW needs to handle dummy data in case of non DWORD address reads" */
	for (byItr = 0; byItr <= byStartAlignSize; byItr++) 
	{
        gDrvLan9253UtilObj.spiPlib->spiWriteRead(&byDummy, 1, &byTmp, 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	}
    do
	{
        gDrvLan9253UtilObj.spiPlib->spiWriteRead(&byDummy, 1, &rxData[0], 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {
        }
		*pbyData++ = rxData[0];
		/* Poll for wait ack or add dummy after each byte if needed  (based on SETCFG) */
	} while (--wXfrLen);
	
	_ECAT_ChipSelectDisable();

}
#endif

#ifdef ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
/* 
    Function: ECAT_Lan9253_Beckhoff_SPIWrite

    This function does Write Access to Non-Ether CAT Core CSR, Ether CAT Core CSR and 
	Process RAM using Write(0x04) command supported by Beckhoff SPI. This function shall be used
	only when PDI is selected as Beckhoff SPI (0x05)
     
    Input : wAddr    -> Address of the register/RAM location to be written
            *pbyData -> Pointer to the data that is to be written
			dwLength -> Number of bytes to be written 

    Output : None
*/
	
void ECAT_Lan9253_Beckhoff_SPIWrite(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength)
{
	UINT8 byBeckhoffCmd = ESC_WR;
    uint8_t  txData[DWORD_LENGTH] = {0,0,0,0};
    uint8_t  rxData[DWORD_LENGTH] = {0,0,0,0};
	
	/* Non Ether CAT Core CSRs are always DWORD aligned and should be accessed by DWORD length */
	if (wAddr >= 0x3000)
	{
		dwLength = 4; 
	}
	
	_ECAT_ChipSelectEnable();
	
	while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
	
    txData[0] = (wAddr & 0x1FE0) >> 5;
    txData[1] = ((wAddr & 0x001F) << 3) | 0x06;
    txData[2] = (HIBYTE(wAddr) & 0xE0) | (byBeckhoffCmd << 2);
    
    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
	_ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    EscALEvent.Byte[0] = rxData[0];
    EscALEvent.Byte[1] = rxData[1];
    
    do
	{
        txData[0] = *pbyData++;
        gDrvLan9253UtilObj.spiPlib->spiWrite(&txData[0], 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
	} while (--dwLength);
	
	_ECAT_ChipSelectDisable();
}

/* 
    Function: ECAT_Lan9253_Beckhoff_SPIRead

    This function does Read Access to Non-Ether CAT Core CSR, Ether CAT Core CSR and 
	Process RAM using 'Read(0x02)' command supported by Beckhoff SPI.
	This function shall be used only when PDI is selected as Beckhoff SPI (0x05)
     
    Input : wAddr    -> Address of the register/RAM location to be written
            *pbyData -> Pointer to the data that is to be written
			dwLength -> Number of bytes to be written 

    Output : None
*/

void ECAT_Lan9253_Beckhoff_SPIRead(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength)
{
	UINT8 byDummy = 0, byBeckhoffCmd = ESC_RD_WAIT_STATE, byTmp = 0;
    uint8_t  txData[DWORD_LENGTH]={0,0,0,0};
	uint8_t  rxData[DWORD_LENGTH]={0,0,0,0};
    
	/* Non Ether CAT Core CSRs are always DWORD aligned and should be accessed by DWORD length */
	if (wAddr >= 0x3000)
	{
		dwLength = 4;
	}
	
	_ECAT_ChipSelectEnable();
	
	/* AL Event register bits will be outputted on SPI line - 0x220, 0x221 and 0x222 */
	while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
	
    txData[0] = (wAddr & 0x1FE0) >> 5;
    txData[1] = ((wAddr & 0x001F) << 3) | 0x06;
    txData[2] = (HIBYTE(wAddr) & 0xE0) | (byBeckhoffCmd << 2);
    
    gDrvLan9253UtilObj.spiPlib->spiWriteRead(txData, 3,rxData,3);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
    EscALEvent.Byte[0] = rxData[0];
    EscALEvent.Byte[1] = rxData[1];
        	
	/* Master can either wait for 240ns time or use Wait state byte after last byte of addr/cmd and 
	   before initiating the clock for data phase. */
	
    byDummy = WAIT_STATE_BYTE;
    gDrvLan9253UtilObj.spiPlib->spiWrite(&byDummy, 1);
    _ECAT_QSPI_SyncWait();    
    while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
    {        
    }
	
	do
	{
        if (1 == dwLength)
		{
			byTmp = READ_TERMINATION_BYTE;
		}
        gDrvLan9253UtilObj.spiPlib->spiWriteRead(&byTmp, 1, &byDummy, 1);
        _ECAT_QSPI_SyncWait();    
        while(gDrvLan9253UtilObj.spiPlib->spiIsBusy())
        {            
        }
		*pbyData++ = byDummy;
	} while (--dwLength);
	
	_ECAT_ChipSelectDisable();
}
#endif

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
    gDrvLan9253UtilObj.timerPlib->timerCallbackSet(_ECAT_TimerInterruptCallback,(uintptr_t) NULL);
    gDrvLan9253UtilObj.timerPlib->timerStart();
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
/* Error Select PIN set. LED status is OFF */
<#if DRV_LAN9253_ERROR_SELECT_PORT_PLIB == "GPIO">
		GPIO_PinSet((GPIO_PIN)${DRV_LAN9253_ERROR_SELECT_PIN});
<#elseif DRV_LAN9253_ERROR_SELECT_PORT_PLIB == "PIO">
		PIO_PinSet((PIO_PIN)${DRV_LAN9253_ERROR_SELECT_PIN});
<#elseif DRV_LAN9253_ERROR_SELECT_PORT_PLIB == "PORT">
		PORT_PinSet((PORT_PIN)${DRV_LAN9253_ERROR_SELECT_PIN});
</#if>
	}
	else
	{
/* Error Select PIN Clear . LED status is ON*/
<#if DRV_LAN9253_ERROR_SELECT_PORT_PLIB == "GPIO">
		GPIO_PinClear((GPIO_PIN)${DRV_LAN9253_ERROR_SELECT_PIN});
<#elseif DRV_LAN9253_ERROR_SELECT_PORT_PLIB == "PIO">
		PIO_PinClear((PIO_PIN)${DRV_LAN9253_ERROR_SELECT_PIN});
<#elseif DRV_LAN9253_ERROR_SELECT_PORT_PLIB == "PORT">
		PORT_PinClear((PORT_PIN)${DRV_LAN9253_ERROR_SELECT_PIN});
</#if>
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
	LAN9253_Init();
}

void ECAT_Util_Initialize(
    const unsigned short int drvIndex,
    const void * const init
)
{
    DRV_LAN9253_UTIL_INIT* lan9253UtilInit  = (DRV_LAN9253_UTIL_INIT *)init;

   
    gDrvLan9253UtilObj.spiTransferStatus    = DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
    gDrvLan9253UtilObj.spiPlib              = lan9253UtilInit->spiPlib;
	
	// Timer PLIB initialization for LAN9253 driver 
    gDrvLan9253UtilObj.timerPlib            = lan9253UtilInit->timerPlib;
    
}

static void ECAT_QSPI_Callback(uintptr_t context)
{
    // transmission completed
    gDrvLan9253UtilObj.spiTransferStatus = DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_COMPLETED;
}

static void ECAT_QSPI_TransmissionFlagClear(void)
{
    // clear transmission flag to false
    gDrvLan9253UtilObj.spiTransferStatus = DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_BUSY;
}

static bool ECAT_QSPI_IsTransmissionBusy(void)
{
    return (gDrvLan9253UtilObj.spiTransferStatus == DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_BUSY);
}

static void ECAT_QSPI_EventHandlerSet(void)
{
    gDrvLan9253UtilObj.spiPlib->spiCallbackRegister(ECAT_QSPI_Callback,0);
}
