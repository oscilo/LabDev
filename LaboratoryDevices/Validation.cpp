#include "Validation.h"

#include "../ValidationServer/packets.h"

#include "globals.h"

#include <QUdpSocket>
#include <QHostInfo>

#define UDP_CLIENT_PORT			12322
#define UDP_SERVER_PORT			12321

#define ATTEMPTS_NUMBER		4

bool FindValidationServer() {
	bool ret = false;

	QUdpSocket answerSock;
	bool res;
	res = answerSock.bind(UDP_CLIENT_PORT, QUdpSocket::ShareAddress);

	int counter = ATTEMPTS_NUMBER;
	while(counter) {
		QUdpSocket requestSock;
		QByteArray requestPackBuf(sizeof(UdpRequestPacket), 0x00);
		UdpRequestPacket *requestPack = (UdpRequestPacket *)requestPackBuf.data();
		requestPack->type = UR_TRY_TO_CONNECT;
		requestSock.writeDatagram(requestPackBuf, QHostAddress::Broadcast, UDP_SERVER_PORT);

		if(answerSock.waitForReadyRead())
			break;

		--counter;
	}

	if(!counter) {
		QMessageBox::critical(0, RUS("Ошибка"), RUS("Ошибка запуска, приложение будет закрыто."));
		return ret;
	}

	counter = ATTEMPTS_NUMBER;
	bool continueLooping = true;
	while(continueLooping) {
		if(!answerSock.waitForReadyRead()) {
			if(!counter)
				break;

			counter--;
			continue;
		}

		QByteArray datagram;
		int size = answerSock.pendingDatagramSize();
		datagram.resize(answerSock.pendingDatagramSize());

		QHostAddress sender;
		quint16 senderPort;
		answerSock.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

		if(datagram.size() != sizeof(UdpAnswerPacket))
			break;

		UdpAnswerPacket *answerPack = (UdpAnswerPacket *)datagram.data();
		switch(answerPack->type) {
			case UA_REQUEST_FOR_PARAMS: {
					QByteArray packBuf(sizeof(UdpRequestPacket), 0x00);
					UdpRequestPacket *packet = (UdpRequestPacket *)packBuf.data();
					packet->type = UR_PARAMS_SEND;
					
					QByteArray hostName = QHostInfo::localHostName().toLocal8Bit();
					QByteArray domainName = QHostInfo::localDomainName().toLocal8Bit();

					memcpy(packet->hostName, hostName.data(), (hostName.size() > (STRING_LEN-1)) ? (STRING_LEN-1) : hostName.size() );
					memcpy(packet->domainName, domainName.data(), (domainName.size() > (STRING_LEN-1)) ? (STRING_LEN-1) : domainName.size() );

					QUdpSocket().writeDatagram(packBuf, sender, UDP_SERVER_PORT);
				}
				break;
			case UA_SETUP_MODE:
				QMessageBox::information(0, RUS("Настройка произведена"), RUS("Настройка окончена, приложение будет закрыто."));
				ret = false;
				continueLooping = false;
				break;
			case UA_DENIED:
				QMessageBox::critical(0, RUS("Ошибка"), RUS("Ошибка запуска, приложение будет закрыто."));
				ret = false;
				continueLooping = false;
				break;
			case UA_ACCESSED:
				ret = true;
				continueLooping = false;
				break;
		}
	}

	return ret;
}