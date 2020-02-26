


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
};
TT t;

unsigned long test() { 
  // max unsigned long 4294967295 // 2^32 - 1  https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
  return millis() + 4294967280;
}

class Metronomms {
  protected:
    unsigned long last = 0;    
    int intervalms;
    char state = -1; // it's unsigned byte - it might be interpreated as character in some cases, so be careful
  public:
    Metronomms(int intervalms): 
      intervalms(intervalms)
    {
      
    }
    void loop() {
      
      unsigned long current = t.t();

      if (state == -1 || current <= last) {

        last = current;
        
        state = 0;

        return;
      }

      unsigned long diff = current - last;

      if (diff >= (unsigned long)intervalms) {
  
        char buf[16];
        
        sprintf(buf, "%lu", current);
        
        Serial.print((String) ":::");
        
        Serial.print(buf);
        
//        Serial.print((String) " ==== ");

        
        
//        Serial.println((String)">: " + buf);

        last = ((unsigned long)current / intervalms) * intervalms;

        state = 1;

        return;
      }

      state = 0;
    }
    char tick() {
      return state;
    }
};

Metronomms m(500);

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

  m.loop();

  if (m.tick()) {

    s += 1;

    loops -= 1;

    Serial.println((String)": " + s);
  }

  
  if (loops == 0) {

    Serial.println("End");
  
    while(1);
  }
}

//will print:
//Init Millis: 0
//Starting...
//offset: 4294963780
//:::4294965780: 1
//:::4294966000: 2
//:::4294966500: 3
//:::4294967000: 4
//:::500: 5
//:::1000: 6
//:::1500: 7
//:::2000: 8
//:::2500: 9
//:::3000: 10
//:::3500: 11
//:::4000: 12
//:::4500: 13
//:::5000: 14
//:::5500: 15
//:::6000: 16
//:::6500: 17
//:::7000: 18
//:::7500: 19
//:::8000: 20
//:::8500: 21
//:::9000: 22
//:::9500: 23
//:::10000: 24
//:::10500: 25
//End
