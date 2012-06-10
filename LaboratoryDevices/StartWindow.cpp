#include "StartWindow.h"

StartWindow::StartWindow(QWidget *parent, Qt::WFlags flags) :
	QMainWindow(parent, flags)
{
	this->setFixedSize(600, 200);

	QGroupBox *gBox = new QGroupBox(this);
	QHBoxLayout *lay = new QHBoxLayout(gBox);
	lay->setMargin(0);

	QToolButton *compBut = new QToolButton(this);
	QToolButton *rendBut = new QToolButton(this);
	QToolButton *quitBut = new QToolButton(this);

	compBut->setIcon(QIcon("./Resources/user-admin.png"));
	compBut->setIconSize(QSize(128, 128));
	compBut->setToolTip(RUS("Àäìèíèñòğàòîğñêèé âõîä"));
	compBut->setAutoRaise(true);
	compBut->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	rendBut->setIcon(QPixmap("./Resources/users.png"));
	rendBut->setIconSize(QSize(128, 128));
	rendBut->setToolTip(RUS("Îáû÷íûé âõîä"));
	rendBut->setAutoRaise(true);
	rendBut->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	quitBut->setIcon(QPixmap("./Resources/right.png"));
	quitBut->setIconSize(QSize(128, 128));
	quitBut->setToolTip(RUS("Âûõîä"));
	quitBut->setAutoRaise(true);
	quitBut->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	connect(compBut, SIGNAL(clicked()), this, SLOT(hide()));
	connect(compBut, SIGNAL(clicked()), this, SLOT(ShowComposer()));
	connect(rendBut, SIGNAL(clicked()), this, SLOT(hide()));
	connect(rendBut, SIGNAL(clicked()), this, SLOT(ShowRender()));
	connect(quitBut, SIGNAL(clicked()), this, SIGNAL(quitSignal()));

	lay->addWidget(compBut);
	lay->addWidget(rendBut);
	lay->addWidget(quitBut);

	setCentralWidget(gBox);
}
StartWindow::~StartWindow() {
}
void StartWindow::ShowRender() {
	LabRender *render = new LabRender;
	render->show();
}
void StartWindow::ShowComposer() {
	LabComposer *composer = new LabComposer;
	composer->show();
}
void StartWindow::showSlot() {
	this->show();
	this->activateWindow();
	this->raise();
}