#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <EEPROM.h>

#define DEBUG 1
#define UPBUTTON 2
#define DOWNBUTTON 3

//#define NORMAL 1
//#define WATERING 2
//#define ERROR_ 3
//#define NEUTRAL 4
//#define THRESHOLD 20
//#define INTERVAL 30
//#define ALLERR 40

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

class Metronombyte {
  protected:
    byte f = 1; // first loop - don't increment
    byte i; // init
    byte r; // rhythm
    byte s; // state
  public:
    Metronombyte(byte rhythm, byte init = 0): r(rhythm), i(init) {}    
    void loop() {      
      if (f) {
        
        f = 0;
      }
      else {
        
        i += 1;

        if (i == r) {

            i = 0;
        }
      }
      
      s = (i % r);
//      Serial.println((String)"r: " + r + " i: " + i + " s: " + s + " c: " + (s == 0 ? "true" : "false")); 
    }
    bool on() {
      return s == 0;
    }
};

class Drops {
  protected:
    byte current = 0;
  public: 
    void next() {
        u8g2.setFont(u8g2_font_open_iconic_thing_2x_t);    
        u8g2.setDrawColor(1); // def 
        u8g2.drawGlyph(65,0 + current, 72); // drop 
        u8g2.drawGlyph(65,22 + current, 72); // drop
        u8g2.drawGlyph(65,42 + current, 72); // drop 
        current += 1;   
        if (current > 21) {
           current = 0;
        }
    }
};

class Seazons {
  protected:
    int battery[16] = { // https://github.com/olikraus/u8g2/wiki/fntgrpunifont
      33, 34, 35, 36, 37, 38, 39, 40
      , 51, 52, 53, 54, 55, 56, 57, 58 
    };
    byte current = len();
    byte len() {
      return sizeof(battery)/sizeof(battery[0]);
    }
  public: 
    void next() {       
      current += 1;
      if (current >= len()) {
        current = 0;
      }    
    }
    void render() {
      u8g2.setFont(u8g2_font_unifont_t_weather);      
      u8g2.drawGlyph(64, 24, battery[current]);  
    }
};

class Eprom {
  protected:
    byte _address;
    int _state;
    int _tmpstate;
    byte _changed = 0;
    int _lastchnagedms;
    int _debounce;
    byte _debug;
  public:
    Eprom(byte address, int debounce, byte debug = 1): 
      _address(address), 
      _debounce(debounce),
      _debug(debug)
    {
      _state = EEPROM.read(_address);
      _tmpstate = _state;
    }
  byte get() {
    return _state;
  }
  void set(int state) {
    _state = state;
    _lastchnagedms = millis();
  }
  // first call loop as early as possible and then use changedAfterDebounce to detect REAL EEPROM write event
  // WARNING; remember that changedAfterDebounce() will not detect it in the same main loop, it will detect it with delay
  void loop() {

    _changed = 0; // reset in this loop, and now let's try to detect if it indeed has changed

    int t = millis();
    
    if (_state != _tmpstate && t - _lastchnagedms > _debounce) {

      _changed = 1;

      _tmpstate = _state;

      if ( _debug ) {
        
        Serial.println((String)"Fake mode write to EEPROM: " + _state); 
      }
      else {

        Serial.println((String)"WARNING: Real write to EEPROM: " + _state); 
        
        EEPROM.write(_address, _state); 
      }
    }
  }
  byte changedAfterDebounce() {
    return _changed;
  }
};

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

class ButtonToggle {
  private:
    Button button;
    bool state;
  public:
    ButtonToggle(byte pin, bool init = false):
      button(pin),
      state(init)
    {}
    // call this loop() function as early as possible in main loop()
    // and then use on() funtion to determine state on/off
    void loop() {
      button.loop();      
      if (button.pressed()) {                           
        state = !state;
      }
    }
    bool on() {
      return state;
    }
};

class ButtonShortLongPressed {
  protected:
    byte pin;
    int thresholdms;
    
    int lastlow;
    byte pressed = 0;
    byte state = 0; // 0 - not pressed, 1 - short press, 2 - long press
  public:
    ButtonShortLongPressed(byte pin, int thresholdms = 200): pin(pin), thresholdms(thresholdms) {}
    // call this function as early as possible in main loop() function
    // then later in loop() logic use pressed() method to detect if button pressed
    void loop(int t) { 
        
        int s = digitalRead(pin); 
        
        if (s == HIGH) { 

            if (pressed != 1) { 

                state   = 0;
              
                pressed = 1;
                
                lastlow = t; 
            }
            
            return;
        } 

        if (s == LOW) {
            
            state = 0;  

            if (pressed == 1) {

                int m = millis();

//                Serial.println((String)"LOW & pressed: " + thresholdms + " diff:" + (m - lastlow));
//                Serial.println((String)"m: " + m);
//                Serial.println((String)"t: " + thresholdms);
      
                if (m - lastlow > thresholdms) {
    
//                    Serial.println("long");
                  
                    state = 2;
                } 
                else {
    
//                    Serial.println("short");
                  
                    state = 1;            
                }  
                   
                pressed = 0;            
            }
        } 
    }
    byte s() {
      return state == 1;
    }
    byte l() {
      return state == 2;
    }
};

