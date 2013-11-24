#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "globals.h"

#include <AbstractFunc.h>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

#define L0_2_COUNT	7
#define L0_3_COUNT	7
#define L_2_COUNT	7
#define L_3_COUNT	7

#define L0_2_START	2128
#define L0_3_START	368
#define L_2_START	1468
#define L_3_START	1148

#define L0_2_LAST	1940
#define L0_3_LAST	3865
#define L_2_LAST	2665
#define L_3_LAST	665

#define L0_2_STEP	88
#define L0_3_STEP	193
#define L_2_STEP	133
#define L_2_STEP	33

class BandPassFilter2Func : public AbstractFunc {
public:
	BandPassFilter2Func() {
		init();

		allowedInputs = QStringList() << "input_signal";

		l0_2 = L0_2_START;
		l0_3 = L0_3_START;
		l_2 = L_2_START;
		l_3 = L_3_START;
	}
	
	void set_l0_2(int l0_2_in) {
		l0_2 = l0_2_in;
	}
	void set_l0_3(int l0_3_in) {
		l0_3 = l0_3_in;
	}
	void set_l_2(int l_2_in) {
		l_2 = l_2_in;
	}
	void set_l_3(int l_3_in) {
		l_3 = l_3_in;
	}

private:
	struct LessThan	{
		bool operator()(const float a, const float b) const	{
			return a < b;
		}
	};

	void init() {
		readFile("./../BlackBoxValues/BandPassFilter2.val");
	}

	void readFile(QString fname) {
		QFile dataFile(fname);
		dataFile.open(QIODevice::ReadOnly);

		if(!dataFile.isOpen())
			return;
		
		int cur_l0_2;
		int cur_l0_3;
		int cur_l_2;
		int cur_l_3;

		QVector<float> value((F_LAST - F_FIRST)/F_STEP + 1);

		for(cur_l0_2 = L0_3_START;	cur_l0_2 <= L0_2_LAST;	cur_l0_2 += L0_2_STEP) {
		for(cur_l0_3 = L0_2_START;	cur_l0_3 <= L0_3_LAST;	cur_l0_3 += L0_3_STEP) {
		for(cur_l_2 = L_3_START;	cur_l_2 <= L_2_LAST;	cur_l_2 += L_2_STEP) {
		for(cur_l_3 = L_2_START;	cur_l_3 <= L_3_LAST;	cur_l_3 += L_2_STEP) {
			dataFile.read((char*)value.data(), value.size()*sizeof(float));
			SetValue(cur_l0_2, cur_l0_3, cur_l_2, cur_l_3, value);
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

	const QVector<float>& GetValue(int l0_2_, int l0_3_, int l_2_, int l_3_) {
		return map[l0_2_][l0_3_][l_2_][l_3_];
	}
	float GetCurrentValue(int offset) {
		return GetValue(l0_2, l0_3, l_2, l_3).at(offset);
	}
	void SetValue(int l0_2_, int l0_3_, int l_2_, int l_3_, QVector<float> value) {
		map[l0_2_][l0_3_][l_2_][l_3_] = value;
	}

	QMap<int, QMap<int, QMap<int, QMap<int, QVector<float>>>>> map;
	QVector<float> freqValues;

	int l0_2;
	int l0_3;
	int l_2;
	int l_3;
};

#endif