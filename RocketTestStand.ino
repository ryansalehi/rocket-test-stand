// Program created by Ryan Salehi
// Created May 30, 2023

#include <SPI.h>
#include <SD.h>
#include "HX711.h"

//Creating const variables for wiring pins
const int chipSelect = 4; //sd card
const int redPin=5,greenPin=6,bluePin=9; //RGB LED
const int loadcellOut=3,loadcellSCK=2; //HX711 and loadcell
const double calibrationFactor=372.406639; //loadcell calibration factor


File mainFile;
HX711 scale;

void setup() {

  //Setup the RGB light
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);

  // Open serial communications and wait for port to open:
  setLED(255,0,0);//set LED to red to signify setup not complete
  delay(1000);
  Serial.begin(57600);
  scale.begin(loadcellOut,loadcellSCK);
  while (!Serial) {
    ; // wait for serial port to connect
  }

 //SD Card Setup ---
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Failed to initialize SD card.");
    return;
  }
  Serial.println("card initialized.");

    // open the file
  mainFile = SD.open("data.txt", FILE_WRITE);
  delay(10);

 //Check if the file opened 
  if(mainFile){
    Serial.println("Data file is opened.");
  }else{
    Serial.println("Error opening data file.");
    setLED(0,0,0);
    delay(2000);
    setLED(255,0,0);
    delay(500);//    Blink LED red to signify failed setup
    setLED(0,0,0);
    delay(2000);
    setLED(255,0,0);
    return;
  }

  scale.set_scale(calibrationFactor);//calibrate and tare the scale
  scale.tare();
  delay(300);

  setLED(0,255,0);//set LED to green to signify setup successful
  delay(2000);
}

void loop() {

  setLED(0,0,255);//set LED to blue to signify "collecting data" status

  unsigned long time = millis();
  double reading = scale.get_units();

  mainFile.print(time);
  mainFile.print(",");
  mainFile.println(reading);  // write data to mainFile
  Serial.print(time);
  Serial.print(",");
  Serial.println(reading);
  mainFile.flush();
}

//function used to set the color of the LED
void setLED(int red,int green,int blue){
  analogWrite(redPin,red);
  analogWrite(greenPin,green);
  analogWrite(bluePin,blue);
}







