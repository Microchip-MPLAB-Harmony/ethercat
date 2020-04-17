/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "definitions.h"
#include "device.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="EtherCAT Main Initialization Data">

/* SPI or SQI PLIB Interface Initialization for ethercat LAN driver */
const DRV_LAN9252_UTIL_SPI_PLIB_INTERFACE drvLAN9252SpiPlibAPI = {

	/* SPI PLIB WriteRead function */
	.spiWriteRead = (DRV_LAN9252_SPI_PLIB_WRITE_READ)QSPI_WriteRead,

	/* SPI PLIB Write function */
	.spiWrite = (DRV_LAN9252_SPI_PLIB_WRITE)QSPI_Write,

	/* SPI PLIB Read function */
	.spiRead = (DRV_LAN9252_SPI_PLIB_READ)QSPI_Read,

	/* SPI PLIB Transfer Status function */
	.spiIsBusy = (DRV_LAN9252_SPI_PLIB_IS_BUSY)QSPI_IsBusy,

	/* SPI PLIB Callback Register */
	.spiCallbackRegister = (DRV_LAN9252_SPI_PLIB_CALLBACK_REGISTER)QSPI_CallbackRegister,
};

const DRV_LAN9252_UTIL_TMR_PLIB_INTERFACE drvLAN9252TimerPlibAPI = {

	.timerCallbackSet = (DRV_LAN9252_TMR_PLIB_CALLBACK_REGISTER)TC0_TimerCallbackRegister,
	
	.timerStart = (DRV_LAN9252_TMR_PLIB_START)TC0_TimerStart,
	
	.timerStop = (DRV_LAN9252_TMR_PLIB_STOP)TC0_TimerStop,
};

/* LAN9252 Driver Initialization Data */
const DRV_LAN9252_UTIL_INIT drvLAN9252InitData = {

	/* SPI PLIB API  interface*/
	.spiPlib = &drvLAN9252SpiPlibAPI,
	
	/* Timer PLIB API Interface */
	.timerPlib = &drvLAN9252TimerPlibAPI,

};

// </editor-fold>


// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************



/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    NVMCTRL_Initialize( );

  
    PORT_Initialize();

    CLOCK_Initialize();




    QSPI_Initialize();

    EVSYS_Initialize();

    EIC_Initialize();

    TC0_TimerInitialize();


	ECAT_Util_Initialize(0, (void *)&drvLAN9252InitData);


    NVIC_Initialize();

}


/*******************************************************************************
 End of File
*/
