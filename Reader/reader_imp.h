/*
  Library for controlling the Nano M6E from ThingMagic
  This is a stripped down implementation of the Mercury API from ThingMagic
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 3rd, 2016
  https://github.com/sparkfun/Simultaneous_RFID_Tag_Reader
  License: Open Source MIT License
  If you use this code please consider buying an awesome board from SparkFun. It's a ton of
  work (and a ton of fun!) to put these libraries together and we want to keep making neat stuff!
  https://opensource.org/licenses/MIT
  The above copyright notice and this permission notice shall be included in all copies or
  substantial portions of the Software.
*/

#include <ti/drivers/UART.h>
#include "stdbool.h"
#include <xdc/runtime/System.h>
#include "default_args.h"
#include <xdc/runtime/Log.h>

#define MAX_MSG_SIZE 255

#define TMR_SR_OPCODE_VERSION 0x03
#define TMR_SR_OPCODE_SET_BAUD_RATE 0x06
#define TMR_SR_OPCODE_READ_TAG_ID_SINGLE 0x21
#define TMR_SR_OPCODE_READ_TAG_ID_MULTIPLE 0x22
#define TMR_SR_OPCODE_WRITE_TAG_ID 0x23
#define TMR_SR_OPCODE_WRITE_TAG_DATA 0x24
#define TMR_SR_OPCODE_KILL_TAG 0x26
#define TMR_SR_OPCODE_READ_TAG_DATA 0x28
#define TMR_SR_OPCODE_CLEAR_TAG_ID_BUFFER 0x2A
#define TMR_SR_OPCODE_MULTI_PROTOCOL_TAG_OP 0x2F
#define TMR_SR_OPCODE_GET_READ_TX_POWER 0x62
#define TMR_SR_OPCODE_GET_WRITE_TX_POWER 0x64
#define TMR_SR_OPCODE_GET_POWER_MODE 0x68
#define TMR_SR_OPCODE_GET_READER_OPTIONAL_PARAMS 0x6A
#define TMR_SR_OPCODE_GET_PROTOCOL_PARAM 0x6B
#define TMR_SR_OPCODE_SET_ANTENNA_PORT 0x91
#define TMR_SR_OPCODE_SET_TAG_PROTOCOL 0x93
#define TMR_SR_OPCODE_SET_READ_TX_POWER 0x92
#define TMR_SR_OPCODE_SET_WRITE_TX_POWER 0x94
#define TMR_SR_OPCODE_SET_REGION 0x97
#define TMR_SR_OPCODE_SET_READER_OPTIONAL_PARAMS 0x9A
#define TMR_SR_OPCODE_SET_PROTOCOL_PARAM 0x9B

#define COMMAND_TIME_OUT  2000 //Number of ms before stop waiting for response from module

//Define all the ways functions can return
#define ALL_GOOD                        0
#define ERROR_COMMAND_RESPONSE_TIMEOUT  1
#define ERROR_CORRUPT_RESPONSE          2
#define ERROR_WRONG_OPCODE_RESPONSE     3
#define ERROR_UNKNOWN_OPCODE            4
#define RESPONSE_IS_TEMPERATURE         5
#define RESPONSE_IS_KEEPALIVE           6
#define RESPONSE_IS_TEMPTHROTTLE        7
#define RESPONSE_IS_TAGFOUND            8
#define RESPONSE_IS_NOTAGFOUND          9
#define RESPONSE_IS_UNKNOWN             10
#define RESPONSE_SUCCESS                11
#define RESPONSE_FAIL                   12

//Define the allowed regions - these set the internal freq of the module
#define REGION_INDIA        0x04
#define REGION_JAPAN        0x05
#define REGION_CHINA        0x06
#define REGION_EUROPE       0x08
#define REGION_KOREA        0x09
#define REGION_AUSTRALIA    0x0B
#define REGION_NEWZEALAND   0x0C
#define REGION_NORTHAMERICA 0x0D
#define REGION_OPEN         0xFF

/***********************************************
 * SOBRECARGA DE METODOS
 */

#define   Reader_Imp_sendMessage5(a,b,c,d,e)   Reader_Imp_sendMessageImp(a,b,c,d,e)
#define   Reader_Imp_sendMessage4(a,b,c,d)   Reader_Imp_sendMessageImp(a,b,c,d,true)
#define   Reader_Imp_sendMessage3(a,b,c)   Reader_Imp_sendMessageImp(a,b,c,COMMAND_TIME_OUT,true)
#define   Reader_Imp_sendMessage2(a,b)   Reader_Imp_sendMessageImp(a,b,0,COMMAND_TIME_OUT,true)
#define   Reader_Imp_sendMessage1(a)   Reader_Imp_sendMessageImp(a,0,0,COMMAND_TIME_OUT,true)
#define   Reader_Imp_sendMessage(...) PP_FUNC_GEN(Reader_Imp_sendMessage,PP_NARG(__VA_ARGS__)(__VA_ARGS__))


