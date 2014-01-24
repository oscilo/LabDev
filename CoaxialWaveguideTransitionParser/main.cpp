#include <QString>
#include <QByteArray>

#include <QRegExp>

#include <QTextStream>

#include <QFile>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

#define L_COUNT		6
#define S_COUNT		5

typedef float* float_ptr;
float_ptr *data;

int GetFCount() {
	int ret = (int)((F_LAST - F_FIRST)/F_STEP) + 1;
	return ret;
}
void CreateDataArray() {
	data = new float_ptr[L_COUNT*S_COUNT];
	for(int i = 0; i < L_COUNT*S_COUNT; ++i)
		data[i] = new float[GetFCount()];
}
void RemoveDataArray() {
	for(int i = 0; i < L_COUNT*S_COUNT; ++i)
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

	QRegExp rx("([-]?\\d+[\\.\\,]?(\\d+)?[E]?[-]?(\\d+)?);?");
	int f_num = 0;
	int s_num = 0;
	int l_num = 0;

	CreateDataArray();

	while( !csvStream.atEnd() ) {
		QString line = csvStream.readLine();
		int offset = 0;

		float value;
		while(-1 != (offset = rx.indexIn(line, offset))) {
			QString valueStr = rx.cap(1);
			value = valueStr.toFloat();

			int column = s_num + l_num*S_COUNT;
			data[column][f_num] = value;

			offset += rx.matchedLength();

			++s_num;
			if(S_COUNT <= s_num) {
				l_num++;
				s_num = 0;
			}
		}

		++f_num;
		s_num = 0;
		l_num = 0;
	}

	QFile dataFile("..\\BlackBoxValues\\CoaxialWaveguideTransition.val");
	dataFile.open(QIODevice::WriteOnly);

	if(!dataFile.isOpen()) {
		RemoveDataArray();
		return 1;
	}

	quint64 dataLength = GetFCount() * sizeof(float);
	for(int i = 0; i < L_COUNT * S_COUNT; ++i) {
		dataFile.write((char*)data[i], dataLength);
		dataFile.flush();
	}
	dataFile.close();

	RemoveDataArray();

	return 1;
}