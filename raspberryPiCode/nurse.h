#ifndef SH_NURSE
#define SH_NURSE
#include "common.h"
#include "ICU.h"

namespace han
{

	class Nurse{
		protected:
			int danger_num;

		public:
			Nurse();
			int danger_emergency();
			int danger_ringer();
			int danger_situation_off();

	};
}

#endif /* SH_NURSE */