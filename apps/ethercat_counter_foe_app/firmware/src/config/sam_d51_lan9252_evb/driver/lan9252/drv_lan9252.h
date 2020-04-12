/*******************************************************************************
 LAN9252 driver header file for Microchip EtherCAT
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9252.h

  Summary
    This file  contains the functional implementation of LAN9252 Hardware 
    Abstraction Layer

  Description
    This file lists Sync Manager & Application Layer Event read and write 
    function defination.
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

#ifndef _LAN9252_H_
#define _LAN9252_H_

/* Includes */
#include "esc.h"
#include "ecatappl.h"
#include "drv_lan9252_ecat_util.h"

/* 9252 HW DEFINES */
#define ECAT_REG_BASE_ADDR 0x0300

#define CSR_DATA_REG_OFFSET 0x00
#define CSR_CMD_REG_OFFSET 0x04
#define PRAM_READ_ADDR_LEN_OFFSET 0x08
#define PRAM_READ_CMD_OFFSET 0x0c
#define PRAM_WRITE_ADDR_LEN_OFFSET 0x10
#define PRAM_WRITE_CMD_OFFSET 0x14

#define PRAM_SPACE_AVBL_COUNT_MASK 0x1f
#define IS_PRAM_SPACE_AVBL_MASK 0x01

#define CSR_DATA_REG ECAT_REG_BASE_ADDR + CSR_DATA_REG_OFFSET
#define CSR_CMD_REG ECAT_REG_BASE_ADDR + CSR_CMD_REG_OFFSET
#define PRAM_READ_ADDR_LEN_REG ECAT_REG_BASE_ADDR + PRAM_READ_ADDR_LEN_OFFSET
#define PRAM_READ_CMD_REG ECAT_REG_BASE_ADDR + PRAM_READ_CMD_OFFSET
#define PRAM_WRITE_ADDR_LEN_REG ECAT_REG_BASE_ADDR + PRAM_WRITE_ADDR_LEN_OFFSET
#define PRAM_WRITE_CMD_REG ECAT_REG_BASE_ADDR + PRAM_WRITE_CMD_OFFSET

#define PRAM_READ_FIFO_REG 0x04
#define PRAM_WRITE_FIFO_REG 0x20

#define HBI_INDEXED_DATA0_REG 0x04
#define HBI_INDEXED_DATA1_REG 0x0c
#define HBI_INDEXED_DATA2_REG 0x14

#define HBI_INDEXED_INDEX0_REG 0x00
#define HBI_INDEXED_INDEX1_REG 0x08
#define HBI_INDEXED_INDEX2_REG 0x10

#define HBI_INDEXED_PRAM_READ_WRITE_FIFO 0x18

#define PRAM_RW_ABORT_MASK (1 << 30)
#define PRAM_RW_BUSY_32B (1 << 31)
#define PRAM_RW_BUSY_8B (1 << 7)
#define PRAM_SET_READ (1 << 6)
#define PRAM_SET_WRITE 0

/* Hardware timer settings */
#define ECAT_TIMER_INC_P_MS 312 /**< \brief 312 ticks per ms*/

///* Interrupt and Timer DEFINES */
#define PDI_Restore_Global_Interrupt()  CRITICAL_SECTION_LEAVE()
#define PDI_Disable_Global_Interrupt()  CRITICAL_SECTION_ENTER()

#ifndef DISABLE_ESC_INT
#define DISABLE_ESC_INT() PDI_Disable_Global_Interrupt()
#endif
#ifndef ENABLE_ESC_INT
#define ENABLE_ESC_INT() PDI_Restore_Global_Interrupt()
#endif

#ifndef HW_GetTimer
#define HW_GetTimer() (PDI_GetTimer()) /**< \brief Access to the hardware timer*/
#endif

#ifndef HW_ClearTimer
#define HW_ClearTimer() (PDI_ClearTimer()) /**< \brief Clear the hardware timer*/
#endif

