#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QVector>

#include <QRegExp>

#include <QTextStream>

#include <QFile>
#include <QDir>

struct DataPoint {
	DataPoint(int l0_2_in, int l0_3_in, int l_2_in, int l_3_in) : 
		l0_2(l0_2_in), l0_3(l0_3_in), l_2(l_2_in), l_3(l_3_in)
	{};

	bool operator < (const DataPoint &a) const {
		return	(l0_2 == a.l0_2) ? 
					( (l0_3 == a.l0_3) ? 
						( (l_2 == a.l_2) ? 
							( (l_3 < a.l_3) ) 
							: (l_2 < a.l_2) ) 
						: (l0_3 < a.l0_3) ) 
					: (l0_2 < a.l0_2);
	}

	bool operator == (const DataPoint &a) const {
		return	(l0_2 == a.l0_2) ? true :
					( (l0_3 == a.l0_3) ? true :
						( (l_2 == a.l_2) ? true :
							(l_3 == a.l_3) ) );
	}

	int l0_2, l0_3, l_2, l_3;
};

int main(int argc, char *argv[]) {
	QString fileName = "./data/filter.txt";

	QRegExp paramsLineRx("_l0_2=(\\d+)[\\.](\\d+)_l0_3=(\\d+)[\\.](\\d+)_l_2=(\\d+)[\\.](\\d+)_l_3=(\\d+)[\\.](\\d+)/");
	QRegExp valueRx("(\\d+)[\\.]?(\\d+)?\\s+([-]?\\d+[\\.]?(\\d+)?[e]?[-]?(\\d+)?)");

	QFile f(fileName);
	f.open(QIODevice::ReadOnly);

	if(!f.isOpen())
		return 0;

	typedef QMap<int, float> ValuesMap;
	QMap<DataPoint, ValuesMap> data;

	int sum = 0;

	QMap<DataPoint, ValuesMap>::iterator it;
	ValuesMap nullValuesMap;
	while(!f.atEnd()) {
		QString line = f.readLine();
		
		if(-1 != paramsLineRx.indexIn(line)) {
			int l0_2 = paramsLineRx.cap(1).toInt()*1000 + paramsLineRx.cap(2).toInt();
			int l0_3 = paramsLineRx.cap(3).toInt()*1000 + paramsLineRx.cap(4).toInt();
			int l_2 = paramsLineRx.cap(5).toInt()*1000 + paramsLineRx.cap(6).toInt();
			int l_3 = paramsLineRx.cap(7).toInt()*1000 + paramsLineRx.cap(8).toInt();
			
			DataPoint curPoint(l0_2, l0_3, l_2, l_3);

			it = data.insert(curPoint, nullValuesMap);
		}

		if(-1 != valueRx.indexIn(line)) {
			QString keyStr = valueRx.cap(2);
			while(keyStr.length() < 3)
				keyStr += "0";
			keyStr = valueRx.cap(1) + keyStr;

			int key = keyStr.toInt();
			float val = valueRx.cap(3).toFloat();

			it->insert(key, val);
		}
	}

	f.close();

	/*
	QTextStream log(stdout);


	for(int i = 0; i < 4; ++i) {
		const QMap<float, int> &cur(l[i]);

		foreach(const float &key, cur.keys()) {
			log << key << " | " << cur.value(key) << endl;
		}

		log << "--" << endl;
	}

	log << endl << sum << endl;
	//*/
	
	#define L0_2_START	4005
	#define L0_2_END	4745
	#define L0_2_STEP	148

	#define L0_3_START	9594
	#define L0_3_END	11369
	#define L0_3_STEP	355
	
	#define L_2_START	10350
	#define L_2_END		12265
	#define L_2_STEP	383
	
	#define L_3_START	8703
	#define L_3_END		10303
	#define L_3_STEP	320

	#define KEY_START	4000
	#define KEY_END		12000
	#define KEY_STEP	8

	
	QFile csvWrite("./../BlackBoxValues/BandPassFilter2.val");
	csvWrite.open(QIODevice::WriteOnly);
	if(!csvWrite.isOpen())
		return 1;

	for(int l0_2	= L0_2_START;	l0_2	<= L0_2_END;	l0_2	+= L0_2_STEP)	{
	for(int l0_3	= L0_3_START;	l0_3	<= L0_3_END;	l0_3	+= L0_3_STEP)	{
	for(int l_2		= L_2_START;	l_2		<= L_2_END;		l_2		+= L_2_STEP)	{
	for(int l_3		= L_3_START;	l_3		<= L_3_END;		l_3		+= L_3_STEP)	{
		DataPoint curPoint(l0_2, l0_3, l_2, l_3);

		ValuesMap &curValuesMap(data[curPoint]);
		int count = ((KEY_END - KEY_START) / KEY_STEP) + 1;
		QVector<float> valVector( count, 0.);
		int i = 0;
		for(int key = KEY_START; key <= KEY_END; key += KEY_STEP) {
			float val = curValuesMap[key];
			valVector[i++] = val;
		}
		csvWrite.write((char*)valVector.data(), valVector.size() * sizeof(float));
	}}}}
	
	csvWrite.close();

	return 0;
}
