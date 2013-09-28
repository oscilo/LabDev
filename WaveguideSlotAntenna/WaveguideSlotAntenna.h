#ifndef WAVEGUIDESLOTANTENNA_H
#define WAVEGUIDESLOTANTENNA_H

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
	void SetPicture(int s, int l);

private:
	void ScalePixmap();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

	QString pixName;
	QPixmap pix;
	QPixmap pixToDraw;
};
////////////////////////////////////////////////////////////////////////////////////////////////
class WaveguideSlotAntenna : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	WaveguideSlotAntenna(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	QString getDeviceName();
	QString getDeviceIDName();

private slots:
	void SlidersUpdate();

private:
	void CreateWidgets();

	WaveguideSlotAntennaFunc *waveguideSlotAntennaFunc;

	QWidget *paintingWidget;

	QSlider *sSlider;
	QSlider *lSlider;
	QLabel *sLabel;
	QLabel *lLabel;
};

#endif // WAVEGUIDESLOTANTENNA_H
