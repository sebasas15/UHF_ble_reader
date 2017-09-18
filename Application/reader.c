#include "reader.h"
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/knl/Queue.h>
#include <string.h>
#include <reader_imp.h>
#include <project_zero.h>


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

//static TMR_Reader r, *rp;
Task_Struct rdTask;
Char rdTaskStack[READER_TASK_STACK_SIZE];

static Queue_Struct readerMsgQ;
static Queue_Handle hReaderMsgQ;
static uint8_t antennaCount = 0x0;
static uint8_t buffer[20];
static read_types_t tipo_de_lectura = NONE;
static uint8_t read_time = 0;
static uint8_t *antennaList = NULL;
static Reader_Imp_Init_Params reader_param = {

                                              .head = 0,
                                              .printDebug = true,
                                              .lightDebug = true,
                                              .sim = true
};

/*********************************************************
 * FUNCIONES LOCALES
 */

static void Reader_taskFxn(UArg a0, UArg a1);
static void Reader_init();
static void Reader_task_init();
static void Reader_leer(uint16_t c);
static void ejecutarComando(reader_msg_t * msg);
static void ejecutarLectura(reader_msg_t * msg);
static bool setupNano(long baudRate);
static void setReadTime(reader_msg_t * msg);
static void setReadType(reader_msg_t * msg);

void Reader_createTask(UART_Handle uh)
{

    Task_Params taskParams;
    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = rdTaskStack;
    taskParams.stackSize = READER_TASK_STACK_SIZE;
    taskParams.priority = READER_TASK_PRIORITY;

      reader_param.uartHandle = uh;

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

    //return;
    Log_info0("iniciando Reader");
    char str[64];
    buffer[0] = 1;
    antennaList = buffer;
    antennaCount = 0x01;
    setupNano(115200);

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
//
//    if (readerCmdType == LEER)
//    {
//        Log_warning0(
//                "Para usar el comando LEER, utilizar el metodo Reader_enqueueReadMsg");
//        return;
//    }

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

    reader_cmd_t comando = msg->cmd;
    switch (comando)
    {

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
    case SET_TIME:
        setReadTime(msg);
        break;
    case SET_READ_TYPE:
        setReadType(msg);
        break;

    }

}

static void setReadTime(reader_msg_t * msg)
{

    read_time = (uint8_t) *msg->pdu;
    //memcpy(read_time,msg->pdu,sizeof(msg->pdu));

}
static void setReadType(reader_msg_t * msg)
{

    tipo_de_lectura = (read_types_t) *msg->pdu;
    Log_info1("estableciendo cliclo de lectura %d", tipo_de_lectura);
    switch (tipo_de_lectura)
    {
    case ONE_SHOT:
        Log_info0("ONE_SHOT");
        break;
    case TIMEOUT:
        Log_info0("TIMEOUT");
        break;
    case PERIODIC:
        Log_info0("PERIODIC");
        break;
    case CONTINOUS:
        Log_info0("CONTINOUS");
        break;
    default:
        break;

    }
}

static void ejecutarLectura(reader_msg_t * msg)
{
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

    char epcStr[] = "13123321234567832123";


//      uint8_t myEPC[12]; //Most EPCs are 12 bytes
//      uint8_t myEPClength;
//      uint8_t responseType = 0;
//
//      while (responseType != RESPONSE_SUCCESS)//RESPONSE_IS_TAGFOUND)
//      {
//        myEPClength = sizeof(myEPC); //Length of EPC is modified each time .readTagEPC is called
//
//        responseType = Reader_Imp_readTagEPC(myEPC, myEPClength, 500); //Scan for a new tag up to 500ms
//      }


    Log_info1("enviando dato %s",epcStr);
    user_enqueueCharDataMsg(APP_MSG_UPDATE_CHARVAL, connHandler,
    READER_SERVICE_SERV_UUID,
                            RS_PAYLOAD_ID, epcStr, strlen(epcStr));

}

static bool setupNano(long baudRate)

{

    Reader_Imp_begin(&reader_param); //Tell the library to communicate over software serial port
    Reader_Imp_getVersion();
    if (reader_param.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
    {
        //This happens if the baud rate is correct but the module is doing a ccontinuous read
        Reader_Imp_stopReading();

        // Serial.println(F("Module continuously reading. Asking it to stop..."));

        //  delay(1500);
    }


//    if (reader_param.msg[0] != ALL_GOOD)
//        return (false);        //Something is not right

    //The M6E has these settings no matter what
    Reader_Imp_setTagProtocol();//Set protocol to GEN2

    Reader_Imp_setAntennaPort();        //Set TX/RX antenna ports to 1

    return (true);        //We are ready to rock
}
