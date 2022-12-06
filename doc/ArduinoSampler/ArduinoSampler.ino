int val = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(19, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  val = digitalRead(19);
  Serial.print(val);
  Serial.print(", ");
  digitalWrite(LED_BUILTIN, val);
  delay(10);             
}
