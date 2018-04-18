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
		

	};
}
#endif /* SH_VEGETATIVE */