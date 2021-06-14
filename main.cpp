#include "mbed.h"

//PowerButton
DigitalIn PBt(D3);      //PowerButton
DigitalOut PLd(LED1);   //PowerLed

//Motor Right
DigitalOut MRF(D7);     //Motor Right Forward
DigitalOut MRB(D6);     //Motor Right Backward
//Motor Left
DigitalOut MLF(D5);     //Motor Left Forward
DigitalOut MLB(D4);     //Motor left Backward

//Ultrasonic
DigitalOut Trig(D9);    //Trig pin (used to send a wave)  
DigitalIn Echo(D8);     //Echo pin (High while wave is sending)

bool Power = false;
Timer BdownT;
bool Turn = false;
Timer pulse;
long echodur;
int echodis;
int correction = 0;

void Powercheck(){
    if (PBt){
        BdownT.start();
        bool pressed = true;
        while(PBt){
            //if power is off than button must be pressed 5 sec, if on then 1 sec.
            if (((BdownT.read()> 5) and (pressed)) or ((BdownT.read()> 1) and (pressed) and (!Power))){
            Power = !Power;
            PLd = Power; //sets led to indicate status of Power variable
            pressed = false;
            }
        }
        BdownT.stop();
        BdownT.reset();
        //ThisThread::sleep_for(100); //debounce button
    }
}


bool object(int dis){
    // trigger sonar to send a ping
        Trig = 1;
        pulse.reset();
        wait_us(10.0);
        Trig = 0;
        while (Echo==0) {};
        pulse.start();
        while (Echo==1) {};
        pulse.stop();
        echodis = (pulse.read_us()-correction)/58.0;
        ThisThread::sleep_for(200);
        return (dis>echodis);
}

// main() runs in its own thread in the OS
int main()
{
    while (true) {
        Powercheck();
        if (Power){
            if (object(75)){
                Turn = true;
            }
            if (Turn){
                MRF = 1;
                MRB = 0;
                MLF = 0;
                MLB = 1;
                ThisThread::sleep_for(80);
                Turn = false;

            }else{
                MRF = 1;
                MRB = 0;
                MLF = 1;
                MLB = 0;
            }
        }
    }
}

