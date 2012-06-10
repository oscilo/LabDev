#ifndef LABFACILITYAREA_H
#define LABFACILITYAREA_H

#include "globals.h"
#include "structures.h"

#include "LabConnectionWidget.h"

class LabFacilityArea : public QScrollArea {
	Q_OBJECT

public:
	LabFacilityArea(QWidget *parent = 0);
	~LabFacilityArea();

	DBRecordList GetDevicesConnections();

signals:
	void SelectedDevicesUpdated(const QList<DeviceInfo>&);

public slots:
	void SelectionUpdated(const QList<DeviceInfo>&);
	void AddConnection(AbstractDevice::PlugingType, QUuid &emitterUuid, QUuid &receiverUuid, QString &emitterStr, QString &receiverStr);

private slots:
	LabConnectionWidget* AddNewConnectionWidget();

private:
	QVBoxLayout *labFacLayout;

	LabConnectionWidget *createConnWidget;

	QList<DeviceInfo> devices;
};

#endif