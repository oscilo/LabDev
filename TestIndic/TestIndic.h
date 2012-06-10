#ifndef TESTINDIC_H
#define TESTINDIC_H

#include "testindic_global.h"

#include <AbstractIndicator.h>

#include "globals.h"

#include <UpperLimitDial.h>
#include <CommonDial.h>

#include "TestDraw.h"

class TESTINDIC_EXPORT TestIndic : public AbstractIndicator
{
	Q_OBJECT
	Q_INTERFACES(AbstractIndicator AbstractDevice)

public:
	TestIndic(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TestIndic();
	
	QString getDeviceName();
	void initProperties();

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

	QSize drawInitSize;
	float scale_k;
};

#endif // TESTINDIC_H
