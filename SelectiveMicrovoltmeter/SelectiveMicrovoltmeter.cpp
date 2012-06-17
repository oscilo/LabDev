#include "SelectiveMicrovoltmeter.h"

SelectiveMicrovoltmeter::SelectiveMicrovoltmeter(QWidget *parent, Qt::WFlags flags) :
	AbstractDevice(parent, flags)
{
	back = new QPixmap("./Resources/SelectiveMicrovoltmeter/back.png");

	this->neededInputs << "input_signal";
	
	QSize temp = back->size();
	temp.scale(300, 300, Qt::KeepAspectRatioByExpanding);
	this->setMinimumSize(temp);
}
SelectiveMicrovoltmeter::~SelectiveMicrovoltmeter() {
	delete back;
}
QString SelectiveMicrovoltmeter::getDeviceName() {
	return RUS("Микровольтметр селективный 86-9");
}
AbstractDevice::DeviceType SelectiveMicrovoltmeter::getDeviceType() {
	return AbstractDevice::DT_METER;
}
void SelectiveMicrovoltmeter::paintEvent(QPaintEvent *event) {
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
void SelectiveMicrovoltmeter::resizeEvent(QResizeEvent *event) {
}