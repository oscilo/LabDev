#ifndef PACKETS_H
#define PACKETS_H

enum UdpRequest : int {
	UR_TRY_TO_CONNECT = 0x30,
	UR_PARAMS_SEND
};

#define STRING_LEN		50

struct UdpRequestPacket {
	UdpRequest type;
	char hostName[50];
	char domainName[50];
};

enum UdpAnswer : int {
	UA_SETUP_MODE = 0x40,
	UA_REQUEST_FOR_PARAMS,
	UA_ACCESSED,
	UA_DENIED
};

struct UdpAnswerPacket {
	UdpAnswer type;
};

#endif