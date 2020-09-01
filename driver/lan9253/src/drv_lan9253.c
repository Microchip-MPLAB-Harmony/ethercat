/*******************************************************************************
 LAN9253 driver source file for Microchip EtherCAT
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9253.c

  Summary
    This file  contains the functional implementation of LAN9252 Hardware 
    Abstraction Layer

  Description
    This file defines enabled Sync Manager & Application Layer Event Requests.
	Added ISR call backs related to Sync Manager & AL Event Request.

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

#include <stdio.h>
#include "ecat_def.h"
#include "ecatslv.h"
#define _LAN9253_HW_ 1
#include "drv_lan9253.h"
#undef _LAN9253_
#define _LAN9253_ 0
#include "ecatappl.h"


BOOL gbALEvtOpEnabled = FALSE; 

/*******************************************************************************
    Function:
        UINT8 LAN9252_Init(void)

    Summary:
        This function initializes LAN9252.

    Description:
  *****************************************************************************/
UINT8 LAN9253_Init(void)
{
	UINT32 intMask;
	UINT32 data=0;

#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS
	do
	{
		MCHP_ESF_IS_PDI_FUNCTIONAL((uint8_t *)&data);
	} while (0x87654321 != data);
	
	// Disable interrupt Interrupt Enable register -->
	// Write 0x5c - 0x00000001
	data = 0x00000000;
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&data, DWORD_LENGTH);
	
	do {
		intMask = 0x93;
		HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

		intMask = 0;
		HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
	} while (intMask != 0x93);
	
	/* Read 0x150 register to check if AL Event output is enabled */
	intMask = 0; 
	HW_EscReadDWord(intMask, ESC_PDI_CONFIG_OFFSET);
	
    if (intMask & AL_EVENT_ENABLE)
    {
		gbALEvtOpEnabled = TRUE; 
    }
    	 
	intMask = 0;
	HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);	
	
	// IRQ enable,IRQ polarity, IRQ buffer type in Interrupt Configuration
	// register. Write 0x54 - 0x00000101
	data = 0x00000101;
		
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&data, DWORD_LENGTH);	
	
	// Write in Interrupt Enable register -->
	// Write 0x5c - 0x00000001
	data = 0x00000001;
		
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&data, DWORD_LENGTH);
		
	// Read Interrupt Status register
	MCHP_ESF_PDI_READ(LAN925x_CSR_INT_STS, (uint8_t*)&data, DWORD_LENGTH);
	
#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

	do
	{
		MCHP_ESF_PDI_READ(LAN925x_BYTE_ORDER_REG, (uint8_t*)&data, DWORD_LENGTH);
	} while (0x87654321 != data);
	
	data = 0x00000000;
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&data, DWORD_LENGTH);

	do {
		intMask = 0x93;
		HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

		intMask = 0;
		HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
	} while (intMask != 0x93);

	/* Read 0x150 register to check if AL Event output is enabled */
	intMask = 0;
	HW_EscReadDWord(intMask, ESC_PDI_CONFIG_OFFSET);
	
	if (intMask & AL_EVENT_ENABLE)
	{
		gbALEvtOpEnabled = TRUE;
	}
	
	intMask = 0;
	HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
	
	data = 0x00000101;
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&data, DWORD_LENGTH);	
	
	data = 0x00000001;
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&data, DWORD_LENGTH);
	
	MCHP_ESF_PDI_READ(LAN925x_CSR_INT_STS, (uint8_t*)&data, DWORD_LENGTH);

#else

	do
	{
		MCHP_ESF_IS_PDI_FUNCTIONAL((uint8_t *)&data);
	} while (0x87654321 != data);

	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&data, DWORD_LENGTH);

	do
    {
		intMask = 0x93;
		HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

		intMask = 0;
		HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
	} while (intMask != 0x93);

	/* Read 0x150 register to check if AL Event output is enabled */
	intMask = 0;
	HW_EscReadDWord(intMask, ESC_PDI_CONFIG_OFFSET);

	if (intMask & AL_EVENT_ENABLE)
	{
		gbALEvtOpEnabled = TRUE;
	}

	intMask = 0;
	HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

	data = 0x00000101;
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&data, DWORD_LENGTH);

	data = 0x00000001;
	MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&data, DWORD_LENGTH);

	MCHP_ESF_PDI_FASTREAD(LAN925x_CSR_INT_STS, (uint8_t*)&data, DWORD_LENGTH);

