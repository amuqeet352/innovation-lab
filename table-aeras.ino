//     Aeras Version 1.0.1
//     Air Quality Monitoring with onboard display

//========== Libraries ==========
#include "SdsDustSensor.h"
#include "DHT.h"
#include <TFT_HX8357.h>
#include "Free_Fonts.h"

//====== Global Variables =======
#define DHTPIN 12
#define DHTTYPE DHT22 //Change DHT11 if required 

int pm25, pm10, T, H, AQI;
float PM25, PM10;
float Temp,Hum,temperature,humidity;

float AQIhi=0;
float AQIlo=0;
float Conchi=0;
float Conclo=0;

//========== Initializers ========
DHT dht(DHTPIN, DHTTYPE);
TFT_HX8357 tft = TFT_HX8357();
SdsDustSensor sds(Serial3);


//================================
//             SETUP
//================================
void setup() {

  dht.begin();
  sds.begin();
  Serial.println(sds.queryFirmwareVersion().toString());
  Serial.println(sds.setQueryReportingMode().toString());
  
  tft.begin();
  tft.setRotation(1);

//=========== LCD Background Objects ===========
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(5,4,150,250,20,TFT_YELLOW);
  tft.fillRoundRect(165,4,150,120,20,TFT_GREEN);
  tft.fillRoundRect(325,4,150,120,20,TFT_GREEN);
  tft.fillRoundRect(165,134,150,120,20,TFT_GREEN);
  tft.fillRoundRect(325,134,150,120,20,TFT_GREEN);

  tft.fillRect(0,310,80,9,TFT_GREEN);
  tft.fillRect(80,310,80,9,TFT_YELLOW);
  tft.fillRect(160,310,80,9,TFT_ORANGE);
  tft.fillRect(240,310,80,9,TFT_RED);
  tft.fillRect(320,310,80,9,TFT_PURPLE);
  tft.fillRect(400,310,80,9,TFT_MAROON);
  

  tft.setTextColor(TFT_BLACK);
  tft.setCursor(45, 39);
  tft.setTextSize(1);
  tft.setFreeFont(FF35);
  tft.print("AQI");

  tft.setTextColor(TFT_BLACK);
  tft.setCursor(207, 27);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("PM2.5");

  tft.setCursor(208, 157);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("PM10");

  tft.setCursor(368, 27);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("TEMP");
  
  tft.setCursor(370, 157);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("HUM");

  tft.setCursor(209, 117);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("ug/m3");

  tft.setCursor(209, 244);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("ug/m3");

  tft.setCursor(385, 114);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("'C");

  tft.setCursor(387, 244);
  tft.setTextSize(1);
  tft.setFreeFont(FF34);
  tft.print("%");

}

//=======================================
//             LOOP
//=======================================

void loop() {

  sds.wakeup();
  delay(3000);
  
  PmResult pm = sds.queryPm();
  if (pm.isOk()){
      PM25 = 0;
      PM10 = 0;
      Temp = 0;
      Hum = 0;
      
      for(int i=0;i<=5;i++){ // 6 minute iterations to get average
        Temp = Temp + dht.readTemperature();
        Hum = Hum + dht.readHumidity();
        PM25 = PM25 + pm.pm25;
        PM10 = PM10 + pm.pm10;
        delay(10000);
      }
      float temperature = Temp/6;
      float humidity = Hum/6;
      float PM25avg = PM25/6;
      float PM10avg = PM10/6;
      pm25 = PM25avg;
      pm10 = PM10avg;
      T = temperature;
      H = humidity;

 //========== AQI Calculation ===========
       if(PM25avg <= 12.0){
          Conchi = 12.0;
          Conclo = 0.0;
          AQIhi = 50;
          AQIlo = 0;
        }
        if(PM25avg >= 12.1 && PM25avg <= 35.4){
          Conchi = 35.4;
          Conclo = 12.1;
          AQIhi = 100;
          AQIlo = 51;
        }
        if(PM25avg >= 35.5 && PM25avg <= 55.4){
          Conchi = 55.4;
          Conclo = 35.5;
          AQIhi = 150;
          AQIlo = 101;
        }
        if(PM25avg >= 55.5 && PM25avg <= 150.4){
          Conchi = 150.4;
          Conclo = 55.5;
          AQIhi = 200;
          AQIlo = 151;
        }
        if(PM25avg >= 150.5 && PM25avg <= 250.4){
          Conchi = 250.4;
          Conclo = 150.5;
          AQIhi = 300;
          AQIlo = 201;
        }
        if(PM25avg >= 250.5 && PM25avg <= 500.4){
          Conchi = 500.4;
          Conclo = 250.4;
          AQIhi = 500;
          AQIlo = 301;
        }
        float aqi=((AQIhi-AQIlo)/(Conchi-Conclo))*(PM25avg-Conclo)+(AQIlo+2);
        delay(200);
        AQI = aqi;
     
  }else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }


