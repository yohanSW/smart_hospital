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
			Fever(void);
	
	
	};

}

#endif /* SH_FEVER */