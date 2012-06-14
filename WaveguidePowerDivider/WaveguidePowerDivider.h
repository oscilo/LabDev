#ifndef WAVEGUIDEPOWERDIVIDER_H
#define WAVEGUIDEPOWERDIVIDER_H

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
	void SetPicture(int l, int r);

private:
	void ScalePixmap();
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

	QMap<int, QMap<int, QString>> pixNames;
	QPixmap pix;
	QPixmap pixToDraw;
};
////////////////////////////////////////////////////////////////////////////////////////////////
class WaveguidePowerDivider : public AbstractBlackBox {
	Q_OBJECT
	Q_INTERFACES(AbstractBlackBox AbstractDevice)

public:
	WaveguidePowerDivider(QWidget *parent = 0, Qt::WFlags flags = 0);
	QString getDeviceName();

private slots:
	void SlidersUpdate();

private:
	void CreateWidgets();

	WaveguidePowerDividerFunc *waveguidePowerDividerLeft;
	WaveguidePowerDividerFunc *waveguidePowerDividerRight;

	QWidget *paintingWidget;

	QSlider *hSlider;
	QSlider *rSlider;
	QLabel *hLabel;
	QLabel *rLabel;
};

#endif