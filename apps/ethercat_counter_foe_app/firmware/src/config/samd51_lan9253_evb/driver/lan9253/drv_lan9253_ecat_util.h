/*******************************************************************************
 EtherCAT header file for Microchip EtherCAT interfacing
 
 Company
    Microchip Technology Inc.

  File Name
    drv_lan9253_ecat_util.h

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

#ifndef DRV_LAN9253_ECAT_UTIL_H
#define DRV_LAN9253_ECAT_UTIL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/
#include "definitions.h"
#include "ecat_def.h"
#include "drv_lan9253_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
  /* __NVIC_PRIO_BITS will be specified when CMSIS is being used. */
  #define ETHERCAT_INT_PRIO_BITS     __NVIC_PRIO_BITS
#else
  /* 15 priority levels */
  #define ETHERCAT_INT_PRIO_BITS     3
#endif

/* This is the lowest interrupt priority for SAM devices and Highest for PIC32M devices that can be used in a call to a "set priority" function. */
#define ETHERCAT_CONFIG_MAX_INTERRUPT_PRIORITY 		4



/* EtherCAT SPI Indirect mode enabled */
#define ETHERCAT_SPI_INDIRECT_MODE_ACCESS				1

#define MCHP_ESF_IS_PDI_FUNCTIONAL(pdata)		ECAT_Lan9253_IsPDIFunctional(pdata)

#ifdef ETHERCAT_SPI_INDIRECT_MODE_ACCESS
/* SPI Indirect mode Access. These APIs can also be used for LAN9252 device */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_Lan925x_SPIWrite(addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)             ECAT_Lan925x_SPIRead(addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata, len)         ECAT_Lan925x_SPIFastRead(addr, pdata, len)
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		ECAT_Lan925x_SPIReadPDRAM(pdata, addr, len)
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	ECAT_Lan925x_SPIFastReadPDRAM(pdata, addr, len)
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)		ECAT_Lan925x_SPIWritePDRAM(pdata, addr, len)

#elif ETHERCAT_SPI_DIRECT_MODE_ACCESS
/* SPI Direct mode Access */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_Lan9253_SPIWrite (addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)             ECAT_Lan9253_SPIRead (addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata, len)			ECAT_Lan9253_SPIFastRead (addr, pdata, len)
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)		

#elif ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
/* SPI Beckhoff mode Access */
#define MCHP_ESF_PDI_WRITE(addr, pdata, len)            ECAT_Lan9253_Beckhoff_SPIWrite (addr, pdata, len)
#define MCHP_ESF_PDI_READ(addr, pdata, len)				ECAT_Lan9253_Beckhoff_SPIRead (addr, pdata, len)
#define MCHP_ESF_PDI_FASTREAD(addr, pdata)			
#define MCHP_ESF_PDI_READ_PDRAM(pdata, addr, len)		
#define MCHP_ESF_PDI_FASTREAD_PDRAM(pdata, addr, len)	
#define MCHP_ESF_PDI_WRITE_PDRAM(pdata, addr, len)
#endif

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
        
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
 // *****************************************************************************
/* DRV_LAN9253 Util Transfer Status

 Summary:
    Defines the data type for LAN9253 Driver Util transfer status.

 Description:
    This will be used to indicate the current transfer status of the
    LAN9253 driver operations.

 Remarks:
    None.
*/

typedef enum
{
    /* Transfer is being processed */
    DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_BUSY,

    /* Transfer is successfully completed*/
    DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_COMPLETED,

    /* Transfer had error */
    DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS_ERROR

} DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS;       
        
// *****************************************************************************
/* LAN9253 Driver Utility Instance Object

  Summary:
    Object used to keep any data required for an instance of the LAN9253 driver.

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
    const DRV_LAN9253_UTIL_SPI_PLIB_INTERFACE*  spiPlib;

    /* SPI peripheral communication transfer status */
    volatile DRV_LAN9253_UTIL_SPI_TRANSFER_STATUS       spiTransferStatus;
	
	/* Timer PLIB API list that will be used by the driver to access the hardware */
    const DRV_LAN9253_UTIL_TMR_PLIB_INTERFACE*  timerPlib;
	

} DRV_LAN9253_UTIL_OBJ;


/* The following data type definitions are used by the functions in this
    interface and should be considered part of it.
*/
/* Global Data Types */
#define CMD_SPI_SETCFG                      0x01
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
#define CMD_QUAD_ADDR_DARA_WRITE            0xE2

