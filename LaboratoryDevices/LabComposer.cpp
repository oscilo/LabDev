#include "LabComposer.h"

#define NEW_FACILITY_BUTTON_ID		-1
#define ERASE_DATA_BASE_BUTTON_ID	-2

LabComposer::LabComposer(QWidget *parent, Qt::WindowFlags flags) :
	QMainWindow(parent, flags), labFacArea(0)
{
	setMinimumSize(600, 600);
	setWindowTitle(RUS("Управление лабораторными работами"));

	QWidget *centralWidget = new QWidget(this);
	centralLayout = new QVBoxLayout(centralWidget);

	CreateTopWidget();
	CreateSelectLabArea();
	CreateFooterWidget();

	setCentralWidget(centralWidget);

	labMerger = new LabMerger;
	labMerger->moveToThread(labMerger);
	labMerger->start();

	connect(this, SIGNAL(RemoveDevices()),
		labMerger, SLOT(RemoveDevices()), Qt::DirectConnection);
	connect(this, SIGNAL(RequestAllDevicesSignal()),
		labMerger, SLOT(RequestAllDevicesSlot()), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(SendDevicesSignal(const QVector<DeviceInstance>&)),
		this, SLOT(ReceiveDevicesSlot(const QVector<DeviceInstance>&)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(CreateDeviceInstanceSignal(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*)),
		this, SLOT(CreateDeviceInstanceSlot(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(ShowMessageBox(const QString&, const QString&, QMessageBox::Icon)),
		this, SLOT(ShowMessageBox(const QString&, const QString&, QMessageBox::Icon)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(PrepareSplash()), 
		this, SLOT(PrepareSplash()), Qt::QueuedConnection);

	splash = new LoadingDllScreen(this);
	connect(labMerger, SIGNAL(ShowLoadMessage(const QString&, int, const QColor&)), splash, SLOT(showMessage(const QString&, int, const QColor&)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(ShowLoadMessage(const QString&, int, const QColor&)), this, SLOT(UpdateSplashGeometry()), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(SetSplashVisible(bool)), splash, SLOT(setVisible(bool)), Qt::QueuedConnection);

	labFacMapper = new QSignalMapper(this);
	connect(labFacMapper, SIGNAL(mapped(QWidget*)), this, SLOT(LabFacilitySelected(QWidget*)));

	init();
}
LabComposer::~LabComposer() {
	splash->close();

	if(labMerger->isRunning())
		labMerger->terminate();

	delete labMerger;
}
void LabComposer::CreateSelectLabArea() {
	QWidget *widgetForArea = new QWidget(this);
	selectLabLayout = new QVBoxLayout(widgetForArea);

	selectLabArea = new QScrollArea(this);
	selectLabArea->setWidgetResizable(true);
	selectLabArea->setWidget(widgetForArea);
	selectLabArea->hide();

	centralLayout->addWidget(selectLabArea);
}
void LabComposer::CreateLabFacilityArea() {
	labFacArea = new LabFacilityArea(this);
	labFacArea->hide();
	
	connect(selectDevices, SIGNAL(SelectionUpdated(const QList<DeviceInfo>&)), labFacArea, SLOT(SelectionUpdated(const QList<DeviceInfo>&)));

	centralLayout->insertWidget(centralLayout->indexOf(footerWidget), labFacArea);
}
void LabComposer::CreateTopWidget() {
	topWidget = new QWidget(this);
	QGridLayout *topLayout = new QGridLayout(topWidget);
	topWidget->hide();

	labName = new QLineEdit(this);

	descrPath = new QLineEdit(this);
	descrPath->setReadOnly(true);

	QPushButton *selectDescr = new QPushButton(RUS("Открыть"), this);
	connect(selectDescr, SIGNAL(clicked()), this, SLOT(SelectManual()));

	selectDevices = new SelectDevicesWidget(this);

	topLayout->addWidget(new QLabel(RUS("Название:"), this),	0, 0);
	topLayout->addWidget(labName,								0, 1, 1, 2);
	topLayout->addWidget(new QLabel(RUS("Описание:"), this),	1, 0);
	topLayout->addWidget(descrPath,								1, 1);
	topLayout->addWidget(selectDescr,							1, 2);
	topLayout->addWidget(selectDevices,							2, 0, 1, 3);

	centralLayout->addWidget(topWidget);
}
void LabComposer::CreateFooterWidget() {
	footerWidget = new QWidget(this);
	QHBoxLayout *footerLayout = new QHBoxLayout(footerWidget);
	footerWidget->hide();
	
	QPushButton *deleteButton = new QPushButton(RUS("Удалить работу"), this);
	QPushButton *saveButton = new QPushButton(RUS("Сохранить"), this);
	QPushButton *backButton = new QPushButton(RUS("Назад"), this);
	
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(DeleteLab()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(SaveLab()));
	connect(backButton, SIGNAL(clicked()), this, SLOT(MoveBack()));
	
	footerLayout->addWidget(deleteButton);
	footerLayout->addStretch();
	footerLayout->addWidget(saveButton);
	footerLayout->addWidget(backButton);

	centralLayout->addWidget(footerWidget);
}
void LabComposer::init() {
	emit RequestAllDevicesSignal();
}
void LabComposer::LabFacilitySelected(QWidget *w) {
	LabFacilityButton *but = qobject_cast<LabFacilityButton*>(w);

	if(!but)
		return;

	lfac = but->GetLabFacility();

	if(ERASE_DATA_BASE_BUTTON_ID == lfac.l_id) {

		QMessageBox msgBox;
		
		msgBox.setWindowTitle(RUS("Очистка"));
		msgBox.setText(RUS("Вы действительно хотите удалить ВСЕ записи из базы данных?"));
		QPushButton *yesButton = msgBox.addButton(RUS("Да"), QMessageBox::YesRole);
		msgBox.addButton(RUS("Нет"), QMessageBox::NoRole);
		msgBox.setIcon(QMessageBox::Question);
		msgBox.setMinimumWidth(400);

		msgBox.exec();

		if(yesButton != msgBox.clickedButton())
			return;
		
		labMerger->EraseDB();
		close();
	}

	CreateLabFacilityArea();
	ShowLabComposition();

	if(NEW_FACILITY_BUTTON_ID != lfac.l_id)
		FillSelectedLab();
}
void LabComposer::PrepareSplash() {
	splash->clearMessage();
	UpdateSplashGeometry();
}
void LabComposer::UpdateSplashGeometry() {
	QWidget *centralWidget = this->centralWidget();
	splash->setGeometry(this->x() + (centralWidget->width()/2) - (splash->width()/2), this->y() + (centralWidget->height()/2) - (splash->height()/2), splash->width(), splash->height());
}
void LabComposer::ReceiveDevicesSlot(const QVector<DeviceInstance> &devices) {
	foreach(DeviceInstance device, devices) {
		DeviceInfo di;
		di.id = device.d_id;
		di.name = device.device->getDeviceName();
		di.type = device.device->getDeviceType();
		di.inputs = device.device->getAllowedInputs();
		di.outputs = device.device->getSignalsKeys(AbstractDevice::SD_OUTPUT_SIGNAL);

		existingDevices << di;
	}

	emit RemoveDevices();

	selectDevices->SetDevicesList(existingDevices);

	FillSelectLabArea();
	ShowLabList();
}
void LabComposer::CreateDeviceInstanceSlot(DeviceInstance *device, AbstractFactory *factory, QMutex *mutex, QWaitCondition *waitCond) {
	mutex->lock();

	device->device = factory->CreateDevice(this);

	waitCond->wakeAll();
	mutex->unlock();
}
void LabComposer::ShowMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon) {
	if(splash->isVisible())
		splash->hide();

	QMessageBox msgBox;
	
	msgBox.setWindowTitle(title);
	msgBox.setText(message);
	msgBox.addButton(RUS("Ок"), QMessageBox::YesRole);
	msgBox.setIcon(icon);
	msgBox.setMinimumWidth(400);

	msgBox.exec();
}
void LabComposer::CreateLabFacilityButton(LabFacility &lf) {
	LabFacilityButton *but = new LabFacilityButton(this);
	but->SetLabFacility(lf);

	selectLabLayout->addWidget(but);
	connect(but, SIGNAL(clicked()), labFacMapper, SLOT(map()));
	labFacMapper->setMapping(but, but);
}
void LabComposer::FillSelectLabArea() {
	foreach(LabFacility lf, labMerger->GetLabList()) {
		CreateLabFacilityButton(lf);
	}

	CreateLabFacilityButton(LabFacility(NEW_FACILITY_BUTTON_ID, RUS("Новая работа")));

	selectLabLayout->addStretch();

	CreateLabFacilityButton(LabFacility(ERASE_DATA_BASE_BUTTON_ID, RUS("Очистить базу данных")));
}
void LabComposer::ShowLabList() {
	if(labFacArea)
		labFacArea->hide();
	topWidget->hide();
	footerWidget->hide();

	selectLabArea->show();
}
void LabComposer::ShowLabComposition() {
	if(labFacArea)
		labFacArea->show();
	topWidget->show();
	footerWidget->show();

	selectLabArea->hide();
}
void LabComposer::FillSelectedLab() {
	QString labNameStr = lfac.l_name;
	QString manualPathStr = labMerger->GetDescriptionFileName(lfac);

	DBRecordList devicesInOrder = labMerger->GetDevicesDBRecordListInOrder(lfac);
	DBRecordList devicesConnections = labMerger->GetDevicesConnections(lfac);

	QList<DeviceInfo> selectedDevices = CreateSelectedDevices(devicesInOrder);

	labName->setText(labNameStr);
	descrPath->setText(QFileInfo(manualPathStr).canonicalFilePath());

	selectDevices->SetExistingSelection(selectedDevices);
	AddExistingConnWidgets(devicesConnections, selectedDevices);
}
void LabComposer::AddExistingConnWidgets(DBRecordList &devicesConnections, QList<DeviceInfo> &selectedDevices) {
	foreach(DBRecord record, devicesConnections) {
		AbstractDevice::PlugingType c_type = (AbstractDevice::PlugingType)(record["c_type"].toInt());
		QUuid emitterUuid = selectedDevices[record["c_emitter_order"].toInt()].uuid;
		QUuid receiverUuid = selectedDevices[record["c_receiver_order"].toInt()].uuid;
		QString emitterStr = record["c_outkey"].toString();
		QString receiverStr = record["c_inpkey"].toString();

		labFacArea->AddConnection(c_type, emitterUuid, receiverUuid, emitterStr, receiverStr);
	}
}
QList<DeviceInfo> LabComposer::CreateSelectedDevices(DBRecordList &devicesInOrder) {
	QList<DeviceInfo> ret;

	foreach(DBRecord record, devicesInOrder) {
		DeviceInfo di = FindDeviceInfoById(existingDevices, record["d_id"].toInt());
		di.order = record["o_order"].toInt();
		di.uuid = QUuid::createUuid();

		ret << di;
	}

	return ret;
}
DeviceInfo LabComposer::FindDeviceInfoById(QList<DeviceInfo> &whereToFind, int d_id) {
	for(int i = 0; i < whereToFind.size(); ++i) {
		DeviceInfo &di(whereToFind[i]);

		if(di.id == d_id)
			return di;
	}

	return DeviceInfo();
}
void LabComposer::ClearSelectLabArea() {
	while(selectLabLayout->count()) {
		QLayoutItem *item = selectLabLayout->itemAt(0);

		if(item->widget())
			delete item->widget();
		else if(item->layout())
			delete item->layout();
		else
			selectLabLayout->removeItem(item);
	}
}
void LabComposer::ClearTopWidget() {
	labName->setText("");
	descrPath->setText("");
	selectDevices->Clear();
}
void LabComposer::MoveBack() {
	ShowLabList();
	DeleteLabFacArea();

	ClearTopWidget();

	ClearSelectLabArea();
	FillSelectLabArea();
}
void LabComposer::SaveLab() {
	if(-1 != lfac.l_id)
		labMerger->DeleteLabFacility(lfac);

	QString labNameStr = labName->text();
	QString manualPathStr = descrPath->text();

	DBRecordList devicesInOrder = selectDevices->GetSelectedDevices();
	DBRecordList devicesConnections = labFacArea->GetDevicesConnections();

	if(labNameStr == "") {
		ShowMessageBox(RUS("Создание"), RUS("Не указано название работы!"), QMessageBox::Critical);
		return;
	}
	if(manualPathStr == "") {
		ShowMessageBox(RUS("Создание"), RUS("Не выбрано описание работы!"), QMessageBox::Critical);
		return;
	}
	if(!devicesInOrder.size()) {
		ShowMessageBox(RUS("Создание"), RUS("Не выбрано ни одного устройства!"), QMessageBox::Critical);
		return;
	}
	if(!devicesConnections.size()) {
		ShowMessageBox(RUS("Создание"), RUS("Нет ни одного соединения между устройствами!"), QMessageBox::Critical);
		return;
	}

	labMerger->CreateNewLabFacility(labNameStr, manualPathStr, devicesInOrder, devicesConnections);

	ShowMessageBox(RUS("Создание"), RUS("Лабораторная работа \"%1\" создана.").arg(labNameStr), QMessageBox::Information);
	MoveBack();
}
void LabComposer::DeleteLab() {
	if(-1 == lfac.l_id)
		return;

	QMessageBox msgBox;
	
	msgBox.setWindowTitle(RUS("Удаление"));
	msgBox.setText(RUS("Вы действительно хотите удалить лабораторную работу\n\"%1\"").arg(lfac.l_name));
	QPushButton *yesButton = msgBox.addButton(RUS("Да"), QMessageBox::YesRole);
	msgBox.addButton(RUS("Нет"), QMessageBox::NoRole);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setMinimumWidth(400);

	msgBox.exec();

	if(yesButton != msgBox.clickedButton())
		return;

	labMerger->DeleteLabFacility(lfac);

	ShowMessageBox(RUS("Удаление"), RUS("Лабораторная работа \"%1\" удалена.").arg(lfac.l_name), QMessageBox::Information);
	MoveBack();
}
void LabComposer::SelectManual() {
	QDir manualDir(QDir::currentPath() + "/../Documents/");
	
	QString mName = QFileDialog::getOpenFileName(	this,
													RUS("Выберите описание лабораторной работы"),
													manualDir.canonicalPath(),
													RUS("Документы (*.doc *.docx *.pdf *.djvu)") );

	descrPath->setText(mName);
}
void LabComposer::DeleteLabFacArea() {
	delete labFacArea;
	labFacArea = 0;
}
void LabComposer::closeEvent(QCloseEvent *event) {
	if(splash->isVisible())
		event->ignore();
	else
		event->accept();
}