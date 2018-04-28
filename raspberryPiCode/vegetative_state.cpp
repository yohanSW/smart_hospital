#include <vegetative_state.h>


using namespace han
{
	Vegetative();
	void Vegetative::aduino_setting(String device_port){
		//get filedescriptor
		if ((fd = serialOpen (device_port, 115200)) < 0){
			fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
			exit(1); //error
		}
 
		//setup GPIO in wiringPi mode
		if (wiringPiSetup () == -1){
			fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
			exit(1); //error
		}
	}
	void get_sensor();
	void give_order(){
		cout << "give_order" << endl;
	}
}