#include "arduino.h"
#include "libAzFunction.h"

void AzFunction::init(int txdenPin, int SerialPort, long baudrate, bool logPrint){
    txden = txdenPin;
    port = SerialPort;
    log = logPrint;
    
    if(log){
        Serial.print("txden:");
        Serial.print(txden);
        Serial.print(" port:Serial");
        Serial.print(SerialPort);
        Serial.print(" baudrate:");
        Serial.println(baudrate);
    }
    pinMode(txden, OUTPUT);
    digitalWrite(txden, LOW);

    switch (port){
        case 0:
            Serial.begin(baudrate);
            break;
        case 1:
            Serial1.begin(baudrate);
            break;
        case 2:
            Serial2.begin(baudrate);
            break;
        case 3:
            Serial3.begin(baudrate);
            break;
        default:
            break;
    }
}

void AzFunction::driverInput(int id, int command){
    if(log){
        Serial.print("ID:");
        Serial.print(id);
        Serial.print(" command:");
        switch(command){
            case 32768:
                Serial.println("RV_POS");
                break;
            case 16384:
                Serial.println("FW_POS");
                break;
            case 128:
                Serial.println("ALM_RST");
                break;
            case 64:
                Serial.println("FREE");
                break;
            case 32:
                Serial.println("STOP");
                break;
            case 16:
                Serial.println("ZHOME");
                break;
            case 8:
                Serial.println("START");
                break;
            case 0:
                Serial.println("OFF");
                break;
            default:
                Serial.println("OPTION");
                break;
        }
    }

    int dataNum = 8;
    sendData[0] = byte(id);
    sendData[1] = 0x06;

    sendData[2] = 0x00;
    sendData[3] = 0x7d;

    sendData[4] = (command >> 8) & 0xff;
    sendData[5] = (command >> 0) & 0xff;

    sendData[6] = crc16(dataNum-2, low);
    sendData[7] = crc16(dataNum-2, up);

    send(dataNum, w);
}

void AzFunction::lrdDriverInput(int id, int command){
    if(log){
        Serial.print("ID:");
        Serial.print(id);
        Serial.print(" command:");
        switch(command){
            case 8192:
                Serial.println("LRD_C_ON");
                break;
            case 2048:
                Serial.println("LRD_HOME");
                break;
            case 256:
                Serial.println("LRD_START");
                break;
            case 4:
                Serial.println("LRD_M2");
                break;
            case 2:
                Serial.println("LRD_M1");
                break;
            case 1:
                Serial.println("LRD_M0");
                break;
            case 0:
                Serial.println("LRD_OFF");
                break;
            default:
                Serial.println("OPTION");
                break;
        }
    }

    int dataNum = 8;
    sendData[0] = byte(id);
    sendData[1] = 0x06;

    sendData[2] = 0x00;
    sendData[3] = 0x1e;

    sendData[4] = (command >> 8) & 0xff;
    sendData[5] = (command >> 0) & 0xff;

    sendData[6] = crc16(dataNum-2, low);
    sendData[7] = crc16(dataNum-2, up);

    send(dataNum, w);
}

bool AzFunction::driverOutput(int id, int signal){
    if(log){
        Serial.print("request :");
        switch(signal){
            case 3:
                Serial.println("START_R");
                break;
            case 4:
                Serial.println("HOME-END");
                break;
            case 5:
                Serial.println("READY");
                break;
            case 6:
                Serial.println("INFO");
                break;
            case 7:
                Serial.println("ALM-A");
                break;
            case 8:
                Serial.println("SYS-BSY");
                break;
            case 13:
                Serial.println("MOVE");
                break;
            case 14:
                Serial.println("IN-POS");
                break;
            case 15:
                Serial.println("TLC");
                break;
            default:
                break;
        }
    }
    int dataNum = 8;

    sendData[0] = byte(id);
    sendData[1] = 0x03;
    
    sendData[2] = 0x00;
    sendData[3] = 0x7f;

    sendData[4] = 0x00;
    sendData[5] = 0x01;

    sendData[6] = crc16(dataNum-2, low);
    sendData[7] = crc16(dataNum-2, up);

    send(dataNum, r);

    delay(10);
    bool data = recvOutput(signal);

    if(log){
        switch(signal){
            case 3:
                Serial.print("START_R");
                break;
            case 4:
                Serial.print("HOME-END");
                break;
            case 5:
                Serial.print("READY");
                break;
            case 6:
                Serial.print("INFO");
                break;
            case 7:
                Serial.print("ALM-A");
                break;
            case 8:
                Serial.print("SYS-BSY");
                break;
            case 13:
                Serial.print("MOVE");
                break;
            case 14:
                Serial.print("IN-POS");
                break;
            case 15:
                Serial.print("TLC");
                break;
            default:
                break;
        }

        Serial.print("  state:");
        if(data == 0)Serial.println("FALSE");
        else Serial.println("TRUE");
    }
    return data;
}

