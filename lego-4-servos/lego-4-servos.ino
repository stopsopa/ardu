 // servo
/*
http://educ8s.tv/arduino-servo-tutorial/
https://www.youtube.com/watch?v=SfmHNb5QAzc
	https://i.imgur.com/UL2XTdh.jpg
*/
#include <Servo.h>

class Button {
  protected:
    byte _pin;
    byte _prevstate = LOW;
    byte _state = LOW;
  public:
    Button(byte pin): _pin(pin) {}
    // call this function as early as possible in main loop() function
    // then later in loop() logic use pressed() method to detect if button pressed
    void loop() { 
        _state = LOW;
        int state = digitalRead(_pin);      
        if (state != _prevstate) {         
          _prevstate = state;
          if (_prevstate == LOW) {
            _state = HIGH;
          }
        }
      
    }
    byte pressed() {
      return _state;
    }
};

Servo servo9;
Servo servo10;
Servo servo11;
Servo servo12;

int min = 0;
int max = 180;

void setup() {
    Serial.begin(9600);
    
    servo9.attach(9);
    servo9.write(min);
    servo10.attach(102);
    servo10.write(min);
    servo11.attach(11);
    servo11.write(min);
    servo12.attach(12);
    servo12.write(min);
      
      Serial.println((String)"start");
}

bool b1 = false;
bool b2 = false;
bool b3 = false;
bool b4 = false;

Button button(8);

void loop() 
{        
//    button.loop();
//
//    if (button.pressed()) {
//
//      if (b1) {
//        
//          Serial.println((String)"8:l");    
//
//          servo4.write(max);
//      }
//      else {
//        
//          Serial.println((String)"8:r");    
//
//          servo4.write(min);       
//      }  
//    
//    delay(1000);  
//
//      b1 = !b1;
//      
//    }
//    if (k5 && lock5 !=) {
//
//        
//    }
          
//    servo1.write(0);   
//    
//    delay(1000);  
//    
//    servo1.write(180);   
//    
//    delay(1000);   
//
//
//               
//    servo2.write(0);   
//    
//    delay(1000);  
//    
//    servo2.write(180);   
//    
//    delay(1000);   
//
//
//               
//    servo3.write(0);   
//    
//    delay(1000);  
//    
//    servo3.write(180);   
//    
//    delay(1000);   
//
//
//               
    servo11.write(0);   
    
    delay(1000);  
    
    servo11.write(180);   
    
    delay(1000);  
}
