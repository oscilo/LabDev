#include "TestBlackBox.h"

TestBlackBox::TestBlackBox(QWidget *parent, Qt::WFlags flags) :
	AbstractBlackBox(parent, flags)
{
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", new BlackBoxFunc);

	att = new AttDialog();
	lbl1 = new QLabel(RUS("Затухание: 0 дБ"));

	lbl1->setMinimumWidth(lbl1->fontMetrics().size(Qt::TextSingleLine, RUS("Затухание: 30,9 дБ")).width());

	att_show = new QCheckBox(RUS("Показать аттенюатор"));
	att_show->setChecked(false);

	sldr = new QSlider(Qt::Horizontal);

	sldr->setRange(0, 300);
	sldr->setValue(0);

	grp = new QGroupBox(RUS("Аттенюатор"));

	lay = new QHBoxLayout(grp);
	
	lay->addWidget(lbl1);
	lay->addWidget(sldr);
	lay->addWidget(att_show);

	hlay = new QHBoxLayout(this);

	hlay->addWidget(grp);

	////////////////////////////////////////////////////////
	connect(sldr, SIGNAL(valueChanged(int)), this, SLOT(set_new_k(int)));
	connect(att, SIGNAL(set_new_alpha(int)), this, SLOT(set_new_k(int)));
	connect(att_show, SIGNAL(stateChanged(int)), this, SLOT(show_hide_att(int)));
	////////////////////////////////////////////////////////
}
QString TestBlackBox::getDeviceName()
{
	return QString::fromLocal8Bit("Фильтр");
}
void TestBlackBox::set_new_k(int new_val)
{
	((BlackBoxFunc*)this->getSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal"))->set_k(new_val/10.);
	lbl1->setText(RUS("Затухание: %1 дБ").arg(new_val/10.));
	if(sldr->value() != new_val)
		sldr->setValue(new_val);

	emit repaintSignal();
}
void TestBlackBox::show_hide_att(int state)
{
	switch(state)
	{
		case Qt::Unchecked:
			att->hide();
			break;

		case Qt::Checked:
			att->show();
			break;
	}
}

Q_EXPORT_PLUGIN2(TestBlackBox, TestBlackBox);