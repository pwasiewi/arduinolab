void setup(){

};

void loop()
{

  delay(1000);
  Serial.begin(57600);
  Serial.println("\r\rBaud-rate = 57600");
  delay(1000);
  Serial.begin(76800);
  Serial.println("\r\rBaud-rate = 76800");
  delay(1000);
  Serial.begin(115200);
  Serial.println("\r\rBaud-rate = 115200");
  delay(1000);
  Serial.begin(230400);
  Serial.println("\r\rBaud-rate = 230400");
  delay(1000);
  Serial.begin(250000);
  Serial.println("\r\rBaud-rate = 250000");
  delay(1000);
  Serial.begin(500000);
  Serial.println("\r\rBaud-rate = 500000");
  delay(1000);
  Serial.begin(1000000);
  Serial.println("\r\rBaud-rate = 1000000");
  delay(1000);
  Serial.begin(2000000);
  Serial.println("\r\rBaud-rate = 2000000");
};