//==================================================
//            Print values on Screen 
//==================================================
  
  tft.fillRect(165,34,150,60,TFT_GREEN);
  tft.fillRect(325,34,150,60,TFT_GREEN);
  tft.fillRect(165,164,150,60,TFT_GREEN);
  tft.fillRect(325,164,150,60,TFT_GREEN);
  tft.fillRect(0,259,640,50,TFT_BLACK);
//-----PM2.5------   
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(195, 42);
  tft.setTextSize(1);
  tft.setTextFont(7);
  tft.print(pm25);
//-----PM10------
  tft.setCursor(195, 167);
  tft.setTextSize(1);
  tft.setTextFont(7);
  tft.print(pm10);
//-----Temp------
  tft.setCursor(367, 42);
  tft.setTextSize(1);
  tft.setTextFont(7);
  tft.print(T);
//-----Humi-----
  tft.setCursor(367, 167);
  tft.setTextSize(1);
  tft.setTextFont(7);
  tft.print(H);


//---- AQI box Color changing depending on range ----
  if(AQI > 0 && AQI <=50){

    tft.fillRoundRect(5,4,150,250,20,TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(45, 39);
    tft.setTextSize(1);
    tft.setFreeFont(FF35);
    tft.print("AQI");

    tft.fillRect(5,94,150,70,TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(35, 104);
    tft.setTextSize(1);
    tft.setTextFont(7);
    tft.print(AQI);
  
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(179, 294);
    tft.setTextSize(1);
    tft.setFreeFont(FF43);
    tft.print("GOOD");
    
  }else if(AQI > 50 && AQI <= 100){

    tft.fillRoundRect(5,4,150,250,20,TFT_YELLOW);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(45, 39);
    tft.setTextSize(1);
    tft.setFreeFont(FF35);
    tft.print("AQI");

    tft.fillRect(5,94,150,70,TFT_YELLOW);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(35, 104);
    tft.setTextSize(1);
    tft.setTextFont(7);
    tft.print(AQI);
  
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(145, 294);
    tft.setTextSize(1);
    tft.setFreeFont(FF43);
    tft.print("MODERATE");
    
  }else if(AQI > 101 && AQI <= 150){

    tft.fillRoundRect(5,4,150,250,20,TFT_ORANGE);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(45, 39);
    tft.setTextSize(1);
    tft.setFreeFont(FF35);
    tft.print("AQI");

    tft.fillRect(5,94,150,70,TFT_ORANGE);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(35, 104);
    tft.setTextSize(1);
    tft.setTextFont(7);
    tft.print(AQI);
  
    tft.setTextColor(TFT_ORANGE);
    tft.setCursor(175, 294);
    tft.setTextSize(1);
    tft.setFreeFont(FF43);
    tft.print("POOR");
    
  }else if(AQI > 150 && AQI <= 200){

    tft.fillRoundRect(5,4,150,250,20,TFT_RED);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(45, 39);
    tft.setTextSize(1);
    tft.setFreeFont(FF35);
    tft.print("AQI");

    tft.fillRect(5,94,150,70,TFT_RED);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(35, 104);
    tft.setTextSize(1);
    tft.setTextFont(7);
    tft.print(AQI);
  
    tft.setTextColor(TFT_RED);
    tft.setCursor(145, 294);
    tft.setTextSize(1);
    tft.setFreeFont(FF43);
    tft.print("UNHEALTHY");
    
  }else if(AQI > 200 && AQI <= 300){

    tft.fillRoundRect(5,4,150,250,20,TFT_PURPLE);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(45, 39);
    tft.setTextSize(1);
    tft.setFreeFont(FF35);
    tft.print("AQI");

    tft.fillRect(5,94,150,70,TFT_PURPLE);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(35, 104);
    tft.setTextSize(1);
    tft.setTextFont(7);
    tft.print(AQI);
  
    tft.setTextColor(TFT_PURPLE);
    tft.setCursor(175, 294);
    tft.setTextSize(1);
    tft.setFreeFont(FF43);
    tft.print("SEVERE");
    
  }else if(AQI > 300 && AQI <= 500){

    tft.fillRoundRect(5,4,150,250,20,TFT_MAROON);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(45, 39);
    tft.setTextSize(1);
    tft.setFreeFont(FF35);
    tft.print("AQI");

    tft.fillRect(5,94,150,70,TFT_MAROON);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(35, 104);
    tft.setTextSize(1);
    tft.setTextFont(7);
    tft.print(AQI);
  
    tft.setTextColor(TFT_MAROON);
    tft.setCursor(145, 294);
    tft.setTextSize(1);
    tft.setFreeFont(FF43);
    tft.print("HAZARDOUS");
    
  }
  // put SDS Sensor on Sleep
   WorkingStateResult state = sds.sleep();
  if (state.isWorking()) {
    Serial.println("Problem with sleeping the sensor.");
  } else {
    Serial.println("Sensor is sleeping");
    delay(6000); // wait 1 minute
  }
  

}
