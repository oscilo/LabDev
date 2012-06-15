#include <QString>
#include <QByteArray>

#include <QRegExp>

#include <QTextStream>

#include <QFile>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

#define H1_COUNT		6
#define H2_COUNT		6

typedef float* float_ptr;
float_ptr *data;

int GetFCount() {
	int ret = (int)((F_LAST - F_FIRST)/F_STEP) + 1;
	return ret;
}
void CreateDataArray() {
	data = new float_ptr[H1_COUNT*H2_COUNT];
	for(int i = 0; i < H1_COUNT*H2_COUNT; ++i)
		data[i] = new float[GetFCount()];
}
void RemoveDataArray() {
	for(int i = 0; i < H1_COUNT*H2_COUNT; ++i)
		delete data[i];
	
	delete data;
}

int main(int argc, char *argv[]) {
	if(argc != 2)
		return 1;

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
	int h1_num = 5;
	int h2_num = 5;

	CreateDataArray();

	while( !csvStream.atEnd() ) {
		QString line = csvStream.readLine();
		int offset = 0;

		float value;
		while(-1 != (offset = rx.indexIn(line, offset))) {
			QString valueStr = rx.cap(1);
			value = valueStr.toFloat();

			int column = h1_num*H1_COUNT + h2_num;
			data[column][f_num] = value;

			offset += rx.matchedLength();

			--h2_num;
			if(0 > h2_num) {
				--h1_num;
				h2_num = 5;
			}
		}

		++f_num;
		h1_num = 5;
		h2_num = 5;
	}

	QFile dataFile("..\\BlackBoxValues\\MicrostripResonator.val");
	dataFile.open(QIODevice::WriteOnly);

	if(!dataFile.isOpen()) {
		RemoveDataArray();
		return 1;
	}

	quint64 dataLength = GetFCount() * sizeof(float);
	for(int i = 0; i < H1_COUNT * H2_COUNT; ++i) {
		dataFile.write((char*)data[i], dataLength);
		dataFile.flush();
	}
	dataFile.close();

	RemoveDataArray();

	return 1;
}