#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

#define L_COUNT		10
#define S_COUNT		10
#define SK_COUNT	10
#define X_COUNT		3

#define L_START		1148
#define S_START		2128
#define SK_START	1468
#define X_START		368

#define L_LAST		194
#define S_LAST		3865
#define SK_LAST		2665
#define X_LAST		434

#define L_STEP		88
#define S_STEP		193
#define SK_STEP		133
#define X_STEP		368

class WaveguideSlotAntennaFunc : public AbstractFunc {
public:
	WaveguideSlotAntennaFunc() {
		init();

		allowedInputs = QStringList() << "input_signal";

		sk = SK_START;
		x = X_START;
		s = S_START;
		l = L_START;
	}
	
	void set_l(int l_in) {
		l = l_in;
	}
	void set_s(int s_in) {
		s = s_in;
	}
	void set_x(int x_in) {
		x = x_in;
	}
	void set_sk(int sk_in) {
		sk = sk_in;
	}

private:
	struct LessThan	{
		bool operator()(const float a, const float b) const	{
			return a < b;
		}
	};

	void init() {
		readFile("./../BlackBoxValues/WaveguideSlotAntenna.val");
	}

	void readFile(QString fname) {
		QFile dataFile(fname);
		dataFile.open(QIODevice::ReadOnly);
		
		int cur_l;
		int cur_s;
		int cur_x;
		int cur_sk;
		
		int l_step = L_STEP;
		int s_step = S_STEP;
		int x_step = X_STEP;
		int sk_step = SK_STEP;

		int last_l = L_LAST;
		int last_s = S_LAST;
		int last_x = X_LAST;
		int last_sk = SK_LAST;
		/*
		int valuesCount = dataFile.size() / (3 * 5 * sizeof(float));

		for(cur_s = S_START; cur_s <= last_s; cur_s += s_step) {
			for(cur_l = L_START; cur_l <= last_l; cur_l += l_step) {
				QByteArray valuesArray = dataFile.read(valuesCount * sizeof(float));
				QVector<float> valuesVector(valuesCount);
				memcpy((char*)valuesVector.data(), valuesArray.data(), valuesCount * sizeof(float));

				map[cur_s][cur_l] = valuesVector;
			}
		}

		for(float f = 8.; f <= 12.; f += 0.004)
			freqValues << f;
		//*/
	}

	float pr_func(float freq, float fst, float fen) {
		/*
		if(allowedInputs.size() != inputs.size())
			return 0;

		if( (freq < 8000.0) || (freq > 12000.) )
			return 0;

		AbstractFunc *add_func = inputs["input_signal"];

		int curS = S_START + (((s - S_START) / S_STEP) * S_STEP);
		int curL = L_START + (((l - L_START) / L_STEP) * L_STEP);
		int nextS = (s - curS) ? (curS + S_STEP) : curS;
		int nextL = (l - curL) ? (curL + L_STEP) : curL;

		const QVector <float> &curValues(map.value(curS).value(curL));
		const QVector <float> &nextValues(map.value(nextS).value(nextL));

		float arg = freq / 1000.;
		int offset = (arg - 8.) / 0.004;

		float totalWeight = ((curS == nextS) ? 0 : S_STEP) + ((curL == nextL) ? 0 : L_STEP);
		float weight = 0.;
		if(totalWeight)
			weight = ((s - curS) + (l - curL)) / totalWeight;

		const float &x1(freqValues.at(offset));
		const float y1 = (curValues.at(offset) * (1 - weight)) + (nextValues.at(offset) * weight);

		const float &x0(freqValues.at(offset - 1));
		const float y0 = (curValues.at(offset - 1) * (1 - weight)) + (nextValues.at(offset - 1) * weight);

		const float &x2(freqValues.at(offset + 1));
		const float y2 = (curValues.at(offset + 1) * (1 - weight)) + (nextValues.at(offset + 1) * weight);

		float &x(arg);

		return	(x - x1)*(x - x2)*y0/( (x0 - x1)*(x0 - x2) ) +
				(x - x0)*(x - x2)*y1/( (x1 - x0)*(x1 - x2) ) +
				(x - x0)*(x - x1)*y2/( (x2 - x0)*(x2 - x1) );
		//*/
		return 1;
	}

	QMap<int, QMap<int, QMap<int, QMap<int, QVector<float>>>>> map;
	QVector<float> freqValues;

	int l;
	int s;
	int sk;
	int x;
};

#endif