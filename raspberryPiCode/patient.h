#ifndef SH_PATIENT
#define SH_PATIENT
#include "common.h"

namespace han
{

	class Patient{
		protected:
			string name;
			int danger;
			std::mutex m;
			int fd;

		public:
			Patient();
			int get_danger(){return danger;}
			void set_danger(int num){this.danger = num;}
			string get_name(){return name;}
			virtual void aduino_setting(string);
			virtual void get_sensor();
			virtual void give_order();

	};
}

#endif /* SH_PATIENT */