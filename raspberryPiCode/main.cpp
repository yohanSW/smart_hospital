#include "common.h"
#include "ICU.h"
#include "vegetative_state.h"
#include "fever.h"
#include "nurse.h"


using namespace han;
//Global Variable
void init_setting(ICU& room);

int main(int argc, char *argv ){
	//Local variable + class
	ICU room1;
	cout << "wowong!"<< endl;


	//setting all (+multi thread)
	init_setting(room1);




	// while loop
	while(true){
		cout << "in while\n"<< endl;




	}
	return 0;
}



void init_setting(ICU& room){
	printf("%s \n", "Raspberry Startup!");
	fflush(stdout);
	room1.setup_raspberry();

	

}
