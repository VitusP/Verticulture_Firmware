/*
 * Project Verticulture_Firmware_Android_V1
 * Description: Android-based firmware
 * Author:Vitus Putra and Rahul Narvekar
 * Date:7/7/2018
 */
PRODUCT_ID(7144);
PRODUCT_VERSION(2);
 //////////////////////////////////////////////////LIBRARY///////////////////////////////////////////////////////////////////////////////////
 #include <Wire.h>
 #include "Adafruit_DHT.h"
 #include "RTClibrary.h"

 //////////////////////////////////////////////////Variable/////////////////////////////////////////////////////////////////////////////////
  //Temperature and Humidity
  int temp_Pin = A0;   // what pin we're connected to
  #define DHTTYPE DHT11
  DHT dht(temp_Pin, DHTTYPE);
  double temp_C = 0.0;
  double temp_F = 0.0;
  double humidity = 0.0;
  //RTClibrary
   RTC_DS3231 rtc;
  //Pump
  int pump_Pin = A1;
  //Light LED
  int led_Pin = A2;

//////////////////////////////////////////////////MAIN/////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //Particle Variable
  Particle.variable("Temp_C", temp_C);
  Particle.variable("Temp_F", temp_F);
  Particle.variable("Humidity", humidity);
  //particle Function
  Particle.function("Pump_State", Pump_Control);
  Particle.function("Light_State", Light_Control);
  //other initialization
  dht.begin();
}

void loop() {
  // The core of your code will likely live here.
  get_DHT_Value();

}

void get_DHT_Value(){
  temp_C = dht.getTempCelcius();
  temp_F = dht.getTempFarenheit();
  humidity = dht.getHumidity();
}

int Pump_Control(String state){
  if(state == "ON")
  {
    //turn on the pump
    return 1;
  }
  else if(state == "OFF")
  {
    //turn off the pump
    return 1;
  }
  return -1;
}

int Light_Control(String state){
  if(state == "ON")
  {
    //turn on the light
    return 1;
  }
  else if(state == "OFF")
  {
    //turn off the light
    return 1;
  }
  return -1;
}
