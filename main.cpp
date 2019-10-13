//#define BLYNK_DEBUG
#define BLYNK_PRINT stdout
#ifdef RASPBERRY
  #include <BlynkApiWiringPi.h>
#else
  #include <BlynkApiLinux.h>
#endif
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>
 
static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);
 
static const char *auth, *serv;
static uint16_t port;
 
#include <BlynkWidgets.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h> //For printf functions
#include <stdlib.h> // For system functions

int buttonPin = 17; //GPIO17 Pin on the Pi
bool buttonState = false;  //Used to store the previous state of the button
int LEDPin = 23;// physical LED
//terminals and lables/=value dsiplay
BlynkTimer timer;
void sensorDataSend(void);
int sensorValue;
void alarm (void);
long lastInterruptTime = 0; //Used for button debounce
//void resetfunc (void);
//void  startRTC (void);


void setup()
{
    //SETUP BLYNK
    Blynk.begin(auth, serv, port);
    //other setup
    pinMode(buttonPin, INPUT); //Set GPIO17 as input
    pullUpDnControl (buttonPin, PUD_UP); //Set GPIO17 internal pull up
    pinMode(LEDPin, OUTPUT);//set external LED

    timer.setInterval(1000L, sensorDataSend); //timer will run every sec
    //buttons
    wiringPiISR (buttonPin, INT_EDGE_FALLING,alarm); // setting up interupt for button to call method hourInc.

}
 
void loop()
{
    Blynk.run();
    timer.run();
}
void sensorDataSend(){

	sensorValue = 15;         // reading sensor from analog pin
  	Blynk.virtualWrite(V2, sensorValue);  // sending sensor value to Blynk app
}

void button(){
if(buttonState != digitalRead(buttonPin)) //check the button state against its last known value, i$
    {
       if(digitalRead(buttonPin) == TRUE) //if true, set the Virtual Pin "V0" to a value of 0 (full of$
        {
            Blynk.virtualWrite(V0, 0);
            digitalWrite (LEDPin, 0);

        }
        else{
            Blynk.virtualWrite(V0, 255);  //Else we set the virtual pin "V0" to a value of 255 (full o$
            digitalWrite (LEDPin, 1);

        }
    }
    else {}    //if last value = current value, we do nothing.

buttonState = digitalRead(buttonPin);  //update the button state.
}//end of button
 
int main(int argc, char* argv[])
{   
    parse_options(argc, argv, auth, serv, port);
    setup();
    digitalWrite(LEDPin,1);
    Blynk.virtualWrite(V0, 1);
    //terminal.print("hello bitch");
    while(true) {
        loop();
	//button();
    }
 
    return 0;
}

void alarm (void)
{
        printf("alarm triggered");
	long interruptTime = millis();
        if (interruptTime - lastInterruptTime>250){
         // start sensing
                printf("dismiss alarm");
                
                digitalWrite(LEDPin,0);
		Blynk.virtualWrite(V0, 0);

		//set lights low
         }//end of if
         lastInterruptTime =interruptTime;
         
}//end of alarm function
