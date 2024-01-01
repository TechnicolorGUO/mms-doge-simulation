#include "pid.h"

int32_t Bias_L,Bias_R;
int32_t encoderL, encoderR;
int32_t distance, angle;
int32_t pwm_L1_bias,pwm_L0_bias,pwm_R1_bias,pwm_R0_bias;
int8_t pwmLe, pwmRi;

// PID settings and gains
double KP_DISTANCE = 0.011;
double KI_DISTANCE = 0.0;
double KD_DISTANCE = 0.008;
double KP_ANGLE = 0.0065;
double KI_ANGLE = 0.0;
double KD_ANGLE = 0.0065;
double AnglRANGE = 30.0;
double DistRANGE = 70.0;

// Setpoints
double setPointDistance = 0;
double setPointAngle = 0;

double aCorrection = 0;
double dCorrection = 0;
double distance_double = static_cast<double>(distance);
double angle_double = static_cast<double>(angle);

AutoPID pidDistance(&distance_double, &setPointDistance, &dCorrection, -DistRANGE, DistRANGE, KP_DISTANCE, KI_DISTANCE, KD_DISTANCE);
AutoPID pidAngle(&angle_double, &setPointAngle, &aCorrection, -AnglRANGE, AnglRANGE, KP_ANGLE, KI_ANGLE, KD_ANGLE);

bool pid_done= false;

void setMotorPWM(int8_t pwmL, int8_t pwmR){
  int8_t PWM[2] = {pwmL,pwmR};
//  Serial.print(PWM[0]);
//  Serial.print(',');
//  Serial.print(PWM[1]);
//  Serial.print(',');
WireWriteDataArray(31,PWM, 2);
}

void PIDwork(){
   int8_t rot_L , rot_R;
  rot_L = rot_R =0;
  resetEncoders();
  pid_done= false;
  while (!pid_done){
      // Read encoder values
      encoderL = getLeftEncoderCounts();
      encoderR = getRightEncoderCounts();
    
      // Calculate distance and angle
      distance = encoderL + (encoderR-encoderL)/2;
      angle = encoderR - encoderL;
      if (RTooClose()&&rot_L ==0)
      {     
      setMotorPWM(32,-32);
      delay(175);
      setMotorPWM(0,0);
      delay(2000);
      rot_L++;
      initializeEncoders();
        };
      if (LTooClose()&&rot_R==0
      ){
      setMotorPWM(-32,32);
      delay(1075);
      setMotorPWM(0,0);
      delay(2000);
      rot_R++;
      initializeEncoders();
        };
        
      distance_double = static_cast<double>(distance);
      angle_double = static_cast<double>(angle);
      if(abs(angle_double) >50000){resetEncoders();}

      Serial.print(encoderL);
      Serial.print(',');
      Serial.print(encoderR);
      Serial.print(',');
      Serial.print(distance_double);
      Serial.print(',');
      Serial.print(angle_double);
        // Run PID controllers
        pidDistance.run();
        pidAngle.run();
        pid_done = (abs(dCorrection)<0.8)&&(abs(aCorrection)<0.5);
        
      Serial.print(',');
      Serial.print(dCorrection);
      Serial.print(',');
      Serial.println(aCorrection);
        
      pwmLe = (dCorrection - aCorrection);
      //pwmLe = pwmLe>=0?((pwmLe+pwm_L1_bias)*100/(100+pwm_L1_bias)):((pwmLe+pwm_L0_bias)*100/(100-pwm_L0_bias));
      pwmLe = pwmLe>=0?(pwmLe*(100-pwm_L1_bias)/100+pwm_L1_bias):((pwmLe)*(100+pwm_L0_bias)/100+pwm_L0_bias);
      pwmRi = (dCorrection + aCorrection);
      //pwmRi = pwmRi>=0?((pwmRi+pwm_R1_bias)*100/(100+pwm_R1_bias)):((pwmRi+pwm_R0_bias)*100/(100-pwm_R0_bias));
      pwmRi = pwmRi>=0?(pwmRi*(100-pwm_R1_bias)/100+pwm_R1_bias):((pwmRi)*(100+pwm_R0_bias)/100+pwm_R0_bias);

      // Control the motors
      setMotorPWM(pwmLe, pwmRi);
    }
    setMotorPWM(0, 0);
    resetEncoders();
    Serial.println("resetPID");
  }

