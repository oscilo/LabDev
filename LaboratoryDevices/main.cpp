#include "globals.h"
#include "StartWindow.h"

Q_DECLARE_METATYPE(QVector<AbstractDevice*>)
Q_DECLARE_METATYPE(AbstractDevicePtr)
Q_DECLARE_METATYPE(LabFacility)
Q_DECLARE_METATYPE(QMessageBox::Icon)
Q_DECLARE_METATYPE(QVector<DeviceInstance>)
Q_DECLARE_METATYPE(QList<DeviceInfo>)

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);

	qRegisterMetaType<QVector<AbstractDevice*>>();
	qRegisterMetaType<AbstractDevicePtr>();
	qRegisterMetaType<LabFacility>();
	qRegisterMetaType<QMessageBox::Icon>();
	qRegisterMetaType<QVector<DeviceInstance>>();
	qRegisterMetaType<QList<DeviceInfo>>();

	StartWindow w(0, Qt::Window | Qt::FramelessWindowHint);
	QObject::connect(&w, SIGNAL(quitSignal()), &a, SLOT(quit()));
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &w, SLOT(showSlot()));

	w.show();

	return a.exec();
}