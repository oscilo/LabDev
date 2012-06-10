#include "LabFacilityButton.h"


LabFacilityButton::LabFacilityButton(QWidget *parent) :
	QToolButton(parent)
{
	this->setAutoRaise(true);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}
LabFacilityButton::~LabFacilityButton() {
}
void LabFacilityButton::SetLabFacility(LabFacility &lf) {
	labFacility = lf;
	this->setText(lf.l_name);
}

LabFacility LabFacilityButton::GetLabFacility() {
	return labFacility;
}