#ifndef MY_NRF24L01P_H
#define MY_NRF24L01P_H

#include "mbed.h"
#include "nRF24L01P.h"

#define TRANSFER_SIZE 8

class my_nrf24l01p
{
    public:
        my_nrf24l01p(void);        
        char txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];
        int txDataCnt = 0;
        int rxDataCnt = 0;
        void checkMessage(void);
        bool hasMessage = false;
    private:
        nRF24L01P _my_nrf24l01p;
        Serial _serial;
};
#endif