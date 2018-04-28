#ifndef SH_VEGETATIVE
#define SH_VEGETATIVE
#include "common.h"
#include "patient.h"

using namespace han{

	class Vegetative: public Patient{
		private:
			int heart_rate;
			double infusion_solution;
			bool wake_up;
			int fd;
			
		public:
			Vegetative();
			Vegetative(String);
			int get_heart_rate(){return heart_rate;}
			void set_heart_rate(int num){this.heart_rate = num;}
			double get_infusion_solution(){return infusion_solution;}
			void set_infusion_solution(double num){this.infusion_solution = num;}
			bool get_wake_up(){return wake_up;}

			void aduino_setting(String device_port);
			void get_sensor();
			void give_order();
	};


}
#endif /* SH_VEGETATIVE */