/*******************************************************************************
 LAN9255 driver source file for Microchip EtherCAT
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9255.c

  Summary
    This file  contains the functional implementation of LAN9255 Hardware 
    Abstraction Layer

  Description
    This file defines enabled Sync Manager & Application Layer Event Requests.
	Added ISR call backs related to Sync Manager & AL Event Request.

  Remarks:
    None.
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
#define _LAN9255_HW_ 1
#include "drv_lan9255.h"
#undef _LAN9255_
#define _LAN9255_ 0
#include "ecatappl.h"

bool gbALEvtOpEnabled = FALSE; 
UALEVENT    gEscALEvent;
void lan9255_interrupt_init(void);

#ifdef ETHERCAT_DEVELOPMENT_TEST
void develop_test(void);

void develop_test(void)
{    
    uint32_t u32WriteMask=1,u32ReadMask=2;
    do
    {
        u32WriteMask++;
        HW_EscWriteDWord(u32WriteMask, ESC_AL_EVENTMASK_OFFSET);

        u32ReadMask = 0;
        HW_EscReadDWord(u32ReadMask, ESC_AL_EVENTMASK_OFFSET);
    }while(u32WriteMask != u32ReadMask);
	
}
#endif

void lan9255_interrupt_init(void)
{
    PDI_Timer_Interrupt();
    
	PDI_IRQ_Interrupt();
}

/*******************************************************************************
    Function:
        uint8_t LAN9255_Init(void)

    Summary:
        This function initializes LAN9255.

    Description:
  *****************************************************************************/
uint8_t LAN9255_Init(void)
{
	uint32_t u32intMask;
	uint32_t u32Data=0;
#if defined(ETHERCAT_COMM_PROTOCOL_SPI)
	#if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS
		do
		{
			MCHP_ESF_PDI_READ(LAN925x_BYTE_ORDER_REG, (uint8_t*)&u32Data, DWORD_LENGTH);
		} while (0x87654321 != u32Data);
		
		// Disable interrupt Interrupt Enable register -->
		// Write 0x5c - 0x00000001
		u32Data = 0x00000000;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);
	    
	    do {
			u32intMask = 0x93;
			HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
	
			u32intMask = 0;
			HW_EscReadDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
		} while (u32intMask != 0x93);
	    
		/* Read 0x150 register to check if AL Event output is enabled */
		u32intMask = 0; 
		HW_EscReadDWord(u32intMask, ESC_PDI_CONFIG_OFFSET);
		
	    if (u32intMask & AL_EVENT_ENABLE)
	    {
			gbALEvtOpEnabled = TRUE; 
	    }
	    	 
		u32intMask = 0;
		HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);	
		
		// IRQ enable,IRQ polarity, IRQ buffer type in Interrupt Configuration
		// register. Write 0x54 - 0x00000101
		u32Data = 0x00000101;
			
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&u32Data, DWORD_LENGTH);	
		
		// Write in Interrupt Enable register -->
		// Write 0x5c - 0x00000001
		u32Data = 0x00000001;
			
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);
			
		// Read Interrupt Status register
		MCHP_ESF_PDI_READ(LAN925x_CSR_INT_STS, (uint8_t*)&u32Data, DWORD_LENGTH);
		
	#elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
	
		do
		{
			MCHP_ESF_PDI_READ(LAN925x_BYTE_ORDER_REG, (uint8_t*)&u32Data, DWORD_LENGTH);
		} while (0x87654321 != u32Data);
		
		u32Data = 0x00000000;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);
	
		do 
	    {
			u32intMask = 0x93;
			HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
	
			u32intMask = 0;
			HW_EscReadDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
		} while (u32intMask != 0x93);
	
		/* Read 0x150 register to check if AL Event output is enabled */
		u32intMask = 0;
		HW_EscReadDWord(u32intMask, ESC_PDI_CONFIG_OFFSET);
		
		if (u32intMask & AL_EVENT_ENABLE)
		{
			gbALEvtOpEnabled = TRUE;
		}
		
		u32intMask = 0;
		HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
		
		u32Data = 0x00000101;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&u32Data, DWORD_LENGTH);	
	
	
		
		u32Data = 0x00000001;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);
	
		
		MCHP_ESF_PDI_READ(LAN925x_CSR_INT_STS, (uint8_t*)&u32Data, DWORD_LENGTH);

	#else

		do
		{
			MCHP_ESF_PDI_READ(LAN925x_BYTE_ORDER_REG, (uint8_t*)&u32Data, DWORD_LENGTH);
		} while (0x87654321 != u32Data);
	
		// Disable interrupt Interrupt Enable register -->
		// Write 0x5c - 0x00000001
		u32Data = 0x00000000;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);
	
		do
	    {
			u32intMask = 0x93;
			HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
	
			u32intMask = 0;
			HW_EscReadDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
		} while (u32intMask != 0x93);
	
		/* Read 0x150 register to check if AL Event output is enabled */
		u32intMask = 0;
		HW_EscReadDWord(u32intMask, ESC_PDI_CONFIG_OFFSET);
	
		if (u32intMask & AL_EVENT_ENABLE)
		{
			gbALEvtOpEnabled = TRUE;
		}
	
		u32intMask = 0;
		HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
	
		u32Data = 0x00000101;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&u32Data, DWORD_LENGTH);
	
		u32Data = 0x00000001;
		MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);
	
		MCHP_ESF_PDI_FASTREAD(LAN925x_CSR_INT_STS, (uint8_t*)&u32Data, DWORD_LENGTH);
	
	#endif 	
