#ifndef SWEEPFREQUENCYGENERATOR_H
#define SWEEPFREQUENCYGENERATOR_H

#include "globals.h"

#include <AbstractSFGenerator.h>

#include <CommonDial.h>

#include "Function.h"

class SweepFrequencyGenerator : public AbstractSFGenerator
{
	Q_OBJECT
	Q_INTERFACES(AbstractSFGenerator AbstractDevice)

public:
	SweepFrequencyGenerator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SweepFrequencyGenerator();

	QString getDeviceName();
	void initProperties();

public slots:
	void set_new_fst(int new_val);
	void set_new_fen(int new_val);
	void set_new_mark1(int new_val);
	void set_new_mark2(int new_val);

private:
	enum DispState
	{
		DS_F_ST = 0,
		DS_F_EN = 1,
		DS_MARK1 = 2,
		DS_MARK2 = 3
	};

	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);

	float scale_k;
	CommonDial *fStSl, *fEnSl, *fMark1Sl, *fMark2Sl;
	DispState dstate;
	QPixmap *back, *lamp, *d[10], *d_;
	QVector<QPoint> doffsets;

	QSize curSize;
	QPixmap resizedBack;
};

#endif // SWEEPFREQUENCYGENERATOR_H