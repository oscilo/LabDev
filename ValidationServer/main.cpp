#include "globals.h"

#include "UdpFinder.h"

RunMode mode = RM_COMMON;

RunMode GetRunMode() {
	return mode;
}

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
	
	QCoreApplication::setOrganizationName(CUR_ORGANIZATION);
	QCoreApplication::setApplicationName(CUR_APPLICATION);

	if( (QCoreApplication::arguments().size() == 2) && (QCoreApplication::arguments().at(1) == SETUP_MODE) )
		mode = RM_SETUP;

	UdpFinder finder;
	finder.start();

	return a.exec();
}