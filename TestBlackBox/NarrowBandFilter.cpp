#include "NarrowBandFilter.h"

////////////////////////////////////////////////////////
class PaintingWidget : public QWidget {
public:
	PaintingWidget(QWidget* parent = 0) : QWidget(parent) {
		pix = QPixmap("./Resources/black_box.png");
		pix = pix.scaledToWidth(pix.width()/1.5, Qt::SmoothTransformation);

		this->setFixedSize(pix.size());
	}
private:
	void paintEvent(QPaintEvent *e) {
		QPainter pntr(this);
		pntr.drawPixmap(0, 0, pix);
	}

	QPixmap pix;
};
////////////////////////////////////////////////////////

NarrowBandFilter::NarrowBandFilter(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", new NarrowBandFilterFunc);

	/*
	att = new AttDialog(this);
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
	//*/

	QGridLayout *lay = new QGridLayout(this);

	QSlider *sliderFront = new QSlider(Qt::Vertical, this);
	QSlider *sliderBottom = new QSlider(Qt::Vertical, this);
	QLabel *lTopFront = new QLabel(RUS("h1"), this);
	QLabel *lNtopFront = new QLabel(RUS("8 мм"), this);
	QLabel *lTopBottom = new QLabel(RUS("h2"), this);
	QLabel *lNtopBottom = new QLabel(RUS("-1 мм"), this);

	PaintingWidget *w = new PaintingWidget(this);

	lay->addWidget(lTopFront, 0, 0);
	lay->addWidget(lNtopFront, 2, 0);
	lay->addWidget(lTopBottom, 0, 1);
	lay->addWidget(lNtopBottom, 2, 1);
	lay->addWidget(sliderFront, 1, 0);
	lay->addWidget(sliderBottom, 1, 1);
	lay->addWidget(w, 0, 2, 3, 1);

	/*///////////////////////////////////////////////////////
	connect(sldr, SIGNAL(valueChanged(int)), this, SLOT(set_new_k(int)));
	connect(att, SIGNAL(set_new_alpha(int)), this, SLOT(set_new_k(int)));
	connect(att_show, SIGNAL(stateChanged(int)), this, SLOT(show_hide_att(int)));
	//*//////////////////////////////////////////////////////
}
QString NarrowBandFilter::getDeviceName()
{
	return QString::fromLocal8Bit("Фильтр");
}
QString NarrowBandFilter::getDeviceIDName() {
	return "NarrowBandFilter";
}
/*
void NarrowBandFilter::set_new_k(int new_val)
{
	((NarrowBandFilterFunc*)this->getSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal"))->set_k(new_val/10.);
	lbl1->setText(RUS("Затухание: %1 дБ").arg(new_val/10.));
	if(sldr->value() != new_val)
		sldr->setValue(new_val);

	emit repaintSignal();
}
void NarrowBandFilter::show_hide_att(int state)
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
//*/