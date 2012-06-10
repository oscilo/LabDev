#ifndef TESTBLACKBOX_H
#define TESTBLACKBOX_H

#include "testblackbox_global.h"

#include <AbstractBlackBox.h>

#include "AttDialog.h"
#include "Function.h"

class TESTBLACKBOX_EXPORT TestBlackBox : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	TestBlackBox(QWidget *parent = 0, Qt::WFlags flags = 0);
	QString getDeviceName();

public slots:
	void set_new_k(int new_val);
	void show_hide_att(int);

private:
	QHBoxLayout *lay, *hlay;
	QLabel *lbl1, *lbl2;
	QSlider *sldr;
	QGroupBox *grp;
	AttDialog *att;
	QCheckBox *att_show;
};

#endif // TESTBLACKBOX_H
