
#include "MotorH.h"
#include <cstdint>

#define H_IN1 PTD4 // ponte H
#define H_IN2 PTA12
#define H_IN3 PTA4
#define H_IN4 PTA5
#define PWM_FREQ 0.01f //frequencia do pwn 1kHz
#define velDir 0.45
#define velEsq 0.55

#define ENC_ESQ_PIN PTD7 // encoders
#define ENC_DIR_PIN PTD6

#define TRIGGER_PIN PTA16 // ultrassom
#define ECHO_PIN PTD1
#define UPDATE_DIST_FREQ .01
#define ULTRASOUND_TIMEOUT_S 1
#define MINIMUN_OBSTACLE_DISTANCE 300

#define LEFT_TURN_DISTANCE 99
#define RIGHT_TURN_DISTANCE 99


motorH::motorH(void):_in1(H_IN1),_in2(H_IN2),_in3(H_IN3),_in4(H_IN4)
    ,_encoderEsq(ENC_ESQ_PIN),_encoderDir(ENC_DIR_PIN)
    ,_mu(TRIGGER_PIN, ECHO_PIN, UPDATE_DIST_FREQ, ULTRASOUND_TIMEOUT_S, NULL)
    ,_serial(USBTX, USBRX)
{
    _in1.period(PWM_FREQ); // seta frequencia do pwm
    _in2.period(PWM_FREQ);
    _in3.period(PWM_FREQ);
    _in4.period(PWM_FREQ);
    _mu.startUpdates(); // inicia medicao do ultrassom
    _encoderEsq.rise(callback(this, &motorH::countEsqIrq)); // interrupção que atualiza encoder
    _encoderDir.rise(callback(this, &motorH::countDirIrq));
    _tout.attach(callback(this,&motorH::distIrq),UPDATE_DIST_FREQ); // interrupção que atualiza distancia do ultrassom
}

void motorH::distIrq(void)
{
    if (debouceCounter < SONIC_DISTANCE_DEBOUNCE_SIZE) // atualiza vetor das ultimas medições
    {
        sonicDistanceDebouncer[debouceCounter] = _mu.getCurrentDistance();
        debouceCounter++;    
    }
    else
    {
        debouceCounter = 0;
    }
    if (debouceCounter == SONIC_DISTANCE_DEBOUNCE_SIZE)
    {
        checkObstacle();
    }
}

void motorH::bypass(void)
{
    while (checkObstacle())
    {
        _serial.printf("\n\rrotina de desvio");
        hardStop();
    }
}

bool motorH::checkObstacle(void)
{
    if(getSonicDistance() < MINIMUN_OBSTACLE_DISTANCE) // obstáculo detectado
    {
        _serial.printf("\n\robstaculo detectado");
        return true;        
    }
    else
    {
        return false;
    }
}

void motorH::moveForward(int dist)
{
    _serial.printf("\n\rmovendo para frente");
    pulsoDir = 0;
    while(pulsoDir < dist)
    {        
        moving = true;
        if(checkObstacle())
        {
            bypass();
        }
        motorDir(1, velDir); 
        motorEsq(1, velEsq);
    }
    stop();
}
void motorH::moveBackwards(int dist)
{
    _serial.printf("\n\rmovendo para tras");    
    pulsoDir = 0;
    while(pulsoDir < dist)
    {
        moving = true;
        if(checkObstacle())
        {
            bypass();
        }
        motorDir(-1, velDir);
        motorEsq(-1, velEsq);
    }
    stop();
}

void motorH::stop(void)
{
    moving = false;
    motorDir(0, 0); // inicia os motores em ponto morto
    motorEsq(0, 0); // inicia os motores em ponto morto
   
}

void motorH::hardStop(void)
{
    moving = false;
    motorDir(2, 0); // trava os motores
    motorEsq(2, 0); // trava os motores    
}

void motorH::turnLeft(void)
{
    _serial.printf("\n\rvirar para esquerda");
    pulsoDir = 0;
    while(pulsoDir < LEFT_TURN_DISTANCE)
    {
        moving = true;
        if(checkObstacle())
        {
            bypass();
        }
        motorDir(1, velDir);
        motorEsq(-1, velEsq);
    }
    stop();
}

