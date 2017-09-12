#include "reader.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/knl/Queue.h>
#include <xdc/runtime/Log.h>
#include <string.h>
//#include "tm_reader.h"

//*********************************************
//CONSTANTES
//*************************************************
#define READPOWER_NULL (-12345)
#define READER_TASK_PRIORITY                     1

#ifndef READER_TASK_STACK_SIZE
#define READER_TASK_STACK_SIZE                   512
#endif

//*******************************************************
//LOCALES
//*******************************************************

static void Reader_taskFxn(UArg a0, UArg a1);
static void Reader_init();
static void Reader_task_init();
static void Reader_leer(uint16_t c);
static void ejecutarComando(reader_msg_t * msg);
static void ejecutarLectura(reader_msg_t * msg);
//static TMR_Reader r, *rp;
Task_Struct rdTask;
Char rdTaskStack[READER_TASK_STACK_SIZE];
static Queue_Struct readerMsgQ;
static Queue_Handle hReaderMsgQ;
static uint8_t antennaCount = 0x0;
static  uint8_t buffer[20];
static uint8_t *antennaList = NULL;

void Reader_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = rdTaskStack;
    taskParams.stackSize = READER_TASK_STACK_SIZE;
    taskParams.priority = READER_TASK_PRIORITY;

    Task_construct(&rdTask, Reader_taskFxn, &taskParams, NULL);
}

static void Reader_taskFxn(UArg a0, UArg a1)
{

    Reader_task_init();

    while (1)
    {
        Semaphore_pend(readSem, Semaphore_PendState_WAIT_FOREVER);

        if (!Queue_empty(hReaderMsgQ))
        {

            reader_msg_t* msg = Queue_dequeue(hReaderMsgQ);
            ejecutarComando(msg);

        }

        Semaphore_post(App_getSem());
    }


}

static void Reader_init()
{


    Log_info0("iniciando Reader");

   // TMR_Status ret;
   // TMR_Region region;
    //int readpower = READPOWER_NULL;
   // uint8_t i;
   // TMR_String model;
    char str[64];
   // TMR_TRD_MetadataFlag metadata = TMR_TRD_METADATA_FLAG_ALL;

   // rp = &r;

  //  ret = TMR_create(rp, "tmr:///com1");
    buffer[0] = 1;
    antennaList = buffer;
    antennaCount = 0x01;

   // ret = TMR_connect(rp);
   // if (TMR_SUCCESS != ret)
    //    Log_warning0(ret);

   // region = TMR_REGION_EU;
   // ret = TMR_paramSet(rp, TMR_PARAM_REGION_ID, &region);

   // model.value = str;
   // model.max = 64;
    //TMR_paramGet(rp, TMR_PARAM_VERSION_MODEL, &model);
   // Log_info0((char*) model.value);
   // if (((0 == strcmp("Sargas", model.value))
   //         || (0 == strcmp("M6e Micro", model.value))
   //         || (0 == strcmp("M6e Nano", model.value))) && (NULL == antennaList))
   // {
   // }

    /**
     * for antenna configuration we need two parameters
     * 1. antennaCount : specifies the no of antennas should
     *    be included in the read plan, out of the provided antenna list.
     * 2. antennaList  : specifies  a list of antennas for the read plan.
     **/
    /**
     if (rp->readerType == TMR_READER_TYPE_SERIAL)
     {
     // Set the metadata flags. Configurable Metadata param is not supported for llrp readers
     // metadata = TMR_TRD_METADATA_FLAG_ANTENNAID | TMR_TRD_METADATA_FLAG_FREQUENCY | TMR_TRD_METADATA_FLAG_PHASE;
     ret = TMR_paramSet(rp, TMR_PARAM_METADATAFLAG, &metadata);
     }
     */


}

void Reader_task_init()
{
    Queue_construct(&readerMsgQ, NULL);
    hReaderMsgQ = Queue_handle(&readerMsgQ);

}

extern void Reader_enqueueReadMsg(read_types_t appMsgType, uint16_t connHandler)
{
    // Allocate memory for the message.
    reader_msg_t *pMsg = ICall_malloc(sizeof(reader_msg_t));

    if (pMsg != NULL)
    {
        pMsg->type = appMsgType;
        pMsg->cmd = LEER;
        // Copy data into message
        //memcpy(pMsg->pdu, pData, len);
        pMsg->connHandler = connHandler;
        // Enqueue the message using pointer to queue node element.
        Queue_enqueue(hReaderMsgQ, &pMsg->_elem);
        // Let application know there's a message.
        Semaphore_post(readSem);
    }
}

extern void Reader_enqueueCmdMsg(reader_cmd_t readerCmdType, uint8_t *pData,
                                 uint16_t len)
{
    reader_msg_t *pMsg = ICall_malloc(sizeof(reader_msg_t) + len);

    if (readerCmdType == LEER)
    {
        Log_warning0(
                "Para usar el comando LEER, utilizar el metodo Reader_enqueueReadMsg");
        return;
    }

    if (pMsg != NULL)
    {
        pMsg->type = NONE;
        pMsg->cmd = readerCmdType;
        // Copy data into message
        memcpy(pMsg->pdu, pData, len);

        // Enqueue the message using pointer to queue node element.
        Queue_enqueue(hReaderMsgQ, &pMsg->_elem);
        // Let application know there's a message.
        Semaphore_post(readSem);
    }

}



static void ejecutarComando(reader_msg_t * msg)
{

    read_types_t comando = msg->type;
    switch (comando){

    case CONECTAR:

        Reader_init();
        break;
    case DESCONECTAR:
    //    TMR_destroy(NULL);
        Log_info0("desconectando");
        break;
    case LEER:
        ejecutarLectura(msg);
        break;

    }

}

static void ejecutarLectura(reader_msg_t * msg){
    read_types_t tipo = msg->type;

    switch (tipo)
    {

    case ONE_SHOT:
        Reader_leer(msg->connHandler);
        break;
    case TIMEOUT:

        break;
    case PERIODIC:

        break;
    case CONTINOUS:

        break;
    default:
        break;
    }


}


/*
 * @brief   Lee datos desde el reader y los encola para su uso en la Task de aplicacion ble.
 *
 *
 * @return  None.
 */
static void Reader_leer(uint16_t connHandler)
{

        char epcStr[] = "1312332";

        Log_info1("enviando dato %s",epcStr);
        user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL,connHandler,READER_SERVICE_SERV_UUID,RS_PAYLOAD_ID,epcStr,strlen(epcStr));


}

