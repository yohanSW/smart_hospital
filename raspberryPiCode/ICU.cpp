#include "ICU.h"


using namespace han
{
	ICU();

	void setup_raspberry(){
				patient1.aduino_setting("/dev/tty0");
				patient2.aduino_setting("/dev/tty1");
				gpio_setting();
	}
	void upload_data(); //kaa����
	
	
	void situation_control(); // ����
}