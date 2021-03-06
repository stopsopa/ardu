

// 2020-03-20 [friday] - v0.0.1 - first implementation

#define UPBUTTON 2

// class starts here ...
class ButtonShortLongPressed {
  protected:
  
    byte pressed = LOW;
    byte waitoncefordelay = HIGH;
    byte state = 0; // 0 - not pressed, 1 - short press, 2 - long press
    unsigned long lastrepeat = 0;
    
    byte pin;    
    unsigned long lasthigh;
    int thresholdms;        // to distinguish short or long press
    byte repeatshortorlong; // 1 - short press, 2 - long press
    int repeatevery;        // interval to repeat pressing button
    int delaybeforerepeat; 
    
  public:
    unsigned long d; // for production this property can be moved to other private properties
    ButtonShortLongPressed(
        byte pin, 
        int thresholdms = 300, 
        int repeatevery = 300,
        int delaybeforerepeat = 2000,
        byte repeatshortorlong = 1
    ): 
      pin(pin), 
      thresholdms(thresholdms),
      repeatevery(repeatevery),   
      delaybeforerepeat(delaybeforerepeat), // how long to wait before it will repetitively start pressing again
      repeatshortorlong(repeatshortorlong)    
    {}
    // call this function as early as possible in main loop() function
    // then later in loop() logic use pressed() method to detect if button pressed
    void loop() { 
        
        int s = digitalRead(pin); 

        unsigned long m = millis();
        
        if (s == HIGH) {

            state       = 0;

            // from second iteration
            if (pressed == HIGH && repeatevery > 0) {
                  
                if (lastrepeat > 0) {

                    unsigned long diff = m - lastrepeat;

                    if (waitoncefordelay == HIGH) {
    
                        if (diff > delaybeforerepeat) {   
    
                            lastrepeat  = m;  

                            waitoncefordelay = LOW;
    
                            state = repeatshortorlong; 
                            
                            Serial.println("ButtonShortLongPressed: waitoncefordelay == LOW");
                        }                        
                    }
                    else {   
    
                        if (diff > repeatevery) {
    
                            state = repeatshortorlong;       
    
                            lastrepeat  = m;  

                            d = m;
                        }            
                    }              
                } 
                else {          

                    lastrepeat  = m;   
                }  
            }

            if (pressed != HIGH) {

                lasthigh    = m;
                
                lastrepeat  = m;
            }
              
            pressed     = HIGH;
            
            return;
        } 

        if (s == LOW) {
            
            state = 0;  

            if (pressed == HIGH && waitoncefordelay != LOW) {

                unsigned long diff = m - lasthigh;
      
                if (diff > thresholdms) {
    
                    Serial.println("ButtonShortLongPressed: long press");
                  
                    state = 2;
                } 
                else {
    
                    Serial.println("ButtonShortLongPressed: short press");
                  
                    state = 1;
                }   
                
                lastrepeat  = m; // update here again to delay repetition
            }
                   
            pressed = LOW; 

            waitoncefordelay = HIGH;
        } 
    }
    byte s() {
      return state == 1;
    }
    byte l() {
      return state == 2;
    }
};
// and class ends here ...
// from that point it's up to you if you export it to separate file or just copy and paste to sketch
// https://www.youtube.com/watch?v=gmDkBQDZIrs

void setup() {
  Serial.begin(9600);
}

ButtonShortLongPressed b(
  DOWNBUTTON, 
  300,    // int thresholdms
  100,    // int repeatevery
  2000,   // delaybeforerepeat
  2
);

long p  = 1000;
long k  = 1000;

void loop(void) {

  b.loop();

  if (b.l()) { // if long then number goes up
    k += 1; 
  }

  if (b.s()) { // if long then number goes down
    k -= 1;
  }

  if (p != k) {
    p = k;
    Serial.println((String)"g:" + b.d + " v:" + k);
  }
}
