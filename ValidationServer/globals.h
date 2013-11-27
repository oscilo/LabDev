#ifndef GLOBALS_H
#define GLOBALS_H

#include <QCoreApplication>

#include <QStringList>
#include <QByteArray>

#include <QSettings>

#include <QCryptographicHash>

#include <QThread>

#include <QUdpSocket>

#define CUR_ORGANIZATION	"VirtualLaboratory"
#define CUR_APPLICATION		"ValidationServer"

#define MACHINES_GROUP		"MachinesGroup"

#define SETUP_MODE			"-setup_mode"

#define UDP_SERVER_PORT			12321
#define UDP_CLIENT_PORT			12322

enum RunMode : int {
	RM_SETUP = 0,
	RM_COMMON
};

RunMode GetRunMode();

#endif