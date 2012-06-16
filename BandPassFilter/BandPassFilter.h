#ifndef BANDPASSFILTER_H
#define BANDPASSFILTER_H

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
	void SetPicture(int h1, int h2);

private:
	void ScalePixmap();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

	QMap<int, QMap<int, QString>> pixNames;
	QPixmap pix;
	QPixmap pixToDraw;
};
////////////////////////////////////////////////////////////////////////////////////////////////
class BandPassFilter : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	BandPassFilter(QWidget *parent = 0, Qt::WFlags flags = 0);
	QString getDeviceName();

private slots:
	void SlidersUpdate();

private:
	void CreateWidgets();

	BandPassFilterFunc *bandPassFilterFunc;

	QWidget *paintingWidget;

	QSlider *h1Slider;
	QSlider *h2Slider;
	QLabel *h1Label;
	QLabel *h2Label;
};

#endif // MICROSTRIPRESONATOR_H