#endif

#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined (ETHERCAT_SQI_INDIRECT_MODE_ACCESS) || defined (ETHERCAT_SQI_DIRECT_MODE_ACCESS)
  
        u32Data = 0x00000000;
        /* Read the Byte Test register */
        do
        {
            MCHP_ESF_PDI_READ(LAN925x_BYTE_ORDER_REG, (uint8_t*)&u32Data, DWORD_LENGTH);
        } while (0x87654321 != u32Data);

        // Disable interrupt Interrupt Enable register -->
        // Write 0x5c - 0x00000000
        u32Data = 0x00000000;
        MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);

        do {
            u32intMask = 0x93;
            HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);

            u32intMask = 0;
            HW_EscReadDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);
        } while (u32intMask != 0x93);

        u32intMask = 0;
        HW_EscWriteDWord(u32intMask, ESC_AL_EVENTMASK_OFFSET);

        u32Data = 0x00000101;
        MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_CONF, (uint8_t*)&u32Data, DWORD_LENGTH);

        u32Data = 0x00000001;
        MCHP_ESF_PDI_WRITE(LAN925x_CSR_INT_EN, (uint8_t*)&u32Data, DWORD_LENGTH);

        MCHP_ESF_PDI_READ(LAN925x_CSR_INT_STS, (uint8_t*)&u32Data, DWORD_LENGTH);
    #endif  
#endif
#ifdef DC_SUPPORTED
	PDI_Init_SYNC_Interrupts();
#endif

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
    uint16_t HW_GetALEventRegister(void)

Summary:
    This function gets the current content of ALEvent register.

Description:
    Returns first two Bytes of ALEvent register (0x220)
  *****************************************************************************/
