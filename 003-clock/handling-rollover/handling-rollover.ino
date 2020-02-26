

#define UPBUTTON 2

#define DOWNBUTTON 3

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

// test class
class TT {
  protected:
    unsigned long i = 0;
  public:
    unsigned long t() {

      if (i == 0) {

        i = (unsigned long)4294967295 - millis() - 15 - 2000;        
  
        char buf[16];
      
        sprintf(buf,"%lu",i);
      
        Serial.print("offset: ");
      
        Serial.println(buf);
      }

      return millis() + i;
    }
} t;

unsigned long test() { 
  // max unsigned long 4294967295 // 2^32 - 1  https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
  return millis() + 4294967280;
}

class Metronomms {
  protected:
    unsigned long last = 0;    
    int intervalms;
    int correct = 0;
    char state = -1; // it's unsigned byte - it might be interpreated as character in some cases, so be careful
    unsigned long ms() {
      return t.t();
    }
  public:
    Metronomms(int intervalms): 
      intervalms(intervalms)
    {}
    void loop() {

      if (state == -1) {

        reset();
      }
      
      unsigned long current = ms();

      if (state == -1 || current <= last) {

        last = current;
        
        state = 0;

        return;
      }

      unsigned long diff = current - last;

      if (diff >= (unsigned long)intervalms) {
  
        char buf[16];
        
        sprintf(buf, "%lu", current);
        
        Serial.print((String) "   :::");
        
        Serial.print(buf);
        
//        Serial.print((String) " ==== ");

//        Serial.println((String)">: " + buf);

        last = (((unsigned long) (current) / intervalms) * intervalms) + correct;

        state = 1;

        return;
      }

      state = 0;
    }
    void reset() {

      last = ms(); 

      correct = last % intervalms;
    }
    char tick() {
      return state;
    }
};

Button b(DOWNBUTTON);

Metronomms m(1000);

// max unsigned long 4294967295 // 2^32 - 1  https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
void setup() {  
  Serial.begin(9600);
  
  char buf[16];
  
  sprintf(buf, "Init Millis: %lu", millis()); // will print 0
  
  delay(1500);  // give enough time to open the serial monitor after uploading  
  
  Serial.println(buf);
  
  Serial.println("Starting...");
}

int loops = 25;

int s = 0;

void loop() {

  b.loop();

  if (b.pressed()) {
  
    Serial.println("pressed");

    m.reset();
  }

  m.loop();

  if (m.tick()) {

    s += 1;

    loops -= 1;

    Serial.println((String)": " + s);
  }

  
//  if (loops == 0) {
//
//    Serial.println("End");
//  
//    while(1);
//  }
}
