// Include application, user and local libraries


// Define variables and constants

uint8_t myLED;


//
// Brief  Setup
// Details  Define the pin the LED is connected to
//
// Add setup code 
void setup() {
  // myLED pin number
  myLED = 13;
  pinMode(myLED, OUTPUT);     
}

//
void loop() {
  digitalWrite(myLED, HIGH);
  delay(500);    
  digitalWrite(myLED, LOW);
  delay(500);    
}