#define HW_EscReadWord(WordValue, Address)                                                                             \
	HW_EscRead(((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2) /**< \brief 16Bit ESC read access*/
#define HW_EscReadDWord(DWordValue, Address)                                                                           \
	HW_EscRead(((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4) /**< \brief 32Bit ESC read access*/
#define HW_EscReadByte(ByteValue, Address)                                                                             \
	HW_EscRead(((MEM_ADDR *)&(ByteValue)), ((UINT16)(Address)), 1) /**< \brief 8Bit ESC read access*/
#define HW_EscReadMbxMem(pData, Address, Len)                                                                          \
	HW_EscRead(                                                                                                        \
	    ((MEM_ADDR *)(pData)), ((UINT16)(Address)), (Len)) /**< \brief The mailbox data is stored in the local uC      \
	                                                          memory therefore the default read function is used.*/

#define HW_EscReadWordIsr(WordValue, Address)                                                                          \
	HW_EscReadIsr(                                                                                                     \
	    ((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2) /**< \brief Interrupt specific 16Bit ESC read access*/
#define HW_EscReadDWordIsr(DWordValue, Address)                                                                        \
	HW_EscReadIsr(                                                                                                     \
	    ((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4) /**< \brief Interrupt specific 32Bit ESC read access*/
#define HW_EscReadByteIsr(ByteValue, Address)                                                                          \
	HW_EscReadIsr(                                                                                                     \
	    ((MEM_ADDR *)&(ByteValue)), ((UINT16)(Address)), 1) /**< \brief Interrupt specific 8Bit ESC read access*/

#define HW_EscWriteWord(WordValue, Address)                                                                            \
	HW_EscWrite(((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2) /**< \brief 16Bit ESC write access*/
#define HW_EscWriteDWord(DWordValue, Address)                                                                          \
	HW_EscWrite(((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4) /**< \brief 32Bit ESC write access*/
#define HW_EscWriteByte(ByteValue, Address)                                                                            \
	HW_EscWrite(((MEM_ADDR *)&(ByteValue)), ((UINT16)(Address)), 1) /**< \brief 8Bit ESC write access*/
#define HW_EscWriteMbxMem(pData, Address, Len)                                                                         \
	HW_EscWrite(                                                                                                       \
	    ((MEM_ADDR *)(pData)), ((UINT16)(Address)), (Len)) /**< \brief The mailbox data is stored in the local uC      \
	     memory therefore the default write function is used.*/

#define HW_EscWriteWordIsr(WordValue, Address)                                                                         \
	HW_EscWriteIsr(                                                                                                    \
	    ((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2) /**< \brief Interrupt specific 16Bit ESC write access*/
#define HW_EscWriteDWordIsr(DWordValue, Address)                                                                       \
	HW_EscWriteIsr(                                                                                                    \
	    ((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4) /**< \brief Interrupt specific 32Bit ESC write access*/
#define HW_EscWriteByteIsr(ByteValue, Address)                                                                         \
	HW_EscWriteIsr(                                                                                                    \
	    ((MEM_ADDR *)&(ByteValue)), ((UINT16)(Address)), 1) /**< \brief Interrupt specific 8Bit ESC write access*/

/* PROTO DEFINES */
#if _lan9252_
#define PROTO
#else
#define PROTO extern
#endif

/* Global variables extern */
PROTO volatile unsigned int restore_intsts;

/* Global functions prototype */
PROTO UINT8 LAN9252_Init(void);
PROTO void  HW_Release(void);

PROTO UINT16 HW_GetALEventRegister(void);
PROTO UINT16 HW_GetALEventRegister_Isr(void);

PROTO void HW_ResetALEventMask(UINT16 intMask);
PROTO void HW_SetALEventMask(UINT16 intMask);

PROTO void HW_EscRead(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
PROTO void HW_EscReadIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len);

PROTO void HW_EscWrite(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
PROTO void HW_EscWriteIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len);

PROTO void HW_DisableSyncManChannel(UINT8 channel);
PROTO void HW_EnableSyncManChannel(UINT8 channel);
PROTO TSYNCMAN ESCMEM *HW_GetSyncMan(UINT8 channel);
PROTO void HW_SetLed(UINT8 RunLed, UINT8 ErrLed);

PROTO UINT16 PDI_GetTimer();
PROTO void   PDI_ClearTimer();

#undef PROTO

#endif
