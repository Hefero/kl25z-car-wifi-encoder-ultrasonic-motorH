#include "mbed.h"
#include "my_nrf24l01p.h"
#include "MotorH.h"

#define DEBUG_MSG_INTERVAL 5.0
#define HAS_MSG_INTERVAL 0.01 // intervalo de checagem entre comandos recebidos pelo modulo nrf25l01p

Serial pc(USBTX, USBRX); // debug via serial

motorH *motor = new motorH(); // classe de controle do motor

my_nrf24l01p *nrf24l01p = new my_nrf24l01p(); // classe de controle do receptor wifi

Ticker debugTicker; // ticker que mostra mensagens de debug
Ticker messageTicker; // ticker que verifica novas mensagens

void hasMessageTicker(){
    if(nrf24l01p->hasMessage){
        pc.printf("\n\rcommand received\n\r");
        nrf24l01p->hasMessage = false; // desabilita flag de nova mensagem disponível
        motor->execute(nrf24l01p->rxData); // executa o comando recebido na mensagem
    }
}

int main() {
    motor->stop(); // inicia com os motores parados
    debugTicker.attach(callback(motor,&motorH::debug),DEBUG_MSG_INTERVAL); // habilita ticker que exibe mensagens de debug do Motor    
    messageTicker.attach(callback(nrf24l01p,&my_nrf24l01p::checkMessage),HAS_MSG_INTERVAL);  // ativa ticker que checa mensagens de comando a cada intervalo 
    while (1) {
        hasMessageTicker(); // executa o comando caso haja mensagem nova recebida
    }
}
