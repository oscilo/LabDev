#include "LabMerger.h"

LabMerger::LabMerger(QObject *parent) :
	QThread(parent), db(0)
{
}
LabMerger::~LabMerger()
{
	RemoveDevices();
	CloseDataBase();
}
void LabMerger::run()
{
	exec();
}
void LabMerger::OpenDataBase()
{
	QMutexLocker locker(&mutex);
	
	if(!db)
	{
		db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
		db->setDatabaseName("./../DB/LaboratoryFacilities.db3");
	}

	if(!db->open())
		throw DeviceException("Error while open DataBase");
}
void LabMerger::CloseDataBase()
{
	QMutexLocker locker(&mutex);
	
	if(db->isOpen())
		db->close();
}
QSqlQuery LabMerger::ExecQuery(QString query)
{
	QMutexLocker locker(&mutex);
	
	QSqlQuery ret = db->exec(query);

	if(!ret.isActive())
		throw DeviceException("QSqlQuery is not active");

	return ret;
}
QVector<LabFacility> LabMerger::GetLabList()
{
	QVector<LabFacility> ret;
	
	OpenDataBase();

	QSqlQuery sq = ExecQuery("select * from lab_facilities;");

	QSqlRecord rec = sq.record();
	int lIdCol = rec.indexOf("l_id");
	int lNameCol = rec.indexOf("l_name");

	while(sq.next())
		ret << LabFacility(sq.value(lIdCol).toInt(), sq.value(lNameCol).toString());

	CloseDataBase();
	
	return ret;
}
QVector<DeviceInstance> LabMerger::GetDevicesInOrder(const LabFacility &lfac)
{
	QVector<DeviceInstance> ret;

	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("select * from order_list_dev where l_id = %1 order by o_order;").arg(lfac.l_id));

	QSqlRecord rec = sq.record();
	int dIdCol = rec.indexOf("d_id");
	int oOrderCol = rec.indexOf("o_order");

	while(sq.next())
		ret << DeviceInstance(sq.value(dIdCol).toInt(), sq.value(oOrderCol).toInt(), 0);

	CloseDataBase();

	return ret;
}
QVector<DeviceInstance> LabMerger::GetAllDevices() {
	QVector<DeviceInstance> ret;

	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("select * from devices;"));

	QSqlRecord rec = sq.record();
	int dIdCol = rec.indexOf("d_id");

	while(sq.next())
		ret << DeviceInstance(sq.value(dIdCol).toInt(), -1, 0);

	CloseDataBase();

	return ret;
}
QMap<int, QString> LabMerger::GetDeviceMap()
{
	QMap<int, QString> ret;

	OpenDataBase();

	QSqlQuery sq = ExecQuery("select * from devices;");

	QSqlRecord rec = sq.record();
	int dIdCol = rec.indexOf("d_id");
	int dFnameCol = rec.indexOf("d_fname");

	while(sq.next())
		ret[sq.value(dIdCol).toInt()] = sq.value(dFnameCol).toString();

	CloseDataBase();

	return ret;
}
void LabMerger::SetDeviceInstanses(QVector<DeviceInstance> &devices)
{
	QMap<int, QString> devMap = GetDeviceMap();

	emit SetSplashVisible(true);

	for(int i = 0; i < devices.size(); ++i) {
		DeviceInstance &device(devices[i]);
		QString &devDllName(devMap[device.d_id]);

		emit ShowLoadMessage(RUS("Loading %1...").arg(devDllName));		//DS
		
		QPluginLoader *pLoader = new QPluginLoader(devDllName, this);
		
		if(!pLoader->load())
			throw DeviceException("Plugin was NOT loaded!");

		QMutex mutex;
		QWaitCondition waitCond;

		mutex.lock();
		emit CreateDeviceInstanceSignal(&device, qobject_cast<AbstractFactory*>(pLoader->instance()), &mutex, &waitCond);
		
		waitCond.wait(&mutex);
		mutex.unlock();

		if(!device.device)
			throw DeviceException("Device was NOT instanteated!");

		loadedPlugins << pLoader;
	}

	emit SetSplashVisible(false);
}
AbstractDevice* LabMerger::FindDeviceInstanceByOrder(const QVector<DeviceInstance> &devices, int o_order)
{
	AbstractDevice *ret = 0;
	
	foreach(DeviceInstance device, devices)	{
		if(device.o_order == o_order)	{
			ret = device.device;
			break;
		}
	}

	if(ret == 0)
		throw DeviceException("Device was not found by order");
	
	return ret;
}
void LabMerger::CreateDeviceConnections(QVector<DeviceInstance> &devices, const LabFacility &lfac)
{
	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("select * from connections where l_id = %1;").arg(lfac.l_id));

	QSqlRecord rec = sq.record();
	int cTypeCol = rec.indexOf("c_type");
	int cEmitterOrderCol = rec.indexOf("c_emitter_order");
	int cReceiverOrderCol = rec.indexOf("c_receiver_order");
	int cOutkeyCol = rec.indexOf("c_outkey");
	int cInpkeyCol = rec.indexOf("c_inpkey");

	while(sq.next()) {
		int pType = sq.value(cTypeCol).toInt();
		AbstractDevice *emitter = FindDeviceInstanceByOrder(devices, sq.value(cEmitterOrderCol).toInt());
		AbstractDevice *receiver = FindDeviceInstanceByOrder(devices, sq.value(cReceiverOrderCol).toInt());
		QString outkey = sq.value(cOutkeyCol).toString();
		QString inpkey = sq.value(cInpkeyCol).toString();

		switch(pType) {
			case AbstractDevice::PT_COMMON:
				AbstractDevice::plugin(emitter, outkey, receiver, inpkey);
				break;

			case AbstractDevice::PT_SFG_INDIC:
				(qobject_cast<AbstractSFGenerator*>(emitter))->plugin(qobject_cast<AbstractIndicator*>(receiver));
				break;

			default:
				break;
		}
	}

	CloseDataBase();
}
void LabMerger::InitDeviceProperties(QVector<DeviceInstance> &devices)
{
	foreach(DeviceInstance device, devices)
	{
		device.device->initProperties();
	}
}
QVector<AbstractDevice*> LabMerger::ObtainInstanceList(const QVector<DeviceInstance> &devices)
{
	QVector<AbstractDevice*> ret;

	foreach(DeviceInstance device, devices) {
		ret << device.device;
	}

	return ret;
}
void LabMerger::RequestDevicesSlot(const LabFacility lfac) {
	try {
		QVector<AbstractDevice*> &ret(loadedDevices);

		QVector<DeviceInstance> devices = GetDevicesInOrder(lfac);
		SetDeviceInstanses(devices);
		CreateDeviceConnections(devices, lfac);
		InitDeviceProperties(devices);
		ret = ObtainInstanceList(devices);

		emit SendDevicesSignal(ret);
	}
	catch(DeviceException &e) {
		emit ShowMessageBox(RUS("Error!"), e.getMess(), QMessageBox::Critical);		//DS
	}
}
void LabMerger::RequestAllDevicesSlot() {
	try {
		RegisterNewDevices();

		QVector<AbstractDevice*> &ret(loadedDevices);

		QVector<DeviceInstance> devices = GetAllDevices();
		SetDeviceInstanses(devices);
		ret = ObtainInstanceList(devices);
		
		emit SendDevicesSignal(devices);
	}
	catch(DeviceException &e) {
		emit ShowMessageBox(RUS("Error!"), e.getMess(), QMessageBox::Critical);		//DS
	}
}
void LabMerger::RemoveDevices()
{
	foreach(AbstractDevice* device, loadedDevices) {
		device->unplug();
		delete device;
	}

	foreach(QPluginLoader* plugin, loadedPlugins) {
		plugin->unload();
	}

	loadedDevices.clear();
	loadedPlugins.clear();
}
bool LabMerger::IsDevicesLoaded()
{
	return loadedDevices.size();
}
QString LabMerger::GetDescriptionFileName(const LabFacility lfac)
{
	QString ret;
	
	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("select * from manuals where l_id = %1;").arg(lfac.l_id));

	QSqlRecord rec = sq.record();
	int mFnameCol = rec.indexOf("m_fname");

	if(sq.next())
		ret  = sq.value(mFnameCol).toString();

	CloseDataBase();

	return ret;
}
void LabMerger::RegisterNewDevices() {
	QFileInfoList fiList = QDir::current().entryInfoList(QStringList() << "*.dll", QDir::Files);
	QMap<int, QString> devMap = GetDeviceMap();
	
	foreach(QFileInfo fi, fiList) {
		QString devDllName(fi.fileName());

		if(-1 == devMap.key(devDllName, -1)) {
			QPluginLoader pLoader(devDllName, this);
			
			if(!pLoader.load())
				continue;
			if( !(qobject_cast<AbstractFactory*>(pLoader.instance())) )
				continue;

			RegisterDll(fi.fileName());

			pLoader.unload();
		}
	}
}
void LabMerger::RegisterDll(QString &name) {
	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("insert into devices (d_fname) values (\"%1\");").arg(name));

	CloseDataBase();
}
void LabMerger::DeleteLabFacility(const LabFacility lf) {
	OpenDataBase();

	CleanDBFromLabFacility(lf);

	CloseDataBase();
}
void LabMerger::CleanDBFromLabFacility(const LabFacility &lf) {
	ExecQuery(QString("delete from lab_facilities where l_id = %1;").arg(lf.l_id));
	ExecQuery(QString("delete from manuals where l_id = %1;").arg(lf.l_id));
	ExecQuery(QString("delete from order_list_dev where l_id = %1;").arg(lf.l_id));
	ExecQuery(QString("delete from connections where l_id = %1;").arg(lf.l_id));
}
void LabMerger::CreateNewLabFacility(QString &labNameStr, QString &manualPathStr, DBRecordList &devicesInOrder, DBRecordList &devicesConnections) {
	OpenDataBase();

	int l_id = InsertLabFacility(labNameStr);
	InsertManual(l_id, manualPathStr);
	InsertDevicesInOrder(l_id, devicesInOrder);
	InsertConnections(l_id, devicesConnections);
	
	CloseDataBase();
}
int LabMerger::InsertLabFacility(QString &labNameStr) {
	int l_id;

	QSqlQuery sq = ExecQuery(QString("insert into lab_facilities (l_name) values (\"%1\");").arg(labNameStr));
	sq = ExecQuery("select last_insert_rowid();");

	QSqlRecord rec = sq.record();
	int lastIndexInserted = rec.indexOf("last_insert_rowid()");

	if(sq.next())
		l_id  = sq.value(lastIndexInserted).toInt();

	return l_id;
}
void LabMerger::InsertManual(int l_id, QString &manualPathStr) {
	QDir dir(QDir::currentPath());
	QString manualFilePath = dir.relativeFilePath(manualPathStr);

	ExecQuery(QString("insert into manuals (l_id, m_fname) values (\"%1\", \"%2\");").arg(l_id).arg(manualFilePath));
}
void LabMerger::InsertDevicesInOrder(int l_id, DBRecordList &devicesInOrder) {
	foreach(DBRecord record, devicesInOrder) {
		ExecQuery(QString("insert into order_list_dev (l_id, d_id, o_order) values (\"%1\", \"%2\", \"%3\");").arg(l_id).arg(record["d_id"].toInt()).arg(record["o_order"].toInt()));
	}
}
void LabMerger::InsertConnections(int l_id, DBRecordList &devicesConnections) {
	foreach(DBRecord record, devicesConnections) {
		ExecQuery(QString("insert into connections (l_id, c_type, c_emitter_order, c_receiver_order, c_outkey, c_inpkey) values (\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\");")
																						.arg(l_id)
																						.arg(record["c_type"].toInt())
																						.arg(record["c_emitter_order"].toInt())
																						.arg(record["c_receiver_order"].toInt())
																						.arg(record.value("c_outkey", "").toString())
																						.arg(record.value("c_inpkey", "").toString()));
	}
}
DBRecordList LabMerger::GetDevicesDBRecordListInOrder(const LabFacility &lf) {
	DBRecordList ret;

	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("select * from order_list_dev where l_id = %1 order by o_order;").arg(lf.l_id));

	QSqlRecord rec = sq.record();
	int dIdCol = rec.indexOf("d_id");
	int oOrderCol = rec.indexOf("o_order");

	while(sq.next()) {
		DBRecord record;
		record["d_id"] = sq.value(dIdCol);
		record["o_order"] = sq.value(oOrderCol);
		ret << record;
	}

	CloseDataBase();

	return ret;
}
DBRecordList LabMerger::GetDevicesConnections(const LabFacility &lf) {
	DBRecordList ret;
	
	OpenDataBase();

	QSqlQuery sq = ExecQuery(QString("select * from connections where l_id = %1;").arg(lf.l_id));

	QSqlRecord rec = sq.record();
	int cTypeCol = rec.indexOf("c_type");
	int cEmitterOrderCol = rec.indexOf("c_emitter_order");
	int cReceiverOrderCol = rec.indexOf("c_receiver_order");
	int cOutkeyCol = rec.indexOf("c_outkey");
	int cInpkeyCol = rec.indexOf("c_inpkey");

	while(sq.next()) {
		DBRecord record;
		record["c_type"] = sq.value(cTypeCol);
		record["c_emitter_order"] = sq.value(cEmitterOrderCol);
		record["c_receiver_order"] = sq.value(cReceiverOrderCol);
		record["c_outkey"] = sq.value(cOutkeyCol);
		record["c_inpkey"] = sq.value(cInpkeyCol);
		ret << record;
	}

	CloseDataBase();

	return ret;
}
void LabMerger::EraseDB() {
	OpenDataBase();

	ExecQuery("delete from connections;");
	ExecQuery("delete from sqlite_sequence where name='connections';");
	ExecQuery("delete from devices;");
	ExecQuery("delete from sqlite_sequence where name='devices';");
	ExecQuery("delete from lab_facilities;");
	ExecQuery("delete from sqlite_sequence where name='lab_facilities';");
	ExecQuery("delete from manuals;");
	ExecQuery("delete from sqlite_sequence where name='manuals';");
	ExecQuery("delete from order_list_dev;");
	ExecQuery("delete from sqlite_sequence where name='order_list_dev';");

	CloseDataBase();
}