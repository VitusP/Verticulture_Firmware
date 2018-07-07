/*
 * Project TheVerticulture
 * Description: Smart IoT Vertical Garden built for everyone, everywhere.
 * Author: Vitus Putra
 * Date: 7/6/2018
 */
PRODUCT_ID(7144);
PRODUCT_VERSION(1);
 // This #include statement was automatically added by the Particle IDE.
 #include "RTClib.h"
 #include "Adafruit_DHT.h"
 #include <Wire.h>
 #include "blynk.h"

 //****************************************************BLYNK***********************************************************************************************
 #define BLYNK_PRINT Serial  // Set serial output for debug prints
 #include <blynk.h>
 char auth[] = "285a62612b4c45e3a1115e1546085365";
 BlynkTimer timer;
 //****************************************************TEMPERATUREandHUMIDITYandLIGHT**********************************************************************
 int TempPin = A0;   
 int pinLightSensor = A1;
 #define DHTTYPE DHT11		// DHT 11
 DHT dht(TempPin, DHTTYPE);
 double temperature = 0.0;
 double humidity = 0.0;
 double lightSensor = 0.0;
 const long intSensorIntervals = 10000; //10 seconds
 unsigned long intMillisSensor = 0;
 //****************************************************RTC**********************************************************************
 RTC_DS3231 rtc;
 //****************************************************Relays**********************************************************************
 int pinPump = 3;
 int intPumpEnabled = 0;
 bool boolPumpOn;
 bool boolLightOn;
 unsigned long intMillisPump = 0;
 const long intPumpIntervals = 20000; //3300000 milliseconds
 int pinLight = 2;
 int intLightEnabled = 0;



 void setup() {
   //Particle.variable() Declaration
   Particle.variable("Temperature", temperature);
   Particle.variable("Humidity", humidity);
   //Particle.variable("Light", lightSensor);
   //Blynk Initialization
   Blynk.begin(auth);
   timer.setInterval(1000L, sentSensorToBlynk);
   //Initialization
   pinMode(pinPump, OUTPUT);
   pinMode(pinLight, OUTPUT);
   pinMode(7, OUTPUT);

   dht.begin();

   if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        //while (1);
   }

   //pump initialization
   intMillisPump = millis();
   boolPumpOn = true;
   pumpOnOff(boolPumpOn);
   //Sensor retreival
   intMillisSensor = millis();
   getSensorValues();

 }

 void loop() {
//Blynk run
   Blynk.run();
   timer.run();
//light control

   DateTime now = rtc.now();
    if (now.hour() >= 17 || now.hour() <= 6) {
      lightOnOff(false);
    }else{
      lightOnOff(true);
    }

//pump control
    //timer-based

    unsigned long currentPumpMillis = millis();
    if ((currentPumpMillis - intMillisPump >= intPumpIntervals) && (intPumpEnabled == 1)){
      pumpOnOff(boolPumpOn);
      intMillisPump = millis();
    }

    //demo
    if (intPumpEnabled == 1){
      pumpOnOff(true);
    }else{
      pumpOnOff(false);
    }
    //light control
    if (intLightEnabled == 0){
      lightOnOff(true);
    }else{
      lightOnOff(false);
    }

    //Read sensor
    unsigned long currentSensorMillis = millis();
    if((currentSensorMillis - intMillisSensor >= intSensorIntervals)){
      getSensorValues();
    }
 }

 void pumpOnOff(boolean pumpOn){
   if(pumpOn){
     digitalWrite(pinPump, HIGH); //Turn on pump
   }else{
     digitalWrite(pinPump, LOW);//turn off pump
   }
   boolPumpOn = !boolPumpOn;
 }

 void lightOnOff(boolean on){
   if(on){
     digitalWrite(pinLight, HIGH);//Turn on light
   }else{
     digitalWrite(pinLight, LOW);//turn off light
   }
   boolLightOn = !boolLightOn;

 }

 void getSensorValues(){
   temperature = dht.getTempCelcius();
   humidity = dht.getHumidity();
 }

 void sentSensorToBlynk(){
   Blynk.virtualWrite(0, (int)temperature);
   Blynk.virtualWrite(1, (int)humidity);
 }

BLYNK_WRITE(V2) //pump enabled
{
   intPumpEnabled = param.asInt();
}

BLYNK_WRITE(V3) //light enabled
{
   intLightEnabled = param.asInt();
}