bool AzFunction::lrdDriverOutput(int id, int signal){
    if(log){
        Serial.print("request :");
        switch(signal){
            case 10:
                Serial.println("LRD_MOVE");
                break;
            case 11:
                Serial.println("LRD_HOME_P");
                break;
            case 13:
                Serial.println("LRD_READY");
                break;
            default:
                break;
        }
    }
    int dataNum = 8;

    sendData[0] = byte(id);
    sendData[1] = 0x03;
    
    sendData[2] = 0x00;
    sendData[3] = 0x20;

    sendData[4] = 0x00;
    sendData[5] = 0x01;

    sendData[6] = crc16(dataNum-2, low);
    sendData[7] = crc16(dataNum-2, up);

    send(dataNum, r);

    delay(10);
    bool data = recvOutput(signal);

    if(log){
        switch(signal){
            case 10:
                Serial.println("LRD_MOVE");
                break;
            case 11:
                Serial.println("LRD_HOME_P");
                break;
            case 13:
                Serial.println("LRD_READY");
                break;
            default:
                break;
        }

        Serial.print("  state:");
        if(data == 0)Serial.println("FALSE");
        else Serial.println("TRUE");
    }
    return data;
}

bool AzFunction::recvOutput(int signal){
    int count = 0;
    switch (port){
        case 0:
            while(Serial.available()>0){
                recvData[count] = Serial.read();
                count++;
            }
            break;
        case 1:
            while(Serial1.available()>0){
                recvData[count] = Serial1.read();
                count++;
            }
            break;
        case 2:
            while(Serial2.available()>0){
                recvData[count] = Serial2.read();
                count++;
            }
            break;
        case 3:
            while(Serial3.available()>0){
                recvData[count] = Serial3.read();
                count++;
            }
            break;
        default:
            break;
    }

    if(log){
        Serial.print("recvData:");
        for(int i=0; i<count; i++){
            Serial.print(recvData[i], HEX);
            Serial.print(",");
        }
        Serial.println();
    }
    int a = (recvData[3] << 8) + recvData[4];

    return (bitRead(a, signal));
}

void AzFunction::setParam(int id, int dataNo, int param, long value){
    if(log){
        Serial.print("ID:");
        Serial.print(id);
        Serial.print(" No:");
        Serial.print(dataNo);
        Serial.print(" param:");
        switch(param){
            case 0:
                Serial.print("MODE");
                break;
            case 2:
                Serial.print("STEPS");
                break;
            case 4:
                Serial.print("SPEED");
                break;
            case 6:
                Serial.print("START_RATE");
                break;
            case 8:
                Serial.print("STOP_RATE");
                break;
        }
        Serial.print(" value:");
        Serial.println(value);
    }

    int dataNum = 13;

    sendData[0] = byte(id);
    sendData[1] = 0x10;

    long addr = 6144 + dataNo*64 + param;
    sendData[2] = (addr >> 8) & 0xff;
    sendData[3] = (addr >> 0) & 0xff;

    sendData[4] = 0x00;
    sendData[5] = 0x02;
    sendData[6] = 0x04;

    sendData[7]  = (value >> 24) & 0xff;
    sendData[8]  = (value >> 16) & 0xff;
    sendData[9]  = (value >>  8) & 0xff;
    sendData[10] = (value >>  0) & 0xff;

    sendData[11] = crc16(dataNum-2, low);
    sendData[12] = crc16(dataNum-2, up);

    send(dataNum, w);
}

long AzFunction::readParam(int id, int dataNo, int param){
    if(log){
        Serial.print("ID:");
        Serial.print(id);
        Serial.print(" No:");
        Serial.print("dataNo");
        Serial.print(" request :");

        switch(param){
            case 0:
                Serial.println("MODE");
                break;
            case 2:
                Serial.println("STEPS");
                break;
            case 4:
                Serial.println("SPEED");
                break;
            case 6:
                Serial.println("START_RATE");
                break;
            case 8:
                Serial.println("STOP_RATE");
                break;
            default:
                break;
        }
    }
    int dataNum = 8;
    sendData[0] = byte(id);
    sendData[1] = 0x03;

    long addr = 6144 + dataNo*64 + param;
    sendData[2] = (addr >> 8) & 0xff;
    sendData[3] = (addr >> 0) & 0xff;

    sendData[4] = 0x00;
    sendData[5] = 0x02;

    sendData[6] = crc16(dataNum-2, low);
    sendData[7] = crc16(dataNum-2, up); 

    send(dataNum, r);

    delay(10);
    long data =  recvParam();

    if(log){
        switch(param){
            case 0:
                Serial.print("MODE");
                break;
            case 2:
                Serial.print("STEPS");
                break;
            case 4:
                Serial.print("SPEED");
                break;
            case 6:
                Serial.print("START_RATE");
                break;
            case 8:
                Serial.print("STOP_RATE");
                break;
            default:
                break;
        }
        Serial.print("  value:");
        Serial.println(data);
    }
    return data;
}