#endif 	

#ifdef DC_SUPPORTED
	PDI_Init_SYNC_Interrupts();
#endif

	PDI_Timer_Interrupt();
	PDI_IRQ_Interrupt();

	return 0;
}

/*******************************************************************************
Function:
    void HW_Release(void)

Summary:
    This function shall be implemented if hardware resources need to be release
            when the sample application stops

Description:
  *****************************************************************************/
void HW_Release(void)
{
}

/*******************************************************************************
Function:
    UINT16 HW_GetALEventRegister(void)

Summary:
    This function gets the current content of ALEvent register.

Description:
    Returns first two Bytes of ALEvent register (0x220)
  *****************************************************************************/
UINT16 HW_GetALEventRegister(void)
{
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS 	
    UINT32_VAL uParam32_1;

    uParam32_1.v[0] = 0x20;
    uParam32_1.v[1] = 0x02;
    uParam32_1.v[2] = 4;
    uParam32_1.v[3] = 0xC0;

    PDI_Disable_Global_Interrupt();

    MCHP_ESF_PDI_WRITE(0x0304, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);	  
    MCHP_ESF_PDI_FASTREAD(0x0300, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

    PDI_Restore_Global_Interrupt();

    if (gbALEvtOpEnabled)
    {
        return EscALEvent.Word;		  
    }
    else
    {
        return uParam32_1.w[0];	  
    }

#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

    UINT32_VAL uParam32_1;

    PDI_Disable_Global_Interrupt();

    MCHP_ESF_PDI_READ(ESC_AL_EVENT_OFFSET, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

    PDI_Restore_Global_Interrupt();

    return EscALEvent.Word;
	  
#else

    UINT32_VAL uParam32_1;

    PDI_Disable_Global_Interrupt();

    MCHP_ESF_PDI_FASTREAD(ESC_AL_EVENT_OFFSET, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

    PDI_Restore_Global_Interrupt();

    if (gbALEvtOpEnabled)
    {
        return EscALEvent.Word;
    }
    else
    {
        return uParam32_1.w[0];
    }
	
#endif 
}

/*******************************************************************************
    Function:
        UINT16 HW_GetALEventRegister_Isr(void)

    Summary:
        The SPI PDI requires an extra ESC read access functions from interrupts service routines.
                The behaviour is equal to "HW_GetALEventRegister()"

    Description:
        Returns	first two Bytes of ALEvent register (0x220)
  *****************************************************************************/
UINT16 HW_GetALEventRegister_Isr(void)
{
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS 
    UINT32_VAL uParam32_1;

    uParam32_1.v[0] = 0x20;
    uParam32_1.v[1] = 0x02;
    uParam32_1.v[2] = 2;
    uParam32_1.v[3] = 0xC0;

    MCHP_ESF_PDI_WRITE(0x0304, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
    #if 0
    do
    {
        MCHP_ESF_PDI_FASTREAD(0x0304, (uint8_t*)&param32_1.Val, DWORD_LENGTH);

    }while(param32_1.v[3] & 0x80);
    #endif
    //delay_us(5);
    MCHP_ESF_PDI_FASTREAD(0x0300, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

    if (gbALEvtOpEnabled)
    {
        return EscALEvent.Word;
    }
    else
    {
        return uParam32_1.w[0];
    }
	  
#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
    UINT32_VAL uParam32_1;

    MCHP_ESF_PDI_READ(ESC_AL_EVENT_OFFSET, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

    return EscALEvent.Word;
	  
#else 
    UINT32_VAL uParam32_1;

    MCHP_ESF_PDI_FASTREAD(ESC_AL_EVENT_OFFSET, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

    if (gbALEvtOpEnabled)
    {
        return EscALEvent.Word;
    }
    else
    {
        return uParam32_1.w[0];
    }

#endif
}


/*******************************************************************************
Function:
    void HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )

Summary:
    This function operates the SPI read access to the EtherCAT ASIC.

Description:
    Input param:
     pData		- Pointer to a byte array which holds data to write or saves read data.
     Address	- EtherCAT ASIC address ( upper limit is 0x1FFF )	for access.
     Len		- Access size in Bytes.
*****************************************************************************/
void HW_EscRead(MEM_ADDR *pmData, UINT16 wAddress, UINT16 wLen) 
{
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0) 
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1) {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2 ) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
	
		uParam32_1.v[0] = (UINT8)wAddress;
		uParam32_1.v[1] = (UINT8)(wAddress >> 8);
		uParam32_1.v[2] = byValidDataLen;
		uParam32_1.v[3] = 0xC0;
	
		PDI_Disable_Global_Interrupt();
		MCHP_ESF_PDI_WRITE(0x0304, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		MCHP_ESF_PDI_FASTREAD(0x0300, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		PDI_Restore_Global_Interrupt();
	
		for (byItr = 0; byItr < byValidDataLen; byItr++)
		*pbyData++ = uParam32_1.v[byItr];

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1) 
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2 ) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
	
		PDI_Disable_Global_Interrupt();
		MCHP_ESF_PDI_READ(wAddress, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		PDI_Restore_Global_Interrupt();
	
		for (byItr = 0; byItr < byValidDataLen; byItr++)
		*pbyData++ = uParam32_1.v[byItr];

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#else

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2 ) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
	
		PDI_Disable_Global_Interrupt();
		MCHP_ESF_PDI_FASTREAD(wAddress, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		PDI_Restore_Global_Interrupt();
	
		for (byItr = 0; byItr < byValidDataLen; byItr++)
		*pbyData++ = uParam32_1.v[byItr];

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#endif
}

/*******************************************************************************
    Function:
        void EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )

    Summary:
        The SPI PDI requires an extra ESC read access functions from interrupts service routines.
                The behaviour is equal to "HW_EscRead()"

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves read data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/

void EscRead(MEM_ADDR *pmData, UINT16 wAddress, UINT16 wLen)
{
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS
	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2 ) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
	
		uParam32_1.v[0] = (UINT8)wAddress;
		uParam32_1.v[1] = (UINT8)(wAddress >> 8);
		uParam32_1.v[2] = byValidDataLen;
		uParam32_1.v[3] = 0xC0;
	
		MCHP_ESF_PDI_WRITE(0x0304, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		MCHP_ESF_PDI_FASTREAD(0x0300, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
	
		for (byItr = 0; byItr < byValidDataLen; byItr++)
		*pbyData++ = uParam32_1.v[byItr];

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}
#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2 ) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
	
		MCHP_ESF_PDI_READ(wAddress, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
	
		for (byItr = 0; byItr < byValidDataLen; byItr++)
		*pbyData++ = uParam32_1.v[byItr];

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#else

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
		
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2 ) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
		
		MCHP_ESF_PDI_FASTREAD(wAddress, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		
		for (byItr = 0; byItr < byValidDataLen; byItr++)
		*pbyData++ = uParam32_1.v[byItr];

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#endif
}

void HW_EscReadIsr(MEM_ADDR *pData, UINT16 wAddress, UINT16 wLen)
{
	UINT8 *pTmpData = (UINT8 *)pData;
  
  if(wAddress>0xfff)
  {
	  /* send the address and command to the ESC */
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS	  
	  MCHP_ESF_PDI_FASTREAD_PDRAM(pTmpData, wAddress, wLen);
	  
#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS 
	  MCHP_ESF_PDI_READ (wAddress, pTmpData, wLen);
#else
      MCHP_ESF_PDI_FASTREAD (wAddress, pTmpData, wLen);
#endif 	  	  	  
  }
  else
  {
	  EscRead(pData,wAddress, wLen);
  }
}

/*******************************************************************************
    Function:
        void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )

    Summary:
        This function operates the SPI write access to the EtherCAT ASIC.

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves write data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/
void HW_EscWrite(MEM_ADDR *pmData, UINT16 wAddress, UINT16 wLen)
{
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}

		for (byItr = 0; byItr < byValidDataLen; byItr++)
		uParam32_1.v[byItr] = *pbyData++;
	
		PDI_Disable_Global_Interrupt();
		MCHP_ESF_PDI_WRITE(0x0300, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);

		uParam32_1.v[0] = (UINT8)wAddress;
		uParam32_1.v[1] = (UINT8)(wAddress >> 8);
		uParam32_1.v[2] = byValidDataLen;
		uParam32_1.v[3] = 0x80;

		MCHP_ESF_PDI_WRITE(0x0304, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		PDI_Restore_Global_Interrupt();

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}

		for (byItr = 0; byItr < byValidDataLen; byItr++)
		uParam32_1.v[byItr] = *pbyData++;
	
		PDI_Disable_Global_Interrupt();
		MCHP_ESF_PDI_WRITE(wAddress, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		PDI_Restore_Global_Interrupt();

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#else
	
	UINT32_VAL uParam32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
	
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}

		for (byItr = 0; byItr < byValidDataLen; byItr++)
		uParam32_1.v[byItr] = *pbyData++;
	
		PDI_Disable_Global_Interrupt();
		MCHP_ESF_PDI_WRITE(wAddress, (uint8_t*)&uParam32_1.Val, DWORD_LENGTH);
		PDI_Restore_Global_Interrupt();

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}

#endif
}

/*******************************************************************************
    Function:
        void HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )

    Summary:
        The SPI PDI requires an extra ESC write access functions from interrupts service routines.
                The behaviour is equal to "HW_EscWrite()"

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves write data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/
void EscWrite(MEM_ADDR *pmData, UINT16 wAddress, UINT16 wLen) 
{
#if ETHERCAT_SPI_INDIRECT_MODE_ACCESS

	UINT32_VAL param32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
		
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}

		for (byItr = 0; byItr < byValidDataLen; byItr++)
		param32_1.v[byItr] = *pbyData++;
		
		MCHP_ESF_PDI_WRITE(0x0300, (uint8_t*)&param32_1.Val, DWORD_LENGTH);

		param32_1.v[0] = (UINT8)wAddress;
		param32_1.v[1] = (UINT8)(wAddress >> 8);
		param32_1.v[2] = byValidDataLen;
		param32_1.v[3] = 0x80;

		MCHP_ESF_PDI_WRITE(0x0304, (uint8_t*)&param32_1.Val, DWORD_LENGTH);

		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}
	
#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

	UINT32_VAL param32_1;
	UINT8 byValidDataLen, byItr;
	UINT8 *pbyData = (UINT8 *)pmData;

	while (wLen > 0)
    {
		byValidDataLen = (wLen > 4) ? 4 : wLen;
		
		if (wAddress & 1)
        {
			byValidDataLen = 1;
		}
		else if (wAddress & 2)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}
		else if (byValidDataLen < 4)
		{
			byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
		}

		for (byItr = 0; byItr < byValidDataLen; byItr++)
		param32_1.v[byItr] = *pbyData++;
		
		MCHP_ESF_PDI_WRITE(wAddress, (uint8_t*)&param32_1.Val, DWORD_LENGTH);
		
		wAddress += byValidDataLen;
		wLen -= byValidDataLen;
	}
	
#else 
		UINT32_VAL param32_1;
		UINT8 byValidDataLen, byItr;
		UINT8 *pbyData = (UINT8 *)pmData;

		while (wLen > 0)
        {
			byValidDataLen = (wLen > 4) ? 4 : wLen;
			
			if (wAddress & 1)
            {
				byValidDataLen = 1;
			}
			else if (wAddress & 2)
			{
				byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
			}
			else if (byValidDataLen < 4)
			{
				byValidDataLen = (byValidDataLen >= 2) ? 2 : 1;
			}

			for (byItr = 0; byItr < byValidDataLen; byItr++) 
			param32_1.v[byItr] = *pbyData++;
			
			MCHP_ESF_PDI_WRITE(wAddress, (uint8_t*)&param32_1.Val, DWORD_LENGTH);
			
			wAddress += byValidDataLen;
			wLen -= byValidDataLen;
		}

#endif
}

void HW_EscWriteIsr(MEM_ADDR *pbyData, UINT16 wAddress, UINT16 wLen)
{
	UINT8 *pTmpData = (UINT8 *)pbyData;

    if(wAddress>0xfff)
    {
        /* start transmission */
  #if ETHERCAT_SPI_INDIRECT_MODE_ACCESS	  
        MCHP_ESF_PDI_WRITE_PDRAM(pTmpData, wAddress, wLen);
  #else
        MCHP_ESF_PDI_WRITE (wAddress, pTmpData, wLen);
  #endif
    }
    else
    {
        EscWrite(pbyData, wAddress, wLen);
    }
}

void HW_EscWriteMailbox(MEM_ADDR *pData, UINT16 Address, UINT16 Len) 
{

	HW_EscWrite(pData, Address, Len);
}

void HW_EscReadMailbox(MEM_ADDR *pData, UINT16 Address, UINT16 Len) 
{
	/* send the address and command to the ESC */
	//ReadPdRam(pTmpData, Address, Len);
	HW_EscRead(pData, Address, Len);
}
