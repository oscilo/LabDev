#ifndef LABCONNECTIONWIDGET_H
#define LABCONNECTIONWIDGET_H

#include "globals.h"
#include "structures.h"

class LabConnectionWidget : public QGroupBox {
	Q_OBJECT

public:
	LabConnectionWidget(QWidget *parent = 0);
	LabConnectionWidget(QList<DeviceInfo> existingDevices, QWidget *parent = 0);
	~LabConnectionWidget();

	bool isValid();
	DBRecord GetConnectionRecord();
	void FillExistingConnection(AbstractDevice::PlugingType, QUuid &emitterUuid, QUuid &receiverUuid, QString &emitterStr, QString &receiverStr);

signals:
	void CreateNewConnection();

public slots:
	void SelectedDevicesUpdated(const QList<DeviceInfo>&);

private slots:
	void TypeChanged(int);
	void ReceiverChanged(int);
	void EmitterChanged(int);

private:
	DeviceInfo FindDeviceByDeviceUuid(const QList<DeviceInfo>&, QUuid &);
	void SetBoxItemsByType(QComboBox*, AbstractDevice::DeviceType);
	void SetBoxItemsByDevice(QComboBox*, const QStringList&);
	void SetBoxCurrentItemByUuid(QComboBox*, const QUuid&);
	void SetBoxCurrentItemByString(QComboBox*, const QString&);
	void SetBoxCurrentItemByPlugingType(QComboBox*, AbstractDevice::PlugingType);
	
	QComboBox *typeBox;
	QComboBox *receiverBox;
	QComboBox *emitterBox;
	QComboBox *outputsBox;
	QComboBox *inputsBox;
	QLabel *signalsKeyLabel;

	QList<DeviceInfo> devices;
};

#endif