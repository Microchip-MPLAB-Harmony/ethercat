/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup EMCY Emergency Messages	
@{
*/

/**
\file emcy.c
\author EthercatSSC@beckhoff.com
\brief Implementation
This file contains the Emergency interface

\version 5.11

<br>Changes to version V5.0:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 MBX3: set application triggered emergency and EoE data to pending if no mailbox queue is supported and another mailbox request is currently handled, Handle only one mailbox request at a time (in case that MAILBPX_QUEUE is disabled)<br>
<br>Changes to version V4.40:<br>
V5.0 EMCY1: enable mailbox interrupt again before "EMCY_SendEmergency()" is abort.<br>
<br>Changes to version V4.10:<br>
V4.40 SOE1: provide MAX_EMCY_SIZE also if CoE is not supported<br>
V4.40 EMCY1: Prevent sending empty mailbox<br>
<br>Changes to version V3.20:<br>
V4.10 EMCY 1: AlStatus comparison fixed
*/

/*---------------------------------------------------------------------------------------
------
------    Includes
------
---------------------------------------------------------------------------------------*/

#include "ecat_def.h"


#include "ecatslv.h"



#define    _EMCY_    1
#include "emcy.h"
#undef      _EMCY_
/*remove definition of _EMCY_ (#ifdef is used in emcy.h)*/
#include "ecatcoe.h"

/*---------------------------------------------------------------------------------------
------
------    constants
------
---------------------------------------------------------------------------------------*/

const UINT16 MBXMEM acEmcyCoeHeader[4] = {COE_HEADER_SIZE + EMCY_SIZE, 0, SWAPWORD(MBX_TYPE_COE<<8), SWAPWORD(COESERVICE_EMERGENCY<<12)};
#define    MAX_EMCY_SIZE    (SIZEOF(acEmcyCoeHeader)+SIZEOF(TEMCYMESSAGE))

/*---------------------------------------------------------------------------------------
------
------    Functions
------
---------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pEmcy             empty emergency buffer

 \brief    This function puts an emergency buffer back to the empty queue
*////////////////////////////////////////////////////////////////////////////////////////

void PutInEmptyEmcyQueue(TEMCYMESSAGE EMCYMEM * pEmcy)
{
    UINT16 lastInQueue = sEmptyEmcyQueue.LastInQueue+1;

    if (lastInQueue == sEmptyEmcyQueue.MaxQueueSize)
    {
        lastInQueue = 0;
    }


    sEmptyEmcyQueue.pQueue[sEmptyEmcyQueue.LastInQueue] = pEmcy;
    sEmptyEmcyQueue.LastInQueue = lastInQueue;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return        empty emergency buffer

 \brief    This function gets an emergency buffer from the empty queue
*////////////////////////////////////////////////////////////////////////////////////////

TEMCYMESSAGE EMCYMEM * GetOutOfEmptyEmcyQueue(void)
{
    TEMCYMESSAGE EMCYMEM * pEmcy;


        ENTER_EMCY_CRITICAL;
    if (sEmptyEmcyQueue.FirstInQueue != sEmptyEmcyQueue.LastInQueue)
    {
        UINT16 firstInQueue = sEmptyEmcyQueue.FirstInQueue;

        pEmcy = sEmptyEmcyQueue.pQueue[firstInQueue];
        firstInQueue++;
        sEmptyEmcyQueue.FirstInQueue = firstInQueue;
        if (sEmptyEmcyQueue.FirstInQueue == sEmptyEmcyQueue.MaxQueueSize)
        {
            sEmptyEmcyQueue.FirstInQueue = 0;
        }
    }
    else
    {
        pEmcy = 0;
    }
    LEAVE_EMCY_CRITICAL;

    return pEmcy;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pEmcy         full emergency buffer

 \return    0: Success.

 \brief    This function puts an emergency buffer in the send queue
*////////////////////////////////////////////////////////////////////////////////////////

void PutInSendEmcyQueue(TEMCYMESSAGE EMCYMEM * pEmcy)
{
    UINT16 lastInQueue;

    
    ENTER_EMCY_CRITICAL;
    lastInQueue = sSendEmcyQueue.LastInQueue+1;

    if (lastInQueue == sSendEmcyQueue.MaxQueueSize)
    {
        lastInQueue = 0;
    }

    sSendEmcyQueue.pQueue[sSendEmcyQueue.LastInQueue] = pEmcy;
    sSendEmcyQueue.LastInQueue = lastInQueue;
    LEAVE_EMCY_CRITICAL;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return        full emergency buffer

 \brief    This function gets an emergency buffer to be sent
*////////////////////////////////////////////////////////////////////////////////////////

TEMCYMESSAGE EMCYMEM * GetOutOfSendEmcyQueue(void)
{
    TEMCYMESSAGE EMCYMEM * pEmcy;

    
    ENTER_EMCY_CRITICAL;
    if (sSendEmcyQueue.FirstInQueue != sSendEmcyQueue.LastInQueue)
    {
        UINT16 firstInQueue = sSendEmcyQueue.FirstInQueue;

        pEmcy = sSendEmcyQueue.pQueue[firstInQueue];
        firstInQueue++;
        sSendEmcyQueue.FirstInQueue = firstInQueue;
        if (sSendEmcyQueue.FirstInQueue == sSendEmcyQueue.MaxQueueSize)
        {
            sSendEmcyQueue.FirstInQueue = 0;
        }
    }
    else
    {
        pEmcy = 0;
    }
    LEAVE_EMCY_CRITICAL;

    return pEmcy;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**

 \brief    This function intialize the Emergency Interface.
*////////////////////////////////////////////////////////////////////////////////////////

void EMCY_Init(void)
{
    UINT8 i;

    /* initialize send queue */
    sSendEmcyQueue.FirstInQueue                    = 0;
    sSendEmcyQueue.LastInQueue                    = 0;
    sSendEmcyQueue.PutInQueueFailedCounter    = 0;
    /* Bug 27.07: MaxQueueSize has to be set to the number of elements */
    sSendEmcyQueue.MaxQueueSize                    = MAX_EMERGENCIES+1;
    for (i = 0; i < MAX_EMERGENCIES; i++)
    {
        sSendEmcyQueue.pQueue[i] = 0;
    }

    /* initialize empty queue */
    sEmptyEmcyQueue.FirstInQueue                    = 0;
    sEmptyEmcyQueue.LastInQueue                    = 0;
    sEmptyEmcyQueue.PutInQueueFailedCounter    = 0;
    /* Bug 27.07: MaxQueueSize has to be set to the number of elements */
    sEmptyEmcyQueue.MaxQueueSize                    = MAX_EMERGENCIES+1;
    for (i = 0; i < MAX_EMERGENCIES; i++)
    {
        sEmptyEmcyQueue.pQueue[i] = 0;
    }

    /* put all buffers in empty queue */
    for (i = 0; i < MAX_EMERGENCIES; i++)
    {
        PutInEmptyEmcyQueue(&asEmcy[i]);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return        1 is the emergency queue is empty

 \brief    This function checks if the emergency queue is empty
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 EMCY_IsQueueEmpty(void)
{
    return (sSendEmcyQueue.FirstInQueue == sSendEmcyQueue.LastInQueue);
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pMbx      empty mailbox buffer

 \brief    This function is called when the next mailbox command can be stored the ESC.
*////////////////////////////////////////////////////////////////////////////////////////

void EMCY_ContinueInd(TMBX MBXMEM * pMbx)
{
    TEMCYMESSAGE EMCYMEM *pEmcy = GetOutOfSendEmcyQueue();
    
    if (pEmcy != NULL)
    {
        /* send next emergency message */
        EMCY_SendEmergency( pEmcy );
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return                    empty emergency buffer

 \brief    This function gets an empty emergency buffer
*////////////////////////////////////////////////////////////////////////////////////////

TEMCYMESSAGE EMCYMEM * EMCY_GetEmcyBuffer(void)
{
    TEMCYMESSAGE EMCYMEM * pEmcy;

    

    pEmcy = GetOutOfEmptyEmcyQueue();

    return pEmcy;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param     pEmcy                emergency message to be sent

 \brief    This function sends an emergency message
*////////////////////////////////////////////////////////////////////////////////////////

UINT8 EMCY_SendEmergency( TEMCYMESSAGE EMCYMEM *pEmcy )
{

    ENTER_MBX_CRITICAL;

    if ( bSendMbxIsFull || ((nAlStatus & STATE_MASK) == STATE_INIT)
        )
    {
        /* send mailbox is full store the emergency message */
        PutInSendEmcyQueue( pEmcy );

        u8MailboxSendReqStored |= EMCY_SERVICE;
    

        LEAVE_MBX_CRITICAL;
        return 0;
    }

    /* buffer psWriteMbx is free if bSendMbxIsFull is not set */
    psWriteMbx = (TMBX MBXMEM *) APPL_AllocMailboxBuffer(MAX_EMCY_SIZE);
    /* it shall be checked if a valid pointer was returned */
    if ( psWriteMbx == NULL )
    {

        LEAVE_MBX_CRITICAL;
        return ALSTATUSCODE_NOMEMORY;
    }

    OBJTOMBXMEMCPY(psWriteMbx, acEmcyCoeHeader, SIZEOF(acEmcyCoeHeader));
    MBXMEMCPY(&psWriteMbx->Data[1], pEmcy, SIZEOF(TEMCYMESSAGE));

    // HBu 02.05.06: emergency buffer has to be put in the empty queue only
    //               if the sending was successful
    if (MBX_MailboxSendReq(psWriteMbx, EMCY_SERVICE) == 0)
    {
        /* put emergency buffer back in the empty queue */
        PutInEmptyEmcyQueue( pEmcy );
    }
    
    LEAVE_MBX_CRITICAL;

    return 0;
}



/** @} */

