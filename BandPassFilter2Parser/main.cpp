#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVector>

#include <QRegExp>

#include <QTextStream>

#include <QFile>
#include <QDir>

int main(int argc, char *argv[]) {
	QString fileName = "./data/filter.txt";

	QRegExp paramsLineRx("_l0_2=(\\d+[\\.]?(\\d+)?)_l0_3=(\\d+[\\.]?(\\d+)?)_l_2=(\\d+[\\.]?(\\d+)?)_l_3=(\\d+[\\.]?(\\d+)?)/");

	QFile f(fileName);
	f.open(QIODevice::ReadOnly);

	if(!f.isOpen())
		return 0;

	//QMap<float, int> l0_2, l0_3, l_2, l_3;
	QMap<float, int> l[4];
	int sum = 0;
	while(!f.atEnd()) {
		QString line = f.readLine();
		if(-1 != paramsLineRx.indexIn(line)) {
			/*
			l0_2[paramsLineRx.cap(1).toFloat()]++;
			l0_3[paramsLineRx.cap(3).toFloat()]++;
			l_2[paramsLineRx.cap(5).toFloat()]++;
			l_3[paramsLineRx.cap(7).toFloat()]++;
			/*/
			l[0][paramsLineRx.cap(1).toFloat()]++;
			l[1][paramsLineRx.cap(3).toFloat()]++;
			l[2][paramsLineRx.cap(5).toFloat()]++;
			l[3][paramsLineRx.cap(7).toFloat()]++;
			//*/
			sum++;
		}
	}

	f.close();

	QTextStream log(stdout);


	for(int i = 0; i < 4; ++i) {
		const QMap<float, int> &cur(l[i]);

		foreach(const float &key, cur.keys()) {
			log << key << " | " << cur.value(key) << endl;
		}

		log << endl;
	}

	log << endl << sum << endl;


	return 0;
}
