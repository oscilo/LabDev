#ifndef _ABSTRACT_DEVICE_H
#define _ABSTRACT_DEVICE_H

#include <QtGlobal>

#include <QWidget>
#include <QMap>
#include <QUuid>

#include <DeviceException.h>
#include <AbstractFunc.h>

class AbstractDevice : public QWidget
{
	Q_OBJECT

public:
	enum PlugingType : uint
	{
		PT_COMMON		= 0x00,
		PT_SFG_INDIC	= 0x01,
	};
	enum SignalDirection : uchar
	{
		SD_INPUT_SIGNAL,
		SD_OUTPUT_SIGNAL,
	};
	enum DeviceType : uint
	{
		DT_NONE		= 0x00,
		DT_SFG,
		DT_BLACK_BOX,
		DT_INDICATOR,
		DT_HFSG,
	};
	AbstractDevice(QWidget *parent = 0, Qt::WindowFlags f = 0) : QWidget(parent, f) {
		uuid = QUuid::createUuid();
	}
	~AbstractDevice() {}

	virtual DeviceType getDeviceType() = 0;
	virtual QString getDeviceName() = 0;
	virtual void initProperties() {}
	QUuid getUuid() {
		return uuid;
	}
	QStringList getSignalsKeys(SignalDirection direction) {
		return selectSignals(direction).keys();
	}
	static void plugin(AbstractDevice *emitter, const QString &outputKey, AbstractDevice *receiver, const QString &inputKey) {
		connect(emitter, SIGNAL(repaintSignal()), receiver, SIGNAL(repaintSignal()));
		receiver->setSignal(SignalDirection::SD_INPUT_SIGNAL, inputKey, emitter->getSignal(SignalDirection::SD_OUTPUT_SIGNAL, outputKey));

		foreach(AbstractFunc* os, receiver->outputSignals) {
			os->setInput(inputKey, receiver->getSignal(SignalDirection::SD_INPUT_SIGNAL, inputKey));
		}
	}
	void unplug() {
		foreach(QString ikey, inputSignals.keys()) {
			removeSignal(SignalDirection::SD_INPUT_SIGNAL, ikey);
		}
	}
	QStringList getAllowedInputs() {
		QStringList ret;

		ret << neededInputs;

		foreach(AbstractFunc *s, outputSignals) {
			ret << s->getAllowedInputs();
		}

		return ret;
	}
signals:
	void repaintSignal();

protected:
	void setSignal(const SignalDirection direction, const QString &key, AbstractFunc *sign)
	{
		selectSignals(direction)[key] = sign;
		
		if(SignalDirection::SD_OUTPUT_SIGNAL == direction)
		{
			foreach(AbstractFunc* os, inputSignals)
			{
				sign->setInput(inputSignals.key(os), os);
			}
		}
	}
	void removeSignal(const SignalDirection direction, const QString &key)
	{
		selectSignals(direction).remove(key);

		if(SignalDirection::SD_INPUT_SIGNAL == direction)
		{
			foreach(AbstractFunc* os, outputSignals)
			{
				os->removeInput(key);
			}
		}
	}
	AbstractFunc* getSignal(const SignalDirection direction, const QString &key)
	{
		return selectSignals(direction)[key];
	}

	SignalsMap& selectSignals(const SignalDirection direction)
	{
		switch(direction)
		{
			case SignalDirection::SD_INPUT_SIGNAL:
				return inputSignals;
			case SignalDirection::SD_OUTPUT_SIGNAL:
				return outputSignals;
			default:
				throw DeviceException("Invalid Direction");
		}
	}

	SignalsMap inputSignals;
	SignalsMap outputSignals;
	QStringList neededInputs;
	
private:
	QUuid uuid;
};

typedef AbstractDevice* AbstractDevicePtr;

Q_DECLARE_INTERFACE(AbstractDevice, "AbstractDevice");

#endif