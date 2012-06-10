#include "SelectDevicesWidget.h"

SelectDevicesWidget::SelectDevicesWidget(QWidget *parent) :
	QGroupBox(parent)
{
	this->setTitle(RUS("Выбор устройств"));
}
SelectDevicesWidget::~SelectDevicesWidget() {
}
void SelectDevicesWidget::SetDevicesList(const QList<DeviceInfo> &devices) {
	totalDevices = devices;

	struct LessThan {
		bool operator()(const DeviceInfo &a, const DeviceInfo &b) const {
			if(a.type == b.type)
				return a.name < b.name;

			return a.type < b.type;
		}
	};

	qStableSort(totalDevices.begin(), totalDevices.end(), LessThan());

	FillWidget();
}
void SelectDevicesWidget::AddCheckLayoutAtPosition(const DeviceInfo &di, int row, int column) {
	CheckDeviceLayout *checkDevice = new CheckDeviceLayout(di, this);
	connect(checkDevice, SIGNAL(UpdateDevices()), this, SLOT(UpdateSelectedDevices()));
	connect(this, SIGNAL(CheckSelectionList(const QList<DeviceInfo>&)), checkDevice, SLOT(CheckSelectionList(const QList<DeviceInfo> &)));

	layout->addLayout(checkDevice, row, column);
	checkLayouts << checkDevice;
}
void SelectDevicesWidget::FillWidget() {
	layout = new QGridLayout(this);

	int row = 0;
	int column = 0;
	int totalDevicesSize = totalDevices.size();
	if(totalDevicesSize < 6) {
		foreach(DeviceInfo di, totalDevices) {
			AddCheckLayoutAtPosition(di, row, column);
			row++;
		}
	}
	else {
		int maxRows = (totalDevicesSize / 2) + ( (totalDevicesSize % 2) ? 1 : 0 );

		foreach(DeviceInfo di, totalDevices) {
			AddCheckLayoutAtPosition(di, row, column);
			row++;
			if(row == maxRows) {
				row = 0;
				column++;
			}
		}
	}
}
void SelectDevicesWidget::UpdateSelectedDevices() {
	selectedDevices.clear();

	int orderOffset = 0;

	foreach(CheckDeviceLayout *curLay, checkLayouts) {
		QList<DeviceInfo> ldi = curLay->GetDeviceList();
		int ldiSize = ldi.size();

		for(int i = 0; i < ldiSize; ++i)
			ldi[i].order += orderOffset;

		orderOffset += ldiSize;
		selectedDevices << ldi;
	}

	emit SelectionUpdated(selectedDevices);
}
DBRecordList SelectDevicesWidget::GetSelectedDevices() {
	DBRecordList ret;

	foreach(DeviceInfo di, selectedDevices) {
		DBRecord record;
		record["d_id"] = di.id;
		record["o_order"] = di.order;

		ret << record;
	}

	return ret;
}
void SelectDevicesWidget::Clear() {
	for(int i = 0; i < layout->count(); ++i) {
		CheckDeviceLayout *lay = qobject_cast<CheckDeviceLayout*>(layout->itemAt(i)->layout());

		if(!lay)
			continue;

		lay->Clear();
	}
}
void SelectDevicesWidget::SetExistingSelection(const QList<DeviceInfo> &existingSelection) {
	emit CheckSelectionList(existingSelection);
	UpdateSelectedDevices();
}