/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef ETHER_CAT_MAIN_H
#define ETHER_CAT_MAIN_H
#include "definitions.h"
#include "ssc/ecat_def.h"


/* Global Data Types */
#define CMD_SERIAL_READ 0x03
#define CMD_FAST_READ 0x0B
#define CMD_DUAL_OP_READ 0x3B
#define CMD_DUAL_IO_READ 0xBB
#define CMD_QUAD_OP_READ 0x6B
#define CMD_QUAD_IO_READ 0xEB
#define CMD_SERIAL_WRITE 0x02
#define CMD_DUAL_DATA_WRITE 0x32
#define CMD_DUAL_ADDR_DATA_WRITE 0xB2
#define CMD_QUAD_DATA_WRITE 0x62
#define CMD_QUAD_ADDR_DARA_WRITE 0xE2

#define CMD_SERIAL_READ_DUMMY 0
#define CMD_FAST_READ_DUMMY 1
#define CMD_DUAL_OP_READ_DUMMY 1
#define CMD_DUAL_IO_READ_DUMMY 2
#define CMD_QUAD_OP_READ_DUMMY 1
#define CMD_QUAD_IO_READ_DUMMY 4
#define CMD_SERIAL_WRITE_DUMMY 0
#define CMD_DUAL_DATA_WRITE_DUMMY 0
#define CMD_DUAL_ADDR_DATA_WRITE_DUMMY 0
#define CMD_QUAD_DATA_WRITE_DUMMY 0
#define CMD_QUAD_ADDR_DARA_WRITE_DUMMY 0

#define ESC_CSR_CMD_REG 0x304
#define ESC_CSR_DATA_REG 0x300
#define ESC_WRITE_BYTE 0x80
#define ESC_READ_BYTE 0xC0
#define ESC_CSR_BUSY 0x80


#ifdef __cplusplus
extern "C" {
#endif

typedef union {
	uint32_t Val;
	uint16_t w[2] __attribute__((packed));
	uint8_t  v[4];
	struct __attribute__((packed)) {
		uint16_t LW;
		uint16_t HW;
	} word;
	struct __attribute__((packed)) {
		uint8_t LB;
		uint8_t HB;
		uint8_t UB;
		uint8_t MB;
	} byte;

} UINT32_VAL;

void    PDI_IRQ_Interrupt();
void    SPIChipSelectSet(void);
void    SPIChipSelectClr(void);
void    EtherCATTestPinSet(void);
void    EtherCATTestPinClr(void);
void	SPIWrite(uint16_t adr, uint8_t *data);
void	SPIRead(uint16_t adr, uint8_t *data);
void	ReadPdRam(UINT8 *pData, UINT16 Address, UINT16 Len);
void	WritePdRam(UINT8 *pData, UINT16 Address, UINT16 Len);
UINT16	PDI_GetTimer();
void	PDI_ClearTimer(void);
void    PDI_Timer_Interrupt(void);
void    PDI_Init_SYNC_Interrupts();
void	stop_timer(void);
void	start_timer(void);
void	HW_SetLed(UINT8 RunLed, UINT8 ErrLed);
void	EtherCATInit();

#ifdef __cplusplus
}
#endif

#endif /* ETHER_CAT_MAIN_H */
