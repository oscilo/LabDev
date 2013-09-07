#include <QString>
#include <QStringList>
#include <QByteArray>

#include <QRegExp>

#include <QTextStream>

#include <QFile>
#include <QDir>

#define F_STEP		0.004
#define F_FIRST		8.
#define F_LAST		12.

#define L_COUNT		5
#define S_COUNT		3
#define SK_COUNT	3
#define X_COUNT		10

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
	QFileInfoList fileList = QDir("./data/").entryInfoList(QStringList() << "*.txt");

	QRegExp fileNameRx("x0 (\\d+)[ ]?(\\d+)?");
	QRegExp paramsLineRx("_d0=(\\d+[\\.]?(\\d+)?)_l=(\\d+[\\.]?(\\d+)?)_sh=(\\d+[\\.]?(\\d+)?)/real");

	QMap<float, QStringList> params;

	foreach(QFileInfo curInfo, fileList) {
		QString fileName = curInfo.fileName();
		if(-1 != fileNameRx.indexIn(curInfo.fileName())) {
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
					params[x0Val] <<	 "sk = " + paramsLineRx.cap(1) + 
										", l = " + paramsLineRx.cap(3) + 
										", s = " + paramsLineRx.cap(5);
				}
			}

			curFileRead.close();
		}
	}
	
	QFile csvWrite("./data/data.csv");
	csvWrite.open(QIODevice::WriteOnly | QIODevice::Text);
	if(!csvWrite.isOpen())
		return 1;

	/*
	QList<int> lens;

	foreach(QStringList curList, params) {
		lens << curList.length();
	}
	//*/
	
	#define PARAMS_LEN	25
	#define X0VALS_LEN	10

	QTextStream csvStream(&csvWrite);

	QList<float> x0Vals = params.keys();

	for(int j = 0; j < X0VALS_LEN; ++j) {
		csvStream << QString::number(x0Vals[j]).replace('.', ",") + ";";
	}
	csvStream << "\n";

	for(int i = 0; i < PARAMS_LEN; ++ i) {
		for(int j = 0; j < X0VALS_LEN; ++j) {
			csvStream << params[x0Vals[j]][i] + ";";
		}
		csvStream << "\n";
	}

	csvWrite.close();

	//"x0 3 68.txt"
	return 1;

	/*
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
	int s_num = 0;
	int l_num = 4;

	CreateDataArray();

	while( !csvStream.atEnd() ) {
		QString line = csvStream.readLine();
		int offset = 0;

		float value;
		while(-1 != (offset = rx.indexIn(line, offset))) {
			QString valueStr = rx.cap(1);
			value = valueStr.toFloat();

			int column = s_num*L_COUNT + l_num;
			data[column][f_num] = value;

			offset += rx.matchedLength();

			--l_num;
			if(0 > l_num) {
				s_num++;
				l_num = 4;
			}
		}

		++f_num;
		s_num = 0;
		l_num = 4;
	}

	QFile dataFile("..\\BlackBoxValues\\MicrostripResonator.val");
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
	*/
}