void motorH::turnRight(void)
{
    _serial.printf("\n\rvirar para direita");
    pulsoDir = 0;
    while(pulsoDir < RIGHT_TURN_DISTANCE)
    {
        moving = true;
        if(checkObstacle())
        {
            bypass();
        }
        motorDir(-1, velDir);
        motorEsq(1, velEsq);
    }
    stop();
}   

void motorH::motorDir(int command, float duty)
{
    switch(command) 
    {
    case 0: // ponto morto
        _in1.write(0);
        _in2.write(0);
        break;
    case 1: // move forward
        _in1.write(0);
        _in2.write(duty);
        break;
    case -1: // move backwards
        _in1.write(duty);
        _in2.write(0);
        break;
    case 2: // hard break
        _in1.write(1.0f);
        _in2.write(1.0f);
        break;
    }
}

void motorH::motorEsq(int command, float duty)
{
    switch(command) 
    {
    case 0: // ponto morto
        _in3.write(0);
        _in4.write(0);
        break;
    case 1: // move forward
        _in3.write(0);
        _in4.write(duty);
        break;
    case -1: // move backwards
        _in3.write(duty);
        _in4.write(0);
        break;
    case 2: // hard break
        _in3.write(1.0f);
        _in4.write(1.0f);
        break;
    }
}

void motorH::execute(char rxData[8])
{
    int dist_x, dist_y;
    char dist_char_x[3], dist_char_y[3];

    dist_char_x[0] = rxData[1];
    dist_char_x[1] = rxData[2];
    dist_char_x[2] = rxData[3];

    dist_char_y[0] = rxData[1];
    dist_char_y[1] = rxData[2];
    dist_char_y[2] = rxData[3];

    dist_x = atoi(dist_char_x);
    dist_y = atoi(dist_char_y);

    // frente esquerda
    if (rxData[0] == '+' && rxData[4] == '+')
    {
        _serial.printf("\n\rcomando frente esquerda");
        if (dist_x > 0) {moveForward(dist_x);}
        if (dist_y > 0) {turnLeft();moveForward(dist_y);}
    }
    // frente direita
    if (rxData[0] == '+' && rxData[4] == '-')
    {
        _serial.printf("\n\rcomando frente direita");
        if (dist_x > 0) {moveForward(dist_x);}
        if (dist_y > 0) {turnRight();moveForward(dist_y);}
    }
    // trás esquerda
    if (rxData[0] == '-' && rxData[4] == '+')
    {
        _serial.printf("\n\rcomando trás esquerda");
        if (dist_x > 0) {moveBackwards(dist_x);}
        if (dist_y > 0) {turnLeft();moveForward(dist_y);}
    }
    // trás direita
    if (rxData[0] == '-' && rxData[4] == '-')
    {
        _serial.printf("\n\rcomando trás direita");
        if (dist_x > 0) {moveBackwards(dist_x);}
        if (dist_y > 0) {turnRight();moveForward(dist_y);}
    }
    // 11111111 panic stop
    if (rxData[0] == '1' && rxData[4] == '1')
    {
        _serial.printf("\n\rcomando parada imediata");
        stop();
    }    
}

void motorH::countEsqIrq(void)
{
    pulsoEsq++;
}

void motorH::countDirIrq(void)
{
    pulsoDir++;    
}

int motorH::getSonicDistance(void) // retorna o menor valor do array
{
    return minValue(sonicDistanceDebouncer);
}

void motorH::debug(void)  // debug function
{
    _serial.printf("\n\resq:%d dir%d dist:%dmm",pulsoEsq,pulsoDir,getSonicDistance());
}

int motorH::minValue(int array[SONIC_DISTANCE_DEBOUNCE_SIZE])
{
    int n = SONIC_DISTANCE_DEBOUNCE_SIZE;
    int mini = INT_MAX;
    for(int i=0; i<n; i++){
        if(array[i]<mini)
        mini = array[i];
    }
    return mini;
}