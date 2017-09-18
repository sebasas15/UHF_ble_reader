/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PROJECTZERO_H
#define PROJECTZERO_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <hci_tl.h>
#include <gap.h>
#include <gatt.h>
#include <gapgattserver.h>
#include <gattservapp.h>
#include <osal_snv.h>
#include <gapbondmgr.h>
#include <peripheral.h>
#include <icall_apimsg.h>

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * TYPEDEFS
 */
typedef enum
{
  APP_MSG_SERVICE_WRITE = 0,   /* A characteristic value has been written     */
  APP_MSG_SERVICE_CFG,         /* A characteristic configuration has changed  */
  APP_MSG_UPDATE_CHARVAL,      /* Request from ourselves to update a value    */
  APP_MSG_GAP_STATE_CHANGE,    /* The GAP / connection state has changed      */
  APP_MSG_BUTTON_DEBOUNCED,    /* A button has been debounced with new value  */
  APP_MSG_SEND_PASSCODE,       /* A pass-code/PIN is requested during pairing */
} app_msg_types_t;

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task creation function for the Simple BLE Peripheral.
 */
extern void ProjectZero_createTask(void);
extern ICall_Semaphore App_getSem();
extern void user_enqueueCharDataMsg( app_msg_types_t appMsgType, uint16_t connHandle,
                                       uint16_t serviceUUID, uint8_t paramID,
                                       uint8_t *pValue, uint16_t len);
extern uint16_t user_getConnH();
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PROJECTZERO_H */
