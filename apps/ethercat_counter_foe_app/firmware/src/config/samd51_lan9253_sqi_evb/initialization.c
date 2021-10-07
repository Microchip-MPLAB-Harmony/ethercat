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

/* SQI PLIB Interface Initialization for ethercat LAN driver */
const DRV_LAN9253_UTIL_SQI_PLIB_INTERFACE drvLAN9253SQIPlibAPI = {
	/* SQI PLIB Command Write function */
	.sqiCommandWrite = (DRV_LAN9253_SQI_PLIB_CMD_WRITE)QSPI_CommandWrite,

	/* SQI PLIB Register Read function */
	.sqiRegisterRead = (DRV_LAN9253_SQI_PLIB_REG_READ)QSPI_RegisterRead,
	
	/* SQI PLIB Register Write function */
	.sqiRegisterWrite = (DRV_LAN9253_SQI_PLIB_REG_WRITE)QSPI_RegisterWrite,

	/* SQI PLIB Memory Read Transfer Status function */
	.sqiMemoryRead = (DRV_LAN9253_SQI_PLIB_MEM_READ)QSPI_MemoryRead,

	/* SQI PLIB Callback Register */
	.sqiMemoryWrite = (DRV_LAN9253_SQI_PLIB_MEM_WRITE)QSPI_MemoryWrite,
};
const DRV_LAN9253_UTIL_TMR_PLIB_INTERFACE drvLAN9253TimerPlibAPI = {

	.timerCallbackSet = (DRV_LAN9253_TMR_PLIB_CALLBACK_REGISTER)TC0_TimerCallbackRegister,
	
	.timerStart = (DRV_LAN9253_TMR_PLIB_START)TC0_TimerStart,
	
	.timerStop = (DRV_LAN9253_TMR_PLIB_STOP)TC0_TimerStop,
};

/* LAN9253 Driver Initialization Data */
const DRV_LAN9253_UTIL_INIT drvLAN9253InitData = {
	/* SPI PLIB API  interface*/
	.sqiPlib = &drvLAN9253SQIPlibAPI,
	
	/* Timer PLIB API Interface */
	.timerPlib = &drvLAN9253TimerPlibAPI,

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



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
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


	ECAT_Util_Initialize(0, (void *)&drvLAN9253InitData);



    NVIC_Initialize();

}


/*******************************************************************************
 End of File
*/
