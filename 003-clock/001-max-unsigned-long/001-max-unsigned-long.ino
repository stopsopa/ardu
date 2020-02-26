
unsigned long t() { 
  // max unsigned long 4294967295 // 2^32 - 1  https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
  return millis() + 4294967280;
}

// max unsigned long 4294967295 // 2^32 - 1  https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
void setup() {  
  Serial.begin(9600);
  
  char buf[16];
  
  sprintf(buf, "Init Millis: %lu", millis()); // will print 0
  
  delay(1500);  // give enough time to open the serial monitor after uploading  
  
  Serial.println(buf);
  
  Serial.println("Starting...");
}

int i = 15;

unsigned long k = t() + 6;

void loop() {
  
  char buf[16];

  sprintf(buf,"%lu",k);

  Serial.print("loop: ");

  Serial.println(buf);

  k += 1;
  
  if (i == 0) {

    Serial.println("End");
  
    while(1);
  }

  i -= 1;
}

//will print:
//Init Millis: 0
//Starting...
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
//Millis: 5
//End
