#include "LabConnectionWidget.h"


LabConnectionWidget::LabConnectionWidget(QWidget *parent) :
	QGroupBox(parent)
{
	QHBoxLayout *lay = new QHBoxLayout(this);

	QToolButton *actionButton = new QToolButton(this);
	actionButton->setAutoRaise(true);
	actionButton->setIcon(QIcon("./Resources/Add.png"));
	connect(actionButton, SIGNAL(clicked()), this, SIGNAL(CreateNewConnection()));

	QLabel *lbl = new QLabel(RUS("Add New Connection"), this);		//DS Добавить новое соединение
	lbl->setAlignment(Qt::AlignCenter);
	
	lay->addWidget(actionButton);
	lay->addWidget(lbl, 1);
}
LabConnectionWidget::LabConnectionWidget(QList<DeviceInfo> existingDevices, QWidget *parent) :
	QGroupBox(parent), devices(existingDevices)
{
	QGridLayout *lay = new QGridLayout(this);

	QToolButton *actionButton = new QToolButton(this);
	actionButton->setAutoRaise(true);
	actionButton->setIcon(QIcon("./Resources/Remove.png"));
	connect(actionButton, SIGNAL(clicked()), this, SLOT(deleteLater()));

	typeBox = new QComboBox(this);
	receiverBox = new QComboBox(this);
	emitterBox = new QComboBox(this);
	outputsBox = new QComboBox(this);
	inputsBox = new QComboBox(this);

	receiverBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	emitterBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	
	QLabel *connTypeLabel =		new QLabel(RUS("Connection type"), this);		//DS Тип соединения
	QLabel *emitterLabel =		new QLabel(RUS("Source"), this);				//DS Источник
	QLabel *receiverLabel =		new QLabel(RUS("Receiver"), this);				//DS Приемник
	QLabel *deviceLabel =		new QLabel(RUS("Device"), this);				//DS Устройство equipment
	signalsKeyLabel =			new QLabel(RUS("Key	signal"), this);			//DS Ключ сигнала

	connTypeLabel->setAlignment(Qt::AlignCenter);
	emitterLabel->setAlignment(Qt::AlignCenter);
	receiverLabel->setAlignment(Qt::AlignCenter);
	deviceLabel->setAlignment(Qt::AlignRight);
	signalsKeyLabel->setAlignment(Qt::AlignRight);

	lay->addWidget(connTypeLabel,	0, 1);
	lay->addWidget(emitterLabel,	0, 3);
	lay->addWidget(receiverLabel,	0, 4);
	lay->addWidget(actionButton,	1, 0);
	lay->addWidget(typeBox,			1, 1);
	lay->addWidget(deviceLabel,		1, 2);
	lay->addWidget(emitterBox,		1, 3);
	lay->addWidget(receiverBox,		1, 4);
	lay->addWidget(signalsKeyLabel, 2, 2);
	lay->addWidget(outputsBox,		2, 3);
	lay->addWidget(inputsBox,		2, 4);
	lay->setColumnStretch(1, 2);
	lay->setColumnStretch(2, 1);
	lay->setColumnStretch(3, 3);
	lay->setColumnStretch(4, 3);

	connect(typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(TypeChanged(int)));
	connect(receiverBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ReceiverChanged(int)));
	connect(emitterBox,	 SIGNAL(currentIndexChanged(int)), this, SLOT(EmitterChanged(int)));

	typeBox->addItem(RUS("Regular"),			AbstractDevice::PT_COMMON);	//DS
	typeBox->addItem(RUS("SFG - VSWR"),	AbstractDevice::PT_SFG_INDIC);		//DS ГКЧ - Индикатор
}
LabConnectionWidget::~LabConnectionWidget() {
}
void LabConnectionWidget::TypeChanged(int index) {
	bool keysVisible;

	if(-1 == index) {
		emitterBox->clear();
		receiverBox->clear();
		return;
	}

	AbstractDevice::PlugingType pt = (AbstractDevice::PlugingType)(typeBox->itemData(index).toUInt());

	switch(pt) {
		case AbstractDevice::PT_COMMON:
			SetBoxItemsByType(emitterBox,  AbstractDevice::DT_NONE);
			SetBoxItemsByType(receiverBox, AbstractDevice::DT_NONE);
			keysVisible = true;
			break;

		case AbstractDevice::PT_SFG_INDIC:
			SetBoxItemsByType(emitterBox,  AbstractDevice::DT_SFG);
			SetBoxItemsByType(receiverBox, AbstractDevice::DT_INDICATOR);
			keysVisible = false;
			break;

		default:
			SetBoxItemsByType(emitterBox,  AbstractDevice::DT_NONE);
			SetBoxItemsByType(receiverBox, AbstractDevice::DT_NONE);
			keysVisible = false;
			break;
	}

	signalsKeyLabel->setVisible(keysVisible);
	outputsBox->setVisible(keysVisible);
	inputsBox->setVisible(keysVisible);
}
void LabConnectionWidget::SetBoxItemsByType(QComboBox *box, AbstractDevice::DeviceType dt) {
	box->clear();

	foreach(DeviceInfo di, devices) {
		if( (di.type == dt) || (dt == AbstractDevice::DT_NONE) )
			box->addItem(QString("%1. %2").arg(di.order).arg(di.name), QVariant::fromValue(di.uuid));
	}
}
DeviceInfo LabConnectionWidget::FindDeviceByDeviceUuid(const QList<DeviceInfo> &whereToFind, QUuid &uuid) {
	foreach(DeviceInfo di, whereToFind) {
		if(uuid == di.uuid)
			return di;
	}

	return DeviceInfo();
}
void LabConnectionWidget::ReceiverChanged(int index) {
	if(-1 == index) {
		inputsBox->clear();
		return;
	}

	QUuid uuid = receiverBox->itemData(index).value<QUuid>();
	SetBoxItemsByDevice(inputsBox, FindDeviceByDeviceUuid(devices, uuid).inputs);
}
void LabConnectionWidget::EmitterChanged(int index) {
	if(-1 == index) {
		outputsBox->clear();
		return;
	}

	QUuid uuid = emitterBox->itemData(index).value<QUuid>();
	SetBoxItemsByDevice(outputsBox, FindDeviceByDeviceUuid(devices, uuid).outputs);
}
void LabConnectionWidget::SetBoxItemsByDevice(QComboBox *box, const QStringList &keys) {
	box->clear();
	box->addItems(keys);
}
void LabConnectionWidget::SelectedDevicesUpdated(const QList<DeviceInfo> &selectedDevices) {
	int index;
	QUuid emitterUuid;
	QUuid receiverUuid;
	QString emitterKey;
	QString receiverKey;

	index  = emitterBox->currentIndex();
	if(-1 != index) {
		emitterUuid = emitterBox->itemData(index).value<QUuid>();
		
		if(-1 == FindDeviceByDeviceUuid(selectedDevices, emitterUuid).order) {
			deleteLater();
			return;
		}
	}
	emitterKey = outputsBox->currentText();

	index  = receiverBox->currentIndex();
	if(-1 != index) {
		receiverUuid = receiverBox->itemData(index).value<QUuid>();
		
		if(-1 == FindDeviceByDeviceUuid(selectedDevices, receiverUuid).order) {
			deleteLater();
			return;
		}
	}
	receiverKey = inputsBox->currentText();

	devices = selectedDevices;

	TypeChanged(typeBox->currentIndex());

	SetBoxCurrentItemByUuid(emitterBox, emitterUuid);
	SetBoxCurrentItemByUuid(receiverBox, receiverUuid);

	SetBoxCurrentItemByString(outputsBox, emitterKey);
	SetBoxCurrentItemByString(inputsBox, receiverKey);
}
void LabConnectionWidget::SetBoxCurrentItemByUuid(QComboBox *box, const QUuid &uuid) {
	for(int i = 0; i < box->count(); ++i) {
		if(uuid == box->itemData(i).value<QUuid>()) {
			box->setCurrentIndex(i);
			break;
		}
	}
}
void LabConnectionWidget::SetBoxCurrentItemByString(QComboBox *box, const QString &text) {
	for(int i = 0; i < box->count(); ++i) {
		if(text == box->itemText(i)) {
			box->setCurrentIndex(i);
			break;
		}
	}
}
void LabConnectionWidget::SetBoxCurrentItemByPlugingType(QComboBox *box, AbstractDevice::PlugingType pt) {
	for(int i = 0; i < box->count(); ++i) {
		AbstractDevice::PlugingType curPt = (AbstractDevice::PlugingType)(typeBox->itemData(i).toUInt());

		if(curPt == pt) {
			box->setCurrentIndex(i);
			break;
		}
	}
}
DBRecord LabConnectionWidget::GetConnectionRecord() {
	DBRecord record;

	QVariant c_type = typeBox->itemData(typeBox->currentIndex());
	QUuid emitterUuid = emitterBox->itemData(emitterBox->currentIndex()).value<QUuid>();
	QUuid receiverUuid = receiverBox->itemData(receiverBox->currentIndex()).value<QUuid>();

	record["c_type"] = c_type;
	record["c_emitter_order"] = FindDeviceByDeviceUuid(devices, emitterUuid).order;
	record["c_receiver_order"] = FindDeviceByDeviceUuid(devices, receiverUuid).order;
	record["c_outkey"] = (c_type.toInt() == AbstractDevice::PT_COMMON) ? outputsBox->currentText() : "";
	record["c_inpkey"] = (c_type.toInt() == AbstractDevice::PT_COMMON) ? inputsBox->currentText() : "";

	return record;
}
bool LabConnectionWidget::isValid() {
	if(0 == devices.size())
		return false;

	if(-1 == typeBox->currentIndex())
		return false;
	if(-1 == receiverBox->currentIndex())
		return false;
	if(-1 == emitterBox->currentIndex())
		return false;
	if(-1 == outputsBox->currentIndex())
		return false;
	if(-1 == inputsBox->currentIndex())
		return false;

	return true;
}
void LabConnectionWidget::FillExistingConnection(AbstractDevice::PlugingType c_type, QUuid &emitterUuid, QUuid &receiverUuid, QString &emitterStr, QString &receiverStr) {
	SetBoxCurrentItemByPlugingType(typeBox, c_type);

	SetBoxCurrentItemByUuid(emitterBox, emitterUuid);
	SetBoxCurrentItemByUuid(receiverBox, receiverUuid);

	SetBoxCurrentItemByString(outputsBox, emitterStr);
	SetBoxCurrentItemByString(inputsBox, receiverStr);
}