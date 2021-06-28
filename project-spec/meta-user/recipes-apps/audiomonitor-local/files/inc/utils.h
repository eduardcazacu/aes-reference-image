#ifndef __INCL_UTILS
#define __INCL_UTILS

#include "libs.h"

#define I2C_AUDIO_ADDR 0x01A

#define REG00_ADC_VOL_L 0x00
#define REG01_ADC_VOL_R 0x01
#define REG02_DAC_VOL_L 0x02
#define REG03_DAC_VOL_R 0x03
#define REG04_ANAL_PATH 0x04
#define REG05_DIGI_PATH 0x05
#define REG06_POWR_MGMT 0x06
#define REG07_DIGI_A_IF 0x07
#define REG08_SMPL_RATE 0x08
#define REG09_DIGI_ACTV 0x09
#define REG15_SOFTW_RST 0x0F
#define REG16_ALC_CTL_1 0x10
#define REG17_ALC_CTL_2 0x11
#define REG18_NOIS_GATE 0x12

#define CNTRL_REG_BASE 0x4120'0000
#define DISTO_REG_BASE 0x4121'0000
#define OCTAV_REG_BASE 0x4122'0000
#define TREMO_REG_BASE 0x4123'0000
#define DELAY_REG_BASE 0x4124'0000

#define MID_READ_VALUE 8388608

#define MIN_RDPKT_SIZE 512
#define MAX_VIEWHEIGHT 35 
#define AVG_BUFFE_SIZE 10
#define HTTP_FRAM_SIZE 16
#define RDPKT_DIV_FRAM 32

const double PI = 3.141592653589793238460;

int Peek(unsigned addr);
int Poke(unsigned addr, unsigned val);

int fileResponse(std::string path, utf8string* msgBody, utf8string* msgType, int* errCode);

int AudioCodec_Read(uint8_t reg, uint16_t* read_data);
int AudioCodec_Write(uint8_t reg, uint16_t write_data);
int AudioCodec_Default();

#endif