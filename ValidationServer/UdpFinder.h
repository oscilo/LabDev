#ifndef UDPFINDER_H
#define UDPFINDER_H

#include "globals.h"

class UdpFinder : public QThread {
	Q_OBJECT
public:
	UdpFinder();

	void run();

private slots:
	void ReadPendingDatagrams();

private:
	QUdpSocket *socket;
};

#endif