#define Reader_Imp_setTagProtocol1(a) Reader_Imp_setTagProtocolImp(a)
#define Reader_Imp_setTagProtocol0() Reader_Imp_setTagProtocolImp(0x05)
#define Reader_Imp_setTagProtocol(...) PP_FUNC_GEN(Reader_Imp_setTagProtocol,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_readTagEPC3(a,b,c)  Reader_Imp_readTagEPCImp(a,b,c)
#define Reader_Imp_readTagEPC2(a,b)  Reader_Imp_readTagEPCImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_readTagEPC(...) PP_FUNC_GEN(Reader_Imp_readTagEPC,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_writeTagEPC3(a,b,c)  Reader_Imp_writeTagEPCImp(a,b,c)
#define Reader_Imp_writeTagEPC2(a,b)  Reader_Imp_writeTagEPCImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_writeTagEPC(...) PP_FUNC_GEN(Reader_Imp_writeTagEPC,PP_NARG(__VA_ARGS__)(__VA_ARGS__))


#define Reader_Imp_readData5(a,b,c,d,e) Reader_Imp_readDataImp(a,b,c,d,e)
#define Reader_Imp_readData4(a,b,c,d) Reader_Imp_readDataImp(a,b,c,d,COMMAND_TIME_OUT)
#define Reader_Imp_readData(...) PP_FUNC_GEN(Reader_Imp_readData,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_writeData5(a,b,c,d,e) Reader_Imp_writeDataImp(a,b,c,d,e)
#define Reader_Imp_writeData4(a,b,c,d) Reader_Imp_writeDataImp(a,b,c,d,COMMAND_TIME_OUT)
#define Reader_Imp_writeData(...) PP_FUNC_GEN(Reader_Imp_writeData,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_readUserData3(a,b,c)  Reader_Imp_readUserDataImp(a,b,c)
#define Reader_Imp_readUserData2(a,b)  Reader_Imp_readUserDataImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_readUserData(...) PP_FUNC_GEN(Reader_Imp_readUserData,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_writeUserData3(a,b,c)  Reader_Imp_writeUserDataImp(a,b,c)
#define Reader_Imp_writeUserData2(a,b)  Reader_Imp_writeUserDataImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_writeUserData(...) PP_FUNC_GEN(Reader_Imp_writeUserData,PP_NARG(__VA_ARGS__)(__VA_ARGS__))


#define Reader_Imp_readKillPW3(a,b,c)  Reader_Imp_readKillPWImp(a,b,c)
#define Reader_Imp_readKillPW2(a,b)  Reader_Imp_readKillPWImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_readKillPW(...) PP_FUNC_GEN(Reader_Imp_readKillPW,PP_NARG(__VA_ARGS__)(__VA_ARGS__))


#define Reader_Imp_writeKillPW3(a,b,c)  Reader_Imp_writeKillPWImp(a,b,c)
#define Reader_Imp_writeKillPW2(a,b)  Reader_Imp_writeKillPWImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_writeKillPW(...) PP_FUNC_GEN(Reader_Imp_writeKillPW,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_readAccessPW3(a,b,c)  Reader_Imp_readAccessPWImp(a,b,c)
#define Reader_Imp_readAccessPW2(a,b)  Reader_Imp_readAccessPWImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_readAccessPW(...) PP_FUNC_GEN(Reader_Imp_readAccessPW,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_writeAccessPW3(a,b,c)  Reader_Imp_writeAccessPWImp(a,b,c)
#define Reader_Imp_writeAccessPW2(a,b)  Reader_Imp_writeAccessPWImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_writeAccessPW(...) PP_FUNC_GEN(Reader_Imp_writeAccessPW,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_readTID3(a,b,c)  Reader_Imp_readTIDImp(a,b,c)
#define Reader_Imp_readTID2(a,b)  Reader_Imp_readTIDImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_readTID(...) PP_FUNC_GEN(Reader_Imp_readTID,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_killTag3(a,b,c)  Reader_Imp_killTagImp(a,b,c)
#define Reader_Imp_killTag2(a,b)  Reader_Imp_killTagImp(a,b,COMMAND_TIME_OUT)
#define Reader_Imp_killTag(...) PP_FUNC_GEN(Reader_Imp_killTag,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

#define Reader_Imp_sendCommand2(a,b)  Reader_Imp_sendCommandImp(a,b)
#define Reader_Imp_sendCommand1(a)  Reader_Imp_sendCommandImp(a,true)
#define Reader_Imp_sendCommand0()  Reader_Imp_sendCommandImp(COMMAND_TIME_OUT,true)
#define Reader_Imp_sendCommand(...) PP_FUNC_GEN(Reader_Imp_sendCommand,PP_NARG(__VA_ARGS__)(__VA_ARGS__))

