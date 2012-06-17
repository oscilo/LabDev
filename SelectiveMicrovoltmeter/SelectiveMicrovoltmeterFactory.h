#ifndef SELECTIVEMICROVOLTMETERFACTORY_H
#define SELECTIVEMICROVOLTMETERFACTORY_H

#include "SelectiveMicrovoltmeterGlobal.h"

#include <AbstractFactory.h>

#include "SelectiveMicrovoltmeter.h"

class SELECTIVEMICROVOLTMETER_EXPORT SelectiveMicrovoltmeterFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif