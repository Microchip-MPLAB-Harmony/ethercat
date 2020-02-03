/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
 * \addtogroup EMCY Emergency Messages
 * @{
 */

/**
\file emcy.h
\author EthercatSSC@beckhoff.com
\brief Defines, Typedef, Variables to handle emergency messages

\version 5.11

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
#include "mailbox.h"

#include "ecatcoe.h"

#ifndef _EMCY_H_
#define _EMCY_H_

/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/


/*---------------------------------------------
-    Emergency error codes
-----------------------------------------------*/
#define    ERROR_PREOP_2_SAFEOP            0xA000 /**< \brief Error occurred during the transition from PreOP to SafeOP*/
#define    ERROR_SAFEOP_2_OP               0xA001 /**< \brief Error occurred during the transition from SafeOP to OP*/


/** \brief Emergency Info*/
typedef struct MBX_STRUCT_PACKED_START
{
    UINT16                      Code; /**< \brief Code of the emergency message*/
    UINT16                      RegData[3]; /**< \brief Emergency data*/
        #define    EMCY_OFFS_ERRORREGISTER     0 /**< \brief Error register offset within the emergency data*/
        #define    EMCY_MASK_ERRORREGISTER     0x00FF /**< \brief Error register mask within the emergency data*/
        #define    EMCY_SHIFT_ERRORREGISTER    0 /**< \brief Error register shift within the emergency data*/
/*the diagcode is stored in the first byte of the Data field*/
        #define    EMCY_OFFS_DIAGCODE          0 /**< \brief Diag code offset within the emergency data*/
        #define    EMCY_MASK_DIAGCODE          0xFF00 /**< \brief Diag code mask within the emergency data*/
        #define    EMCY_SHIFT_DIAGCODE         8 /**< \brief Diag code shift within the emergency data*/
        #define    EMCY_OFFS_DIAGDATA          1 /**< \brief Diag data offset within the emergency data*/
}MBX_STRUCT_PACKED_END
TEMCY;


#define EMCY_SIZE   SIZEOF(TEMCY) /**< \brief size of the emergency data*/


/** \brief Emergency Message*/
typedef struct MBX_STRUCT_PACKED_START
{
    TEMCY                            Emcy; /**< \brief Emergency Info*/
}MBX_STRUCT_PACKED_END
TEMCYMESSAGE;


/**\brief Handling a list of emergencies*/
typedef struct
{
    UINT16                      FirstInQueue; /**< Index to first queue element*/
    UINT16                      LastInQueue; /**< Index to last queue element*/
    UINT16                      PutInQueueFailedCounter;
    UINT16                      MaxQueueSize; /**< \brief Maximum elements in the queue*/
    TEMCYMESSAGE EMCYMEM *      pQueue[MAX_EMERGENCIES+1]; /**< \brief List of emergency messages*/
} TEMCYQUEUE;



#endif //_EMCY_H_

#if defined(_EMCY_) && (_EMCY_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif

/*-----------------------------------------------------------------------------------------
------
------    Global Variables
------
-----------------------------------------------------------------------------------------*/
PROTO    TEMCYQUEUE EMCYMEM     sSendEmcyQueue; /**< \brief Queue of emergencies to be send*/
PROTO    TEMCYQUEUE EMCYMEM     sEmptyEmcyQueue; /**< \brief Queue of empty (currently not used) emergencies*/
PROTO    TEMCYMESSAGE EMCYMEM   asEmcy[(MAX_EMERGENCIES)]; /**< Buffer including all available emergencies*/


/*-----------------------------------------------------------------------------------------
------
------    Global Functions
------
-----------------------------------------------------------------------------------------*/
PROTO    void PutInEmptyEmcyQueue(TEMCYMESSAGE EMCYMEM * pEmcy);
PROTO    TEMCYMESSAGE EMCYMEM * GetOutOfEmptyEmcyQueue(void);
PROTO    void PutInSendEmcyQueue(TEMCYMESSAGE EMCYMEM * pEmcy);
PROTO    TEMCYMESSAGE EMCYMEM * GetOutOfSendEmcyQueue(void);
PROTO    void EMCY_Init(void);
PROTO    UINT8 EMCY_IsQueueEmpty(void);
PROTO    void EMCY_ContinueInd(TMBX MBXMEM * pMbx);
PROTO    TEMCYMESSAGE EMCYMEM * EMCY_GetEmcyBuffer(void);
PROTO    UINT8 EMCY_SendEmergency(TEMCYMESSAGE EMCYMEM *pEmcy);

#undef PROTO
/** @}*/
