#ifndef SELECTDEVICESWIDGET_H
#define SELECTDEVICESWIDGET_H

#include "globals.h"
#include "structures.h"

class CheckDeviceLayout : public QHBoxLayout {
	Q_OBJECT

public:
	CheckDeviceLayout(const DeviceInfo &di, QWidget *parent = 0) : QHBoxLayout(parent), curDI(di) {
		check = new QCheckBox(di.name, parent);
		
		spin = new QSpinBox(parent);
		spin->setRange(1, 5);
		spin->hide();

		connect(check, SIGNAL(clicked(bool)), spin, SLOT(setVisible(bool)));
		connect(check, SIGNAL(clicked(bool)), this, SLOT(OnOffSlot(bool)));
		connect(spin, SIGNAL(valueChanged(int)), this, SLOT(CountChanged(int)));

		this->addWidget(check, 1);
		this->addWidget(spin);
	}
	~CheckDeviceLayout() {
	}

	QList<DeviceInfo>& GetDeviceList() {
		return devices;
	}

signals:
	void UpdateDevices();

public slots:
	void Clear() {
		spin->setValue(1);
		spin->hide();
		check->setChecked(false);
	}
	void CheckSelectionList(const QList<DeviceInfo> &existingSelection) {
		QList<DeviceInfo> newDevices;
		foreach(DeviceInfo di, existingSelection) {
			if(di.id == curDI.id) {
				di.order = newDevices.size();
				newDevices << di;
			}
		}

		if(0 == newDevices.size())
			Clear();
		else {
			check->setChecked(true);
			spin->show();
			spin->setValue(newDevices.size());
			devices = newDevices;
		}
	}

private slots:
	void OnOffSlot(bool action) {
		if(action)
			emit CountChanged(spin->value());
		else
			emit CountChanged(0);
	}
	void CountChanged(int count) {
		int oldSize = devices.size();

		if(oldSize == count)
			return;

		if(oldSize < count) {
			int flag = count - oldSize;
			while(flag) {
				DeviceInfo di = curDI;
				di.order = devices.size();
				di.uuid = QUuid::createUuid();
				devices << di;
				flag--;
			}
		}
		else {
			int flag = oldSize - count;
			while(flag) {
				devices.removeLast();
				flag--;
			}
		}

		emit UpdateDevices();
	}

private:
	DeviceInfo curDI;
	QList<DeviceInfo> devices;

	QCheckBox *check;
	QSpinBox *spin;
};

class SelectDevicesWidget : public QGroupBox {
	Q_OBJECT

public:
	SelectDevicesWidget(QWidget *parent = 0);
	~SelectDevicesWidget();

	void SetDevicesList(const QList<DeviceInfo>&);
	DBRecordList GetSelectedDevices();

signals:
	void SelectionUpdated(const QList<DeviceInfo>&);
	void CheckSelectionList(const QList<DeviceInfo>&);

public slots:
	void Clear();
	void SetExistingSelection(const QList<DeviceInfo>&);

private slots:
	void UpdateSelectedDevices();

private:
	void FillWidget();
	void AddCheckLayoutAtPosition(const DeviceInfo &, int row, int column);

	QList<DeviceInfo> totalDevices;
	QList<DeviceInfo> selectedDevices;
	QList<CheckDeviceLayout*> checkLayouts;

	QGridLayout *layout;
};

#endif