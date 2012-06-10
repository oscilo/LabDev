#include <QtCore/QCoreApplication>

#include <QRegExp>
#include <QString>

#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QString fname("./../BlackBoxValues/NarrowBandFilterS21.txt");
	QString fnameToWrite("./../BlackBoxValues/NarrowBandFilterS21.val");

	QString curDir = QDir::current().canonicalPath();

	QFile f(fname);
	f.open(QIODevice::ReadOnly | QIODevice::Text);

	QFile fw(fnameToWrite);
	fw.open(QIODevice::WriteOnly);

	if(!f.isOpen())
		return 1;

	QRegExp rx("^\\s+([\\.\\d\\-]+)\\s+([\\.\\d\\-]+)");

	while(!f.atEnd()) {
		QString line = QString(f.readLine());
		
		if(-1 != rx.indexIn(line)) {
			float arg = rx.cap(1).toFloat();
			float val = rx.cap(2).toFloat();

			fw.write((char*)&arg, sizeof(float));
			fw.write((char*)&val, sizeof(float));
		}
	}

	f.close();
	fw.close();

	/*
	fw.open(QIODevice::ReadOnly);

	while(!fw.atEnd()) {
		float arg;
		float val;
		
		fw.read((char*)&arg, sizeof(float));
		fw.read((char*)&val, sizeof(float));
	}

	fw.close();
	//*/

	return a.exec();
}
