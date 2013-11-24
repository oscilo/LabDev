#ifndef BANDPASSFILTER2_H
#define BANDPASSFILTER2_H

#include <AbstractBlackBox.h>

#include "Function.h"

////////////////////////////////////////////////////////////////////////////////////////////////
class PaintingWidget : public QWidget {
	Q_OBJECT
public:
	PaintingWidget(QWidget* parent = 0);

signals:
	void repaintSignal();

public slots:
	void SetPicture();

private:
	void ScalePixmap();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

	QString pixName;
	QPixmap pix;
	QPixmap pixToDraw;
};
////////////////////////////////////////////////////////////////////////////////////////////////
class BandPassFilter2 : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	BandPassFilter2(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	QString getDeviceName();
	QString getDeviceIDName();

private slots:
	void SlidersUpdate();

private:
	void CreateWidgets();

	BandPassFilter2Func *bandPassFilter2Func;

	QWidget *paintingWidget;
	
	QSlider *l0_2Slider;
	QSlider *l0_3Slider;
	QSlider *l_2Slider;
	QSlider *l_3Slider;
	QLabel *l0_2Label;
	QLabel *l0_3Label;
	QLabel *l_2Label;
	QLabel *l_3Label;
};

#endif // BANDPASSFILTER2_H
