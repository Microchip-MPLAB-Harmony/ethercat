/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9255_ecat_util.h

  Summary
    ETherCAT header file which interface between EtherCAT driver and the 
    Micorchip H3 Peripheral libraries.

  Description
    This file defines the interface between EtherCAT driver and Microchip H3 
    Peripheral libraries.

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

#ifndef DRV_LAN9255_ECAT_UTIL_H
#define DRV_LAN9255_ECAT_UTIL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include "definitions.h"
#include "ecat_def.h"
#include "drv_lan9255_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END
<#if __PROCESSOR?matches("PIC32M.*") == false>
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
  /* __NVIC_PRIO_BITS will be specified when CMSIS is being used. */
  #define ETHERCAT_INT_PRIO_BITS     __NVIC_PRIO_BITS
#else
  /* 15 priority levels */
  #define ETHERCAT_INT_PRIO_BITS     3
#endif
<#else>
	/* PIC32M IPL masking*/
	#define ETHERCAT_INTPRIORITYLEVEL_MASK 	0xFFFF03FF
	/* The number bits need to be shifted to set the Priority value */
	#define ETHERCAT_PRIORITY_SHIFT_BITS	10
</#if>

/* This is the lowest interrupt priority for SAM devices and Highest for PIC32M devices that can be used in a call to a "set priority" function. */
#define ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY 		${DRV_LAN9255_INT_PRIO}
#define ETHERCAT_IS_SUPPORT_DUMMY_CYCLE

/*
 * The Dummy cycles needed for read transaction introduced by two methods
 * - Dummy clock method - dummy clocks feed to SQI structure
 * - Dummy read method  - dummy read happen and data extraction happen through application READ API
 * 
 * ETHERCAT_DUMMY_READ_EN - enables the Dummy read method, (define ETHERCAT_DUMMY_READ_EN to enable)
 * Dummy read method is selected by default, Undefine ETHERCAT_DUMMY_READ_EN to enable dummy clock method
 */
#define ETHERCAT_DUMMY_READ_EN

/* EEPROM Emulation Feature can be enabled by uncommenting below macro for LAN9255*/
#define ETHERCAT_EEPROM_EMULATION_SUPPORT               1

<#if DRV_LAN9255_PROTOCOL == "SPI">
#define ETHERCAT_COMM_PROTOCOL_SPI     1

<#if DRV_LAN9255_SPI_MODE_ACCESS_TYPE 		== "ETHERCAT_SPI_INDIRECT_MODE_ACCESS">
/* EtherCAT SPI Indirect mode enabled */
#define ETHERCAT_SPI_INDIRECT_MODE_ACCESS				1
<#elseif DRV_LAN9255_SPI_MODE_ACCESS_TYPE 	== "ETHERCAT_SPI_DIRECT_MODE_ACCESS">
/* EtherCAT SPI Direct mode enabled */
#define ETHERCAT_SPI_DIRECT_MODE_ACCESS					1
<#elseif DRV_LAN9255_SPI_MODE_ACCESS_TYPE 	== "ETHERCAT_SPI_BECKHOFF_MODE_ACCESS">
/* EtherCAT SPI Beckhoff mode enabled */
#define ETHERCAT_SPI_BECKHOFF_MODE_ACCESS				1
</#if>

<#elseif DRV_LAN9255_PROTOCOL == "SQI">
#define ETHERCAT_COMM_PROTOCOL_SQI     1

<#if DRV_LAN9255_SQI_MODE_ACCESS_TYPE 		== "ETHERCAT_SQI_INDIRECT_MODE_ACCESS">
/* EtherCAT SQI Indirect mode enabled */
#define ETHERCAT_SQI_INDIRECT_MODE_ACCESS				1
<#elseif DRV_LAN9255_SQI_MODE_ACCESS_TYPE 	== "ETHERCAT_SQI_DIRECT_MODE_ACCESS">
/* EtherCAT SQI Direct mode enabled */
#define ETHERCAT_SQI_DIRECT_MODE_ACCESS					1
</#if>
</#if>

#define MCHP_ESF_IS_PDI_FUNCTIONAL(pdata)		ECAT_Lan9255_IsPDIFunctional(pdata)

<#if DRV_LAN9255_PROTOCOL == "SPI">
<#if DRV_LAN9255_SPI_MODE_ACCESS_TYPE 		== "ETHERCAT_SPI_INDIRECT_MODE_ACCESS">
/* SPI Indirect mode Access. These APIs can also be used for LAN9252 device */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_Lan925x_SPIWrite(addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)             ECAT_Lan925x_SPIRead(addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata, len)         ECAT_Lan925x_SPIFastRead(addr, pdata, len)
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		ECAT_Lan925x_SPIReadPDRAM(pdata, addr, len)
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	ECAT_Lan925x_SPIFastReadPDRAM(pdata, addr, len)
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)		ECAT_Lan925x_SPIWritePDRAM(pdata, addr, len)