//char* d(int k) {
//    char buf[2];
//    if (k > 99) {
//      k = 99;
//    }
//    sprintf (buf, "%d",  k);
//    return buf;
//}

class Display {
  protected:  
    Drops drops;
    Seazons seazons;
    Metronombyte m;
    byte state = 1; // 1 - seazons, 2 - drops, 3 - wrench
    void mainClear(byte x, byte y, byte w, byte h, bool white = false) {  
        if (white) {
          u8g2.setDrawColor(1);
          u8g2.drawBox(x, y, w, h); 
        }
        else {      
          u8g2.setDrawColor(0);
          u8g2.drawBox(x, y, w, h);
          u8g2.setDrawColor(1);
        }        
    }  
    void printStr(
      byte x, byte y, byte w, byte h, 
      byte a, byte b,
      uint8_t *font, 
      char* d, 
      bool white = false
    ) {      
      mainClear(x, y, w, h, white);
      if (white) {
        u8g2.setDrawColor(0); 
      } 
      u8g2.setFont(font);
      u8g2.drawUTF8(a, b, d);    
      u8g2.setDrawColor(1); // def
    } 
    void printChar(
      byte x, byte y, byte w, byte h, 
      byte a, byte b,
      uint8_t *font, 
      int d, 
      bool white = false
    ) { 
      mainClear(x, y, w, h, white);
      if (white) {
        u8g2.setDrawColor(0); 
      } 
      u8g2.setFont(font);     
      u8g2.drawGlyph(a, b, 72); // https://github.com/olikraus/u8g2/wiki/fntgrpiconic#open_iconic_check_1x     
      u8g2.setDrawColor(1); // def
    }
  public:
    Display():
      drops(),
      seazons(),
      m(20)
    {
      
    }
    void loop() {
      m.loop();
    }
    void render(
      int t,  
      byte state, 
      byte input,
      Eprom _threshold,
      Eprom _interval,
      Eprom _watering,
      Eprom _allowederr    
    ) {
      switch(state) {
        case 1:   
          if (m.on()) {
            seazons.next(); 
          } 
          seazons.render(); 
          break;
        case 2:        
          drops.next();  
          break;
        default:
          wrench(); 
          break;
      }  
      
      char buf[2];
//    int input[5] = { // https://github.com/olikraus/u8g2/wiki/fntgrpunifont
//      0, // INPUTNEUTRAL    
//      1, // INPUTTHRESHOLD  
//      2, // INPUTINTERVAL   
//      3, // INPUTWATERING   
//      4, // INPUTALLERR     
//    };
      
      sprintf (buf, "%d",  _threshold.get());
      clearThreshold();
      threshold(buf, input == 1); 
      
      sprintf (buf, "%d",  _interval.get());
      clearCheckInterval();
      checkInterval(buf, input == 2); 
      
      sprintf (buf, "%d",  _watering.get());
      clearWateringTime();
      wateringTime(buf, input == 3);  
      
      sprintf (buf, "%d",  _allowederr.get());
      clearErrorCounter();
      errorCounter(buf, input == 4);    
      
      sprintf (buf, "%d",  "99");
      clearTimeLeft();
      timeLeft(buf);   
      
    }    
    void setState(byte s) {
      state = s;
    }
    void clearWrench(bool white = false) {
      mainClear(58,0,30,32, white);
    }
    void clearThreshold(bool white = false) {
      mainClear(0,0,36,33, white);
    }
    void clearSensor(bool white = false) {
      mainClear(90,0,32,32, white);
    }
    void clearCheckInterval(bool white = false) {
      mainClear(38,0,20,7, white);
    }
    void clearWateringTime(bool white = false) {
      mainClear(38,16,20,7, white);
    }
    void clearErrorCounter(bool white = false) {
      mainClear(38,25,20,7, white);
    }
    void clearTimeLeft(bool white = false) {
      mainClear(38,8,20,7, white);
    }
    void threshold(char* d, bool white = false) {  
      
//    // threshold big
//    u8g2.drawBox(0,0,36,33); u8g2.setDrawColor(0);     
//      u8g2.setFont(u8g2_font_logisoso28_tn);
//      u8g2.drawStr(0,30,"50");    
//    u8g2.setDrawColor(1); // def 
    
      printStr(  
        0,0,36,33,
        0,30,
        u8g2_font_logisoso28_tn,
        d,
        white
      ); 
    }
    void wateringTime(char* d, bool white = false) {  
      
//    // watering time 3
//    u8g2.drawBox(38,16,20,7); u8g2.setDrawColor(0); 
//      u8g2.setFont(u8g2_font_4x6_tf); 
//      u8g2.drawStr(40,22,"30s");    
//    u8g2.setDrawColor(1); // def
    
      printStr(  
        38,16,20,7,
        40,22,
        u8g2_font_4x6_tf,
        d,
        white
      ); 
    }
    void errorCounter(char* d, bool white = false) {  
      
//    // error counter 4
//    u8g2.drawBox(38,25,20,7); u8g2.setDrawColor(0);    
//      u8g2.setFont(u8g2_font_4x6_tf);
//      const char* c = "0:3";      
//      u8g2.drawStr(40,31,c);    
//    u8g2.setDrawColor(1); // def
    
      printStr(  
        38,25,20,7,
        40,31,
        u8g2_font_4x6_tf,
        d,
        white
      ); 
    }
    void timeLeft(char* d) {          
      u8g2.setDrawColor(1); // def 
      u8g2.setFont(u8g2_font_4x6_tf); 
      u8g2.drawStr(40,14,d);    
      u8g2.setDrawColor(1); // def 
    }
    void sensor(char* d, bool white = false) {  

//    // sensor value
////    u8g2.drawBox(90,0,32,32); u8g2.setDrawColor(0); // will not be used
//      u8g2.setFont(u8g2_font_logisoso28_tn);
//      const char* b = "90";
//      u8g2.drawStr(88,30,b);
////    u8g2.setDrawColor(1); // def
       
      printStr(  
        90,0,32,32,
        88,30,
        u8g2_font_logisoso28_tn,
        d,
        white
      ); 
    }
    void checkInterval(char* d, bool white = false) {  

//    // check interval 1
//    u8g2.drawBox(38,0,20,7); u8g2.setDrawColor(0); 
//      u8g2.setFont(u8g2_font_4x6_tf); 
//      const char* i = "100s";    
//      u8g2.drawStr(40,6,i);    
//    u8g2.setDrawColor(1); // def
       
      printStr(  
        38,0,20,7,
        40,6,
        u8g2_font_4x6_tf,
        d,
        white
      ); 
    }
    void wrench(bool white = false) {  
     
      // wrench ok
//      u8g2.drawBox(62,7,20,20); u8g2.setDrawColor(0);  
//        u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);      
//        u8g2.drawGlyph(64, 25, 72); // https://github.com/olikraus/u8g2/wiki/fntgrpiconic#open_iconic_check_1x  
//      u8g2.setDrawColor(1); // def      
       
      printChar(  
        62,7,20,20,
        64, 25,
        u8g2_font_open_iconic_embedded_2x_t,
        72,
        white
      ); 
    }
} display;

