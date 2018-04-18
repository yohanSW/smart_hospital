#ifndef SH_VEGETATIVE
#define SH_VEGETATIVE
#include "common.h"
#include "patient.h"

using namespace han{

	class Vegetative: public Patient{
		protected:
			int heart_rate;
			double infusion_solution;
			bool wake_up;
		public:
			Vegetative();
			int get_heart_rate(){return heart_rate;}
			double get_infusion_solution(){return infusion_solution;}
			bool get_wake_up(){return wake_up;}
			void get_sensor();
			void give_order();
	};
}
#endif /* SH_VEGETATIVE */