#define CMD_SERIAL_READ_DUMMY               0
#define CMD_FAST_READ_DUMMY                 1
#define CMD_DUAL_OP_READ_DUMMY              1
#define CMD_DUAL_IO_READ_DUMMY              2
#define CMD_QUAD_OP_READ_DUMMY              1
#define CMD_QUAD_IO_READ_DUMMY              4
#define CMD_SERIAL_WRITE_DUMMY              0
#define CMD_DUAL_DATA_WRITE_DUMMY           0
#define CMD_DUAL_ADDR_DATA_WRITE_DUMMY      0
#define CMD_QUAD_DATA_WRITE_DUMMY           0
#define CMD_QUAD_ADDR_DARA_WRITE_DUMMY      0

#define ESC_CSR_CMD_REG                     0x304
#define ESC_CSR_DATA_REG                    0x300
#define ESC_WRITE_BYTE                      0x80
#define ESC_READ_BYTE                       0xC0
#define ESC_CSR_BUSY                        0x80

#define SETCFG_MAX_DATA_BYTES                       39
#define ONE_BYTE_MAX_XFR_LEN                        127

#define WAIT_STATE_BYTE                             0xFF
#define READ_TERMINATION_BYTE                       0xFF

#define DWORD_LENGTH                                4

/* Wait signal states */
#define WAIT                                        FALSE
#define ACK                                         TRUE

/* Beckhoff SPI Commands */
#define ESC_RD                                      0x02
#define ESC_WR                                      0x04
#define ESC_RD_WAIT_STATE                           0x03

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
void    ECAT_Lan9253_IsPDIFunctional(uint8_t *pbyData);
void    _ECAT_ChipSelectDisable(void);
void    _ECAT_ChipSelectEnable(void);
UINT16	ECAT_PDI_TimerGet();
void	ECAT_PDI_TimerClear(void);
void    ECAT_PDI_TimerInterrupt(void);
void    ECAT_ESCIRQInitialization();
void	ECAT_PDI_TimerStop(void);
void	ECAT_PDI_TimerStart(void);
void	ECAT_HWSetlED(UINT8 RunLed, UINT8 ErrLed);
void	ECAT_Initialization();
void    CRITICAL_SECTION_ENTER(void);
void    CRITICAL_SECTION_LEAVE(void);

// *****************************************************************************
/* Function:
    void ECAT_Util_Initialize(const unsigned short int drvIndex,  const void * const init)

  Summary:
    Initializes the LAN9253 Driver Utils

  Description:
    This routine initializes the LAN9253 driver making it ready for
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


#ifdef ETHERCAT_SPI_BECKHOFF_MODE_ACCESS
	/* Function Prototypes */
	void ECAT_Lan9253_Beckhoff_SPIWrite(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
	void ECAT_Lan9253_Beckhoff_SPIRead(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
#endif
#ifdef ETHERCAT_SPI_DIRECT_MODE_ACCESS
	/* Function Prototypes */
	void ECAT_Lan9253_SPIWrite(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
	void ECAT_Lan9253_SPIRead(uint16_t wAddr, uint8_t *pbydata, uint32_t dwLength);
	void ECAT_Lan9253_SPIFastRead(uint16_t wAddr, uint8_t *pbyData, uint32_t dwLength);
#endif
#ifdef ETHERCAT_SPI_INDIRECT_MODE_ACCESS
	/* Function Prototypes */
	void ECAT_Lan925x_SPIWrite(uint16_t wAdddr, uint8_t *pbyData, uint8_t wLen);
	void ECAT_Lan925x_SPIRead(uint16_t wAddr, uint8_t *pbyData, uint8_t wLen);
	void ECAT_Lan925x_SPIFastRead(uint16_t wAddr, uint8_t *pbyData, uint8_t wLen);
	void ECAT_Lan925x_SPIReadPDRAM(UINT8 *pbyData, UINT16 wAddr, UINT16 wLen);
	void ECAT_Lan925x_SPIFastReadPDRAM(UINT8 *pbyData, UINT16 wAddr, UINT16 wLen);
	void ECAT_Lan925x_SPIWritePDRAM(UINT8 *pbyData, UINT16 wAddr, UINT16 wLen);
#endif

#ifdef __cplusplus
}
#endif

#endif /* DRV_LAN9253_ECAT_UTIL_H */
