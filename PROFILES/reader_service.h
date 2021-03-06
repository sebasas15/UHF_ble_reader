/******************************************************************************
 * Filename:       Reader_Service.h
 *
 * Description:    This file contains the Reader_Service service definitions and
 *                 prototypes.
 *
 *                 Generated by:
 *                 BDS version: 1.1.3135.0
 *                 Plugin:      Texas Instruments BLE SDK GATT Server plugin 1.0.8
 *                 Time:        Thu Sep 07 2017 05:39:13 GMT-04:00
 *

 * Copyright (c) 2015, Texas Instruments Incorporated
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
 *
 *****************************************************************************/

#ifndef _READER_SERVICE_H_
#define _READER_SERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <bcomdef.h>

/*********************************************************************
 * CONSTANTS
 */
// Service UUID
#define READER_SERVICE_SERV_UUID 0x7C06
#define READER_SERVICE_SERV_UUID_BASE128(uuid) 0x6F, 0x2A, 0x03, 0x60, 0xA9, 0x59, 0x6B, 0x8C, 0xA9, 0x4D, 0x7F, 0x9B, LO_UINT16(uuid), HI_UINT16(uuid), 0x3C, 0xBD

// payload Characteristic defines
#define RS_PAYLOAD_ID                 0
#define RS_PAYLOAD_UUID               0x9DF9
#define RS_PAYLOAD_UUID_BASE128(uuid) 0x6F, 0x2A, 0x03, 0x60, 0xA9, 0x59, 0x6B, 0x8C, 0xA9, 0x4D, 0x7F, 0x9B, LO_UINT16(uuid), HI_UINT16(uuid), 0x3C, 0xBD
#define RS_PAYLOAD_LEN                16
#define RS_PAYLOAD_LEN_MIN            16

// iniciado Characteristic defines
#define RS_INICIADO_ID                 1
#define RS_INICIADO_UUID               0x648B
#define RS_INICIADO_UUID_BASE128(uuid) 0x6F, 0x2A, 0x03, 0x60, 0xA9, 0x59, 0x6B, 0x8C, 0xA9, 0x4D, 0x7F, 0x9B, LO_UINT16(uuid), HI_UINT16(uuid), 0x3C, 0xBD
#define RS_INICIADO_LEN                1
#define RS_INICIADO_LEN_MIN            1

// ciclo de lectura Characteristic defines
#define RS_CICLO_DE_LECTURA_ID                 2
#define RS_CICLO_DE_LECTURA_UUID               0x2359
#define RS_CICLO_DE_LECTURA_UUID_BASE128(uuid) 0x6F, 0x2A, 0x03, 0x60, 0xA9, 0x59, 0x6B, 0x8C, 0xA9, 0x4D, 0x7F, 0x9B, LO_UINT16(uuid), HI_UINT16(uuid), 0x3C, 0xBD
#define RS_CICLO_DE_LECTURA_LEN                1
#define RS_CICLO_DE_LECTURA_LEN_MIN            1

// time Characteristic defines
#define RS_TIME_ID                 3
#define RS_TIME_UUID               0xC4EF
#define RS_TIME_UUID_BASE128(uuid) 0x6F, 0x2A, 0x03, 0x60, 0xA9, 0x59, 0x6B, 0x8C, 0xA9, 0x4D, 0x7F, 0x9B, LO_UINT16(uuid), HI_UINT16(uuid), 0x3C, 0xBD
#define RS_TIME_LEN                8
#define RS_TIME_LEN_MIN            8


// estado Characteristic defines
#define RS_ESTADO_ID                 4
#define RS_ESTADO_UUID               0x748B
#define RS_ESTADO_UUID_BASE128(uuid) 0x6F, 0x2A, 0x03, 0x60, 0xA9, 0x59, 0x6B, 0x8C, 0xA9, 0x4D, 0x7F, 0x9B, LO_UINT16(uuid), HI_UINT16(uuid), 0x3C, 0xBD
#define RS_ESTADO_LEN                1
#define RS_ESTADO_LEN_MIN            1


/*********************************************************************
 * TYPEDEFS
 */

// Fields in characteristic "payload"
//   Field "body" format: uint128, bits: 128

// Fields in characteristic "iniciado"
//   Field "estado" format: uint8, bits: 8

// Fields in characteristic "ciclo de lectura"
//   Field "tipo" format: uint8, bits: 8
//     Enumerations for tipo
//       ONE_SHOT -- 0
//       TIMEOUT -- 1
//       PERIODIC -- 2
//       CONTIOUS -- 3
typedef enum {
    NONE   = 0,
    ONE_SHOT,
    TIMEOUT,
    PERIODIC,
    CONTINOUS,


} read_types_t;

// Fields in characteristic "time"
//   Field "valor" format: uint64, bits: 64

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*ReaderServiceChange_t)( uint16_t connHandle, uint16_t svcUuid, uint8_t paramID, uint8_t *pValue, uint16_t len );

typedef struct
{
  ReaderServiceChange_t        pfnChangeCb;     // Called when characteristic value changes
  ReaderServiceChange_t        pfnCfgChangeCb;  // Called when characteristic CCCD changes
} ReaderServiceCBs_t;



/*********************************************************************
 * API FUNCTIONS
 */


/*
 * ReaderService_AddService- Initializes the ReaderService service by registering
 *          GATT attributes with the GATT server.
 *
 *    rspTaskId - The ICall Task Id that should receive responses for Indications.
 */
extern bStatus_t ReaderService_AddService( uint8_t rspTaskId );

/*
 * ReaderService_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t ReaderService_RegisterAppCBs( ReaderServiceCBs_t *appCallbacks );

/*
 * ReaderService_SetParameter - Set a ReaderService parameter.
 *
 *    param - Profile parameter ID
 *    len   - length of data to write
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern bStatus_t ReaderService_SetParameter( uint8_t param, uint16_t len, void *value );

/*
 * ReaderService_GetParameter - Get a ReaderService parameter.
 *
 *    param - Profile parameter ID
 *    len   - pointer to a variable that contains the maximum length that can be written to *value.
              After the call, this value will contain the actual returned length.
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern bStatus_t ReaderService_GetParameter( uint8_t param, uint16_t *len, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _READER_SERVICE_H_ */

