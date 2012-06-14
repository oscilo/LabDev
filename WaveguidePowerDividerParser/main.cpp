#include <QString>
#include <QByteArray>
#include <QMap>

#include <QRegExp>

#include <QTextStream>

#include <QFile>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

QMap<int, int> rCount;
int totalCount = 0;

typedef float* float_ptr;
float_ptr *data;

int GetFCount() {
	int ret = (int)((F_LAST - F_FIRST)/F_STEP) + 1;
	return ret;
}
void CreateDataArray() {
	data = new float_ptr[totalCount];
	for(int i = 0; i < totalCount; ++i)
		data[i] = new float[GetFCount()];
}
void RemoveDataArray() {
	for(int i = 0; i < totalCount; ++i)
		delete data[i];
	
	delete data;
}

int main(int argc, char *argv[]) {
	if(argc != 2)
		return 1;

	rCount[0] = 2;
	rCount[1] = 4;
	rCount[2] = 5;
	rCount[3] = 5;
	rCount[4] = 5;
	foreach(int curCount, rCount) {
		totalCount += curCount;
	}

	QString fileName(argv[1]);
	QFile csvDataFile(fileName);

	csvDataFile.open(QIODevice::ReadOnly);
	
	if(!csvDataFile.isOpen())
		return 1;
	
	QByteArray csvData = csvDataFile.readAll();
	csvDataFile.close();

	csvData.replace(',', '.');

	QTextStream csvStream(&csvData, QIODevice::ReadOnly);

	QRegExp rx("(-?\\d+\\.\\d+);?");
	int f_num = 0;
	int counter = 0;

	CreateDataArray();

	while( !csvStream.atEnd() ) {
		QString line = csvStream.readLine();
		int offset = 0;

		float value;
		while(-1 != (offset = rx.indexIn(line, offset))) {
			QString valueStr = rx.cap(1);
			value = valueStr.toFloat();

			data[counter][f_num] = value;

			offset += rx.matchedLength();

			counter++;
		}

		++f_num;
		counter = 0;
	}

	QFile dataFile("..\\BlackBoxValues\\WaveguidePowerDivider.val");
	dataFile.open(QIODevice::WriteOnly);

	if(!dataFile.isOpen()) {
		RemoveDataArray();
		return 1;
	}

	quint64 dataLength = GetFCount() * sizeof(float);
	for(int i = 0; i < totalCount; ++i) {
		dataFile.write((char*)data[i], dataLength);
		dataFile.flush();
	}
	dataFile.close();

	RemoveDataArray();

	return 1;
}