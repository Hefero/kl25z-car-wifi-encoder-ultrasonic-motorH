#include "mbed.h"
#include "my_nrf24l01p.h"
#include "MotorH.h"

#define DEBUG_MSG_INTERVAL 5.0
#define HAS_MSG_INTERVAL 0.01

Serial pc(USBTX, USBRX);

motorH *motor = new motorH(); // classe de controle do motor

my_nrf24l01p *nrf24l01p = new my_nrf24l01p(); // classe de controle do receptor wifi

Ticker debugTicker;
Ticker messageTicker;

void hasMessageTicker(){
    if(nrf24l01p->hasMessage){
        pc.printf("\n\rcommand received\n\r");
        nrf24l01p->hasMessage = false;
        motor->execute(nrf24l01p->rxData);        
    }
}

int main() {
    motor->stop();
    debugTicker.attach(callback(motor,&motorH::debug),DEBUG_MSG_INTERVAL); // habilita debugger do Motor    
    messageTicker.attach(callback(nrf24l01p,&my_nrf24l01p::checkMessage),HAS_MSG_INTERVAL);  // checa mensagens a cada intervalo 
    while (1) {
        hasMessageTicker(); // executa o comando caso haja mensagem nova recebida
    }
}
