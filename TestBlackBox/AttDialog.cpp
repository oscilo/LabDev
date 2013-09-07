#include "AttDialog.h"

AttDialog::AttDialog(QWidget *parent, Qt::WindowFlags f) :
	QDialog(parent, f)
{
	tpix = new QPixmap("./Resources/TAtten.png");
	ppix = new QPixmap("./Resources/PAtten.png");
	pix = tpix;
	this->setMinimumHeight(pix->height());

	QGridLayout *grTop = new QGridLayout(this);
	QGroupBox *grBox = new QGroupBox(RUS("Parameters"));			//DS

	QGridLayout *gr = new QGridLayout(grBox);
	
	tType = new QRadioButton(RUS("Т-shape"));			//DS
	pType = new QRadioButton(RUS("П-shape"));			//DS
	QLabel *lR0 = new QLabel(RUS("R0, Ohm"));
	leR0 = new QLineEdit("50");
	QLabel *llR1 = new QLabel(RUS("R1, Ohm"));
	lR1 = new QLabel(RUS("-"));
	QLabel *llR2 = new QLabel(RUS("R2, Ohm"));
	lR2 = new QLabel(RUS("-"));
	lAlpha = new QLabel(RUS("Attenuation: 0 dB"));
	sAlpha = new QSlider(Qt::Horizontal);
	QLabel *lN = new QLabel(RUS("Number of Sections"));
	cbN = new QComboBox;

	tType->setChecked(true);

	sAlpha->setRange(0, 300);
	sAlpha->setValue(0);

	cbN->addItems(QStringList() << "1" << "2" << "3" << "4");

	gr->addWidget(tType,	0, 1);
	gr->addWidget(pType,	0, 2);
	gr->addWidget(lR0,		1, 1);
	gr->addWidget(leR0,		1, 2);
	gr->addWidget(llR1,		2, 1);
	gr->addWidget(lR1,		2, 2);
	gr->addWidget(llR2,		3, 1);
	gr->addWidget(lR2,		3, 2);
	gr->addWidget(lN,		4, 1);
	gr->addWidget(cbN,		4, 2);
	gr->addWidget(lAlpha,	5, 1);
	gr->addWidget(sAlpha,	5, 2);
	
	QPushButton *sint = new QPushButton(RUS("Calculate"));
	
	grTop->addWidget(grBox, 0, 1, 1, 2);
	grTop->addWidget(sint,	1, 2);
	grTop->setColumnMinimumWidth(0, pix->width());

	connect(tType, SIGNAL(clicked(bool)), this, SLOT(changeType()));
	connect(pType, SIGNAL(clicked(bool)), this, SLOT(changeType()));
	connect(sAlpha, SIGNAL(valueChanged(int)), this, SLOT(alpha_changed(int)));
	connect(sint, SIGNAL(clicked(bool)), this, SLOT(start_synthesis()));
}
AttDialog::~AttDialog() {
}
void AttDialog::paintEvent(QPaintEvent *event)
{
	QPainter pntr(this);
	
	pntr.drawPixmap(0, 0, *pix);
}
void AttDialog::changeType()
{
	if(tType->isChecked())
		pix = tpix;
	else
		pix = ppix;

	this->repaint();
}
void AttDialog::alpha_changed(int new_val)
{
	lAlpha->setText(RUS("Затухание: %1 дБ").arg(new_val/10.));
}
void AttDialog::start_synthesis()
{
	N = cbN->currentText().toInt();

	alpha = sAlpha->value()/10.;
	R0 = leR0->text().toFloat();

	float alpha_one = alpha/((float)N);

	float k0 = pow(10, alpha/20);
	float k1 = (k0 - 1) / (k0 + 1);
	float k2 = 2*k0 / (k0*k0 - 1);

	if(tType->isChecked())
	{
		R1 = R0 * k1;
		R2 = R0 * k2;
	}
	else
	{
		R1 = R0 / k1;
		R2 = R0 / k2;
	}

	lR1->setText(QString("%1").arg(R1));
	lR2->setText(QString("%1").arg(R2));

	emit set_new_alpha(sAlpha->value());
}