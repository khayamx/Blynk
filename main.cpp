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
 
int buttonPin = 17; //GPIO17 Pin on the Pi
bool buttonState = false;  //Used to store the previous state of the button
int LEDPin = 23;// physical LED
//terminals and lables/=value dsiplay
WidgetTerminal terminal(V1);
//Widget
BlynkTimer timer;
void sensorDataSend(void);
int sensorValue;

/*BLYNK_READ(V1)
{
	terminal.println("Hello bitch");
	terminal.flush();

}*/
void setup()
{
    //SETUP BLYNK
    Blynk.begin(auth, serv, port);
    //other setup
    pinMode(buttonPin, INPUT); //Set GPIO17 as input
    pullUpDnControl (buttonPin, PUD_UP); //Set GPIO17 internal pull up
    pinMode(LEDPin, OUTPUT);//set external LED

    //terminal stuff
	/*
    terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
    terminal.println(F("-------------"));
    terminal.println(F("Type 'Marco' and get a reply, or type"));
    terminal.println(F("anything else and get it printed back."));
    terminal.flush();*/
    timer.setInterval(1000L, sensorDataSend); //timer will run every sec
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



}
 
int main(int argc, char* argv[])
{   
    parse_options(argc, argv, auth, serv, port);
    setup();
    digitalWrite(LEDPin,1);
    Blynk.virtualWrite(V0, 1);
    //terminal.print("hello bitch");
    while(true) {
        loop();
	button();
    }
 
    return 0;
}

