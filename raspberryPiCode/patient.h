#ifndef SH_PATIENT
#define SH_PATIENT
#include "common.h"

using namespace han
{

	class Patient{
		protected:
			String name;
			int danger;
		

		public:
			Patient();
			int get_danger(){return danger;}
			String get_name(){return name;}
			virtual void get_sensor();
			virtual void give_order();

	};
}

#endif /* SH_PATIENT */