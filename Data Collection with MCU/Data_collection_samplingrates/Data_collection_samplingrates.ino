//INCLUDES
#include "ICM_20948.h"
#include "Protocentral_MAX30205.h"
#include "protocentral_Max30003.h"
#include <SparkFun_Bio_Sensor_Hub_Library.h>
#include <Wire.h>
#include <SPI.h>
#include <Time.h>

//DEFINES
#define AD0_VAL 1           // The value of the last bit of the I2C address. (CHANGE TO 1)

// ADRESSES
int IMU_Address = 102;   //0x68
int TEMP_Address = 64;  //0x48
int PPG_Address = 85;   //0x55
int resPin =  4;  
int mfioPin = 5;  
unsigned long t;
int width = 118; 
int samples = 1600; 
int pulseWidthVal;
int sampleVal;
int k = 0;
float temperature;

//OBJECTS
ICM_20948_I2C myICM;
MAX30205 tempSensor;
MAX30003 max30003;
SparkFun_Bio_Sensor_Hub bioHub(resPin, mfioPin);
bioData body;






//SETUP
void setup() {
  Serial.begin(115200);
  
  Wire.begin();

  //PPG
  int result = bioHub.begin();
  if (result == 0) // Zero errors!
    Serial.println("Sensor started!");

  Serial.println("Configuring Sensor...."); 
  int error = bioHub.configSensorBpm(MODE_ONE); // Configure Sensor and BPM mode , MODE_TWO also available
  if (error == 0){// Zero errors.
    Serial.println("Sensor configured.");
  }
  else {
    Serial.println("Error configuring sensor.");
    Serial.print("Error: "); 
    Serial.println(error); 
  }

  // Set pulse width.
  error = bioHub.setPulseWidth(width);
  if (error == 0){// Zero errors.
    Serial.println("Pulse Width Set.");
  }
  else {
    Serial.println("Could not set Pulse Width.");
    Serial.print("Error: "); 
    Serial.println(error); 
  }

  // Check that the pulse width was set. 
  pulseWidthVal = bioHub.readPulseWidth();
//  Serial.print("Pulse Width: ");
//  Serial.println(pulseWidthVal);

  // Set sample rate per second. Remember that not every sample rate is
  // available with every pulse width. Check hookup guide for more information.  
  error = bioHub.setSampleRate(samples);
  if (error == 0){// Zero errors.
//    Serial.println("Sample Rate Set.");
  }
  else {
//    Serial.println("Could not set Sample Rate!");
//    Serial.print("Error: "); 
//    Serial.println(error); 
  }

  // Check sample rate.
  sampleVal = bioHub.readSampleRate();
//  Serial.print("Sample rate is set to: ");
//  Serial.println(sampleVal); 
  
  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up. 
//  Serial.println("Loading up the buffer with data....");
  delay(4000);



  //IMU
  Serial.println("test");
  myICM.begin( Wire, AD0_VAL );       // Activate IMU sensor





//  //TEMP
    while (!tempSensor.scanAvailableSensors()) {
      //Serial.println("Couldn't find the temperature sensor, please connect the sensor." );
      delay(30000);
    }
  tempSensor.begin();                 





//  //ECG
  pinMode(MAX30003_CS_PIN, OUTPUT);
  digitalWrite(MAX30003_CS_PIN, HIGH); //disable device
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  bool ret = max30003.max30003ReadInfo();
  if (ret) {
//    Serial.println("Max30003 read ID Success");
  } 
  else {
  while (!ret) {
    //stay here untill the issue is fixed.
    ret = max30003.max30003ReadInfo();
    delay(10000);
  }
  
}
//Serial.println("Initialising the chip ...");
max30003.max30003Begin();   // initialize MAX30003






myICM.getAGMT();                // The values are only updated when you call 'getAGMT'
body = bioHub.readSensorBpm();
max30003.getEcgSamples();   //It reads the ecg sample and stores it to max30003.ecgdata .
temperature = tempSensor.getTemperature(); // read temperature for every 30ms


}







//LOOP
void loop() {
  if(k % 4 == 0){
        int time1 = millis();

      body = bioHub.readSensor();
          int time2 = millis();
    int timedif = time2-time1;
//    Serial.println(timedif);
  }
  if(k % 6 == 0){
      myICM.getAGMT();
  }
  if(k % 120 == 0){
      temperature = tempSensor.getTemperature(); 
  }
  if(k % 1 == 0){                                 
      max30003.getEcgSamples();   
      }
  
  t = millis();

  Serial.print(t);          //Time in milliseconds
  Serial.print(",");
  printScaledAGMT(&myICM);  //3 acceleration axes of IMU (x, y, z)
  Serial.print(",");

  Serial.print(body.irLed);  //IR from PPG (raw signal)
  Serial.print(",");       
  Serial.print(body.redLed); //RED from PPG (raw signal)
  Serial.print(",");
  Serial.print(temperature); //Temperature
  Serial.print(",");
  Serial.println(max30003.ecgdata);   //ECG data
  k++; 
//


  }





// ECG - 0/1 - MAX
// PPG - 5/8 - 50
// IMU - 1/2 - 60
// TEM - 0/1 - 1000
// CO2 - ??? - 1000





//FUNCTIONS
void printFormattedFloat(float val, uint8_t leading, uint8_t decimals) {
  float aval = abs(val);
  if (val < 0) {
    Serial.print("-");
  }
  else {
    Serial.print("");
  }
  for ( uint8_t indi = 0; indi < leading; indi++ ) {
    uint32_t tenpow = 0;
    if ( indi < (leading - 1) ) {
      tenpow = 1;
    }
    for (uint8_t c = 0; c < (leading - 1 - indi); c++) {
      tenpow *= 10;
    }
    if ( aval < tenpow) {
      Serial.print("0");
    }
    else {
      break;
    }
  }
  if (val < 0) {
    Serial.print(-val, decimals);
  }
  else {
    Serial.print(val, decimals);
  }
}
void printScaledAGMT( ICM_20948_I2C *sensor ) {
  printFormattedFloat( sensor->accX(), 5, 2 );
  Serial.print(",");
  printFormattedFloat( sensor->accY(), 5, 2 );
  Serial.print(",");
  printFormattedFloat( sensor->accZ(), 5, 2 );
//  Serial.print(", ");
//  printFormattedFloat(sensor->gyrX(), 5, 2);
//  Serial.print(", ");
//  printFormattedFloat(sensor->gyrY(), 5, 2);
//  Serial.print(", ");
//  printFormattedFloat(sensor->gyrZ(), 5, 2);
}
