/*******************************************************************************
 LAN9252 driver source file for Microchip EtherCAT
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9252.c

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
#define _LAN9252_ 1
#include "drv_lan9252.h"
#undef _LAN9252_
#define _LAN9252_ 0
#include "ecatappl.h"


#define LAN9252_BYTE_ORDER_REG 0x64
#define LAN9252_CSR_INT_CONF 0x54
#define LAN9252_CSR_INT_EN 0x5C
#define LAN9252_CSR_INT_STS 0x58

/*******************************************************************************
    Function:
        UINT8 LAN9252_Init(void)

    Summary:
        This function initializes LAN9252.

    Description:
  *****************************************************************************/
UINT8 LAN9252_Init(void)
{
	UINT32 intMask;
	UINT32 data=0;

	// Read BYTE-ORDER register 0x64.
	do 
	{
		SPIRead(LAN9252_BYTE_ORDER_REG, (uint8_t*)&data);
	} while (0x87654321 != data);

	// Disable interrupt Interrupt Enable register -->
	// Write 0x5c - 0x00000001
	data = 0x00000000;
	SPIWrite(LAN9252_CSR_INT_EN, (uint8_t*)&data);
  
	do {
		intMask = 0x93;
		HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

		intMask = 0;
		HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
	} while (intMask != 0x93);

	intMask = 0;
	HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

	// IRQ enable,IRQ polarity, IRQ buffer type in Interrupt Configuration
	// register. Write 0x54 - 0x00000101
	data = 0x00000101;
	SPIWrite(LAN9252_CSR_INT_CONF, (uint8_t*)&data);


	// Write in Interrupt Enable register -->
	// Write 0x5c - 0x00000001
	data = 0x00000001;
	SPIWrite(LAN9252_CSR_INT_EN, (uint8_t*)&data);

	// Read Interrupt Status register
	SPIRead(LAN9252_CSR_INT_STS, (uint8_t*)&data);

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
	UINT32_VAL param32_1;

	param32_1.v[0] = 0x20;
	param32_1.v[1] = 0x02;
	param32_1.v[2] = 4;
	param32_1.v[3] = 0xC0;

	PDI_Disable_Global_Interrupt();
	SPIWrite(0x0304, (uint8_t*)&param32_1.Val);
#if 0
	do {
	  SPIRead(0x0304, (uint8_t*)&param32_1.Val);
	} while (param32_1.v[3] & 0x80);
#endif
	//delay_us(5);
	SPIRead(0x0300, (uint8_t*)&param32_1.Val);

	PDI_Restore_Global_Interrupt();

	return param32_1.w[0];
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
	UINT32_VAL param32_1;

	param32_1.v[0] = 0x20;
	param32_1.v[1] = 0x02;
	param32_1.v[2] = 2;
	param32_1.v[3] = 0xC0;

	SPIWrite(0x0304, (uint8_t*)&param32_1.Val);
#if 0
	do
	{
	  SPIRead(0x0304, (uint8_t*)&param32_1.Val);

	}while(param32_1.v[3] & 0x80);
#endif
	//delay_us(5);
	SPIRead(0x0300, (uint8_t*)&param32_1.Val);

	return param32_1.w[0];
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
void HW_EscRead(MEM_ADDR *pmData, UINT16 Address, UINT16 Len) 
{
	UINT32_VAL param32_1;
	UINT8 validDataLen, j;
	UINT8 *pData = (UINT8 *)pmData;

	while (Len > 0) {
		validDataLen = (Len > 4) ? 4 : Len;
		
		if (Address & 1) {
			validDataLen = 1;
		}
		else if (Address & 2)
		{
			validDataLen = (validDataLen >= 2 ) ? 2 : 1;
		}
		else if (validDataLen < 4)
		{
			validDataLen = (validDataLen >= 2) ? 2 : 1;
		}
		
		param32_1.v[0] = (UINT8)Address;
		param32_1.v[1] = (UINT8)(Address >> 8);
		param32_1.v[2] = validDataLen;
		param32_1.v[3] = 0xC0;
		
		PDI_Disable_Global_Interrupt();
		SPIWrite(0x0304, (uint8_t*)&param32_1.Val);
		SPIRead(0x0300, (uint8_t*)&param32_1.Val);
		PDI_Restore_Global_Interrupt();
		
		for (j = 0; j < validDataLen; j++)
		*pData++ = param32_1.v[j];

		Address += validDataLen;
		Len -= validDataLen;
	}
}

/*******************************************************************************
    Function:
        void HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )

    Summary:
        The SPI PDI requires an extra ESC read access functions from interrupts service routines.
                The behaviour is equal to "HW_EscRead()"

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves read data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/

void EscRead(MEM_ADDR *pmData, UINT16 Address, UINT16 Len)
{
	UINT32_VAL param32_1;
	UINT8 validDataLen, j;
	UINT8 *pData = (UINT8 *)pmData;

	while (Len > 0) {
		validDataLen = (Len > 4) ? 4 : Len;
		
		if (Address & 1) {
			validDataLen = 1;
		}
		else if (Address & 2)
		{
			validDataLen = (validDataLen >= 2 ) ? 2 : 1;
		}
		else if (validDataLen < 4)
		{
			validDataLen = (validDataLen >= 2) ? 2 : 1;
		}
		
		param32_1.v[0] = (UINT8)Address;
		param32_1.v[1] = (UINT8)(Address >> 8);
		param32_1.v[2] = validDataLen;
		param32_1.v[3] = 0xC0;
		
		SPIWrite(0x0304, (uint8_t*)&param32_1.Val);
		SPIRead(0x0300, (uint8_t*)&param32_1.Val);
		
		for (j = 0; j < validDataLen; j++)
		*pData++ = param32_1.v[j];

		Address += validDataLen;
		Len -= validDataLen;
	}
}

void HW_EscReadIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
	UINT8 *pTmpData = (UINT8 *)pData;
	if(Address>0xfff)
	{
	  /* send the address and command to the ESC */
	  ReadPdRam(pTmpData, Address, Len);
	}
	else
	{
	  EscRead(pData,Address, Len);
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
void HW_EscWrite(MEM_ADDR *pmData, UINT16 Address, UINT16 Len)
{
	UINT32_VAL param32_1;
	UINT8 validDataLen, j;
	UINT8 *pData = (UINT8 *)pmData;

	while (Len > 0) {
		validDataLen = (Len > 4) ? 4 : Len;
		
		if (Address & 1) {
			validDataLen = 1;
		}
		else if (Address & 2)
		{
			validDataLen = (validDataLen >= 2) ? 2 : 1;
		}
		else if (validDataLen < 4)
		{
			validDataLen = (validDataLen >= 2) ? 2 : 1;
		}

		for (j = 0; j < validDataLen; j++)
		param32_1.v[j] = *pData++;
		
		PDI_Disable_Global_Interrupt();
		SPIWrite(0x0300, (uint8_t*)&param32_1.Val);

		param32_1.v[0] = (UINT8)Address;
		param32_1.v[1] = (UINT8)(Address >> 8);
		param32_1.v[2] = validDataLen;
		param32_1.v[3] = 0x80;

		SPIWrite(0x0304, (uint8_t*)&param32_1.Val);
		PDI_Restore_Global_Interrupt();

		Address += validDataLen;
		Len -= validDataLen;
	}
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
void EscWrite(MEM_ADDR *pmData, UINT16 Address, UINT16 Len) 
{
	UINT32_VAL param32_1;
	UINT8 validDataLen, j;
	UINT8 *pData = (UINT8 *)pmData;

	while (Len > 0) {
		validDataLen = (Len > 4) ? 4 : Len;
		
		if (Address & 1) {
			validDataLen = 1;
		}
		else if (Address & 2)
		{
			validDataLen = (validDataLen >= 2) ? 2 : 1;
		}
		else if (validDataLen < 4)
		{
			validDataLen = (validDataLen >= 2) ? 2 : 1;
		}

		for (j = 0; j < validDataLen; j++)
		param32_1.v[j] = *pData++;
		
		SPIWrite(0x0300, (uint8_t*)&param32_1.Val);

		param32_1.v[0] = (UINT8)Address;
		param32_1.v[1] = (UINT8)(Address >> 8);
		param32_1.v[2] = validDataLen;
		param32_1.v[3] = 0x80;

		SPIWrite(0x0304, (uint8_t*)&param32_1.Val);

		Address += validDataLen;
		Len -= validDataLen;
	}
}

void HW_EscWriteIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
	UINT8 *pTmpData = (UINT8 *)pData;
	if(Address>0xfff)
	{
		/* start transmission */
		WritePdRam(pTmpData, Address, Len);
	}
	else
	{
		EscWrite(pData, Address, Len);
	}
}

void HW_EscWriteMailbox(MEM_ADDR *pData, UINT16 Address, UINT16 Len) {

	HW_EscWrite(pData, Address, Len);
}

void HW_EscReadMailbox(MEM_ADDR *pData, UINT16 Address, UINT16 Len) {
	
	/* send the address and command to the ESC */
	//ReadPdRam(pTmpData, Address, Len);
	HW_EscRead(pData, Address, Len);
}