#ifndef NARROWBANDFILTER_H
#define NARROWBANDFILTER_H

#include <AbstractBlackBox.h>

#include "AttDialog.h"
#include "Function.h"

class NarrowBandFilter : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	NarrowBandFilter(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	QString getDeviceName();
	QString getDeviceIDName();

public slots:
	//void set_new_k(int new_val);
	//void show_hide_att(int);

private:
	/*
	QHBoxLayout *lay, *hlay;
	QLabel *lbl1, *lbl2;
	QSlider *sldr;
	QGroupBox *grp;
	AttDialog *att;
	QCheckBox *att_show;
	//*/
};

#endif // NARROWBANDFILTER_H
