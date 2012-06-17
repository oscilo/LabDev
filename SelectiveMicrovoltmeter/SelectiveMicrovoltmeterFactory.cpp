#include "SelectiveMicrovoltmeterFactory.h"

AbstractDevice* SelectiveMicrovoltmeterFactory::CreateDevice(QWidget *parent, Qt::WFlags flags)
{
	return new SelectiveMicrovoltmeter(parent, flags);
}

Q_EXPORT_PLUGIN2(SelectiveMicrovoltmeterFactory, SelectiveMicrovoltmeterFactory);