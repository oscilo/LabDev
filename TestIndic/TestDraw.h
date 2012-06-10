#ifndef TESTDRAW_H
#define TESTDRAW_H

#include "globals.h"

#include <AbstractDrawArea.h>

class TestDraw : public AbstractDrawArea
{
	Q_OBJECT

public:
	TestDraw(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TestDraw();

public slots:
	void set_new_mark_h(int new_val);
	void set_new_m_line(int new_val);
	void set_new_lim(int new_val);

private:
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent *event);
	QPixmap *area_pic, *point_pic;
	QPixmap pointScaled;
	QPixmap areaScaled;

	float scale_k;
	int LEN;
	int TL;
	
	QSize sizeOnInit;
};

#endif // TESTDRAW_H
