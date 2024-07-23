#include <Wire.h>
#include <PeakDetection.h> 
#include "ICM_20948.h"  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU


#define WIRE_PORT Wire  // Your desired Wire port.      Used when "USE_SPI" is not defined
#define AD0_VAL   1     // The value of the last bit of the I2C address.
                        // On the SparkFun 9DoF IMU breakout the default is 1, and when
                        // the ADR jumper is closed the value becomes 0

ICM_20948_I2C myICM;  // Otherwise create an ICM_20948_I2C object


PeakDetection peakDetection; 

void setup(){
  Serial.begin(115200);
  Wire.begin();
  pinMode(5, OUTPUT);
  bool initialized = false;
  while (!initialized)
  {

    myICM.begin(Wire, AD0_VAL);


    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      Serial.println(F("Trying again..."));
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }

  Serial.println(F("Device connected!"));

  peakDetection.begin(70,1.5, 0.9);
  delay(1000);
}

float var1 = 100;
float var2 = 100;
float var3 = 100;
float var4 = 100;
float var5 = 100;
float var6 = 100;
float var7 = 100;
float var8 = 100;
float var9 = 100;
float var10 = 100;

void loop(){
  digitalWrite(5, LOW);
  
  int yn;
  int temp[256] = {};
  for (int i = 0; i<256; i++){
    myICM.getAGMT();
    yn = myICM.accX();
    temp[i] = yn;  
    //Serial.println(temp[i]);  
  }
  //Serial.println(temp[230]);      

  /*
  Serial.println("1");
  double vImag[256] = {};
  const uint16_t samples = 256;
  Serial.println("2");
  FFT.Windowing(temp, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD); 
  Serial.println("3");
  FFT.Compute(temp, vImag, samples, FFT_FORWARD);
  //FFT.ComplexToMagnitude(temp, vImag, samples);
  const double samplingFrequency = 100;
  double x = FFT.MajorPeak(temp, samples, samplingFrequency);
  Serial.println(x, 6);
  */

  
  var1 = variance(temp, 256);
  Serial.print(var1);  
  Serial.print(",");   
  Serial.print(var2);  
  Serial.print(","); 
  Serial.print(var3);  
  Serial.print(","); 
  Serial.print(var4);  
  Serial.print(","); 
  Serial.print(var5);  
  Serial.print(","); 
  Serial.print(var6);  
  Serial.print(","); 
  Serial.print(var7);  
  Serial.print(","); 
  Serial.print(var8);  
  Serial.print(","); 
  Serial.print(var9);  
  Serial.print(","); 
  Serial.print(var10);   
  Serial.println(""); 

  
  if ((abs(var1) < 60) &&
      (abs(var2) < 60) &&
      (abs(var3) < 60) &&
      (abs(var4) < 60) &&
      (abs(var5) < 60) &&
      (abs(var6) < 60) &&
      (abs(var7) < 60) &&
      (abs(var8) < 60) &&
      (abs(var9) < 60) &&
      (abs(var10) < 60)) {
    Serial.println("Apnea is detected");
    Serial.println("Waiting 10 seconds");
    delay(10*1000);
    digitalWrite(5, HIGH);
    //Serial.println("Pumping 10 seconds");
    //delay(10*1000);
    //digitalWrite(5, LOW);
    //delay(100000000000000);
  }
  
  
  var10 = var9;
  var9 = var8;
  var8 = var7;
  var7 = var6;
  var6 = var5;
  var5 = var4;
  var4 = var3;
  var3 = var2;
  var2 = var1;  
}


float variance(int a[], int n) 
{   
    // Compute mean (average of elements) 
    int sum = 0; 
    
    for (int i = 0; i < n; i++) sum += a[i];    
    int mean = (int)sum / (int)n; 
    // Compute sum squared differences with mean. 
    int sqDiff = 0; 
    for (int i = 0; i < n; i++) 
        sqDiff += (a[i] - mean) * (a[i] - mean); 
    return (float)sqDiff / n; 
} 


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
}

float storeScaledAGMT( ICM_20948_I2C *sensor ) {
  return sensor->accY();
}
