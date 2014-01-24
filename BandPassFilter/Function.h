#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

#define H1_START		0
#define H2_START		0
#define H1_STEP			20
#define H2_STEP			20

class BandPassFilterFunc : public AbstractFunc {
public:
	BandPassFilterFunc() {
		init();

		allowedInputs = QStringList() << "input_signal";
		h1 = 0;
		h2 = 0;
	}

	void set_h1(int h1_in) {
		h1 = h1_in;
	}
	void set_h2(int h2_in) {
		h2 = h2_in;
	}

private:
	struct LessThan	{
		bool operator()(const float a, const float b) const	{
			return a < b;
		}
	};

	void init() {
		readFile("./../BlackBoxValues/BandPassFilter.val");
	}

	void readFile(QString fname) {
		QFile dataFile(fname);
		dataFile.open(QIODevice::ReadOnly);

		int cur_h1;
		int cur_h2;

		int h1_step = H1_STEP;
		int h2_step = H2_STEP;

		int last_h1 = 100;
		int last_h2 = 100;

		int valuesCount = dataFile.size() / (6 * 6 * sizeof(float));

		for(cur_h1 = H1_START; cur_h1 <= last_h1; cur_h1 += h1_step) {
			for(cur_h2 = H2_START; cur_h2 <= last_h2; cur_h2 += h2_step) {
				QByteArray valuesArray = dataFile.read(valuesCount * sizeof(float));
				QVector<float> valuesVector(valuesCount);
				memcpy((char*)valuesVector.data(), valuesArray.data(), valuesCount * sizeof(float));

				map[cur_h1][cur_h2] = valuesVector;
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

		int curH1 = H1_START + (((h1 - H1_START) / H1_STEP) * H1_STEP);
		int curH2 = H2_START + (((h2 - H2_START) / H2_STEP) * H2_STEP);
		int nextH1 = (h1 - curH1) ? (curH1 + H1_STEP) : curH1;
		int nextH2 = (h2 - curH2) ? (curH2 + H2_STEP) : curH2;

		const QVector <float> &curValues(map.value(curH1).value(curH2));
		//const QVector <float> &nextValues(map.value(nextH1).value(nextH2));

		float arg = freq / 1000.;
		int offset = (arg - 8.) / 0.004;
		
		/*
		float totalWeight = ((curH1 == nextH1) ? 0 : H1_STEP) + ((nextH1 == nextH2) ? 0 : H2_STEP);
		float weight = 0.;
		if(totalWeight)
			weight = ((h1 - curH1) + (h2 - curH2)) / totalWeight;

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
		return curValues.at(offset);
	}

	QMap<int, QMap<int, QVector<float>>> map;
	QVector<float> freqValues;

	int h1;
	int h2;
};

#endif