uint16_t HW_GetALEventRegister(void)
{
#if defined (ETHERCAT_COMM_PROTOCOL_SPI)    
	#if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS 	
	    UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
	    u32Val.v[0] = 0x20;
	    u32Val.v[1] = 0x02;
	    u32Val.v[2] = 4;
	    u32Val.v[3] = ESC_READ_BYTE;
	
	    PDI_Disable_Global_Interrupt();
	
	    MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);	  
	    MCHP_ESF_PDI_FASTREAD(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
	    PDI_Restore_Global_Interrupt();
	
	    if (gbALEvtOpEnabled)
	    {
	        return gEscALEvent.Word;		  
	    }
	    else
	    {
	        return u32Val.w[0];	  
	    }
	
	#elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
	
	    UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
        
	    PDI_Disable_Global_Interrupt();
	
	    MCHP_ESF_PDI_READ(ESC_AL_EVENT_OFFSET, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
	    PDI_Restore_Global_Interrupt();
	
        if(gbALEvtOpEnabled)
	    {
            return gEscALEvent.Word;
	    }
	    else
	    {
	        return u32Val.w[0];
	    }
		  
	#else
	
	    UINT32_VAL u32Val;
        
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
	    PDI_Disable_Global_Interrupt();
	
	    MCHP_ESF_PDI_FASTREAD(ESC_AL_EVENT_OFFSET, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
	    PDI_Restore_Global_Interrupt();
	
	    if (gbALEvtOpEnabled)
	    {
	        return gEscALEvent.Word;
	    }
	    else
	    {
	        return u32Val.w[0];
	    }
		
	#endif
#endif
#if defined (ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS 	
        UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
        u32Val.v[0] = 0x20;
        u32Val.v[1] = 0x02;
        u32Val.v[2] = 4;
        u32Val.v[3] = ESC_READ_BYTE;

        PDI_Disable_Global_Interrupt();

        MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);	  
        MCHP_ESF_PDI_READ(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

        PDI_Restore_Global_Interrupt();

        if (gbALEvtOpEnabled)
        {
            return gEscALEvent.Word;		  
        }
        else
        {
            return u32Val.w[0];	  
        }

    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
        PDI_Disable_Global_Interrupt();

        MCHP_ESF_PDI_READ(ESC_AL_EVENT_OFFSET, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

        PDI_Restore_Global_Interrupt();

        return u32Val.w[0];

    #endif
#endif
}

/*******************************************************************************
    Function:
        uint16_t HW_GetALEventRegister_Isr(void)

    Summary:
        The SPI PDI requires an extra ESC read access functions from interrupts service routines.
                The behaviour is equal to "HW_GetALEventRegister()"

    Description:
        Returns	first two Bytes of ALEvent register (0x220)
  *****************************************************************************/
uint16_t HW_GetALEventRegister_Isr(void)
{
#if defined (ETHERCAT_COMM_PROTOCOL_SPI)    
	#if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS 
	    UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
	    u32Val.v[0] = 0x20;
	    u32Val.v[1] = 0x02;
	    u32Val.v[2] = 2;
	    u32Val.v[3] = ESC_READ_BYTE;
	
	    MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	   
	    MCHP_ESF_PDI_FASTREAD(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
	    if (gbALEvtOpEnabled)
	    {
	        return gEscALEvent.Word;
	    }
	    else
	    {
	        return u32Val.w[0];
	    }
		  
	#elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
	    UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
	    MCHP_ESF_PDI_READ(ESC_AL_EVENT_OFFSET, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
        if(gbALEvtOpEnabled)
	    {
            return gEscALEvent.Word;
	    }
	    else
	    {
	        return u32Val.w[0];
	    }
		  
	#else 
	    UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
	    MCHP_ESF_PDI_FASTREAD(ESC_AL_EVENT_OFFSET, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
	
	    if (gbALEvtOpEnabled)
	    {
	        return gEscALEvent.Word;
	    }
	    else
	    {
	        return u32Val.w[0];
	    }
	
	#endif
#endif
    
#if defined (ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS 
        UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
        u32Val.v[0] = 0x20;
        u32Val.v[1] = 0x02;
        u32Val.v[2] = 2;
        u32Val.v[3] = ESC_READ_BYTE;

        MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
        MCHP_ESF_PDI_READ(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

        if (gbALEvtOpEnabled)
        {
            return gEscALEvent.Word;
        }
        else
        {
            return u32Val.w[0];
        }
    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS
        UINT32_VAL u32Val;
        u32Val.w[0] = 0;
        u32Val.w[1] = 0;
        MCHP_ESF_PDI_READ(ESC_AL_EVENT_OFFSET, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

        return u32Val.w[0];
  
    #endif
#endif
}


/*******************************************************************************
Function:
    void HW_EscRead( MEM_ADDR *pData, uint16_t u16Address, uint16_t u16Len )

Summary:
    This function operates the SPI read access to the EtherCAT ASIC.

Description:
    Input param:
     pData		- Pointer to a byte array which holds data to write or saves read data.
     Address	- EtherCAT ASIC address ( upper limit is 0x1FFF )	for access.
     Len		- Access size in Bytes.
*****************************************************************************/
void HW_EscRead(MEM_ADDR *pmData, uint16_t u16Address, uint16_t u16Len) 
{
#if defined (ETHERCAT_COMM_PROTOCOL_SPI)    
	#if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS
	
		UINT32_VAL u32Val;
		uint8_t u8ValidDataLen, u8Itr;
		uint8_t *pu8Data = (uint8_t *)pmData;
	
		while (u16Len > 0) 
	    {
			u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;
		
			if (u16Address & 1) {
				u8ValidDataLen = 1;
			}
			else if (u16Address & 2)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
			}
			else if (u8ValidDataLen < 4)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
			}
		
			u32Val.v[0] = (uint8_t)u16Address;
			u32Val.v[1] = (uint8_t)(u16Address >> 8);
			u32Val.v[2] = u8ValidDataLen;
			u32Val.v[3] = 0xC0;
		
			PDI_Disable_Global_Interrupt();
			MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
			MCHP_ESF_PDI_FASTREAD(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
			PDI_Restore_Global_Interrupt();
		
			for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
	        {
	            *pu8Data++ = u32Val.v[u8Itr];
	        }
	
			u16Address += u8ValidDataLen;
			u16Len -= u8ValidDataLen;
		}
	
	#elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
	
		UINT32_VAL u32Val;
		uint8_t u8ValidDataLen, u8Itr;
		uint8_t *pu8Data = (uint8_t *)pmData;
	
		while (u16Len > 0)
	    {
			u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;
		
			if (u16Address & 0x1) 
	        {
				u8ValidDataLen = 1;
			}
			else if (u16Address & 0x2)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
			}
			else if (u8ValidDataLen < 4)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
			}
		
			PDI_Disable_Global_Interrupt();
			MCHP_ESF_PDI_READ(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
			PDI_Restore_Global_Interrupt();
		
			for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
	        {
	            *pu8Data++ = u32Val.v[u8Itr];
	        }
	
			u16Address += u8ValidDataLen;
			u16Len -= u8ValidDataLen;
		}
	
	#else
	
		UINT32_VAL u32Val;
        u32Val.v[0] = 0;
        u32Val.v[1] = 0;
        u32Val.v[2] = 0;
        u32Val.v[3] = 0;
        
		uint8_t u8ValidDataLen, u8Itr;
		uint8_t *pu8Data = (uint8_t *)pmData;
	
		while (u16Len > 0)
	    {
			u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;
		
			if (u16Address & 0x1)
	        {
				u8ValidDataLen = 1;
			}
			else if (u16Address & 0x2)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
			}
			else if (u8ValidDataLen < 4)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
			}
		
			PDI_Disable_Global_Interrupt();
			MCHP_ESF_PDI_FASTREAD(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
			PDI_Restore_Global_Interrupt();
		
			for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
	        {
	            *pu8Data++ = u32Val.v[u8Itr];
	        }
	
			u16Address += u8ValidDataLen;
			u16Len -= u8ValidDataLen;
		}
	
	#endif
#endif

#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_READ_BYTE;

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            MCHP_ESF_PDI_READ(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }
    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS
        
        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_READ(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }
    #endif
#endif    
}

/*******************************************************************************
    Function:
        void EscRead( MEM_ADDR *pData, uint16_t Address, uint16_t Len )

    Summary:
        The SPI PDI requires an extra ESC read access functions from interrupts service routines.
                The behaviour is equal to "HW_EscRead()"

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves read data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/

void EscRead(MEM_ADDR *pmData, uint16_t u16Address, uint16_t u16Len)
{
#if defined(ETHERCAT_COMM_PROTOCOL_SPI)    
    #if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS
        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_READ_BYTE;

            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            MCHP_ESF_PDI_FASTREAD(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }
    #elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            MCHP_ESF_PDI_READ(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #else

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;
        u32Val.v[0] = 0;
        u32Val.v[1] = 0;
        u32Val.v[2] = 0;
        u32Val.v[3] = 0;
        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            MCHP_ESF_PDI_FASTREAD(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #endif
#endif

#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS
        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 1) 
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_READ_BYTE;

            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            MCHP_ESF_PDI_READ(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }
    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) 
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            MCHP_ESF_PDI_READ(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }
    #endif
#endif    
}

void HW_EscReadIsr(MEM_ADDR *pmData, uint16_t u16Address, uint16_t u16Len)
{
#if defined(ETHERCAT_COMM_PROTOCOL_SPI)    
	uint8_t *pu8Data = (uint8_t *)pmData;
  
    if(u16Address>0xfff)
    {
        /* send the address and command to the ESC */
  #if ETHERCAT_SPI_INDIRECT_MODE_ACCESS	  
        MCHP_ESF_PDI_FASTREAD_PDRAM(pu8Data, u16Address, u16Len);
  #elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
        MCHP_ESF_PDI_READ (u16Address, pu8Data, u16Len);
  #elif ETHERCAT_SPI_DIRECT_MODE_ACCESS
        MCHP_ESF_PDI_FASTREAD (u16Address, pu8Data, u16Len);
  #endif 	  	  	  
    }
    else
    {
        EscRead(pmData,u16Address, u16Len);
    }
#endif
#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    uint8_t *pu8Data = (uint8_t *)pmData;
    if(u16Address>0xfff)
    {
        /* send the address and command to the ESC */
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS	  
        MCHP_ESF_PDI_READ_PDRAM(pu8Data, u16Address, u16Len);
    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS
        
        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;

        while (u16Len > 0) 
        {

            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2 ) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            MCHP_ESF_PDI_READ(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                *pu8Data++ = u32Val.v[u8Itr];
            }

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }
    #endif 	  	  	  
    }
    else
    {
        EscRead(pmData, u16Address, u16Len);
    }
#endif    
}

/*******************************************************************************
    Function:
        void HW_EscWrite( MEM_ADDR *pData, uint16_t Address, uint16_t Len )

    Summary:
        This function operates the SPI write access to the EtherCAT ASIC.

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves write data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/
void HW_EscWrite(MEM_ADDR *pmData, uint16_t u16Address, uint16_t u16Len)
{
#if defined(ETHERCAT_COMM_PROTOCOL_SPI)    
    #if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen=0, u8Itr=0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_WRITE(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_WRITE_BYTE;

            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }                

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #else

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #endif
#endif
#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_WRITE(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_WRITE_BYTE;

            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS
        
        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            PDI_Disable_Global_Interrupt();
            MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
            PDI_Restore_Global_Interrupt();

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #endif
#endif        
}

/*******************************************************************************
    Function:
        void EscWrite( MEM_ADDR *pData, uint16_t Address, uint16_t Len )

    Summary:
        The SPI PDI requires an extra ESC write access functions from interrupts service routines.
                The behavior is equal to "HW_EscWrite()"

    Description:
        Input param:
        pData			- Pointer to a byte array which holds data to write or saves write data.
        param Address	- EtherCAT ASIC address ( upper limit is 0x1FFF ) for access.
        param Len		- Access size in Bytes.
  *****************************************************************************/
void EscWrite(MEM_ADDR *pmData, uint16_t u16Address, uint16_t u16Len) 
{
#if defined(ETHERCAT_COMM_PROTOCOL_SPI)    
    #if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            MCHP_ESF_PDI_WRITE(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_WRITE_BYTE;

            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #elif defined ETHERCAT_SPI_BECKHOFF_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen, u8Itr;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #else 
        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen=0, u8Itr=0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0)
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1)
            {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #endif
#endif
#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
        uint8_t u8ValidDataLen = 0, u8Itr = 0;
        uint8_t *pu8Data = (uint8_t *)pmData;

        while (u16Len > 0) 
        {
            u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

            if (u16Address & 0x1) {
                u8ValidDataLen = 1;
            }
            else if (u16Address & 0x2)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }
            else if (u8ValidDataLen < 4)
            {
                u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
            }

            for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }

            MCHP_ESF_PDI_WRITE(ESC_CSR_DATA_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u32Val.v[0] = (uint8_t)u16Address;
            u32Val.v[1] = (uint8_t)(u16Address >> 8);
            u32Val.v[2] = u8ValidDataLen;
            u32Val.v[3] = ESC_WRITE_BYTE;

            MCHP_ESF_PDI_WRITE(ESC_CSR_CMD_REG, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

            u16Address += u8ValidDataLen;
            u16Len -= u8ValidDataLen;
        }

    #elif defined ETHERCAT_SQI_DIRECT_MODE_ACCESS

        UINT32_VAL u32Val;
		uint8_t u8ValidDataLen = 0, u8Itr = 0;
		uint8_t *pu8Data = (uint8_t *)pmData;

		while (u16Len > 0) 
        {
			u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;
			
			if (u16Address & 0x1) {
				u8ValidDataLen = 1;
			}
			else if (u16Address & 0x2)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
			}
			else if (u8ValidDataLen < 4)
			{
				u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
			}

			for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
            {
                u32Val.v[u8Itr] = *pu8Data++;
            }
			
			MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);
			
			u16Address += u8ValidDataLen;
			u16Len -= u8ValidDataLen;
		}
    #endif
