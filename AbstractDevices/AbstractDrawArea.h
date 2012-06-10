#ifndef ABSTRACTDRAWAREA_H
#define ABSTRACTDRAWAREA_H

#include <QWidget>

#include <AbstractFunc.h>

class AbstractDrawArea : public QWidget
{
	Q_OBJECT

public:
	AbstractDrawArea(QWidget *parent = 0, Qt::WFlags flags = 0) :	QWidget(parent, flags) {}
	~AbstractDrawArea(){}
	
	void setInputSignals(SignalsMap *_inputSignals)
	{
		inputSignals = _inputSignals;
	}

public slots:
	void set_new_fst(float new_val)
	{
		fst = new_val;
		this->repaint();
	}
	void set_new_fen(float new_val)
	{
		fen = new_val;
		this->repaint();
	}
	void set_new_mark1(float new_val)
	{
		mark1 = new_val;
		this->repaint();
	}
	void set_new_mark2(float new_val)
	{
		mark2 = new_val;
		this->repaint();
	}
	void set_new_mark_h(float new_val)
	{
		mark_h = new_val;
		this->repaint();
	}
	void set_new_m_line(float new_val)
	{
		m_line = new_val;
		this->repaint();
	}
	void set_new_lim(float new_val)
	{
		lim = new_val;
		this->repaint();
	}

protected:
	float fst, fen, mark1, mark2, mark_h, m_line, lim;
	//AbstractFunc *func_to_visualise;

	SignalsMap *inputSignals;
};

#endif