long AzFunction::recvParam(){
    int count = 0;
    switch (port){
        case 0:
            while(Serial.available()>0){
                recvData[count] = Serial.read();
                count++;
            }
            break;
        case 1:
            while(Serial1.available()>0){
                recvData[count] = Serial1.read();
                count++;
            }
            break;
        case 2:
            while(Serial2.available()>0){
                recvData[count] = Serial2.read();
                count++;
            }
            break;
        case 3:
            while(Serial3.available()>0){
                recvData[count] = Serial3.read();
                count++;
            }
            break;
        default:
            break;
    }

    if(log){
        Serial.print("recvData:");
        for(int i=0; i<count; i++){
            Serial.print(recvData[i], HEX);
            Serial.print(",");
        }
     }
     Serial.println();

    int data = (recvData[3] <<24) + (recvData[4] << 16) + (recvData[5] << 8) + recvData[6];

    return data;
}

void AzFunction::directDrive(int id, long dataNo, long driveMode, long steps, long speed, long startRate, long stopRate, long driveCurrent){
    if(log){
        Serial.print("ID:");
        Serial.print(id);
        Serial.print(" No:");
        Serial.print(dataNo);
        Serial.print(" MODE");
        Serial.print(driveMode);
        Serial.print(" STEPS");
        Serial.print(steps);
        Serial.print(" SPEED");
        Serial.print(speed);
        Serial.print(" START_RATE:");
        Serial.print(startRate);
        Serial.print(" STOP_RATE:");
        Serial.print(startRate);
        Serial.print(" CURRENT:");
        Serial.println(driveCurrent);
    }
    
    int dataNum = 41;

    sendData[0] = byte(id);
    sendData[1] = 0x10;
    
    sendData[2] = 0x00;
    sendData[3] = 0x58;
    sendData[4] = 0x00;
    sendData[5] = 0x10;
    sendData[6] = 0x20;

    sendData[7]  = (dataNo >> 24) & 0xff;
    sendData[8]  = (dataNo >> 16) & 0xff;
    sendData[9]  = (dataNo >>  8) & 0xff;
    sendData[10] = (dataNo >>  0) & 0xff;

    sendData[11] = (driveMode >> 24) & 0xff;
    sendData[12] = (driveMode >> 16) & 0xff;
    sendData[13] = (driveMode >>  8) & 0xff;
    sendData[14] = (driveMode >>  0) & 0xff;

    sendData[15] = (steps >> 24) & 0xff;
    sendData[16] = (steps >> 16) & 0xff;
    sendData[17] = (steps >>  8) & 0xff;
    sendData[18] = (steps >>  0) & 0xff;

    sendData[19] = (speed >> 24) & 0xff;
    sendData[20] = (speed >> 16) & 0xff;
    sendData[21] = (speed >>  8) & 0xff;
    sendData[22] = (speed >>  0) & 0xff;

    sendData[23] = (startRate >> 24) & 0xff;
    sendData[24] = (startRate >> 16) & 0xff;
    sendData[25] = (startRate >>  8) & 0xff;
    sendData[26] = (startRate >>  0) & 0xff;

    sendData[27] = (stopRate >> 24) & 0xff;
    sendData[28] = (stopRate >> 16) & 0xff;
    sendData[29] = (stopRate >>  8) & 0xff;
    sendData[30] = (stopRate >>  0) & 0xff;

    sendData[31] = (driveCurrent >> 24) & 0xff;
    sendData[32] = (driveCurrent >> 16) & 0xff;
    sendData[33] = (driveCurrent >>  8) & 0xff;
    sendData[34] = (driveCurrent >>  0) & 0xff;

    sendData[35] = 0x00;
    sendData[36] = 0x00;
    sendData[37] = 0x00;
    sendData[38] = 0x01;

    sendData[39] = crc16(dataNum-2, low);
    sendData[40] = crc16(dataNum-2, up); 

    send(dataNum, w);
}

void AzFunction::send(int sendDataNum, bool writeOnly){
    if(log){
        Serial.print("send:");
        for(int i=0; i<sendDataNum; i++){
            Serial.print(sendData[i], HEX);
            Serial.print(",");
        }
        Serial.println(" ");
    }

    digitalWrite(txden, HIGH);

    switch (port){
        case 0:
            Serial.write(sendData, sendDataNum);
            break;
        case 1:
            Serial1.write(sendData, sendDataNum);
            break;
        case 2:
            Serial2.write(sendData, sendDataNum);
            break;
        case 3:
            Serial3.write(sendData, sendDataNum);
            break;
        default:
            break;
    }
    if(writeOnly)delay(25);//delay(25);
    else delay(10);//delay(10);
    digitalWrite(txden, LOW);
}

byte AzFunction::crc16(int elementNum, bool uplow){
    unsigned short i = 0xffff;
    int j = 0;
    for(int m = 0; m < elementNum; m++){
        i = i ^ sendData[m];

        for(int n = 0; n < 8; n++){
            j = bitRead(i, 0);
            i = i >> 1;
            if (j)i = i ^ 0xA001;
        }
    }

    if(uplow == up)i = i >> 8;
    else i = i & 0x00FF;

    return i;
}
