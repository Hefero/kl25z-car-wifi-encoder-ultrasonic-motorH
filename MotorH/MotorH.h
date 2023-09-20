#ifndef MOTORH_H
#define MOTORH_H

#include "mbed.h"
#include "ultrasonic.h"



class motorH
{
    public:
        motorH(void);
        /**iniates the class with the specified trigger pin, echo pin, update speed and timeout**/
        void motorDir(int command, float duty);
        /**iniates the class with the specified trigger pin, echo pin, update speed, timeout and method to call when the distance changes**/
        void motorEsq(int command, float duty);
        /**pauses measuring the distance**/
        void moveForward(int dist);
        void moveBackwards(int dist);
        void turnLeft(void);
        void turnRight(void);
        void stop(void);
        void hardStop(void);
        void execute(char rxData[8]);
        int pulsoEsq = 0;
        int pulsoDir = 0;
        int getSonicDistance(void);
        void debug(void);
        bool moving = false;
        bool obstacle = false;
        void bypass(void);
    private:        
        void countEsqIrq(void);
        void countDirIrq(void);
        void distIrq();
        PwmOut _in1;
        PwmOut _in2;
        PwmOut _in3;
        PwmOut _in4;
        Serial _serial;
        ultrasonic _mu;
        InterruptIn _encoderEsq;//(PTD7);
        InterruptIn _encoderDir;//(PTD6);
        Ticker _tout;
};
#endif