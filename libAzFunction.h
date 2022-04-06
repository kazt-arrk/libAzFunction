#ifndef libAzFunction_H_
#define libAzFunction_H_
#include "arduino.h"

#define arrayNum 50

//param
#define MODE        0
#define STEPS       2
#define SPEED       4
#define START_RATE  6
#define STOP_RATE   8

//command
#define RV_POS      32768  
#define FW_POS      16384 
#define RV_JOG_P    8192  
#define FW_JOG_P    4096   
#define R_IN11      2048   
#define R_IN10      1024  
#define R_IN9       512    
#define R_IN8       256  
#define ALM_RST     128 
#define FREE        64 
#define STOP        32 
#define ZHOME       16 
#define START       8 
#define R_IN2       4 
#define R_IN1       2      
#define R_IN0       1
#define OFF         0

//signal
#define START_R  3
#define HOME_END    4
#define READY       5
#define INFO        6
#define ALM_A       7
#define SYS_BSY     8
#define MOVE        13
#define IN_POS      14
#define TLC         15

//LRD-command
#define LRD_C_ON 8192
#define LRD_STOP 4096
#define LRD_HOME 2048
#define LRD_RVS 1024
#define LRD_FWD 512
#define LRD_START 256
#define LRD_M5 32
#define LRD_M4 16
#define LRD_M3 8
#define LRD_M2 4
#define LRD_M1 2
#define LRD_M0 1
#define LRD_OFF 0

//LRD-signal
#define LRD_MOVE 10
#define LRD_HOME_P 11
#define LRD_READY 13


class AzFunction{
    
    public:
        void init(int txdenPin, int SerialPort, long baudrate, bool logPrint);
        void driverInput(int id, int command);
        void lrdDriverInput(int id, int command);
        bool driverOutput(int id, int signal);
        bool lrdDriverOutput(int id, int signal);
        void setParam(int id, int dataNo, int param, long value);
        long readParam(int id, int dataNo, int param);
        void directDrive(int id, long dataNo, long dirveMode, long speed, long steps, long startRate, long stopRate, long driveCurrent);

    private:
        int txden, port;
        bool log;
        const bool w = true;
        const bool r = false;
        byte sendData[arrayNum];
        int sendDataNum;
        long recvData[arrayNum];
        bool up = true, low = false;
        unsigned long t_base = 0;
        unsigned long t_diff = 0;

        void send(int sendDataNum, bool writeOnly);
        long recvParam();
        bool recvOutput(int signal);
        byte crc16(int elementNum, bool uplow);
};
#endif
