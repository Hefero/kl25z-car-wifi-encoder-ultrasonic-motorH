#ifndef MOTORH_H
#define MOTORH_H

#include "mbed.h"
#include "ultrasonic.h"

#define SONIC_DISTANCE_DEBOUNCE_SIZE 15

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
        void bypass(void);
        int sonicDistanceDebounced;
        bool checkObstacle(void);
    private:
        int minValue(int array[SONIC_DISTANCE_DEBOUNCE_SIZE]);
        void countEsqIrq(void);
        void countDirIrq(void);
        bool bypassing = false;
        void distIrq();
        int debouceCounter = 0;
        int sonicDistanceDebouncer[SONIC_DISTANCE_DEBOUNCE_SIZE] = { 0 };
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