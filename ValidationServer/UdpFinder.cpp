#include "UdpFinder.h"

#include "packets.h"

UdpFinder::UdpFinder() {
	this->moveToThread(this);
}
void UdpFinder::run() {
	socket = new QUdpSocket(this);
	bool res = socket->bind(UDP_SERVER_PORT, QUdpSocket::ShareAddress);

	connect(socket, SIGNAL(readyRead()), this, SLOT(ReadPendingDatagrams()));

	this->exec();
}
void UdpFinder::ReadPendingDatagrams() {
	while (socket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(socket->pendingDatagramSize());
		
		QHostAddress sender;
		quint16 senderPort;

		socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

		if(datagram.size() == sizeof(UdpRequestPacket)) {
			const UdpRequestPacket *pack = (const UdpRequestPacket*)datagram.data();

			switch(pack->type) {
				case UdpRequest::UR_TRY_TO_CONNECT: {
					QUdpSocket answerSock;
					QByteArray answerPackBuf(sizeof(UdpAnswerPacket), 0x00);
					UdpAnswerPacket *answerPack = (UdpAnswerPacket *)answerPackBuf.data();
					answerPack->type = UA_REQUEST_FOR_PARAMS;
					answerSock.writeDatagram(answerPackBuf, sender, UDP_CLIENT_PORT);
				}
					break;

				case UdpRequest::UR_PARAMS_SEND: {
					QString hashRes(QCryptographicHash::hash(datagram, QCryptographicHash::Md5).toHex());

					QSettings settings;
					settings.beginGroup(MACHINES_GROUP);
					
					QUdpSocket answerSock;
					QByteArray answerPackBuf(sizeof(UdpAnswerPacket), 0x00);
					UdpAnswerPacket *answerPack = (UdpAnswerPacket *)answerPackBuf.data();

					if(RM_SETUP == GetRunMode()) {
						if( !(settings.allKeys().contains(hashRes)) )
							settings.setValue(hashRes, 1);
						
						answerPack->type = UA_SETUP_MODE;
					}
					else {
						if(settings.allKeys().contains(hashRes))
							answerPack->type = UA_ACCESSED;
						else
							answerPack->type = UA_DENIED;
					}
					
					answerSock.writeDatagram(answerPackBuf, sender, UDP_CLIENT_PORT);

					settings.endGroup();
				}
					break;

				default:
					break;
			}
		}
	}
}