class Main {
  protected:  
//    int input[5] = { // https://github.com/olikraus/u8g2/wiki/fntgrpunifont
//      0, // INPUTNEUTRAL    
//      1, // INPUTTHRESHOLD  
//      2, // INPUTINTERVAL   
//      3, // INPUTWATERING   
//      4, // INPUTALLERR     
//    };
    byte input = 0; 
    ButtonShortLongPressed up;
    ButtonShortLongPressed down;
  public: 
    Eprom threshold;
    Eprom interval;
    Eprom watering;
    Eprom allowederr;
    enum State {
      STATEWORKING  = 1, // NICE ANIMATION
      STATEWATERING = 2, // DROPS ANIMATION
      STATEERROR    = 3, // WRENCH ICON - ALL ACTION STOPPED
    } state; 
    Main(): 
      threshold(0, 2000, DEBUG),
      interval(1, 2000, DEBUG),
      watering(2, 2000, DEBUG),
      allowederr(3, 2000, DEBUG),
      up(UPBUTTON),
      down(DOWNBUTTON)
    {
      state = STATEWORKING;
    }
    void setState(State s) { 
      // 1 - seazons, 2 - drops, 3 - wrench 
      // STATEWORKING STATEWATERING STATEERROR
      state = s;
      display.setState(s);
    }
    void switchInput() {
      
      Serial.println((String)"input: " + input); 

//      input += 1;
      
//      Serial.println((String)"input +: " + input); 
    }
    void render(int t) {
      display.render(
        t,      
        state,  
        input, 
        threshold,
        interval,
        watering,
        allowederr
      );
    }
    void loop(int t) {
      threshold.loop();
      interval.loop();
      watering.loop();
      allowederr.loop();
      display.loop();  
      down.loop(t);
      up.loop(t);

      bool l = false;

      if (up.l()) {
        l = true;
        input += 1;
        if (input > 4) {
          input = 0;
        }              
//        Serial.println((String)"up l: " + input); 
      }

      if (down.l()) {
        l = true;
        if (input == 0) {
          input = 4;
        }
        else {
          input -= 1;
        }              
//        Serial.println((String)"down l: " + input); 
      }

      if (l) {   
        Serial.println((String)"l has changed: " + input); 
      }
      if (up.s()) {  
            
            byte i;

            switch (input) { 
              case 0: // INPUTNEUTRAL
                break;    
              case 1: // INPUTTHRESHOLD
                i = threshold.get();
                if (i < 255) {
                  i += 1;
                }
                threshold.set(i);
                break;    
              case 2: // INPUTINTERVAL
                i = interval.get();
                if (i > 0) {
                  i += 1;
                }
                interval.set(i);
                break;    
              case 3: // INPUTWATERING
                i = watering.get();
                if (i > 0) {
                  i += 1;
                }
                watering.set(i);
                break;    
              default: // INPUTALLERR
                i = allowederr.get();
                if (i > 0) {
                  i += 1;
                }
                allowederr.set(i);
                break; 
            }  
        }
  
        if (down.s()) {
            
            byte i;

            switch (input) { 
              case 0: // INPUTNEUTRAL
                break;    
              case 1: // INPUTTHRESHOLD
                i = threshold.get();
                if (i > 0) {
                  i -= 1;
                }
                threshold.set(i);
                break;    
              case 2: // INPUTINTERVAL
                i = interval.get();
                if (i > 0) {
                  i -= 1;
                }
                interval.set(i);
                break;    
              case 3: // INPUTWATERING
                i = watering.get();
                if (i > 0) {
                  i -= 1;
                }
                watering.set(i);
                break;    
              default: // INPUTALLERR
                i = allowederr.get();
                if (i > 0) {
                  i -= 1;
                }
                allowederr.set(i);
                break; 
            }  
        }          
    }
} m;

