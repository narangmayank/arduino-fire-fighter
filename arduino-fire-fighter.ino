/*

 Smart Fire Fighter Using Arduino Nano
 Author   :   Mayank
 Period   :   Nov. 2020 - Dec. 2020
 
 */

#include <Servo.h>        // Include Servo Library

int ps=0;                 //  Previous State of Switch
int count=0;              //  Counter
int button=2;             //  Switch to Control the Robo
bool fire=false;

#define left_sensor 3     //  Left flame sensor Connected to pin D3
#define middle_sensor 4   //  Middle flame sensor connected to pin D4
#define right_sensor 5    //  Right flame sensor connected to pin D5

#define red_led 7         //  Danger sign
#define green_led 8       //  Safe Sign
#define blue_led 9        //  Activeness of Robo

#define LM1 12            // Left motor
#define LM2 13           
#define RM1 0             // Right motor
#define RM2 1          

#define pump1 10          // Pump
#define pump2 11         

Servo myservo;            // Instance of Servo Class
int angle=0;              // Angle for servo motor
 

void ExtinguishFire()
{     
  delay(100);                // Hold to get steady
  digitalWrite(LM1, HIGH);   // Stop the Robo
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, HIGH);
   
  digitalWrite(pump1,1);     // Pump ON
  digitalWrite(pump2,0);
  myservo.write(90);         // Set Servo to 90 degree
  delay(100);
  
  for(angle=90;angle<=150;angle++)  // Sweep Servo from 90 to 150
  {
    myservo.write(angle);
    delay(15);
  }
  for(angle=150;angle>=30;angle--)  // Sweep Servo from 150 to 30
  {
    myservo.write(angle);
    delay(15);
  }
  for(angle=30;angle<=90;angle++)   // Sweep Servo from 30 to 90
  {
    myservo.write(angle);
    delay(15);
  }
  
  delay(100);
  digitalWrite(pump1,0);   // Pump OFF
  digitalWrite(pump2,0);
}

void setup() {
  
  pinMode(button,INPUT_PULLUP);         // Button --> Input with PullUp

  pinMode(blue_led,OUTPUT);             // LED's --> Output
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  
  pinMode(left_sensor,INPUT_PULLUP);   // Sensor --> Input
  pinMode(middle_sensor,INPUT_PULLUP);
  pinMode(right_sensor,INPUT_PULLUP);

  pinMode(LM1, OUTPUT);                // Motor Control pins --> Output
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
   
  pinMode(pump1,OUTPUT);              // Pump Control pins to Output
  pinMode(pump2,OUTPUT);
    
  myservo.attach(6);                  // Servo is attched to pin D6 (PWM)
}

void loop() {

  myservo.write(0);                // Set Servo to 0 degree
  delay(100);
  int  cs=digitalRead(button);     // Read the Switch  Current State
  if(ps and not cs)                // Detect falling edge 
  {
    count++;                       // Increment Counter
  }
  
  if(count==1)                     // Now Switch is pressed and our Robo is Active
  {  
    digitalWrite(blue_led,HIGH);   // Blue LED On
    
    int left_reading   =  digitalRead(left_sensor);     // Read the Sensors data
    int middle_reading =  digitalRead(middle_sensor);
    int right_reading  =  digitalRead(right_sensor);

    while (fire == true)                                // Extinguish the fire till it's stop               
    {
      ExtinguishFire();          
      if (digitalRead(left_sensor) == 1 && digitalRead(middle_sensor) == 1 && digitalRead(right_sensor) == 1)         
      {
        fire = false;                                   // Reset the Fire Status if Fire is stopped
      }
    }
    
    if (left_reading == 0)           // Fire is in the left
    {
      digitalWrite(red_led,HIGH);    // Red   LED On (Danger)
      digitalWrite(green_led,LOW);   // Green LED Off
      
      digitalWrite(LM1, HIGH);       // Move left
      digitalWrite(LM2, HIGH);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);
      delay(400);                  
      fire = true;                   // Set the Fire status
    }
    else if (middle_reading == 0)    // Fire is in the Straight 
    {
      digitalWrite(red_led,HIGH);    // Red   LED On (Danger)
      digitalWrite(green_led,LOW);   // Green LED Off
      
      digitalWrite(LM1, HIGH);       // Move forward
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);
      delay(400);
      fire = true;                   // Set the Fire status
    }
    else if (right_reading == 0)     // Fire is in the right 
    {
      digitalWrite(red_led,HIGH);    // Red   LED On (Danger)
      digitalWrite(green_led,LOW);   // Green LED Off
      
      digitalWrite(LM1, HIGH);       // Move right
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, HIGH);
      delay(400);
      fire = true;                   // Set the Fire status
    }
    else                             // No fire is detected
    { 
      digitalWrite(green_led,HIGH);  // Green LED On (Safe)
      digitalWrite(red_led,LOW);     // Red   LED Off
                                 
      digitalWrite(LM1, HIGH);       // Do not move
      digitalWrite(LM2, HIGH);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, HIGH);
      fire = false;                  // Reset the Fire status
    }
  }
  
  if(count==2)                       // De Activate the Robo
  {
    digitalWrite(blue_led,LOW);      // All LED's Off
    digitalWrite(green_led,LOW);
    digitalWrite(red_led,LOW);
    count=0;                         // Reset the Counter
   }
  ps= cs;
  delay(200);
}
