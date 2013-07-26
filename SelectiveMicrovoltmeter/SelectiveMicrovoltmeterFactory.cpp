#include "SelectiveMicrovoltmeterFactory.h"

AbstractDevice* SelectiveMicrovoltmeterFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags)
{
	return new SelectiveMicrovoltmeter(parent, flags);
}