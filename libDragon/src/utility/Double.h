#ifndef __DRAONG_DOUBLE_H__
#define __DRAGON_DOUBLE_H__

#include "../core/Macro.h"
#include <math.h>

NS_DRAGON

	const double EPSILON  = 0.0000001;
 	class Double {
	public:
		static bool Equal(double lhs, double rhs) {
				if (fabs(lhs - rhs) <= EPSILON) 
						return true;
				else
						return false;
		}

		static bool Greater(double lhs, double rhs) {
				if ((lhs - rhs) >= EPSILON)
					return true;
				else 
					return false;
		}

		static bool Lesser(double lhs, double rhs) {
				if ((lhs - rhs) <= -EPSILON)
					return true;
				else 
					return false;
		}

		static bool Range(double d ,double low, double high)
		{
			if ((Greater(d, low)) && (Lesser(d, high)))
				return true;
			else 
				return false;
		}
	};

NS_END

#endif