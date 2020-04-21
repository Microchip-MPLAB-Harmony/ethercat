/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9252_ecat_util.h

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

#ifndef DRV_LAN9252_ECAT_UTIL_H
#define DRV_LAN9252_ECAT_UTIL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include "definitions.h"
#include "ecat_def.h"
#include "drv_lan9252_definitions.h"

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
#define ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY 		${DRV_LAN9252_INT_PRIO}


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
 // *****************************************************************************
/* DRV_LAN9252 Util Transfer Status

 Summary:
    Defines the data type for LAN9252 Driver Util transfer status.

 Description:
    This will be used to indicate the current transfer status of the
    LAN9252 driver operations.

 Remarks:
    None.
*/

typedef enum
{
    /* Transfer is being processed */
    DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_BUSY,

    /* Transfer is successfully completed*/
    DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_COMPLETED,

    /* Transfer had error */
    DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS_ERROR

} DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS;       
        
// *****************************************************************************
/* LAN9252 Driver Utility Instance Object

  Summary:
    Object used to keep any data required for an instance of the LAN9252 driver.

  Description:
    None.

  Remarks:
    None.
*/

typedef struct
{
    /* Flag to indicate this object is in use  */
    bool                                   inUse;

    /* SPI PLIB API list that will be used by the driver to access the hardware */
    const DRV_LAN9252_UTIL_SPI_PLIB_INTERFACE*  spiPlib;

    /* SPI peripheral communication transfer status */
    volatile DRV_LAN9252_UTIL_SPI_TRANSFER_STATUS       spiTransferStatus;
	
	/* Timer PLIB API list that will be used by the driver to access the hardware */
    const DRV_LAN9252_UTIL_TMR_PLIB_INTERFACE*  timerPlib;
	

} DRV_LAN9252_UTIL_OBJ;


/* The following data type definitions are used by the functions in this
    interface and should be considered part of it.
*/
/* Global Data Types */
/* EtherCAT SPI Read Command */
#define CMD_SERIAL_READ 0x03
/* EtherCAT SPI Fast read command */
#define CMD_FAST_READ 0x0B
#define CMD_DUAL_OP_READ 0x3B
#define CMD_DUAL_IO_READ 0xBB
#define CMD_QUAD_OP_READ 0x6B
#define CMD_QUAD_IO_READ 0xEB
        
/* EtherCAT SPI Write Command */
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

} UINT32_VAL;


void    PDI_IRQ_Interrupt(void);

void    ethercat_chipSelectSet(void);
void    ethercat_chipSelectClear(void);
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
void CRITICAL_SECTION_ENTER(void);
void CRITICAL_SECTION_LEAVE(void);

// *****************************************************************************
/* Function:
    void ECAT_Util_Initialize(const unsigned short int drvIndex,  const void * const init)

  Summary:
    Initializes the LAN9252 Driver Utils

  Description:
    This routine initializes the LAN9252 driver making it ready for
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


#ifdef __cplusplus
}
#endif

#endif /* DRV_LAN9252_ECAT_UTIL_H */
