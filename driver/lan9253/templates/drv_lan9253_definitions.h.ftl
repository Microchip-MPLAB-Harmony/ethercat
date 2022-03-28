/*******************************************************************************
  LAN 9253 Driver Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_lan9253_definitions.h

  Summary:
    LAN9252 Driver Definitions Header File

  Description:
    This file provides implementation-specific definitions for the LAN9253 
	driver's system interface.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_LAN9253_DEFINITIONS_H
#define DRV_LAN9253_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <device.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

<#if DRV_LAN9253_PROTOCOL == "SPI">
// *****************************************************************************
// *****************************************************************************
// Section: SPI peripheral Data Types
// *****************************************************************************
// *****************************************************************************

typedef void (* DRV_LAN9253_SPI_PLIB_CALLBACK)( uintptr_t );

typedef bool (* DRV_LAN9253_SPI_PLIB_WRITE_READ)(void*, size_t, void *, size_t);

typedef bool (* DRV_LAN9253_SPI_PLIB_WRITE)(void*, size_t);

typedef bool (* DRV_LAN9253_SPI_PLIB_READ)(void*, size_t);

typedef bool (* DRV_LAN9253_SPI_PLIB_IS_BUSY)(void);

typedef void (* DRV_LAN9253_SPI_PLIB_CALLBACK_REGISTER)(DRV_LAN9253_SPI_PLIB_CALLBACK, uintptr_t);

// *****************************************************************************
/* LAN9253 Driver SPI PLIB Interface Data

  Summary:
    Defines the data required to initialize the LAN9253 driver SPI PLIB Interface.

  Description:
    This data type defines the data required to initialize the LAN9252 driver SPI
    PLIB Interface.

  Remarks:
    None.
*/

typedef struct
{

    /* LAN9253 PLIB SPI writeRead API */
    DRV_LAN9253_SPI_PLIB_WRITE_READ                spiWriteRead;

    /* LAN9253 PLIB SPI or SQI write API */
    DRV_LAN9253_SPI_PLIB_WRITE                     spiWrite;

    /* LAN9253 PLIB SPI or SQI read API */
    DRV_LAN9253_SPI_PLIB_READ                      spiRead;

    /* LAn9253 PLIB SPI or SQI Transfer status API */
    DRV_LAN9253_SPI_PLIB_IS_BUSY                   spiIsBusy;

    /* LAN9253 PLIB SPI or SQI callback register API */
    DRV_LAN9253_SPI_PLIB_CALLBACK_REGISTER         spiCallbackRegister;

} DRV_LAN9253_UTIL_SPI_PLIB_INTERFACE;

<#elseif DRV_LAN9253_PROTOCOL == "SQI">
// *****************************************************************************
/* LAN9253 Driver SQI PLIB Interface Data

  Summary:
    Defines the data required to initialize the LAN9253 driver SQI PLIB Interface.

  Description:
    This data type defines the data required to initialize the LAN9252 driver
    PLIB Interface.

  Remarks:
    None.
*/

/* Pointer to write command to QSPI slave device. */
typedef bool (*DRV_LAN9253_SQI_PLIB_CMD_WRITE)( qspi_command_xfer_t *qspi_command_xfer, uint32_t address );

/* Pointer to read particular register of QSPI slave device. */
typedef bool (*DRV_LAN9253_SQI_PLIB_REG_READ)( qspi_register_xfer_t *qspi_register_xfer, uint32_t *rx_data, uint8_t rx_data_length );

/* Pointer to write to particular register of QSPI slave device. */
typedef bool (*DRV_LAN9253_SQI_PLIB_REG_WRITE)( qspi_register_xfer_t *qspi_register_xfer, uint32_t *tx_data, uint8_t tx_data_length );

/* Pointer to read from the specified address of the flash device. */
typedef bool (*DRV_LAN9253_SQI_PLIB_MEM_READ)( qspi_memory_xfer_t *qspi_memory_xfer, uint32_t *rx_data, uint32_t rx_data_length, uint32_t address );

