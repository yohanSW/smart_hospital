#include "common.h"
#include "ICU.h"
#include "vegetative_state.h"
#include "fever.h"
#include "nurse.h"

using namespace std;
using namespace han;
//Global Variable
void init_setting(ICU& room);

int main(int argc, char *argv ){
	//Local variable + class
	ICU room1;
	cout << "wowong!"<< endl;


	//setting all (+multi thread)
	init_setting(room1);
	std::thread t1(&(room1.patient1.get_sensor()));
	std::thread t2(&(room2.patient2.get_sensor()));

	// while loop
	while(true){
		cout << "in while\n"<< endl;


	}
	t1.join();
	t2.join();
	return 0;
}



void init_setting(ICU& room){
	printf("%s \n", "main setting Start!");
	fflush(stdout);
	room1.setup_raspberry();
}
