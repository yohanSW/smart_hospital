#include "common.h"
#include "ICU.h"
#include "vegetative_state.h"
#include "fever.h"
#include "nurse.h"


//Global Variable
void init_setting(ICU &room);

int main(int argc, char *argv[] ){
	using namespace han;
	//Local variable + class
	ICU room1;
	cout << "wowong!"<< endl;


	//setting all (+multi thread)
	init_setting(room1);
	thread t1(&(room1.patient1.get_sensor()));
	thread t2(&(room1.patient2.get_sensor()));

	// while loop
	while(true){
		cout << "in while\n"<< endl;
		//kaa part

	}
	t1.join();
	t2.join();
	return 0;
}



void init_setting(ICU &room){
	printf("%s \n", "main setting Start!");
	fflush(stdout);
	room.setup_raspberry();
}
