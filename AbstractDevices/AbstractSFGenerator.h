#ifndef ABSTRACTSFGENERATOR_H
#define ABSTRACTSFGENERATOR_H

#include <QWidget>

#include <AbstractDevice.h>
#include <AbstractIndicator.h>

class AbstractSFGenerator: public AbstractDevice
{
	Q_OBJECT

public:
	AbstractSFGenerator(QWidget *parent = 0, Qt::WindowFlags flags = 0) : AbstractDevice(parent, flags) {}
	~AbstractSFGenerator() {}

	
	DeviceType getDeviceType()
	{
		return DeviceType::DT_SFG;
	}

	void plugin(AbstractIndicator *_indic)
	{
		connect(this, SIGNAL(fst_changed(float)), _indic, SLOT(set_new_fst(float)));
		connect(this, SIGNAL(fen_changed(float)), _indic, SLOT(set_new_fen(float)));
		connect(this, SIGNAL(mark1_changed(float)), _indic, SLOT(set_new_mark1(float)));
		connect(this, SIGNAL(mark2_changed(float)), _indic, SLOT(set_new_mark2(float)));
	}

signals:
	void fst_changed(float new_val);
	void fen_changed(float new_val);
	void mark1_changed(float new_val);
	void mark2_changed(float new_val);

public slots:
	void set_new_fst(float new_val)
	{
		fst = new_val;
		emit fst_changed(new_val);
	}
	void set_new_fen(float new_val)
	{
		fen = new_val;
		emit fen_changed(new_val);
	}
	void set_new_mark1(float new_val)
	{
		mark1 = new_val;
		emit mark1_changed(new_val);
	}
	void set_new_mark2(float new_val)
	{
		mark2 = new_val;
		emit mark2_changed(new_val);
	}

	virtual void set_new_fst(int new_val) = 0;
	virtual void set_new_fen(int new_val) = 0;
	virtual void set_new_mark1(int new_val) = 0;
	virtual void set_new_mark2(int new_val) = 0;

protected:
	float fmin, fmax, fst, fen, mark1, mark2;
};

Q_DECLARE_INTERFACE(AbstractSFGenerator, "AbstractSFGenerator");

#endif