<#elseif DRV_LAN9255_SPI_MODE_ACCESS_TYPE == "ETHERCAT_SPI_DIRECT_MODE_ACCESS">
/* SPI Direct mode Access */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_Lan9255_SPIWrite (addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)             ECAT_Lan9255_SPIRead (addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata, len)			ECAT_Lan9255_SPIFastRead (addr, pdata, len)
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)		

<#elseif DRV_LAN9255_SPI_MODE_ACCESS_TYPE == "ETHERCAT_SPI_BECKHOFF_MODE_ACCESS">
/* SPI Beckhoff mode Access */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_Lan9255_Beckhoff_SPIWrite (addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)				ECAT_Lan9255_Beckhoff_SPIRead (addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata)			
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)
</#if>
</#if>

<#if DRV_LAN9255_PROTOCOL == "SQI">
<#if DRV_LAN9255_SQI_MODE_ACCESS_TYPE == "ETHERCAT_SQI_INDIRECT_MODE_ACCESS">
/* SQI Indirect mode Access */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_LAN925x_SQIWrite(addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)             ECAT_LAN925x_SQIRead(addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata, len)         
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		ECAT_LAN925x_SQIReadPDRAM(pdata, addr, len)
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)		ECAT_LAN925x_SQIWritePDRAM(pdata, addr, len)

<#elseif DRV_LAN9255_SQI_MODE_ACCESS_TYPE == "ETHERCAT_SQI_DIRECT_MODE_ACCESS">
/* SQI Direct mode Access */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_LAN925x_SQIWrite (addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)             ECAT_LAN925x_SQIRead (addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata, len)			
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)		

</#if>
</#if>

/* Timer functions */

#define MCHP_ESF_PDI_TIMER_CONFIG()						ECAT_PDI_TimerInterrupt()
#define MCHP_ESF_PDI_GET_TIMER()						ECAT_PDI_TimerGet()
#define MCHP_ESF_PDI_CLR_TIMER()						ECAT_PDI_TimerClear()
#define MCHP_ESF_PDI_START_TIMER()						ECAT_PDI_TimerStart()
#define MCHP_ESF_PDI_STOP_TIMER()						ECAT_PDI_TimerStop()



#define MCHP_ESF_CRITICAL_SECTION_ENTER()				CRITICAL_SECTION_ENTER()
#define MCHP_ESF_CRITICAL_SECTION_LEAVE()				CRITICAL_SECTION_LEAVE()        
        
#define PDI_Init_SYNC_Interrupts()                      ECAT_SyncInterruptsInitialization()
#define PDI_IRQ_Interrupt()                             ECAT_ESCIRQInitialization()
        
#define HW_SetLed(RunLed, ErrLed)                       ECAT_HWSetlED(RunLed, ErrLed)     
        
#define PDI_Timer_Interrupt()                           ECAT_PDI_TimerInterrupt()
 
<#if DRV_LAN9255_PROTOCOL == "SPI"> 
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
 // *****************************************************************************
/* DRV_LAN9255 Util Transfer Status

 Summary:
    Defines the data type for LAN9255 Driver Util transfer status.

 Description:
    This will be used to indicate the current transfer status of the
    LAN9255 driver operations.

 Remarks:
    None.
*/

typedef enum
{
    /* Transfer is being processed */
    DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_BUSY,

    /* Transfer is successfully completed*/
    DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_COMPLETED,

    /* Transfer had error */
    DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS_ERROR

} DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS;       
</#if>        
// *****************************************************************************
/* LAN9255 Driver Utility Instance Object

  Summary:
    Object used to keep any data required for an instance of the LAN9255 driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use  */
    bool                                   inUse;
<#if DRV_LAN9255_PROTOCOL == "SPI"> 
    /* SPI PLIB API list that will be used by the driver to access the hardware */
    const DRV_LAN9255_UTIL_SPI_PLIB_INTERFACE 	*spiPlib;

    /* SPI peripheral communication transfer status */
    volatile DRV_LAN9255_UTIL_SPI_TRANSFER_STATUS       spiTransferStatus;
	
<#elseif DRV_LAN9255_PROTOCOL == "SQI">
	/* SQI PLIB API list that will be used by the driver to access the hardware */
	const DRV_LAN9255_UTIL_SQI_PLIB_INTERFACE 	*sqiPlib;
