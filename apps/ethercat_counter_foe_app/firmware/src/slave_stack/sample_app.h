/*******************************************************************************
  Sample application header file 

  Company:
    Microchip Technology Inc.

  File Name:
    sample_app.h

  Summary:
    This header file is the list of prototypes and defines

  Description:
    This header file is modified for PDO input and output processing. 
	This file can be replaced with the generated sample file by the SSC tool.
*******************************************************************************/

/*****************************************************************************
 Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.

Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software 
and any derivatives exclusively with Microchip products. It is your 
responsibility to comply with third party license terms applicable to your 
use of third party software (including open source software) that may 
accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR 
PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE 
FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN 
ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************/
/**
\file sample_app.h
\brief sample_app function prototypes and defines

\version 1.0.0.11
 */

 
 #ifndef _SAMPLE_APP_H_
#define _SAMPLE_APP_H_


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#include "ecatappl.h"

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/

#endif //_SAMPLE_APP_H_

//include custom application object dictionary 
#include "sample_appObjects.h"


#if defined(_SAMPLE_APP_) && (_SAMPLE_APP_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif


PROTO void APPL_Application(void);
#if EXPLICIT_DEVICE_ID
PROTO UINT16 APPL_GetDeviceID(void);
#endif

void BL_FOE_Application(void);
void APP_FW_startDownload(void);
void APP_FW_StateBOOTtoINIT(void);
BOOL APP_FW_GetDownloadStateFinished(void);

#if (ETHERCAT_EEPROM_EMULATION_SUPPORT == true)
#define ESC_EEPROM_CONTROL_OFFSET                 0x0502
/**
EEPROM_READ_SIZE: Only required if EEPROM emulation is active. This value defines the number of bytes which will be read per opertion. */
#ifndef EEPROM_READ_SIZE
#define EEPROM_READ_SIZE                          0x8
#endif

/*Number of bytes - needs to be calculate checksum*/
#define ESC_EEPROM_CONFIG_BYTES                   0x0E

PROTO void Emulation_Init();
#endif

PROTO UINT16 HW_EepromReload ();
PROTO UINT16 HW_EepromWrite(UINT32 wordaddr);

PROTO void   APPL_AckErrorInd(UINT16 stateTrans);
PROTO UINT16 APPL_StartMailboxHandler(void);
PROTO UINT16 APPL_StopMailboxHandler(void);
PROTO UINT16 APPL_StartInputHandler(UINT16 *pIntMask);
PROTO UINT16 APPL_StopInputHandler(void);
PROTO UINT16 APPL_StartOutputHandler(void);
PROTO UINT16 APPL_StopOutputHandler(void);

PROTO UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize);
PROTO void APPL_InputMapping(UINT16* pData);
PROTO void APPL_OutputMapping(UINT16* pData);

#undef PROTO
/** @}*/

