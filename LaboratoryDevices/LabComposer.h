#ifndef LABCOMPOSER_H
#define LABCOMPOSER_H

#include "globals.h"
#include "structures.h"

#include "LabMerger.h"
#include "LoadingDllScreen.h"
#include "LabFacilityButton.h"
#include "LabFacilityArea.h"
#include "SelectDevicesWidget.h"

class LabComposer : public QMainWindow {
	Q_OBJECT

public:
	LabComposer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LabComposer();

signals:
	void RequestAllDevicesSignal();
	void RemoveDevices();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void ReceiveDevicesSlot(const QVector<DeviceInstance>&);
	void CreateDeviceInstanceSlot(DeviceInstance*, AbstractFactory*, QMutex*, QWaitCondition*);
	void ShowMessageBox(const QString &title, const QString &message, QMessageBox::Icon icon);
	void PrepareSplash();
	void UpdateSplashGeometry();
	void LabFacilitySelected(QWidget*);
	void SaveLab();
	void MoveBack();
	void DeleteLab();
	void SelectManual();

private:
	void init();
	void ShowLabComposition();
	void ShowLabList();
	void CreateTopWidget();
	void CreateFooterWidget();
	void CreateSelectLabArea();
	void CreateLabFacilityArea();
	void CreateLabFacilityButton(LabFacility&);
	void FillSelectLabArea();
	void FillSelectedLab();
	void ClearSelectLabArea();
	void ClearTopWidget();
	void DeleteLabFacArea();

	DeviceInfo FindDeviceInfoById(QList<DeviceInfo> &whereToFind, int d_id);
	QList<DeviceInfo> CreateSelectedDevices(DBRecordList &devicesInOrder);
	void AddExistingConnWidgets(DBRecordList&, QList<DeviceInfo>&);

	QWidget *topWidget;
	QWidget *footerWidget;
	QLineEdit *labName;
	QLineEdit *descrPath;
	SelectDevicesWidget *selectDevices;
	
	LabFacilityArea *labFacArea;
	QScrollArea *selectLabArea;

	QVBoxLayout *selectLabLayout;
	QVBoxLayout *centralLayout;

	QSignalMapper *labFacMapper;

	LabFacility lfac;
	LabMerger *labMerger;
	LoadingDllScreen *splash;
	QList<DeviceInfo> existingDevices;
};

#endif