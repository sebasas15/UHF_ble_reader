/*
 * reader.h
 *
 *  Created on: 08-09-2017
 *      Author: seba
 */

#ifndef APPLICATION_READER_H_
#define APPLICATION_READER_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "reader_service.h"
#include <ti/sysbios/knl/Queue.h>
#include "project_zero.h"

typedef enum {

    CONECTAR = 0,
    DESCONECTAR,
    LEER

} reader_cmd_t;

typedef struct
{
  Queue_Elem       _elem;
  read_types_t  type;           //tipos de lectura cuando el comando es leer
  reader_cmd_t cmd;             //tipos de comandos disponibles
  uint16_t connHandler;
  uint8_t          pdu[];
} reader_msg_t;


extern void Reader_createTask(void);

extern void Reader_enqueueCmdMsg(reader_cmd_t readerCmdType, uint8_t *pData,
                                 uint16_t len);
extern void Reader_enqueueReadMsg(read_types_t appMsgType, uint16_t connHandler);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_READER_H_ */
