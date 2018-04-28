#ifndef SH_FEVER
#define SH_FEVER
#include "common.h"
#include "patient.h"

using namespace han
{

	class Fever: public Patient{
		private:
			int heart_rate;
			double temp;
	
		public:
			Fever();
			int get_heart_rate(){return heart_rate;}
			void set_heart_rate(int num){this.heart_rate = num;}
			double get_temp(){return temp;}
			void set_temp(double num){this.temp = num;}

			void aduino_setting(String device_port);
			void get_sensor();
			void give_order();
	};

}

#endif /* SH_FEVER */