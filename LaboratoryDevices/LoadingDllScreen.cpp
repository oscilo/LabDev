#include "LoadingDllScreen.h"

LoadingDllScreen::LoadingDllScreen(QWidget *parent) :
	QSplashScreen(parent)
{
	this->setFixedSize(400, 50);

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->setMargin(5);

	QGroupBox *gbox = new QGroupBox(this);

	QVBoxLayout *glay = new QVBoxLayout(gbox);
	glay->setMargin(0);

	lbl = new QLabel(this);
	glay->addWidget(lbl);

	lay->addWidget(gbox);
}
void LoadingDllScreen::showMessage(const QString &message, int alignment, const QColor &color)
{
	lbl->setText(message);
	lbl->setAlignment((Qt::Alignment)alignment);

	emit messageChanged(lbl->text());
}
void LoadingDllScreen::clearMessage()
{
	lbl->setText("");

	emit messageChanged(lbl->text());
}