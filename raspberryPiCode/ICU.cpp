#include "ICU.h"

namespace han
{

	void ICU::gpio_setting(){
		//setup GPIO in wiringPi mode
		if (wiringPiSetupGpio() == -1){
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		exit(1);
		}
		
		//FAN�� ��ǳ�� IO ��Ʈ ��ȣ ����
		pinMode(FAN, OUTPUT);
		pinMode(BUTTON, INPUT);
		pinMode(LED_RED, OUTPUT);
		pinMode(LED_YELLOW, OUTPUT);

	}

	void ICU::setup_raspberry(){
				patient1.aduino_setting("/dev/tty0");
				patient2.aduino_setting("/dev/tty1");
				gpio_setting();
	}

	void ICU::upload_data(); //kaa����
	
	
	void ICU::situation_control(){

		int danger = get_danger_situation();
		int room_temperature = get_room_temp();
		int set_room_temp = 26;

		switch(danger){
			case 1: danger = nurse.danger_emergency(); break;
			case 2: danger = nurse.danger_ringer(); break;
			case 3: danger = nurse.danger_situation_off(); break;
			case 0: break;
		}

		if(set_room_temp < room_temperature){
			digitalWrite(FAN, HIGH);
		}
		else digitalWrite(FAN, LOW);

	}// ����
}