#endif            
}

void HW_EscWriteIsr(MEM_ADDR *pmData, uint16_t u16Address, uint16_t u16Len)
{
#if defined(ETHERCAT_COMM_PROTOCOL_SPI)    
	uint8_t *pu8Data = (uint8_t *)pmData;

    if(u16Address>0xfff)
    {
        /* start transmission */
  #if defined ETHERCAT_SPI_INDIRECT_MODE_ACCESS	  
        MCHP_ESF_PDI_WRITE_PDRAM(pu8Data, u16Address, u16Len);
  #else
        MCHP_ESF_PDI_WRITE (u16Address, pu8Data, u16Len);
  #endif
    }
    else
    {
        EscWrite(pmData, u16Address, u16Len);
    }
#endif
#if defined(ETHERCAT_COMM_PROTOCOL_SQI)
    uint8_t *pu8Data = (uint8_t *)pmData;

    if(u16Address>0xfff)
    {
        /* start transmission */
        #if defined ETHERCAT_SQI_INDIRECT_MODE_ACCESS	  
            MCHP_ESF_PDI_WRITE_PDRAM(pu8Data, u16Address, u16Len);
        #else
         
            UINT32_VAL u32Val;
            uint8_t u8ValidDataLen = 0, u8Itr = 0;

            while (u16Len > 0) 
            {
                u8ValidDataLen = (u16Len > 4) ? 4 : u16Len;

                if (u16Address & 0x1) 
                {
                    u8ValidDataLen = 1;
                }
                else if (u16Address & 0x2)
                {
                    u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
                }
                else if (u8ValidDataLen < 4)
                {
                    u8ValidDataLen = (u8ValidDataLen >= 2) ? 2 : 1;
                }

                for (u8Itr = 0; u8Itr < u8ValidDataLen; u8Itr++)
                {
                    u32Val.v[u8Itr] = *pu8Data++;
                }

                MCHP_ESF_PDI_WRITE(u16Address, (uint8_t*)&u32Val.Val, DWORD_LENGTH);

                u16Address += u8ValidDataLen;
                u16Len -= u8ValidDataLen;
            }
        #endif
    }
    else
    {
        EscWrite(pmData, u16Address, u16Len);
    }
#endif    
}

void HW_EscWriteMailbox(MEM_ADDR *pData, uint16_t Address, uint16_t Len) 
{

	HW_EscWrite(pData, Address, Len);
}

void HW_EscReadMailbox(MEM_ADDR *pData, uint16_t Address, uint16_t Len) 
{
	/* send the address and command to the ESC */
	//ReadPdRam(pTmpData, Address, Len);
	HW_EscRead(pData, Address, Len);
}

#if !defined ETHERCAT_EEPROM_EMULATION_SUPPORT
uint8_t HW_EepromReload ()
{
    return 0;
}
#endif