</#if>
	/* Timer PLIB API list that will be used by the driver to access the hardware */
    const DRV_LAN9255_UTIL_TMR_PLIB_INTERFACE	*timerPlib;
	

} DRV_LAN9255_UTIL_OBJ;

/* SPI/SQI Clock Period in nano seconds */
#define CLK_PERIOD_1MHZ		(1000)

<#if DRV_LAN9255_PLIB == "QSPI">
	<#lt>#define DRV_LAN9255_BAUDRATE_PDI_FREQ       ${.vars["${DRV_LAN9255_PLIB?lower_case}"].QSPI_BAUD_RATE/1000000}
<#else>
	<#lt>#define DRV_LAN9255_BAUDRATE_PDI_FREQ       ${.vars["${DRV_LAN9255_PLIB?lower_case}"].SPI_BAUD_RATE/1000000}
</#if>

/*
 * The Dummy cycles needed for read transaction introduced by two methods
 * - Dummy clock method - dummy clocks feed to SQI structure
 * - Dummy read method  - dummy read happen and data extraction happen through application READ API
 * 
 * DUMMY_READ_EN - enables the Dummy read method, (define DUMMY_READ_EN to enable)
 * Dummy read method is selected by default, Undefine DUMMY_READ_EN to enable dummy clock method
 */
#define ECAT_DUMMY_READ_EN

// Internal Access Time (IAT) in Nano seconds (ns) based on Hardware Design
#define IAT_NULL		0
#define IAT_BYRD		360
#define IAT_BYWR		265
#define IAT_DWRD		600
#define IAT_DWDWR		505

/* The following data type definitions are used by the functions in this
    interface and should be considered part of it.
*/
/* Global Data Types */
#define CMD_SPI_SETCFG                      0x01
#define CMD_SQI_SETCFG                      0x01
/* EtherCAT SPI Read Command */
#define CMD_SERIAL_READ                     0x03
/* EtherCAT SPI Fast read command */
#define CMD_FAST_READ                       0x0B
#define CMD_DUAL_OP_READ                    0x3B
#define CMD_DUAL_IO_READ                    0xBB
#define CMD_QUAD_OP_READ                    0x6B
#define CMD_QUAD_IO_READ                    0xEB
        
/* EtherCAT SPI Write Command */
#define CMD_SERIAL_WRITE                    0x02
#define CMD_DUAL_DATA_WRITE                 0x32
#define CMD_DUAL_ADDR_DATA_WRITE            0xB2
#define CMD_QUAD_DATA_WRITE                 0x62
#define CMD_QUAD_ADDR_DATA_WRITE            0xE2
#define CMD_ENABLE_SQI                      0x38
#define CMD_RESET_SQI                       0xFF

#define CMD_SERIAL_READ_DUMMY               0
#define CMD_SQI_SETCFG_DUMMY                0
#define CMD_SPI_FAST_READ_DUMMY             1
#define CMD_SQI_FAST_READ_DUMMY             3
#define CMD_DUAL_OP_READ_DUMMY              1
#define CMD_DUAL_IO_READ_DUMMY              2
#define CMD_QUAD_OP_READ_DUMMY              1
#define CMD_QUAD_IO_READ_DUMMY              4
#define CMD_SERIAL_WRITE_DUMMY              0
#define CMD_DUAL_DATA_WRITE_DUMMY           0
#define CMD_DUAL_ADDR_DATA_WRITE_DUMMY      0
#define CMD_QUAD_DATA_WRITE_DUMMY           0
#define CMD_QUAD_ADDR_DARA_WRITE_DUMMY      0

#define QSPI_SPI_ONE_BYTE_CLK_COUNT         8
#define QSPI_SQI_ONE_BYTE_CLK_COUNT         2

#define ESC_CSR_CMD_REG                     0x304
#define ESC_CSR_DATA_REG                    0x300
#define ESC_WRITE_BYTE                      0x80
#define ESC_READ_BYTE                       0xC0
#define ESC_CSR_BUSY                        0x80

#define ECAT_PRAM_RD_ADDR_LENGTH_REG		0x308 
#define ECAT_PRAM_RD_CMD_REG				0x30C 
#define ECAT_PRAM_WR_ADDR_LENGTH_REG		0x310
#define ECAT_PRAM_WR_CMD_REG				0x314
#define ECAT_PRAM_WR_DATA_FIFO_REG          0x20
#define ECAT_PRAM_RD_DATA_FIFO_REG          0x04

#define SETCFG_MAX_DATA_BYTES               39
#define ONE_BYTE_MAX_XFR_LEN                127

