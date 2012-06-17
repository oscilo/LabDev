#include "WaveguideTBridge.h"

WaveguideTBridge::WaveguideTBridge(QWidget *parent, Qt::WFlags flags) :
	AbstractDevice(parent, flags)
{
	back = new QPixmap("./Resources/WaveguideTBridge/back.png");
}
WaveguideTBridge::~WaveguideTBridge() {
	delete back;
}
QString WaveguideTBridge::getDeviceName() {
	return RUS("Волноводный Т-мост");
}
AbstractDevice::DeviceType WaveguideTBridge::getDeviceType() {
	return AbstractDevice::DT_METER;
}
void WaveguideTBridge::paintEvent(QPaintEvent *event) {
	QSize neededSize = back->size();
	neededSize.scale(this->size(), Qt::KeepAspectRatio);
	if(resizedBack.size() != neededSize)
		resizedBack = back->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	
	QPixmap pix = resizedBack;
	
	QPoint offset(0, 0);
	QSize curSize = pix.size();
	float scale = ( (float)(pix.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != this->width())
		offset.setX( (this->width()  - curSize.width()) / 2 );
	else
		offset.setY( (this->height()  - curSize.height()) / 2 );

	QPainter pntr(this);
	QPainter pntr_pix(&pix);

	pntr.drawPixmap(offset, pix);
}
void WaveguideTBridge::resizeEvent(QResizeEvent *event) {
}