#ifndef ABSTRACTINDICATOR_H
#define ABSTRACTINDICATOR_H

#include <QWidget>

#include <AbstractDrawArea.h>
#include <AbstractDevice.h>

class AbstractIndicator : public AbstractDevice
{
	Q_OBJECT

public:
	AbstractIndicator(AbstractDrawArea *draw_ptr, QWidget *parent = 0, Qt::WindowFlags flags = 0)
			: AbstractDevice(parent, flags)
	{
		draw = draw_ptr;
		draw->setInputSignals(&this->inputSignals);
		connect(this, SIGNAL(fst_changed(float)), draw, SLOT(set_new_fst(float)));
		connect(this, SIGNAL(fen_changed(float)), draw, SLOT(set_new_fen(float)));
		connect(this, SIGNAL(mark1_changed(float)), draw, SLOT(set_new_mark1(float)));
		connect(this, SIGNAL(mark2_changed(float)), draw, SLOT(set_new_mark2(float)));
		connect(this, SIGNAL(mark_h_changed(float)), draw, SLOT(set_new_mark_h(float)));
		connect(this, SIGNAL(m_line_changed(float)), draw, SLOT(set_new_m_line(float)));
		connect(this, SIGNAL(lim_changed(float)), draw, SLOT(set_new_lim(float)));
	}
	~AbstractIndicator() {}

	DeviceType getDeviceType()
	{
		return DeviceType::DT_INDICATOR;
	}

signals:
	void fst_changed(float new_val);
	void fen_changed(float new_val);
	void mark1_changed(float new_val);
	void mark2_changed(float new_val);
	void mark_h_changed(float new_val);
	void m_line_changed(float new_val);
	void lim_changed(float new_val);

public slots:
	void set_new_fst(float new_val)
	{
		emit fst_changed(new_val);
	}
	void set_new_fen(float new_val)
	{
		emit fen_changed(new_val);
	}
	void set_new_mark1(float new_val)
	{
		emit mark1_changed(new_val);
	}
	void set_new_mark2(float new_val)
	{
		emit mark2_changed(new_val);
	}
	void set_new_mark_h(float new_val)
	{
		emit mark_h_changed(new_val);
	}
	void set_new_m_line(float new_val)
	{
		emit m_line_changed(new_val);
	}
	void set_new_lim(float new_val)
	{
		emit lim_changed(new_val);
	}

protected:
	AbstractDrawArea *draw;
};

Q_DECLARE_INTERFACE(AbstractIndicator, "AbstractIndicator");

#endif // ABSTRACTINDICATOR_H
