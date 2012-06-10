#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <AbstractFunc.h>

class SFGFunc : public AbstractFunc
{
private:
	float pr_func(float freq, float f_st = 0, float f_en = 0)
	{
		return 1;
	}
};

#endif