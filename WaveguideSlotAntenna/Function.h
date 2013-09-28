#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

#define L_COUNT		10
#define S_COUNT		10
#define SK_COUNT	10
#define X_COUNT		10

#define L_START		1148
#define SK_START	2128
#define S_START		1468
#define X_START		368

#define L_LAST		1940
#define SK_LAST		3865
#define S_LAST		2665
#define X_LAST		665

#define L_STEP		88
#define SK_STEP		193
#define S_STEP		133
#define X_STEP		33

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

		if(!dataFile.isOpen())
			return;
		
		int cur_l;
		int cur_s;
		int cur_x;
		int cur_sk;

		QVector<float> value((F_LAST - F_FIRST)/F_STEP + 1);

		for(cur_x = X_START;	cur_x <= X_LAST;	cur_x += X_STEP) {
		for(cur_sk = SK_START;	cur_sk <= SK_LAST;	cur_sk += SK_STEP) {
		for(cur_l = L_START;	cur_l <= L_LAST;	cur_l += L_STEP) {
		for(cur_s = S_START;	cur_s <= S_LAST;	cur_s += S_STEP) {
			dataFile.read((char*)value.data(), value.size()*sizeof(float));
			SetValue(cur_x, cur_s, cur_sk, cur_l, value);
		}}}}
	}

	float pr_func(float freq, float fst, float fen) {
		if(allowedInputs.size() != inputs.size())
			return 0;

		if( (freq < 8000.0) || (freq > 12000.) )
			return 0;

		AbstractFunc *add_func = inputs["input_signal"];
		
		float arg = freq / 1000.;
		int offset = (arg - 8.) / 0.004;

		return GetCurrentValue(offset);
	}

	const QVector<float>& GetValue(int x_, int s_, int sk_, int l_) {
		return map[x_][sk_][l_][s_];
	}
	float GetCurrentValue(int offset) {
		return GetValue(x, s, sk, l).at(offset);
	}
	void SetValue(int x_, int s_, int sk_, int l_, QVector<float> value) {
		map[x_][sk_][l_][s_] = value;
	}

	QMap<int, QMap<int, QMap<int, QMap<int, QVector<float>>>>> map;
	QVector<float> freqValues;

	int l;
	int s;
	int sk;
	int x;
};

#endif