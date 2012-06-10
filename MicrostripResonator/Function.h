#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

class MicrostripResonatorFunc : public AbstractFunc
{
public:
	MicrostripResonatorFunc() {
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
		readFile("./../BlackBoxValues/MicrostripResonator.val");
	}

	void readFile(QString fname) {
		QFile dataFile(fname);
		dataFile.open(QIODevice::ReadOnly);

		int cur_l = 18;
		int cur_s = 10;

		int l_step = 2;
		int s_step = 5;

		int last_l = 26;
		int last_s = 20;

		int valuesCount = dataFile.size() / (3 * 5 * sizeof(float));

		for(; cur_s <= last_s; cur_s += s_step) {
			for(; cur_l <= last_l; cur_l += l_step) {
				QByteArray valuesArray = dataFile.read(valuesCount * sizeof(float));
				QVector<float> valuesVector(valuesCount);
				memcpy((char*)valuesVector.data(), valuesArray.data(), valuesCount * sizeof(float));

				map[cur_s][cur_l] = valuesVector;
			}
		}

		for(float f = 8.; f <= 12.; f += 0.004)
			freqValues << f;

		/*
		QFile fw(fname);
		fw.open(QIODevice::ReadOnly);

		while(!fw.atEnd()) {
			float arg;
			float val;
			
			fw.read((char*)&arg, sizeof(float));
			fw.read((char*)&val, sizeof(float));
			map[arg] = val;
			mapKeys << arg;
		}

		fw.close();

		mapKeys = map.keys().toVector();
		qStableSort(mapKeys.begin(), mapKeys.end(), LessThan());
		//*/
	}

	float pr_func(float freq, float fst, float fen) {
		//*
		if(allowedInputs.size() != inputs.size())
			return 0;

		if( (freq < 8000.0) || (freq > 12000.) )
			return 0;

		AbstractFunc *add_func = inputs["input_signal"];

		const QVector <float> &curValues(map.value(s).value(l));

		float arg = freq / 1000.;
		int offset = (arg - 8.) / 0.004;

		const float &x1(freqValues.at(offset));
		const float &y1(curValues.at(offset));

		const float &x0(freqValues.at(offset - 1));
		const float &y0(curValues.at(offset - 1));

		const float &x2(freqValues.at(offset + 1));
		const float &y2(curValues.at(offset + 1));

		float &x(arg);

		return	(x - x1)*(x - x2)*y0/( (x0 - x1)*(x0 - x2) ) +
				(x - x0)*(x - x2)*y1/( (x1 - x0)*(x1 - x2) ) +
				(x - x0)*(x - x1)*y2/( (x2 - x0)*(x2 - x1) );
		/*/
		if(allowedInputs.size() != inputs.size())
			return 0;

		AbstractFunc *add_func = inputs["input_signal"];

		float arg = freq / 1000.;
		QVector<float>::iterator it = qLowerBound(mapKeys.begin(), mapKeys.end(), arg, LessThan());

		if(it == mapKeys.end())
			return 0;

		float &x1(*it);
		float &y1(map[*it]);

		it--;
		float &x0(*it);
		float &y0(map[*it]);

		it+=2;
		float &x2(*it);
		float &y2(map[*it]);

		float &x(arg);

		return	(x - x1)*(x - x2)*y0/( (x0 - x1)*(x0 - x2) ) +
				(x - x0)*(x - x2)*y1/( (x1 - x0)*(x1 - x2) ) +
				(x - x0)*(x - x1)*y2/( (x2 - x0)*(x2 - x1) );
		//*/
	}

	QMap<int, QMap<int, QVector<float>>> map;
	QVector<float> freqValues;

	int l;
	int s;
};

#endif