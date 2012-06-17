#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

class WaveguideTBridgeFunc : public AbstractFunc {
public:
	WaveguideTBridgeFunc() {
		allowedInputs = QStringList() << "input_signal";
	}

private:
	float pr_func(float freq, float fst, float fen) {
		return	1.;
	}
};

#endif