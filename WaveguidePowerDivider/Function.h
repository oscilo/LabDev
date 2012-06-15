#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

#define H_START		20
#define R_START		10
#define H_STEP		20
#define R_STEP		10

class WaveguidePowerDividerFunc : public AbstractFunc {
public:
	WaveguidePowerDividerFunc() {
		init();

		allowedInputs = QStringList() << "input_signal";
		h = 20;
		r = 10;
	}

	void set_h(int h_in) {
		h = h_in;
	}
	void set_r(int r_in) {
		r = r_in;
	}

private:
	struct LessThan	{
		bool operator()(const float a, const float b) const	{
			return a < b;
		}
	};

	void init() {
		readFile("./../BlackBoxValues/WaveguidePowerDivider.val");
	}

	void readFile(QString fname) {
		QFile dataFile(fname);
		dataFile.open(QIODevice::ReadOnly);

		int cur_h;
		int cur_r;

		int h_step = H_STEP;
		int r_step = R_STEP;

		int last_h = 100;
		
		QMap<int, int> rLast;
		rLast[20] = 20;
		rLast[40] = 40;
		rLast[60] = 50;
		rLast[80] = 50;
		rLast[100] = 50;

		int valuesCount = dataFile.size() / ( (2 + 3 + 5 + 5 + 5) * sizeof(float));

		for(cur_h = H_START; cur_h <= last_h; cur_h += h_step) {
			for(cur_r = R_START; cur_r <= rLast[cur_h]; cur_r += r_step) {
				QByteArray valuesArray = dataFile.read(valuesCount * sizeof(float));
				QVector<float> valuesVector(valuesCount);
				memcpy((char*)valuesVector.data(), valuesArray.data(), valuesCount * sizeof(float));

				map[cur_h][cur_r] = valuesVector;
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

		int curH = H_START + (((h - H_START) / H_STEP) * H_STEP);
		int curR = R_START + (((r - R_START) / R_STEP) * R_STEP);
		int nextH = (h - curH) ? (curH + H_STEP) : curH;
		int nextR = (r - curR) ? (curR + R_STEP) : curR;

		const QVector <float> &curValues(map.value(curH).value(curR));
		const QVector <float> &nextValues(map.value(nextH).value(nextR));

		float arg = freq / 1000.;
		int offset = (arg - 8.) / 0.004;

		float totalWeight = ((curH == nextH) ? 0 : H_STEP) + ((curR == nextR) ? 0 : R_STEP);
		float weight = 0.;
		if(totalWeight)
			weight = ((h - curH) + (r - curR)) / totalWeight;

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

	int h;
	int r;
};

#endif