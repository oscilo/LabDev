#include "microstripresonator.h"

MicrostripResonator::MicrostripResonator(QWidget *parent, Qt::WFlags flags) :
	AbstractBlackBox(parent, flags)
{
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", new MicrostripResonatorFunc);
}
QString MicrostripResonator::getDeviceName() {
	return RUS("Микрополосковый резонатор");
}