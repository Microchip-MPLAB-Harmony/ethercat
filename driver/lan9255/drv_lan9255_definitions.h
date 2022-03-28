/*******************************************************************************
  LAN 9252 Driver Definitions Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_lan9255_definitions.h

  Summary:
    LAN9255 Driver Definitions Header File

  Description:
    This file provides implementation-specific definitions for the LAN9255 
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

#ifndef DRV_LAN9255_DEFINITIONS_H
#define DRV_LAN9255_DEFINITIONS_H

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


// *****************************************************************************
// *****************************************************************************
// Section: SPI or SQI peripheral Data Types
// *****************************************************************************
// *****************************************************************************

typedef void (* DRV_LAN9255_SPI_PLIB_CALLBACK)( uintptr_t );

typedef bool (* DRV_LAN9255_SPI_PLIB_WRITE_READ)(void*, size_t, void *, size_t);

typedef bool (* DRV_LAN9255_SPI_PLIB_WRITE)(void*, size_t);

typedef bool (* DRV_LAN9255_SPI_PLIB_READ)(void*, size_t);

typedef bool (* DRV_LAN9255_SPI_PLIB_IS_BUSY)(void);

typedef void (* DRV_LAN9255_SPI_PLIB_CALLBACK_REGISTER)(DRV_LAN9255_SPI_PLIB_CALLBACK, uintptr_t);

// *****************************************************************************
/* LAN9252 Driver PLIB Interface Data

  Summary:
    Defines the data required to initialize the LAN9255 driver PLIB Interface.

  Description:
    This data type defines the data required to initialize the LAN9252 driver
    PLIB Interface.

  Remarks:
    None.
*/

typedef struct
{

    /* LAN9255 PLIB SPI or SQI writeRead API */
    DRV_LAN9255_SPI_PLIB_WRITE_READ                spiWriteRead;

    /* LAN9255 PLIB SPI or SQI write API */
    DRV_LAN9255_SPI_PLIB_WRITE                     spiWrite;

    /* LAN9255 PLIB SPI or SQI read API */
    DRV_LAN9255_SPI_PLIB_READ                      spiRead;

    /* LAn9255 PLIB SPI or SQI Transfer status API */
    DRV_LAN9255_SPI_PLIB_IS_BUSY                   spiIsBusy;

    /* LAN9255 PLIB SPI or SQI callback register API */
    DRV_LAN9255_SPI_PLIB_CALLBACK_REGISTER         spiCallbackRegister;

} DRV_LAN9255_UTIL_SPI_PLIB_INTERFACE;

// *****************************************************************************
// *****************************************************************************
// Section:Timer peripheral Data Types
// *****************************************************************************
// *****************************************************************************

typedef void (* DRV_LAN9255_TMR_PLIB_CALLBACK)( uintptr_t );
typedef void (* DRV_LAN9255_TMR_PLIB_CALLBACK_REGISTER)(DRV_LAN9255_TMR_PLIB_CALLBACK, uintptr_t);
typedef void (* DRV_LAN9255_TMR_PLIB_START)(void);
typedef void (* DRV_LAN9255_TMR_PLIB_STOP)(void);
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
	  /* LAN9255 PLIB Timer callback register API */
    DRV_LAN9255_TMR_PLIB_CALLBACK_REGISTER    timerCallbackSet;
	
    /* LAN9255 PLIB Timer Start API */
    DRV_LAN9255_TMR_PLIB_START                timerStart;
    
   /* LAN9255 PLIB Timer Stop API */
    DRV_LAN9255_TMR_PLIB_STOP               timerStop;

} DRV_LAN9255_UTIL_TMR_PLIB_INTERFACE;


// *****************************************************************************
/* LAN9255 Driver Util Initialization Data

  Summary:
    Defines the data required to initialize the LAN9252 driver Util

  Description:
    This data type defines the data required to initialize or the LAN9252 driver Util.

  Remarks:
    None.
*/

typedef struct
{
    /* Identifies the SPI or SQI PLIB API set to be used by the driver to access the
     * peripheral. */
    const DRV_LAN9255_UTIL_SPI_PLIB_INTERFACE*      spiPlib;    

    /* Identifies the Timer PLIB API set to be used by the driver to access the
     * peripheral. */
    const DRV_LAN9255_UTIL_TMR_PLIB_INTERFACE*      timerPlib;

} DRV_LAN9255_UTIL_INIT;


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // #ifndef DRV_LAN9255_DEFINITIONS_H

/*******************************************************************************
 End of File
*/
