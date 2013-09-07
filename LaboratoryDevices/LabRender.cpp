#include "LabRender.h"

#include <windows.h>
#include <shellapi.h>
#include <string>

LabRender::LabRender(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	setMinimumSize(500, 200);
	setWindowTitle(RUS("Laboratory Workshop"));		//DS Выбор лабораторной установки

	centralWidget = new QMdiArea(this);
	centralWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	centralWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(centralWidget);

	devicesMapper = new QSignalMapper(this);
	connect(devicesMapper, SIGNAL(mapped(QWidget*)), this, SLOT(ShowHideDevice(QWidget *)));

	labMerger = new LabMerger;
	labMerger->moveToThread(labMerger);
	labMerger->start();

	connect(this, SIGNAL(RequestDevicesSignal(const LabFacility)), labMerger, SLOT(RequestDevicesSlot(const LabFacility)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(SendDevicesSignal(const QVector<AbstractDevice*>&)), this, SLOT(ReceiveDevicesSlot(const QVector<AbstractDevice*>&)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(CreateDeviceInstanceSignal(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*)),
		this, SLOT(CreateDeviceInstanceSlot(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*)), Qt::QueuedConnection);
	connect(labMerger, SIGNAL(ShowMessageBox(const QString&, const QString&, QMessageBox::Icon)),
		this, SLOT(ShowMessageBox(const QString&, const QString&, QMessageBox::Icon)));

	splash = new LoadingDllScreen(centralWidget);
	connect(labMerger, SIGNAL(ShowLoadMessage(const QString&, int, const QColor&)), splash, SLOT(showMessage(const QString&, int, const QColor&)));
	connect(labMerger, SIGNAL(SetSplashVisible(bool)), splash, SLOT(setVisible(bool)));

	CreateMenus();
}
LabRender::~LabRender()
{
	CloseLabFacility();

	if(labMerger->isRunning())
		labMerger->terminate();

	delete labMerger;
}
void LabRender::CreateMenus()
{
	labFacMenu = menuBar()->addMenu(RUS("Work"));			//DS
	
	QActionGroup *labFacActions = CreateLabActions();
	foreach(QAction *act, labFacActions->actions())
	{
		labFacMenu->addAction(act);
	}
	connect(labFacMenu, SIGNAL(triggered(QAction*)), this, SLOT(ChangeLabFacility(QAction*)));

	devicesMenu = menuBar()->addMenu(RUS("Device"));		//DS
	devicesMenu->setDisabled(true);
	
	CreateFooterDevicesAction();

	descrAction = menuBar()->addAction(RUS("Read me"));	//DS	Описание
	connect(descrAction, SIGNAL(triggered()), this, SLOT(ShowLabDescription()));
	descrAction->setDisabled(true);

	closeAction = menuBar()->addAction(RUS("Exit"));		//DS Выход
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	//DS
	aboutAction = menuBar()->addAction(RUS("About ..."));	
	//QAction *aboutAction = menuBar()->addAction(RUS(L"О программе"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(ShowAboutSlot()));

}

//DS
void LabRender::ShowAboutSlot() {
	QDialog msg;
	msg.setWindowTitle(RUS("About Programm"));

	QString message(RUS("Virtual Laboratory Workshop\n"
						"Version 1.0 \n"
						"\n"
						"			© 2013 Southern Federal University\n"
						"			E-mail: ds@sfedu.ru\n"));

	QGridLayout *lay = new QGridLayout(&msg);
	QLabel *text = new QLabel(message, &msg);

	QPushButton *okBut = new QPushButton(RUS("OK"), &msg);
	connect(okBut, SIGNAL(clicked()), &msg, SLOT(close()));
	
	lay->addWidget(text,	0, 0, 1, 2);
	lay->addWidget(okBut,	2, 1);
	lay->setColumnStretch(0, 1);

	msg.exec();
}


QActionGroup* LabRender::CreateLabActions()
{
	QActionGroup *ret = new QActionGroup(this);
	ret->setExclusive(true);

	QVector<LabFacility> lfacs = labMerger->GetLabList();

	foreach(LabFacility curLfac, lfacs)
	{
		QAction *act = new QAction(curLfac.l_name, this);
		act->setCheckable(true);
		act->setData(curLfac.l_id);
		ret->addAction(act);
	}

	return ret;
}
bool LabRender::IsLabFacilityOpen()
{
	bool ret = false;

	return ret;
}
void LabRender::ShowLabDescription()
{
	QString file = labMerger->GetDescriptionFileName(lfac);
	QFileInfo fi(file);

	QString filePath = "file:///" + fi.canonicalFilePath();

	QDesktopServices::openUrl(filePath);
}
void LabRender::CloseLabFacility()
{
	if(labMerger->IsDevicesLoaded())
		labMerger->RemoveDevices();

	centralWidget->closeAllSubWindows();

	devicesMenu->setDisabled(true);
	devicesMenu->clear();

	descrAction->setDisabled(true);
}
void LabRender::ChangeLabFacility(QAction *act)
{
	CloseLabFacility();
	
	devicesMenu->setDisabled(false);
	descrAction->setDisabled(false);

	lfac = LabFacility(act->data().toInt(), act->text());

	splash->clearMessage();
	splash->setGeometry( this->x() + (centralWidget->width()/2) - (splash->width()/2), this->y() + (centralWidget->height()/2) - (splash->height()/2), splash->width(), splash->height());

	setWindowTitle(lfac.l_name);

	emit RequestDevicesSignal(lfac);
}
void LabRender::ReceiveDevicesSlot(const QVector<AbstractDevice*> &devices)
{
	foreach(AbstractDevice *dev, devices)
	{	
		QString deviceName = dev->getDeviceName();

		QMdiSubWindow *subWindow = centralWidget->addSubWindow(dev,	Qt::Window |
																	Qt::WindowTitleHint |
																	Qt::WindowContextHelpButtonHint |
																	Qt::CustomizeWindowHint);
		dev->setWindowTitle(deviceName);
		dev->show();

		QAction *action = devicesMenu->addAction(deviceName);
		action->setCheckable(true);
		action->setChecked(dev->isVisible());
		connect(action, SIGNAL(triggered()), devicesMapper, SLOT(map()));
		devicesMapper->setMapping(action, dev);

		devicesMenu->addAction(action);
	}

	AddFooterDevicesAction();
}
void LabRender::ShowHideDevice(QWidget *device)
{
	QMdiSubWindow *subWindow = FindMdiSubWindow(device);
	subWindow->setVisible(!subWindow->isVisible());
}
QMdiSubWindow* LabRender::FindMdiSubWindow(QWidget *widget)
{
	QUuid uuid = qobject_cast<AbstractDevice*>(widget)->getUuid();

	foreach(QMdiSubWindow *window, centralWidget->subWindowList()) {
        AbstractDevice *device = qobject_cast<AbstractDevice*>(window->widget());

        if(device->getUuid() == uuid)
            return window;
    }

    return 0;
}
void LabRender::AddFooterDevicesAction()
{
	devicesMenu->addSeparator();
	devicesMenu->addAction(cascadeAction);
	devicesMenu->addAction(tileAction);
	devicesMenu->addSeparator();
	devicesMenu->addAction(tabbedAction);
}
void LabRender::CreateFooterDevicesAction()
{
	cascadeAction = new QAction(RUS("Cascade"), this);		//	DS
	connect(cascadeAction, SIGNAL(triggered()), centralWidget, SLOT(cascadeSubWindows()));
	
	tileAction = new QAction(RUS("Tile"), this);			//DS
	connect(tileAction, SIGNAL(triggered()), centralWidget, SLOT(tileSubWindows()));

	tabbedAction = new QAction(RUS("Laying"), this);		//DS
	tabbedAction->setCheckable(true);
	tabbedAction->setChecked(false);
	connect(tabbedAction, SIGNAL(triggered(bool)), this, SLOT(SetTabbedSubWindows(bool)));
}
void LabRender::SetTabbedSubWindows(bool isTabbed)
{
	if(isTabbed) {
		centralWidget->setViewMode(QMdiArea::TabbedView);
		cascadeAction->setDisabled(true);
		tileAction->setDisabled(true);
	}
	else {
		centralWidget->setViewMode(QMdiArea::SubWindowView);
		cascadeAction->setDisabled(false);
		tileAction->setDisabled(false);
	}
}
void LabRender::CreateDeviceInstanceSlot(DeviceInstance *device, AbstractFactory *factory, QMutex* mutex, QWaitCondition* waitCond) {
	mutex->lock();

	device->device = factory->CreateDevice(this);

	waitCond->wakeAll();
	mutex->unlock();
}
void LabRender::ShowMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon)
{
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
void LabRender::closeEvent(QCloseEvent *event) {
	if(splash->isVisible())
		event->ignore();
	else {
		int ret = QMessageBox::question(this, RUS("Exit"), RUS("Exit ?"), QMessageBox::Yes, QMessageBox::No);
		(QMessageBox::Yes == ret) ? event->accept() : event->ignore();
	}
}