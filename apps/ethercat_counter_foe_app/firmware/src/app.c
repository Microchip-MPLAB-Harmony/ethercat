/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ethercat_configuration.h"
#include "drv_lan9252_ecat_util.h"
#include "slave_stack/applInterface.h"
#include "slave_stack/sample_app.h"

#if defined(ETHERCAT_USE_FOE)  
//-----------------------------------------------------------------------------
const uint32_t gAppBankBOffsetAddr = APP_NVM_BANKB_START_ADDRESS;
// Bank B application need to started from this below location.
const uint32_t gApplicationStartAddr   = (uint32_t)(APP_NVM_BANKB_START_ADDRESS + APP_BOOTLOADER_SIZE);

void APP_FlashWrite( uint32_t startAddress,uint8_t *flash_data )
{
    uint32_t   flashStartAddress=0;
    int         pageCnt=0;
    
    flashStartAddress = gAppBankBOffsetAddr+startAddress;
	while(NVMCTRL_IsBusy()){}

    /* Erase the block */
    NVMCTRL_BlockErase((uint32_t)flashStartAddress);

    while(NVMCTRL_IsBusy()){}

    for (pageCnt=0; pageCnt < APP_PAGES_IN_ERASE_BLOCK; pageCnt++)
	{
        /* If write mode is manual, */
        /* Program 512 byte page */
        NVMCTRL_PageWrite((uint32_t *)flash_data, (uint32_t)flashStartAddress);
        while(NVMCTRL_IsBusy()){}

        flash_data = flash_data + APP_PAGE_SIZE;
        flashStartAddress = flashStartAddress + APP_PAGE_SIZE;        
	}
}

static void APP_BankSwitch(void)
{
	/* wait until it is ready to accept a new command */
	while (NVMCTRL_IsBusy());
	
	/* execute the command BKSWRST (Bank swap and system reset) */
	NVMCTRL_BankSwap();
	
	/* wait until command done */
	while (NVMCTRL_IsBusy());    
    
	/* At the end of this command, code starts running from 0x00000 upon reset*/
}

//-----------------------------------------------------------------------------
static void APP_RunApplication(void)
{
	uint32_t msp = *(uint32_t *)(gApplicationStartAddr);
	uint32_t reset_vector = *(uint32_t *)(gApplicationStartAddr + 4);

	if (0xffffffff == msp)
    {
        return;
    }

	__set_MSP(msp);

	asm("bx %0"::"r" (reset_vector));
}
#endif

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;   
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {
                // EtherCAT Initialization after NVIC initialization
                EtherCATInit();
                MainInit();
#if defined(ETHERCAT_USE_FOE)  
                BL_FOE_Application();
                
                bRunApplication = TRUE;
#endif                
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            do
            {		
#if defined(ETHERCAT_USE_FOE)                
                if(APP_FW_GetDownloadStateFinished() == 1)
                {
                    APP_BankSwitch();
                    APP_RunApplication();
                }
#endif                
                MainLoop();

            } while (bRunApplication == TRUE);
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            break;
        }
    }
}
/*******************************************************************************
 End of File
 */
