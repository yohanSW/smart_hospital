#include <DHT11.h>    //���̺귯�� �ҷ���
int pin=2;            //Signal �� ����� �Ƶ��̳��� �ɹ�ȣ
DHT11 dht11(pin);        
void setup()
{
   Serial.begin(9600); //��żӵ� ����
}
 
void loop()
{
  int err;
  float temp, humi;
  if((err=dht11.read(humi, temp))==0) //�µ�, ���� �о�ͼ� ǥ��
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();
  }
  else                                //������ ��� ó��
  {
    Serial.println();
    Serial.print("Error No :");
    Serial.print(err);
    Serial.println();    
  }
  delay(1000);                        //1�ʸ��� ����
}
