/////////////////////////////////////////////////////////////////////////////
////  INITIALIZE   //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "ICM_20948.h"
#include "Protocentral_MAX30205.h"
#include "protocentral_Max30003.h"
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>
#include <SPI.h>
#include <Time.h>

#define AD0_VAL 1   // The value of the last bit of the I2C address

MAX30003 max30003;
ICM_20948_I2C myICM;

/////////////////////////////////////////////////////////////////////////////
////   SETUP   //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Wire.begin();

  /////////////////////// 
  ////// IMU SETUP ////// 
  /////////////////////// 
  Serial.println("test");
  myICM.begin( Wire, AD0_VAL );       // Activate IMU sensor
  
  /////////////////////// 
  ////// ECG SETUP ////// 
  /////////////////////// 
  pinMode(MAX30003_CS_PIN, OUTPUT);
  digitalWrite(MAX30003_CS_PIN, HIGH); //disable device
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  bool ret = max30003.max30003ReadInfo();
  if (ret) {
     Serial.println("Max30003 read ID Success");
  } 
  else {
  while (!ret) {
    ret = max30003.max30003ReadInfo();
    delay(10000);
  }
  }
  max30003.max30003Begin();   // Initialize MAX30003

  /////////////////////// 
  ////// DATA CALL ////// 
  /////////////////////// 
  myICM.getAGMT();            // The values are only updated when you call 'getAGMT'
  max30003.getEcgSamples();   //It reads the ecg sample and stores it to max30003.ecgdata

}

int HR_ref;
int HR;
int RR_ref;
int RR;
unsigned long RR_start;
unsigned long RR_curr;
int IMU;

void loop() {
  myICM.getAGMT();
  IMU = myICM->accX
  //Serial.println(IMU);
  int mySensVals[128];
  for
   
  //Need a queue to store all the data
  //Just do FFT
 
}


/*
void loop() {

   // GET HR REFERENCE
   max30003.getHRandRR();   //It will store HR to max30003.heartRate and rr to max30003.RRinterval.
   int HR_ref = max30003.heartRate;
   int HR = max30003.heartRate;

   // DETECT HR DROP
   while ((HR_ref - HR)) < 5 {
       HR = max30003.heartRate; 
   }

   // GET RR REFERENCE
   myICM.getAGMT();
   int RR = ?? 
   RR_start = millis()/1000
   RR_now = millis()/1000
   
   // DETECT RR DROP
   while true {
       if RR < 10 {
           analogWrite(A0, 255); // TRIGGER PUMP
           delay(1000000)
       }

       if (RR_now - RR_start) > 120
           break
   }
   

}
*/


/////////////////////////////////////////////////////////////////////////////
////   PSEUDOCODE   /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Read HR from ECG
// If HR drops down by 5, read RR from IMU
// If RR drops down to 10 within 2 minutes, TRIGGER
// Else return to top
