#include "mbed.h"
#include "my_nrf24l01p.h"
#include "MotorH.h"

#define DEBUG_MSG_INTERVAL 5.0
#define HAS_MSG_INTERVAL 0.01
#define MAX_EXECUTION_TIME 60.0

Serial pc(USBTX, USBRX);

motorH *motor = new motorH(); // classe de controle do motor

my_nrf24l01p *nrf24l01p = new my_nrf24l01p(); // classe de controle do receptor wifi

Ticker debugTicker;
Ticker messageTicker;
Timeout executionTicker;

void hasMessageTicker(){
    if(nrf24l01p->hasMessage){        
        pc.printf("command received\n\r");
        motor->execute(nrf24l01p->rxData);
        nrf24l01p->hasMessage = false;
    }
}

int main() {
    motor->stop();
    debugTicker.attach(callback(motor,&motorH::debug),DEBUG_MSG_INTERVAL); //habilita debugger do Motor    
    messageTicker.attach(callback(nrf24l01p,&my_nrf24l01p::checkMessage),HAS_MSG_INTERVAL);    
    while (1) {
        hasMessageTicker();
    }
}