#define WAIT_STATE_BYTE                     0xFF
#define READ_TERMINATION_BYTE               0xFF

#define DWORD_LENGTH                        4

/* Wait signal states */
#define WAIT                                FALSE
#define ACK                                 TRUE

/* Beckhoff SPI Commands */
#define ESC_RD                              0x02
#define ESC_WR                              0x04
#define ESC_RD_WAIT_STATE                   0x03

typedef enum _SPI_TYPE {
    SINGLE_SPI = 1,
    DUAL_SPI = 2,
    QUAD_SPI = 4,
} SPI_TYPE;


typedef enum _SET_CFG_DATA_BYTE_ORDER {
    SPI_READ_INITIAL_OFFSET = 0,
    SPI_READ_INTRA_DWORD_OFFSET,
    SPI_READ_INTER_DWORD_OFFSET,
    SPI_FASTREAD_INITIAL_OFFSET = 3,
    SPI_FASTREAD_INTRA_DWORD_OFFSET,
    SPI_FASTREAD_INTER_DWORD_OFFSET,
    SDOR_INITIAL_OFFSET = 6,
    SDOR_INTRA_DWORD_OFFSET,
    SDOR_INTER_DWORD_OFFSET,
    SDIOR_INITIAL_OFFSET = 9,
    SDIOR_INTRA_DWORD_OFFSET,
    SDIOR_INTER_DWORD_OFFSET,
    SQOR_INITIAL_OFFSET = 12,
    SQOR_INTRA_DWORD_OFFSET,
    SQOR_INTER_DWORD_OFFSET,
    SQIOR_INITIAL_OFFSET = 15,
    SQIOR_INTRA_DWORD_OFFSET,
    SQIOR_INTER_DWORD_OFFSET,
    SPI_WRITE_INITIAL_OFFSET = 18,
    SPI_WRITE_INTRA_DWORD_OFFSET,
    SPI_WRITE_INTER_DWORD_OFFSET,
    SDDW_INITIAL_OFFSET = 21,
    SDDW_INTRA_DWORD_OFFSET,
    SDDW_INTER_DWORD_OFFSET,
    SDADW_INITIAL_OFFSET = 24,
    SDADW_INTRA_DWORD_OFFSET,
    SDADW_INTER_DWORD_OFFSET,
    SQDW_INITIAL_OFFSET = 27,
    SQDW_INTRA_DWORD_OFFSET,
    SQDW_INTER_DWORD_OFFSET,
    SQADW_INITIAL_OFFSET = 30,
    SQADW_INTRA_DWORD_OFFSET,
    SQADW_INTER_DWORD_OFFSET,
    SQI_FASTREAD_INITIAL_OFFSET = 33,
    SQI_FASTREAD_INTRA_DWORD_OFFSET,
    SQI_FASTREAD_INTER_DWORD_OFFSET,
    SQI_WRITE_INITIAL_OFFSET = 36,
    SQI_WRITE_INTRA_DWORD_OFFSET,
    SQI_WRITE_INTER_DWORD_OFFSET,
} SET_CFG_DATA_BYTE_ORDER_T;

// *****************************************************************************
/* EtherCAT UNIT32 bit type

  Summary:
    Definition to represent an EtherCAT 32 bit type

  Description:
    This type describes the EtherCAT 32bit type.

  Remarks:
    None.
*/
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

}UINT32_VAL;


void    ECAT_SyncInterruptsInitialization(void);
void    ECAT_Lan9255_IsPDIFunctional(uint8_t *pbyData);
<#if DRV_LAN9255_PROTOCOL == "SPI">
void    _ECAT_ChipSelectDisable(void);
void    _ECAT_ChipSelectEnable(void);
</#if>
uint16_t	ECAT_PDI_TimerGet(void);
void	ECAT_PDI_TimerClear(void);
void    ECAT_PDI_TimerInterrupt(void);
void    ECAT_ESCIRQInitialization(void);
void	ECAT_PDI_TimerStop(void);
void	ECAT_PDI_TimerStart(void);
void	ECAT_HWSetlED(uint8_t RunLed, uint8_t ErrLed);
void	ECAT_Initialization(void);
void    CRITICAL_SECTION_ENTER(void);
void    CRITICAL_SECTION_LEAVE(void);
<#if DRV_LAN9255_PROTOCOL == "SQI">
void	ECAT_SQI_SetConfiguration(uint8_t *byDummyByteCnt);
void    ECAT_SQI_SetCfg_dataInit(void);
void    ECAT_SQI_EnableQuadMode(void);
void    ECAT_SQI_DisableQuadMode(void);
<#elseif DRV_LAN9255_PROTOCOL == "SPI">
void	ECAT_SPI_SetConfiguration(uint8_t *byDummyByteCnt);
void    ECAT_SPI_SetCfg_dataInit(void);
void    ECAT_SPI_DisableQuadMode(void);
</#if>
// *****************************************************************************
/* Function:
    void ECAT_Util_Initialize(const unsigned short int drvIndex,  const void * const init)

  Summary:
    Initializes the LAN9255 Driver Utils

  Description:
    This routine initializes the LAN9255 driver making it ready for
    clients to open and use. The initialization data is specified by the init
    parameter. It is a single instance driver, so this API should be called
    only once.

  Precondition:
    None.

  Parameters:
    drvIndex - Identifier for the instance to be initialized.

    init  - Pointer to the init data structure containing any data necessary to
            initialize the driver utility adapter .

  Returns:
    None
*/

