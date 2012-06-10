#include "LabFacilityArea.h"

LabFacilityArea::LabFacilityArea(QWidget *parent) :
	QScrollArea(parent)
{
	QWidget *widgetForArea = new QWidget(this);
	labFacLayout = new QVBoxLayout(widgetForArea);

	createConnWidget = new LabConnectionWidget(this);
	connect(createConnWidget, SIGNAL(CreateNewConnection()), this, SLOT(AddNewConnectionWidget()));

	labFacLayout->addWidget(createConnWidget);
	labFacLayout->addStretch();
	
	this->setWidgetResizable(true);
	this->setWidget(widgetForArea);
}
LabFacilityArea::~LabFacilityArea() {
}
LabConnectionWidget* LabFacilityArea::AddNewConnectionWidget() {
	LabConnectionWidget *conn = new LabConnectionWidget(devices, this);
	connect(this, SIGNAL(SelectedDevicesUpdated(const QList<DeviceInfo>&)), conn, SLOT(SelectedDevicesUpdated(const QList<DeviceInfo>&)));

	int createConnWidgIndex = labFacLayout->indexOf(createConnWidget);
	labFacLayout->insertWidget(createConnWidgIndex, conn);

	return conn;
}
void LabFacilityArea::SelectionUpdated(const QList<DeviceInfo> &devList) {
	devices = devList;
	
	emit SelectedDevicesUpdated(devices);
}
DBRecordList LabFacilityArea::GetDevicesConnections() {
	DBRecordList ret;

	for(int i = 0; i < labFacLayout->count(); ++i) {
		QWidget *w = labFacLayout->itemAt(i)->widget();

		if(!w)
			continue;

		LabConnectionWidget *conn = qobject_cast<LabConnectionWidget*>(w);

		if(!conn)
			continue;

		if(conn->isValid())
			ret << conn->GetConnectionRecord();
	}

	return ret;
}
void LabFacilityArea::AddConnection(AbstractDevice::PlugingType c_type, QUuid &emitterUuid, QUuid &receiverUuid, QString &emitterStr, QString &receiverStr) {
	LabConnectionWidget *conn = AddNewConnectionWidget();
	conn->FillExistingConnection(c_type, emitterUuid, receiverUuid, emitterStr, receiverStr);
}