void PIDsetup(){
  // Initialize encoder values
  encoderL = 0;
  encoderR = 0;

  // Initialize distance and angle variables
  distance = 0;
  angle = 0;

  // Set initial setpoints
  setPointDistance = 0;
  setPointAngle = 0;
  
  pidDistance.setTimeStep(25);
  pidAngle.setTimeStep(25);

  int32_t encoder_start_temp = getLeftEncoderCounts();
  for(pwm_L1_bias =40;pwm_L1_bias<100 && abs(encoder_start_temp-getLeftEncoderCounts())<5;pwm_L1_bias++){
    setMotorPWM(pwm_L1_bias, 0);
    delay(100);
  }
  pwm_L1_bias-=3;
  setMotorPWM(0, 0);
  delay(400);

  encoder_start_temp = getLeftEncoderCounts();
  for(pwm_L0_bias = -40;pwm_L0_bias>-100 && abs(encoder_start_temp-getLeftEncoderCounts())<5;pwm_L0_bias--){
    setMotorPWM(pwm_L0_bias, 0);
    delay(100);
  }
  pwm_L0_bias+=3;
  setMotorPWM(0, 0);
  delay(400);

  encoder_start_temp = getRightEncoderCounts();
  for(pwm_R1_bias = 40;pwm_R1_bias<100 && abs(encoder_start_temp-getRightEncoderCounts())<5;pwm_R1_bias++){
    setMotorPWM(0, pwm_R1_bias);
    delay(100);
  }
  pwm_R1_bias-=3;
  setMotorPWM(0, 0);
  delay(400);

  encoder_start_temp = getRightEncoderCounts();
  for(pwm_R0_bias = -40;pwm_R0_bias>-100 && abs(encoder_start_temp-getRightEncoderCounts())<5;pwm_R0_bias--){
    setMotorPWM(0, pwm_R0_bias);
    delay(100);
  }
  pwm_R0_bias+=3;
  setMotorPWM(0, 0);
  resetEncoders();
}



void Move(int n){
    AnglRANGE = 20.0;
    DistRANGE = 80.0;
    setPointAngle = 0;
    setPointDistance= -1350 * n ;
    pidAngle.setOutputRange(-AnglRANGE,AnglRANGE);
    pidDistance.setOutputRange(-DistRANGE,DistRANGE);
    PIDwork();
    setMotorPWM(0, 0);
}

void Turn(int n){
  switch(n){
    case 0:
      break;
    case 1:
      setMotorPWM(-42,42);
      delay(475);
      setMotorPWM(0,0);
      delay(1000);
      initializeEncoders();
      break;
    case 2:
      setMotorPWM(-42,42);
      delay(475);
      setMotorPWM(-42,42);
      delay(475);
      setMotorPWM(0,0);
      initializeEncoders();
      delay(1000);      
      break;

    case -1:
      setMotorPWM(42,-42);
      delay(485);
      setMotorPWM(0,0);
      delay(1000);
      initializeEncoders();
      break;
  }
  setMotorPWM(0, 0);
//    AnglRANGE = 90.0;
//    DistRANGE = 10.0;
//    setPointAngle = 700*n;
//    setPointDistance= 0;
//    pidAngle.setOutputRange(-AnglRANGE,AnglRANGE);
//    pidDistance.setOutputRange(-DistRANGE,DistRANGE);
//    PIDwork();
//    setMotorPWM(0, 0);
//    initializeEncoders();
}
