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
#include "ecat_main.h"
#include "slave_stack/applInterface.h"
#include "slave_stack/sample_app.h"

#define APP_STATUS_LED0       PIN_PC18

// for LED0 ON and OFF
#define PC18  GPIO(GPIO_PORTC,18)
//-----------------------------------------------------------------------------
#define BOOTLOADER_SIZE       0
#define NVM_START_ADDRESS     (0x80000)
#define PAGE_SIZE             FLASH_PAGE_SIZE
#define ERASE_BLOCK_SIZE      8192
#define PAGES_IN_ERASE_BLOCK  ERASE_BLOCK_SIZE/PAGE_SIZE

#define DATA_SIZE             ERASE_BLOCK_SIZE
#define WORDS(x)              ((int)((x) / sizeof(uint32_t)))


static uint32_t flash_addr = NVM_START_ADDRESS;
volatile uint32_t APPLICATION_START   =  (FLASH_ADDR + BOOTLOADER_SIZE);

void flash_write_task(UINT32 startAddress,UINT8 *flash_data)
{
	uint32_t *dst = (uint32_t *)(flash_addr+startAddress);
	uint32_t *src = (uint32_t *)flash_data;

	NVMCTRL_REGS->NVMCTRL_ADDR = flash_addr+startAddress;

	// Lock region size is always bigger than the row size
	NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_UR;
	while(NVMCTRL_IsBusy());

	NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_EB;
	while(NVMCTRL_IsBusy());

	for (int page = 0; page < PAGES_IN_ERASE_BLOCK; page++)
	{
		for (int i = 0; i < WORDS(PAGE_SIZE); i++)
			dst[i] = src[i];
		
		NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_CMDEX_KEY | NVMCTRL_CTRLB_CMD_WP;
		while(NVMCTRL_IsBusy());

		src += WORDS(PAGE_SIZE);
		dst += WORDS(PAGE_SIZE);
	}
}

static void APP_BankSwitch(void)
{
	/* wait until it is ready to accept a new command */
	while (NVMCTRL_IsBusy());
	
	/* execute the command BKSWRST (Bank swap and system reset) */
	NVMCTRL_BankSwap();
	
	/* wait until command done */
	//while (NVMCTRL_IsBusy());
	/* At the end of this command, code starts running from 0x00000 upon reset*/
}

//-----------------------------------------------------------------------------
static void run_application(void)
{
	uint32_t msp = *(uint32_t *)(APPLICATION_START);
	uint32_t reset_vector = *(uint32_t *)(APPLICATION_START + 4);

	if (0xffffffff == msp)
	return;

	__set_MSP(msp);

	asm("bx %0"::"r" (reset_vector));
}
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

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
    EtherCATInit();
    MainInit();

	BL_FOE_Application();
	
	bRunApplication = TRUE;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
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

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            // Set pin direction to output to LED0 Turn ON
            //gpio_set_pin_direction(PC18, GPIO_DIRECTION_OUT);
            do
            {		
                if(APP_FW_GetDownloadStateFinished() == 1)
                {
                    APP_BankSwitch();
                    run_application();
                }
                MainLoop();

            } while (bRunApplication == TRUE);
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