void ECAT_Util_Initialize(const unsigned short int drvIndex,  const void * const init);
<#if DRV_LAN9255_PROTOCOL == "SPI">
<#if DRV_LAN9255_SPI_MODE_ACCESS_TYPE 		== "ETHERCAT_SPI_BECKHOFF_MODE_ACCESS">
	/* Function Prototypes */
	void ECAT_Lan9255_Beckhoff_SPIWrite(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
	void ECAT_Lan9255_Beckhoff_SPIRead(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
<#elseif DRV_LAN9255_SPI_MODE_ACCESS_TYPE 	== "ETHERCAT_SPI_DIRECT_MODE_ACCESS">	
	/* Function Prototypes */
	void ECAT_Lan9255_SPIWrite(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
	void ECAT_Lan9255_SPIRead(uint16_t wAddr, uint8_t *pbydata, uint32_t dwLength);
	void ECAT_Lan9255_SPIFastRead(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
<#elseif DRV_LAN9255_SPI_MODE_ACCESS_TYPE 	== "ETHERCAT_SPI_INDIRECT_MODE_ACCESS">
	/* Function Prototypes */
	void ECAT_Lan925x_SPIWrite(uint16_t wAdddr, uint8_t *pbyData, uint8_t wLen);
	void ECAT_Lan925x_SPIRead(uint16_t wAddr, uint8_t *pbyData, uint8_t wLen);
	void ECAT_Lan925x_SPIFastRead(uint16_t wAddr, uint8_t *pbyData, uint8_t wLen);
	void ECAT_Lan925x_SPIReadPDRAM(uint8_t *pbyData, uint16_t wAddr, uint16_t wLen);
	void ECAT_Lan925x_SPIFastReadPDRAM(uint8_t *pbyData, uint16_t wAddr, uint16_t wLen);
	void ECAT_Lan925x_SPIWritePDRAM(uint8_t *pbyData, uint16_t wAddr, uint16_t wLen);
</#if>
<#elseif DRV_LAN9255_PROTOCOL == "SQI">
<#if DRV_LAN9255_SQI_MODE_ACCESS_TYPE 		== "ETHERCAT_SQI_DIRECT_MODE_ACCESS">
	/* Function Prototypes */
	void    ECAT_LAN925x_SQIWrite(uint16_t u16Addr, uint8_t *pu8Data, uint32_t u32Length);
	void    ECAT_LAN925x_SQIRead(uint16_t u16Addr, uint8_t *pu8data, uint32_t u32Length);
<#elseif DRV_LAN9255_SQI_MODE_ACCESS_TYPE 		== "ETHERCAT_SQI_INDIRECT_MODE_ACCESS">
	/* Function Prototypes */
	void    ECAT_LAN925x_SQIWrite(uint16_t u16Adddr, uint8_t *pu8Data, uint8_t u8Len);
	void    ECAT_LAN925x_SQIRead(uint16_t u16Addr, uint8_t *pu8Data, uint8_t u8Len);
	void    ECAT_LAN925x_SQIFastRead(uint16_t u16Addr, uint8_t *pu8Data, uint8_t u8Len);
	void    ECAT_LAN925x_SQIReadPDRAM(uint8_t *pu8Data, uint16_t u16Addr, uint16_t u16Len);
	void    ECAT_LAN925x_SQIFastReadPDRAM(uint8_t *pu8Data, uint16_t u16Addr, uint16_t u16Len);
	void    ECAT_LAN925x_SQIWritePDRAM(uint8_t *pu8Data, uint16_t u16Addr, uint16_t u16Len);
</#if>
</#if>
#ifdef __cplusplus
}
#endif

#endif /* DRV_LAN9255_ECAT_UTIL_H */
