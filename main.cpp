#include "mbed.h"
#include "nRF24L01P.h"
#include "my_nrf24l01p.h"
#include "MotorH.h"

#define DEBUG_MSG_INTERVAL 5.0
#define MAX_EXECUTION_TIME 60.0

Serial pc(USBTX, USBRX); // tx, rx

motorH *motor = new motorH(); // classe de controle do motor

my_nrf24l01p *nrf24l01p = new my_nrf24l01p(); // classe de controle do receptor wifi

DigitalOut greenLED(LED_GREEN);
DigitalOut redLED(LED_RED);
Ticker timer1;
Ticker timer2;

int main() {
    motor->stop();
    timer1.attach(callback(motor,&motorH::debug),DEBUG_MSG_INTERVAL); //habilita debugger do Motor
    
    while (1) {
        nrf24l01p->checkMessage();
        if(nrf24l01p->hasMessage){
            //timer2.attach(callback(motor,&motorH::execute(nrf24l01p->rxData)),MAX_EXECUTION_TIME);
            motor->execute(nrf24l01p->rxData);
            nrf24l01p->hasMessage = false;
        }
    }
}
