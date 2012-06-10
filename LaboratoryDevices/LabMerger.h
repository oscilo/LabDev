#ifndef LAB_MERGER_H
#define LAB_MERGER_H

#include "globals.h"

#include <AbstractDevice.h>
#include <AbstractFactory.h>
#include <AbstractSFGenerator.h>
#include <AbstractIndicator.h>
#include <DeviceException.h>

struct LabFacility
{
	LabFacility(int l_id_, QString l_name_) :
		l_id(l_id_), l_name(l_name_) {}

	LabFacility() {}

	int l_id;
	QString l_name;
};
struct DeviceInstance
{
	DeviceInstance() {}
	DeviceInstance(int d_id_, int o_order_, AbstractDevice* device_) :
		d_id(d_id_), o_order(o_order_), device(device_) {}

	int d_id;
	int o_order;
	AbstractDevice* device;
};

class LabMerger : public QThread
{
	Q_OBJECT

public:
	LabMerger(QObject *parent = 0);
	~LabMerger();

	void run();

	QVector<LabFacility> GetLabList();
	bool IsDevicesLoaded();
	QString GetDescriptionFileName(const LabFacility);
	void DeleteLabFacility(const LabFacility);
	void CreateNewLabFacility(QString &labNameStr, QString &manualPathStr, DBRecordList &devicesInOrder, DBRecordList &devicesConnections);
	DBRecordList GetDevicesDBRecordListInOrder(const LabFacility&);
	DBRecordList GetDevicesConnections(const LabFacility&);
	void EraseDB();

signals:
	void SendDevicesSignal(const QVector<DeviceInstance>&);
	void SendDevicesSignal(const QVector<AbstractDevice*>&);
	void ShowLoadMessage(const QString&, int = Qt::AlignCenter, const QColor& = Qt::black);
	void CreateDeviceInstanceSignal(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*);
	void ShowMessageBox(const QString&, const QString&, QMessageBox::Icon);
	void SetSplashVisible(bool);
	void PrepareSplash();

public slots:
	void RequestDevicesSlot(const LabFacility);
	void RequestAllDevicesSlot();
	void RemoveDevices();

private:
	void OpenDataBase();
	void CloseDataBase();
	QSqlQuery ExecQuery(QString);

	void SetDeviceInstanses(QVector<DeviceInstance>&);
	QVector<DeviceInstance> GetDevicesInOrder(const LabFacility&);
	void CreateDeviceConnections(QVector<DeviceInstance>&, const LabFacility&);
	QVector<AbstractDevice*> ObtainInstanceList(const QVector<DeviceInstance>&);
	QMap<int, QString> GetDeviceMap();
	AbstractDevice *FindDeviceInstanceByOrder(const QVector<DeviceInstance>&, int);
	void InitDeviceProperties(QVector<DeviceInstance>&);
	QVector<DeviceInstance> GetAllDevices();
	void RegisterNewDevices();
	void RegisterDll(QString &name);

	int InsertLabFacility(QString&);
	void InsertManual(int, QString&);
	void InsertDevicesInOrder(int, DBRecordList&);
	void InsertConnections(int, DBRecordList&);
	void CleanDBFromLabFacility(const LabFacility&);

	QSqlDatabase *db;
	QVector<QPluginLoader*> loadedPlugins;
	QVector<AbstractDevice*> loadedDevices;

	QMutex mutex;
};

#endif