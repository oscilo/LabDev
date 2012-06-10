#ifndef MICROSTRIPRESONATOR_H
#define MICROSTRIPRESONATOR_H

#include <AbstractBlackBox.h>

#include "Function.h"

class MicrostripResonator : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	MicrostripResonator(QWidget *parent = 0, Qt::WFlags flags = 0);
	QString getDeviceName();

private:
	void CreateWidgets();
};

#endif // MICROSTRIPRESONATOR_H
