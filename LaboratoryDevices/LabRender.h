#ifndef LAB_RENDER_H
#define LAB_RENDER_H

#include "globals.h"

#include "LabMerger.h"
#include "LoadingDllScreen.h"

class LabRender : public QMainWindow
{
	Q_OBJECT

public:
	LabRender(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~LabRender();

signals:
	void RequestDevicesSignal(const LabFacility);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void ChangeLabFacility(QAction*);
	void ShowLabDescription();
	void ShowHideDevice(QWidget*);
	void SetTabbedSubWindows(bool);
	void ReceiveDevicesSlot(const QVector<AbstractDevice*>&);
	void CreateDeviceInstanceSlot(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*);
	void ShowMessageBox(const QString&, const QString&, QMessageBox::Icon);

	//DS
	void ShowAboutSlot();

private:
	void CreateMenus();
	void CreateFooterDevicesAction();
	void AddFooterDevicesAction();
	QActionGroup* CreateLabActions();
	QMdiSubWindow* FindMdiSubWindow(QWidget*);

	bool IsLabFacilityOpen();
	void CloseLabFacility();

	QMenu *labFacMenu;
	QMenu *devicesMenu;
	QAction *descrAction;
	QAction *closeAction;

	//DS
	QAction *aboutAction;
	
	QAction *tileAction;
	QAction *cascadeAction;
	QAction *tabbedAction;
	
	LabFacility lfac;
	LabMerger *labMerger;
	LoadingDllScreen *splash;

	QMdiArea *centralWidget;
	QSignalMapper *devicesMapper;
};

#endif
