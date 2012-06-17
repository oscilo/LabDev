#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

#define S_START		35
#define L_START		0
#define S_STEP		30
#define L_STEP		20

class CoaxialWaveguideTransitionFunc : public AbstractFunc {
public:
	CoaxialWaveguideTransitionFunc() {
		init();

		allowedInputs = QStringList() << "input_signal";
		s = 10;
		l = 18;
	}

	void set_l(int l_in) {
		l = l_in;
	}
	void set_s(int s_in) {
		s = s_in;
	}

private:
	struct LessThan	{
		bool operator()(const float a, const float b) const	{
			return a < b;
		}
	};

	void init() {
		readFile("./../BlackBoxValues/CoaxialWaveguideTransition.val");
	}

	void readFile(QString fname) {
		QFile dataFile(fname);
		dataFile.open(QIODevice::ReadOnly);

		int cur_l;
		int cur_s;

		int l_step = L_STEP;
		int s_step = S_STEP;

		int last_l = 100;
		int last_s = 155;

		int valuesCount = dataFile.size() / (6 * 5 * sizeof(float));

		for(cur_l = L_START; cur_l <= last_l; cur_l += l_step) {
			for(cur_s = S_START; cur_s <= last_s; cur_s += s_step) {
				QByteArray valuesArray = dataFile.read(valuesCount * sizeof(float));
				QVector<float> valuesVector(valuesCount);
				memcpy((char*)valuesVector.data(), valuesArray.data(), valuesCount * sizeof(float));

				map[cur_l][cur_s] = valuesVector;
			}
		}

		for(float f = 8.; f <= 12.; f += 0.004)
			freqValues << f;
	}

	float pr_func(float freq, float fst, float fen) {
		if(allowedInputs.size() != inputs.size())
			return 0;

		if( (freq < 8000.0) || (freq > 12000.) )
			return 0;

		AbstractFunc *add_func = inputs["input_signal"];

		int curS = S_START + (((s - S_START) / S_STEP) * S_STEP);
		int curL = L_START + (((l - L_START) / L_STEP) * L_STEP);
		int nextS = (s - curS) ? (curS + S_STEP) : curS;
		int nextL = (l - curL) ? (curL + L_STEP) : curL;

		const QVector <float> &curValues(map.value(curL).value(curS));
		const QVector <float> &nextValues(map.value(nextL).value(nextS));

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
	}

	QMap<int, QMap<int, QVector<float>>> map;
	QVector<float> freqValues;

	int l;
	int s;
};

#endif