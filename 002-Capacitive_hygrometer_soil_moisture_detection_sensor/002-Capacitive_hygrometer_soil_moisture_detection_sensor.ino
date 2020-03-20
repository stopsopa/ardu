




// http://www.circuitstoday.com/arduino-soil-moisture-sensor

// Sketch uses 4204 bytes (13%) of program storage space. Maximum is 30720 bytes.
// Global variables use 282 bytes (13%) of dynamic memory, leaving 1766 bytes for local variables. Maximum is 2048 bytes.

/***************************************************
 This example reads Capacitive Soil Moisture Sensor.
 
 Created 2015-10-21
 By berinie Chen <bernie.chen@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/





int pin = A0;

// Millis() uses unsigned longs which is a 32 bit number.  Translates to around 4 billion millseconds (4294967296 if you're feeling pedantic) or 49 days and 17 hours........
// https://forum.arduino.cc/index.php?topic=43858.msg317785#msg317785
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

class Hygrometer {
  protected:

    byte pin;

    //int out = 1023; // after autocalibration
    int out; // = 1023;
    
    // it can reach value around 196 if suddenly placed in water
    //int in = 195; // after autocalibration
    int in; // = 195;
    
    // the purpose of autocalibration is to find most extreme values
    // in air and in the water
    // WARNING: zero this number ON PRODUCTION to stop allowing autocalibration
    int mcx; // = 60;
    
    // -----------
    
    int outx = out;
    
    int inx = in;
    
    byte mapped = 0;
    
    int raw = 0;
  public:
    Hygrometer(byte pin, int in, int out, int maxcalibratepoints = 0):
      pin(pin),
      in(in),
      out(out),
      mcx(maxcalibratepoints)
    {}
    void loop(byte calibratecycle = 0) {

      raw = analogRead(pin);

      if (calibratecycle) {
      
        if (raw < inx && mcx > 0) {
        
          Serial.println("inx -1");
          
          inx -= 1;
          
          mcx -= 1;
        }
        
        if (raw > outx && mcx > 0) {
        
          Serial.println("outx +1");
          
          outx += 1;
          
          mcx -= 1;
        }
      }
      
      mapped = map(raw, outx, inx, 0, 100);
    }
    byte get() {
      return mapped;
    }

    // normally on production you can comment those functions out
    int getInx() {
      return inx;
    }
    int getOutx() {
      return outx;
    }
    int getOut() {
      return out;
    }
    int getIn() {
      return in;
    }
    int getMcx() {
      return mcx;
    }
    int getMapped() {
      return mapped;
    }
    int getRaw() {
      return raw;
    }
};

int mcx = 0;

Hygrometer h(pin, 195, 1023, 0);
//Hygrometer h(pin, 200, 900, 20);

Metronombyte m(10);

void setup() {

   Serial.begin(9600);

   Serial.println("Reading From the Sensor ...");

   delay(500);
}

void loop() {
    
    m.loop();

    h.loop(m.on() ? 1 : 0);

    if (m.on()) {
    
        Serial.print((String)"raw:" + h.getRaw() + " calc:" + h.get() + "% inx:" + h.getIn() + "->" + h.getInx() + " outx:" + h.getOut() + "->" + h.getOutx() + " mc:" + mcx + "->" + h.getMcx());
        
        Serial.println("");
    }
   delay(100); // get rid of this delay, don't know at the moment how
 }
