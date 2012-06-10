#ifndef LABFACILITYBUTTON_H
#define LABFACILITYBUTTON_H

#include "globals.h"
#include "LabMerger.h"

class LabFacilityButton : public QToolButton {
	Q_OBJECT

public:
	LabFacilityButton(QWidget *parent = 0);
	~LabFacilityButton();

	void SetLabFacility(LabFacility &lf);
	LabFacility GetLabFacility();

private:
	LabFacility labFacility;
};

#endif