
// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial btSerial(3,4);//RX, TX
float pregyroY=0; 
float pregyroX=0;
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int const swpin1 =5;
int const swpin2 =6;
int const swpin3 =7;
int const swpin4 =8;
int const swpin5 =9;


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  btSerial.begin(9600);

  pinMode(swpin1, INPUT);
  pinMode(swpin2, INPUT);
  pinMode(swpin3, INPUT);
  pinMode(swpin4, INPUT);
  pinMode(swpin5, INPUT);
  
}
void loop(){
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  //Serial.print("AcX = "); Serial.print(AcX);
  //Serial.print(" | AcY = "); Serial.print(AcY);
//  Serial.print(" | AcZ = "); Serial.print(AcZ);
//  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
//  Serial.print(" | GyX = "); Serial.print(GyX);
//  Serial.print(" | GyY = "); Serial.print(GyY);
//  Serial.print(" | GyZ = "); Serial.println(GyZ);
  float gyroY, gyroX, pregyroY, pregyroX;

  int Sensitivity = 100;  
  gyroY = GyY / Sensitivity / 1.1 * 1;
  gyroX = GyX / Sensitivity * -1;

  float gain = 1;
  int filterx = gyroX * gain + (1-gain)*pregyroX;
  int filtery = gyroY * gain + (1-gain)*pregyroY;

  int sw1 = digitalRead(swpin1);
  int sw2= digitalRead(swpin2);
  int sw3= digitalRead(swpin3);
  int sw4 = digitalRead(swpin4);
  int sw5= digitalRead(swpin5);
  
  btSerial.print(filterx);
  btSerial.print(',');
  btSerial.print(filtery);
  btSerial.print(',');
  btSerial.print(sw1);
  btSerial.print(',');
  btSerial.print(sw2);
  btSerial.print(',');
  btSerial.print(sw3);
  btSerial.print(',');
  btSerial.print(sw4);
  btSerial.print(',');
  btSerial.println(sw5);

 
  
  

  pregyroY= gyroY;
  pregyroX= gyroX;
  
}
