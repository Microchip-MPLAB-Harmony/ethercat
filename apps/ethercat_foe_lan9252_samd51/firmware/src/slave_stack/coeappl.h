/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
 * \addtogroup CoE CAN Application Profile over EtherCAT
 * @{
 */

/**
\file coeappl.h
\author EthercatSSC@beckhoff.com

\version 5.12

<br>Changes to version V5.11:<br>
V5.12 COE4: add timestamp object (0x10F8) and update diagnosis handling<br>
V5.12 ECAT5: update Sync error counter/flag handling,check enum memory alignment depending on the processor,in case of a polled timer disable ESC interrupts during DC_CheckWatchdog<br>
<br>Changes to version V5.01:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */

/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "objdef.h"


#ifndef _COEAPPL_H_
#define _COEAPPL_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/


/*---------------------------------------------
-    0x1018
-----------------------------------------------*/
/**
* \brief 0x1018 (Identity) object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
    UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
    UINT32   u32VendorID; /**< \brief EtherCAT vendor ID */
    UINT32   u32Productcode; /**< \brief Product code*/
    UINT32   u32Revision; /**< \brief Revision number*/
    UINT32   u32Serialnumber; /**< \brief Serial number*/
} OBJ_STRUCT_PACKED_END
TOBJ1018;





/*---------------------------------------------
-    0x1C00
-----------------------------------------------*/
/**
* \brief 0x1C00 (Sync manager type) object structure
*/
typedef struct OBJ_STRUCT_PACKED_START {
    UINT16   u16SubIndex0; /**< \brief SubIndex 0*/
/*ECATCHANGE_START(V5.12)*/
    UINT16   aEntries[2]; /**< \brief Entry buffer (one for two SyncManagers)*/
/*ECATCHANGE_END(V5.12)*/
} OBJ_STRUCT_PACKED_END
TOBJ1C00;

#define CHECK_DC_OVERRUN_IN_MS      4000 /**<\brief cycle time in ms to check the DC system time for overrun*/

#endif //_COEAPPL_H_

#if defined(_COEAPPL_) && (_COEAPPL_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif

/*-----------------------------------------------------------------------------------------
------
------    Global variables
------
-----------------------------------------------------------------------------------------*/

/*ECATCHANGE_START(V5.12) COE4*/
PROTO UINT64 u64Timestamp
#ifdef _COEAPPL_
= { 0 }
#endif
;
/*ECATCHANGE_END(V5.12) COE4*/

PROTO UINT32 u32LastDc32Value;
PROTO UINT32 u32CheckForDcOverrunCnt;
/*-----------------------------------------------------------------------------------------
------
------    Global functions
------
-----------------------------------------------------------------------------------------*/
/*ECATCHANGE_START(V5.12) ECAT5*/
PROTO void COE_UpdateSyncErrorStatus(void);
/*ECATCHANGE_END(V5.12) ECAT5*/

PROTO void COE_ObjInit(void);
PROTO void COE_Main(void);
PROTO UINT16 COE_ObjDictionaryInit(void);
PROTO UINT16 COE_AddObjectToDic(TOBJECT OBJMEM * pNewObjEntry);
PROTO void COE_RemoveDicEntry(UINT16 index);
PROTO void COE_ClearObjDictionary(void);
PROTO OBJCONST TOBJECT OBJMEM * COE_GetObjectDictionary(void);
/*ECATCHANGE_START(V5.12) COE4*/
PROTO void COE_SyncTimeStamp(void);
/*ECATCHANGE_END(V5.12) COE4*/

#undef PROTO
/** @}*/
