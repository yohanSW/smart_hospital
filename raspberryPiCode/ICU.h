#ifndef SH_ICU
#define SH_ICU
#include "common.h"

using namespace han
{

	class ICU{
		private:
			double room_temp;
			int danger_situation;
			Fever patient1;
			Vegetative patient2;
			Nurse nurse;
		public:
			ICU();
			int get_danger_situation(){return danger_situation;}
			double get_room_temp(){return room_temp;}
			void gpio_setting(); // ����
			void setup_raspberry();
			void upload_data(); //kaa����

			void situation_control(); // ����

};

}
#endif /* SH_ICU */