typedef struct Reader_Imp_Init_Params {

    uint8_t head ; //Tracks the length of the incoming message as we poll the software serial

    bool printDebug ; //Flag to print the serial commands we are sending to the Serial port for debug

    bool lightDebug; //system or Log for logging (system_printf = false; log_info = true)

    UART_Handle uartHandle;

    bool sim;           // solo simulacion, no enviar nada efectivamente

    uint8_t msg[MAX_MSG_SIZE];

}  Reader_Imp_Init_Params;



   extern bool Reader_Imp_begin(Reader_Imp_Init_Params * param); //If user doesn't specify then Serial will be used

   extern void Reader_Imp_enableDebugging(); //Turn on command sending and response printing. If user doesn't specify then Serial will be used
   extern void Reader_Imp_disableDebugging();

   extern void Reader_Imp_setBaud(long baudRate);
   extern void Reader_Imp_getVersion();
   extern void Reader_Imp_setReadPower(int16_t powerSetting);
   extern void Reader_Imp_getReadPower();
   extern void Reader_Imp_setWritePower(int16_t powerSetting);
   extern void Reader_Imp_getWritePower();
   extern void Reader_Imp_setRegion(uint8_t region);
   extern void Reader_Imp_setAntennaPort();
   extern void Reader_Imp_setAntennaSearchList();
   extern void Reader_Imp_setTagProtocolImp(uint8_t protocol );

   extern void Reader_Imp_startReading(); //Disable filtering and start reading continuously
   extern void Reader_Imp_stopReading(); //Stops continuous read. Give 1000 to 2000ms for the module to stop reading.

   extern void Reader_Imp_enableReadFilter();
   extern void Reader_Imp_disableReadFilter();

   extern void Reader_Imp_setReaderConfiguration(uint8_t option1, uint8_t option2);
   extern void Reader_Imp_getOptionalParameters(uint8_t option1, uint8_t option2);
   extern void Reader_Imp_setProtocolParameters(void);
   extern void Reader_Imp_getProtocolParameters(uint8_t option1, uint8_t option2);

   extern uint8_t Reader_Imp_parseResponse();

   extern uint8_t Reader_Imp_getTagEPCBytes(); //Pull number of EPC data bytes from record response.
   extern uint8_t Reader_Imp_getTagDataBytes(); //Pull number of tag data bytes from record response. Often zero.
   extern uint16_t Reader_Imp_getTagTimestamp(); //Pull timestamp value from full record response
   extern uint32_t Reader_Imp_getTagFreq(); //Pull Freq value from full record response
   extern int8_t Reader_Imp_getTagRSSI(); //Pull RSSI value from full record response

   extern bool Reader_Imp_check();

   extern uint8_t Reader_Imp_readTagEPCImp(uint8_t *epc, uint8_t *epcLength, uint16_t timeOut);
   extern uint8_t Reader_Imp_writeTagEPCImp(char *newID, uint8_t newIDLength, uint16_t timeOut );

   extern uint8_t Reader_Imp_readDataImp(uint8_t bank, uint32_t address, uint8_t *dataRead, uint8_t *dataLengthRead, uint16_t timeOut);
   extern uint8_t Reader_Imp_writeDataImp(uint8_t bank, uint32_t address, uint8_t *dataToRecord, uint8_t dataLengthToRecord, uint16_t timeOut);

   extern uint8_t Reader_Imp_readUserDataImp(uint8_t *userData, uint8_t *userDataLength, uint16_t timeOut);
   extern uint8_t Reader_Imp_writeUserDataImp(uint8_t *userData, uint8_t userDataLength, uint16_t timeOut);

   extern uint8_t Reader_Imp_readKillPWImp(uint8_t *password, uint8_t *passwordLength, uint16_t timeOut);
   extern uint8_t Reader_Imp_writeKillPWImp(uint8_t *password, uint8_t passwordLength, uint16_t timeOut );

   extern uint8_t Reader_Imp_readAccessPWImp(uint8_t *password, uint8_t *passwordLength, uint16_t timeOut );
   extern uint8_t Reader_Imp_writeAccessPWImp(uint8_t *password, uint8_t passwordLength, uint16_t timeOut);

   extern uint8_t Reader_Imp_readTIDImp(uint8_t *tid, uint8_t *tidLength, uint16_t timeOut);

   extern uint8_t Reader_Imp_killTagImp(uint8_t *password, uint8_t passwordLength, uint16_t timeOut);

   extern void Reader_Imp_sendMessageImp(uint8_t opcode, uint8_t *data, uint8_t size , uint16_t timeOut , bool waitForResponse );
   extern void Reader_Imp_sendCommandImp(uint16_t timeOut, bool waitForResponse);

   extern void Reader_Imp_printMessageArray(void);

   extern uint16_t Reader_Imp_calculateCRC(uint8_t *u8Buf, uint8_t len);

    //Variables

    //This is our universal msg array, used for all communication
    //Before sending a command to the module we will write our command and CRC into it
    //And before returning, response will be recorded into the msg array. Default is 255 bytes.

    //uint16_t tags[MAX_NUMBER_OF_TAGS][12]; //Assumes EPC won't be longer than 12 bytes
    //uint16_t tagRSSI[MAX_NUMBER_OF_TAGS];
    //uint16_t uniqueTags = 0;



