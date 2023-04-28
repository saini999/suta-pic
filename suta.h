#include <16LF1574.h>
#device ADC=10
#FUSES INTRC_IO
#FUSES NOWDT
#FUSES NOPUT
#FUSES NOPROTECT
#FUSES PLL_SW

#FUSES MCLR                     //Master Clear pin enabled
#FUSES BROWNOUT                 //Reset when brownout detected
#FUSES NOWRT                    //Program memory not write protected
#FUSES PPS1WAY                  //Allows only one reconfiguration of peripheral pins
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES BORV25                   //Brownout reset at 2.5V
#FUSES LVP                      //Low Voltage Programming on B3(PIC16) or B5(PIC18)

#use delay(internal=8MHz)
//#use FIXED_IO( C_outputs=PIN_C2,PIN_C1,PIN_C0 )
#define sutanum   PIN_A0
#define sutachannel 0
#define phase1   PIN_A1
#define phase2   PIN_A2
#define bypass   PIN_A3
#define faultout   PIN_C0
#define okout   PIN_C1
#define motorout   PIN_C2
#define timeCal 1000 // time recorded in ms / timeCal
#define sutaDelay 5000 //ms
#define blinkDelay 500 //ms
#define phaseDelay 10 //ms
#define anglemin 60 //digrees
#define anglemax 150 //digrees
