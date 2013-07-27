#ifndef REDUCTIONINDICATOR_H
#define REDUCTIONINDICATOR_H

#include <AbstractIndicator.h>

#include "globals.h"

#include <UpperLimitDial.h>
#include <CommonDial.h>

#include "ReductionIndicatorDraw.h"

class ReductionIndicator : public AbstractIndicator
{
	Q_OBJECT
	Q_INTERFACES(AbstractIndicator AbstractDevice)

public:
	ReductionIndicator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ReductionIndicator();
	
	QString getDeviceName();
	void initProperties();
	QString getDeviceIDName();

signals:
	void mark_h_changed(int new_val);
	void m_line_changed(int new_val);
	void lim_changed(int new_val);

public slots:
	void set_new_mark_h(int new_val);
	void set_new_m_line(int new_val);
	void set_new_lim(int new_val);

private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

	UpperLimitDial *limitSl;
	CommonDial *markHSl, *mLineSl;
	QPixmap *back, *shk[31], *shkRisk;

	QPixmap resizedBack;
	QSize drawInitSize;
	float scale_k;
};

#endif // REDUCTIONINDICATOR_H
