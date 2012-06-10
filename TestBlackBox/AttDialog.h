#ifndef ATT_DIALOG_H
#define ATT_DIALOG_H

#include "globals.h"

class AttDialog : public QDialog
{
	Q_OBJECT

public:
	AttDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
	~AttDialog();

signals:
	void set_new_alpha(int);

public slots:
	void changeType();
	void alpha_changed(int);
	void start_synthesis();

private:
	void paintEvent(QPaintEvent * event);

	int N;
	float R0, R1, R2, alpha;

	QRadioButton *tType, *pType;
	QPixmap *pix, *tpix, *ppix;
	QComboBox *cbN;
	QLineEdit *leR0;
	QLabel *lR1, *lR2, *lAlpha;
	QSlider *sAlpha;
};

#endif