/* Pointer to write to the specified address of the flash device. */
typedef bool (*DRV_LAN9253_SQI_PLIB_MEM_WRITE)( qspi_memory_xfer_t *qspi_memory_xfer, uint32_t *tx_data, uint32_t tx_data_length, uint32_t address );

typedef struct
{
    /* Pointer to write command to QSPI slave device. */
    DRV_LAN9253_SQI_PLIB_CMD_WRITE sqiCommandWrite;

    /* Pointer to read particular register of QSPI slave device. */
    DRV_LAN9253_SQI_PLIB_REG_READ sqiRegisterRead;

    /* Pointer to write to particular register of QSPI slave device. */
    DRV_LAN9253_SQI_PLIB_REG_WRITE sqiRegisterWrite;

    /* Pointer to read from the specified address of the flash device. */
    DRV_LAN9253_SQI_PLIB_MEM_READ sqiMemoryRead;

    /* Pointer to write to the specified address of the flash device. */
    DRV_LAN9253_SQI_PLIB_MEM_WRITE sqiMemoryWrite;

} DRV_LAN9253_UTIL_SQI_PLIB_INTERFACE;
</#if>
// *****************************************************************************
// *****************************************************************************
// Section:Timer peripheral Data Types
// *****************************************************************************
// *****************************************************************************

typedef void (* DRV_LAN9253_TMR_PLIB_CALLBACK)( uintptr_t );
typedef void (* DRV_LAN9253_TMR_PLIB_CALLBACK_REGISTER)(DRV_LAN9253_TMR_PLIB_CALLBACK, uintptr_t);
typedef void (* DRV_LAN9253_TMR_PLIB_START)(void);
typedef void (* DRV_LAN9253_TMR_PLIB_STOP)(void);
// *****************************************************************************
/* LAN9252 Driver Timer PLIB Interface Data

  Summary:
    Defines the data required to initialize the LAN9252 driver Timer PLIB Interface.

  Description:
    This data type defines the data required to initialize the LAN9252 driver
    Timer PLIB Interface.

  Remarks:
    None.
*/

typedef struct
{
	  /* LAN9253 PLIB Timer callback register API */
    DRV_LAN9253_TMR_PLIB_CALLBACK_REGISTER    timerCallbackSet;
	
    /* LAN9253 PLIB Timer Start API */
    DRV_LAN9253_TMR_PLIB_START                timerStart;
    
   /* LAN9253 PLIB Timer Stop API */
    DRV_LAN9253_TMR_PLIB_STOP               timerStop;

} DRV_LAN9253_UTIL_TMR_PLIB_INTERFACE;


// *****************************************************************************
/* LAN9253 Driver Util Initialization Data

  Summary:
    Defines the data required to initialize the LAN9253 driver Util

  Description:
    This data type defines the data required to initialize or the LAN9252 driver Util.

  Remarks:
    None.
*/

typedef struct
{
<#if DRV_LAN9253_PROTOCOL == "SPI">
    /* Identifies the SPI PLIB API set to be used by the driver to access the
     * peripheral. */
    const DRV_LAN9253_UTIL_SPI_PLIB_INTERFACE*      spiPlib;  
<#elseif DRV_LAN9253_PROTOCOL == "SQI">	
	 /* Identifies the SQI PLIB API set to be used by the driver to access the
     * peripheral. */
    const DRV_LAN9253_UTIL_SQI_PLIB_INTERFACE*      sqiPlib; 
</#if>
    /* Identifies the Timer PLIB API set to be used by the driver to access the
     * peripheral. */
    const DRV_LAN9253_UTIL_TMR_PLIB_INTERFACE*      timerPlib;

} DRV_LAN9253_UTIL_INIT;


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_LAN9253_DEFINITIONS_H

/*******************************************************************************
 End of File
*/
