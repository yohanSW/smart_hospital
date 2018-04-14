#include<DHT11.h>
int pin=2;
DHT11 dht11(pin);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0)
  {
    Serial.print("!");
    Serial.print(temp);
    Serial.print("@");
    Serial.print(humi);
    Serial.print("#");
    Serial.println();
  }
  
  else{
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();
  }
  delay(DHT11_RETRY_DELAY);
}