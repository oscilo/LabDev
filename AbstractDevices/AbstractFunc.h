#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include <QStringList>
#include <QMap>

class AbstractFunc
{
public:
	AbstractFunc() {}

	float operator()(float freq = 0, float f_st = 0, float f_en = 0) {
		return pr_func(freq, f_st, f_en);
	}
	void setInput(const QString &_key, AbstractFunc *_input) {
		if(allowedInputs.contains(_key))
			inputs[_key] = _input;
	}
	int removeInput(const QString &_key) {
		return inputs.remove(_key);
	}
	QStringList getAllowedInputs() {
		return allowedInputs;
	}

protected:
	virtual float pr_func(float freq = 0, float f_st = 0, float f_en = 0) = 0;

	QMap<QString, AbstractFunc*> inputs;
	QStringList allowedInputs;
};

typedef QMap<QString, AbstractFunc*> SignalsMap;

#endif