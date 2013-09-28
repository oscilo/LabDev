#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVector>

#include <QRegExp>

#include <QTextStream>

#include <QFile>
#include <QDir>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

#define L_COUNT		10
#define S_COUNT		10
#define SK_COUNT	10
#define X_COUNT		10

#define L_START		11.48
#define SK_START	21.28
#define S_START		14.68
#define X_START		3.68

#define L_LAST		19.4
#define SK_LAST		38.65
#define S_LAST		26.65
#define X_LAST		6.65

#define L_STEP		0.88
#define SK_STEP		1.93
#define S_STEP		1.33
#define X_STEP		0.33

struct DataPoint {
	DataPoint() {}
	DataPoint(float l_, float sk_, float s_) : sk(sk_), l(l_), s(s_) {}

	bool operator < (const DataPoint &a) const {
		return (sk == a.sk) ? ( (l == a.l) ? (s < a.s) : (l < a.l) ) : (sk < a.sk);
	}

	float sk;
	float l;
	float s;

	QVector<float> data;
};

int main(int argc, char *argv[]) {
	QFileInfoList fileList = QDir("./data/").entryInfoList(QStringList() << "*.txt");

	QRegExp fileNameRx("0 (\\d+)[ ]?(\\d+)?");
	QRegExp paramsLineRx("_l=(\\d+[\\.]?(\\d+)?)_d0=(\\d+[\\.]?(\\d+)?)_sh=(\\d+[\\.]?(\\d+)?)/real");
	QRegExp valueRx("\\d+[\\.]?[\\d]?\\s+([-]?\\d+[\\.]?(\\d+)?)");

	QMap<float, QList<DataPoint>> params;

	DataPoint *curDataPoint = 0;
	foreach(QFileInfo curInfo, fileList) {
		QString fileName = curInfo.fileName();
		if(-1 != fileNameRx.indexIn(fileName)) {
			float x0Val = fileNameRx.cap(1).toInt();
			if(fileNameRx.captureCount() == 2)
				x0Val += fileNameRx.cap(2).toFloat() / 100;

			QFile curFileRead(curInfo.absoluteFilePath());
			curFileRead.open(QIODevice::ReadOnly | QIODevice::Text);
			if(!curFileRead.isOpen())
				continue;

			while(!curFileRead.atEnd()) {
				QString line = curFileRead.readLine();
				if(-1 != paramsLineRx.indexIn(line)) {
					params[x0Val] << DataPoint( paramsLineRx.cap(1).toFloat(),
													paramsLineRx.cap(3).toFloat(),
													paramsLineRx.cap(5).toFloat());
					curDataPoint = &(params[x0Val].last());
				}

				if(curDataPoint && (-1 != valueRx.indexIn(line))) {
					curDataPoint->data << valueRx.cap(1).toFloat();
				}
			}

			qSort(params[x0Val]);

			curFileRead.close();
		}
	}
	
	QFile csvWrite("./../BlackBoxValues/WaveguideSlotAntenna.val");
	csvWrite.open(QIODevice::WriteOnly);
	if(!csvWrite.isOpen())
		return 1;
	
	QList<float> x0Vals = params.keys();
	qSort(x0Vals);

	foreach(float curx0, x0Vals) {
		foreach(const DataPoint &dataPoint, params.value(curx0)) {
			csvWrite.write((char*)dataPoint.data.data(), dataPoint.data.size()*sizeof(float));
		}
	}

	csvWrite.close();
	return 1;
}