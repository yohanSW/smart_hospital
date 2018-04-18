#ifndef SH_FEVER
#define SH_FEVER
#include "common.h"
#include "patient.h"

using namespace han
{

	class Fever: public Patient{
		protected:
			int heart_rate;
			double temp;
	
		public:
			Fever();
			int get_heart_rate(){return heart_rate;}
			double get_temp(){return temp;}
			void get_sensor();
			void give_order();
	};

}

#endif /* SH_FEVER */