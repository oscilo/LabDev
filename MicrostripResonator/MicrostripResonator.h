#ifndef MICROSTRIPRESONATOR_H
#define MICROSTRIPRESONATOR_H

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

	QMap<int, QMap<int, QString>> pixNames;
	QPixmap pix;
	QPixmap pixToDraw;
};
////////////////////////////////////////////////////////////////////////////////////////////////
class MicrostripResonator : public AbstractBlackBox
{
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	MicrostripResonator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	QString getDeviceName();
	QString getDeviceIDName();

private slots:
	void SlidersUpdate();

private:
	void CreateWidgets();

	MicrostripResonatorFunc *microstripResonatorFunc;

	QWidget *paintingWidget;

	QSlider *sSlider;
	QSlider *lSlider;
	QLabel *sLabel;
	QLabel *lLabel;
};

#endif // MICROSTRIPRESONATOR_H
