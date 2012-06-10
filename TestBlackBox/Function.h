#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

class NarrowBandFilterFunc : public AbstractFunc
{
public:
	NarrowBandFilterFunc() {
		init();

		allowedInputs = QStringList() << "input_signal";
		k = 0;
	}

	void set_k(float k_in) {
		k = k_in;
	}

private:
	struct LessThan	{
		bool operator()(const float a, const float b) const	{
			return a < b;
		}
	};

	void init() {
		readFile("./../BlackBoxValues/NarrowBandFilterS21.val");
	}

	void readFile(QString fname) {
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
	}

	float pr_func(float freq, float fst, float fen) {
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
	}

	QMap<float, float> map;
	QVector<float> mapKeys;
	float k;
};

#endif