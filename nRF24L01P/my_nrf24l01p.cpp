#include "my_nrf24l01p.h"

#define MOSI PTD2
#define MISO PTD3
#define SCK PTC5
#define CSN PTD0
#define CE PTD5
#define IRQ PTA13

#define RX_ADDRESS 12344321


my_nrf24l01p::my_nrf24l01p(void):_my_nrf24l01p(PTD2, PTD3, PTC5, PTD0, PTD5, PTA13),_serial(USBTX, USBRX){    
    _serial.printf( "Initializing Receiver\r\n");
    _my_nrf24l01p.powerUp();
    _my_nrf24l01p.setTransferSize(TRANSFER_SIZE);
    _my_nrf24l01p.setReceiveMode();
    _my_nrf24l01p.setRxAddress(RX_ADDRESS);
    _my_nrf24l01p.enable();
    // Display the (default) setup of the nRF24L01+ chip
    _serial.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  _my_nrf24l01p.getRfFrequency() );
    _serial.printf( "nRF24L01+ Output power : %d dBm\r\n",  _my_nrf24l01p.getRfOutputPower() );
    _serial.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", _my_nrf24l01p.getAirDataRate() );
    _serial.printf( "Type keys to test transfers:\r\n  (transfers are grouped into %d characters)\r\n", TRANSFER_SIZE );
}

void my_nrf24l01p::checkMessage(void)
{
    if ( _my_nrf24l01p.readable() ) { // If we've received anything in the nRF24L01+...
            rxDataCnt = _my_nrf24l01p.read( NRF24L01P_PIPE_P0, rxData, sizeof( rxData ) );
            for ( int i = 0; rxDataCnt > 0; rxDataCnt--, i++ ) {
                _serial.putc( rxData[i] );
            }            
            hasMessage = true;
        }
}