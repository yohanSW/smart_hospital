#ifndef SH_ICU
#define SH_ICU
#include "common.h"

using namespace han
{

	class ICU{
		protected:
			double room_temp;
			int danger_situation;
		
		public:
			ICU();
			int get_danger_situation(){return danger_situation;}
			double get_room_temp(){return room_temp;}
			void dangerLED();

};

}
#endif /* SH_ICU */