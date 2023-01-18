#include <Wire.h>

long loopTimer, loopTimer2;
int temperature;
double accelPitch;
double accelRoll;
int acc_x, acc_y, acc_z;
int dx, dy, dz;
int a1, a2, a3, second = 0;
double accel_x, accel_y, accel_z;
double gyroRoll, gyroPitch, gyroYaw;
int gyro_x, gyro_y, gyro_z;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
double rotation_x, rotation_y, rotation_z;
double freq, dt;
double tau = 0.98;
double roll = 0;
int velocity, linear_accel = 0;
double pitch = 0;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
unsigned long previousMillis;

const unsigned long period = 1000; 

// 250 deg/s --> 131.0, 500 deg/s --> 65.5, 1000 deg/s --> 32.8, 2000 deg/s --> 16.4
long scaleFactorGyro = 65.5;

// 2g --> 16384 , 4g --> 8192 , 8g --> 4096, 16g --> 2048
long scaleFactorAccel = 8192;


void setup() {
  // Start
  Wire.begin();
  Serial.begin(115200);
  startMillis = millis();  //initial start time

  // Setup the registers of the MPU-6050 and start up
  setup_mpu_6050_registers();

  // Calibration
 Serial.println("");

  // Take 3000 readings for each coordinate and then find average offset
  for (int cal_int = 0; cal_int < 3000; cal_int ++){
    if(cal_int % 200 == 0)
    read_mpu_6050_data();
    gyro_x_cal += gyro_x;
    gyro_y_cal += gyro_y;
    gyro_z_cal += gyro_z;

    delay(3);
  }

  /*Serial.print("\nNote 1: Yaw is not filtered and will drift!\n");
  Serial.print("\nNote 2: Make sure sampling frequency is ~250 Hz\n");
  Serial.print("Sampling Frequency (Hz)\t\t");
  Serial.print("Roll (deg)\t\t");
  Serial.print("Pitch (deg)\t\t");
  Serial.print("Yaw (deg)\t\t\n");
  delay(2000);*/

  // Reset the loop timer
  loopTimer = micros();
  loopTimer2 = micros();

}


void loop() {
  freq = 1/((micros() - loopTimer2) * 1e-6);
  loopTimer2 = micros();
  
  
   //get the current "time" (actually the number of milliseconds since the program started)

 if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
   
    startMillis = currentMillis; 
    second = second + 1;
  }
/*Serial.print(second);*/
  // Read the raw acc data from MPU-6050
  previousMillis = millis();
  read_mpu_6050_data();
  currentMillis = millis();
  dt= currentMillis - previousMillis;
  // Subtract the offset calibration value
  gyro_x -= gyro_x_cal;
  gyro_y -= gyro_y_cal;
  gyro_z -= gyro_z_cal;

  // Convert to instantaneous degrees per second
  rotation_x = (double)gyro_x / (double)scaleFactorGyro;
  rotation_y = (double)gyro_y / (double)scaleFactorGyro;
  rotation_z = (double)gyro_z / (double)scaleFactorGyro;

  // Convert to g force
  accel_x = (double)acc_x / (double)scaleFactorAccel;
  accel_y = (double)acc_y / (double)scaleFactorAccel;
  accel_z = (double)acc_z / (double)scaleFactorAccel;

//  // Complementary filter
//  accelPitch = atan2(accel_y, accel_z) * RAD_TO_DEG;
//  accelRoll = atan2(accel_x, accel_z) * RAD_TO_DEG;

//  pitch = (tau)*(pitch + rotation_x*dt) + (1-tau)*(accelPitch);
//  roll = (tau)*(roll - rotation_y*dt) + (1-tau)*(accelRoll);
//
//  gyroPitch += rotation_x*dt;
//  gyroRoll -= rotation_y*dt;
//  gyroYaw += rotation_z*dt;

  // Visualize just the roll
  // Serial.print(roll); Serial.print(",");
  // Serial.print(gyroRoll); Serial.print(",");
  // Serial.println(accelRoll);

  // Visualize just the pitch
  // Serial.print(pitch); Serial.print(",");
  // Serial.print(gyroPitch); Serial.print(",");
  // Serial.println(accelPitch);

  // Data out serial monitor
  /*Serial.print(freq,0);   Serial.print(",");
  Serial.print(roll,1);   Serial.print(",");
  Serial.print(pitch,1);  Serial.print(",");
  Serial.println(gyroYaw,1);*/

     Serial.print(accel_x-0.05);
     Serial.print(",");
     Serial.print(accel_y+0.03);
     Serial.print(",");
     Serial.print(accel_z+10.77);
     Serial.print(",");
     Serial.println(dt);
     
       
    
    
   /* a1+= accel_x;
    a2+=accel_y;
    a3+=(accel_z+8.78);
    Serial.println(" \n linear accelerations in X, Y, Z");

     Serial.print(a1);
    Serial.print(" ");
    Serial.print(a2);
    Serial.print(" ");
    Serial.print(a3);
    Serial.prin1t(" ");
    delay(500);
   */


 
  /*   Serial.println(" accelerations in X, Y, Z");*/
/*velocityx = (accel_x * second);
velocityy = (accel_y * second);
velocityz = (accel_z * second);
     Serial.print(velocityx);
     Serial.print(" ");
     Serial.print(velocityy);
     Serial.print(" ");
     Serial.print(velocityz);
     Serial.print(" ");
     delay(500);*/
//  Serial.print("displacement");
//  Serial.print(dx);

//  Serial.println(", displacement in Y: ");
//  Serial.print(dy);
//    
//
//  Serial.println(",displacement in Z: ");
//  Serial.print(dz);
//  Serial.println(" metre");
 /* dx = (dx+((((accel_x) * (accel_x))/32768)/2.0)*second);

         dy = (dy+((((accel_y)*(accel_y))/32768)/2.0)*second);

         dz = (dz+((((accel_z)*(accel_z))/32768)/2.0)*second);
                 Serial.println(" linear accelerations in X, Y, Z");

     Serial.print(dx);
    Serial.print(" ");
    Serial.print(dy);
    Serial.print(" ");
    Serial.print(dz);
    Serial.print(" ");
     Serial.print(" m/s^2");*/
  
  // Wait until the loopTimer reaches 4000us (250Hz) before next loop
  while (micros() - loopTimer <= 4000);
  loopTimer = micros();
}


void read_mpu_6050_data() {
  // Subroutine for reading the raw data
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 14);

  // Read data --> Temperature falls between acc and gyro registers
  acc_x = Wire.read() << 8 | Wire.read();
  acc_y = Wire.read() << 8 | Wire.read();
  acc_z = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() <<8 | Wire.read();
  gyro_x = Wire.read()<<8 | Wire.read();
  gyro_y = Wire.read()<<8 | Wire.read();
  gyro_z = Wire.read()<<8 | Wire.read();
}


void setup_mpu_6050_registers() {
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x18);
  Wire.endTransmission();

  // Configure the accelerometer
  // Wire.write(0x__);
  // Wire.write; 2g --> 0x00, 4g --> 0x08, 8g --> 0x10, 16g --> 0x18
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x08);
  Wire.endTransmission();

  // Configure the gyro
  // Wire.write(0x__);
  // 250 deg/s --> 0x00, 500 deg/s --> 0x08, 1000 deg/s --> 0x10, 2000 deg/s --> 0x18
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x18);
  Wire.endTransmission();
  
}