//Button button(UPBUTTON);
//ButtonToggle buttontoggle(UPBUTTON);
//ButtonShortLongPressed buttonshortlongpressed(UPBUTTON);

void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
}

//byte a = 0;
//byte b = 0;

int t;
void loop(void) {

  t = millis();

//  buttontoggle.loop();
//  button.loop();
//  buttonshortlongpressed.loop(t);
     
//  if (buttonshortlongpressed.s()) { 
//    
//    a = !a;
//    
//    
//    if (a) { 
//      m.setState(Main::STATEWORKING);
////      display.wrench();
//    }   
//    else {
//      m.setState(Main::STATEWATERING);
////      display.wrench(true);
//    }    
//  }  
//  if (buttonshortlongpressed.l()) { 
//
//    m.setState(Main::STATEERROR);
//    b = !b;
//  }   

  m.loop(t);

  u8g2.firstPage();    
  do {    
//    display.wrench();
//    display.checkInterval("120s");
//    display.sensor("56");
//    display.errorCounter("2:3");
//    display.timeLeft("56s");
//    display.wateringTime("13s");
//    display.threshold("67s");

    m.render(t);
//    
//    if (a) { 
//      m.setState(Main::STATEWORKING);
////      display.wrench();
//    }   
//    else {
//      m.setState(Main::STATEWATERING);
////      display.wrench(true);
//    }    
//    if (b) { 
//      display.wateringTime("80");
//    }   
//    else {
//      display.wateringTime("90", true);
//    }    
    
  } while ( u8g2.nextPage() );
}

  // https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#introduction
//  u8g2.firstPage();
//  do {
//    u8g2.setFont(u8g2_font_ncenB14_tr);
////    u8g2.setFont(u8x8_font_chroma48medium8_r);
//    u8g2.drawStr(0,20,"Arduino nano");
//  } while ( u8g2.nextPage() );
//  delay(1000);

  // drawing
  // https://github.com/olikraus/u8g2/wiki/u8g2reference#drawbox
//  u8g2.firstPage();
//  do {
//    u8g2.drawBox(3,7,25,15);
//  } while ( u8g2.nextPage() );
//  delay(1000);
