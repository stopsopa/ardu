int r = 4;

void setup() {
  Serial.begin(9600);

  pinMode(r, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(r, HIGH);
  Serial.println("HIGH");
  delay(2000);
  
  digitalWrite(r, LOW);
  Serial.println("LOW");
  delay(2000);
}
