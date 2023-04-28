#include <suta.h>

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#ZERO_RAM

int ms;
int8 sutax, sutafinish, curState;
unsigned long int time, timerSuta, timerBlink, timerPhase;
int t1_start, t1_end;//,t2_end;// t3_start, t3_end;
float dif1;//, dif2;
int angle1;
//int angle2;
float pf;
int1 phq, sutadone, okstate, blink;


#INT_TIMER0
void  TIMER0_isr(void) 
{
   ms++;
}

#INT_TIMER2
void  TIMER2_isr(void) 
{
   time++;
}


int readADC(int pin){
   set_adc_channel(pin);
   return read_adc();
}

void motor(int1 state){
   if(state == 1){
      output_high(motorout);
   } else {
      output_low(motorout);
   }
}
void ok(int1 state){
   if(state == 1){
      output_high(okout);
   } else {
      output_low(okout);
   }
}
void fault(int1 state){
   if(state == 1){
      output_high(faultout);
   } else {
      output_low(faultout);
   }
}


void stop(void) {
   blink = 0;
   ok(0);
   fault(1);
   motor(0);
}

void getPhq(void) {

    while (!input(phase1))
    ms = 0;
    t1_start = ms;
    while (!input(phase2))
    t1_end = ms;
     dif1 = t1_end - t1_start;
     pf = dif1 / timeCal;
     pf = pf * 50.0 * 360.0;
     angle1 = pf;
     ms = 0;
     if(angle1 > 115 && angle1 < 125) {
         phq = 1;
      } else {
         phq = 0;
      }
}


void getsutanum(void){
   sutax = readADC(sutachannel) / 10;
}

void runSetup(void) {
   setup_oscillator(OSC_8MHZ);
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4|RTCC_8_BIT);      //512 us overflow
   setup_timer_2(T2_DIV_BY_64,195,16);      //6.2 ms overflow, 100 ms interrupt


   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(GLOBAL);
   getPhq();
   sutadone = 0;
}



void okblink(void){
   if(blink == 1){
      if((timerBlink + blinkDelay) <= time){
         okstate = !okstate;
         ok(okstate);
         timerBlink = time;
      }
   }
}

void updateState (void) {
   if(curState == 1){
      motor(1);
      fault(0);
      blink = 1;
   } else {
      motor(0);
      fault(0);
      blink = 1;
   }
}

void runSuta(void) {
   if((timerSuta + sutaDelay) <= time){
      curState = !curState;
      updateState();
      sutafinish++;
      timerSuta = time;
   }
}

void getPhase(void) {
   if((timerPhase + phaseDelay) <= time){
      getPhq();
   }
}

void runNormal(void){
   blink = 0;
   motor(1);
   ok(1);
   fault(0);
}

void doLoop(void){
      getPhase();
      getsutanum();
      okblink();
      if(sutafinish/2 < sutax){sutadone = 0;}else{sutadone = 1;}
      if(sutadone == 0 && phq == 1){
         runSuta();
      } else if (sutadone == 1 && phq == 1){
         runNormal();
      } else {
         stop();
      }
}

void main()
{
   runSetup();
   
   while(TRUE)
   {
    doLoop(); //TODO: User Code
   }

}
