
// max unsigned long 4294967295 // 2^32 - 1  https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
void setup() {  
   Serial.begin(9600);
   delay(1500);  // give enough time to open the serial monitor after uploading
   Serial.println("Starting...");
}

int i = 20;

unsigned long k = 4294967280;

void loop() {
  
  char buf[16];

  sprintf(buf,"Millis: %lu",k);

  Serial.println(buf);

  k += 1;
  
  if (i == 0) {

    Serial.println("End");
  
    while(1);
  }

  i -= 1;
}

//will print:
//Starting...
//Millis: 4294967280
//Millis: 4294967281
//Millis: 4294967282
//Millis: 4294967283
//Millis: 4294967284
//Millis: 4294967285
//Millis: 4294967286
//Millis: 4294967287
//Millis: 4294967288
//Millis: 4294967289
//Millis: 4294967290
//Millis: 4294967291
//Millis: 4294967292
//Millis: 4294967293
//Millis: 4294967294
//Millis: 4294967295
//Millis: 0
//Millis: 1
//Millis: 2
//Millis: 3
//Millis: 4
//End
