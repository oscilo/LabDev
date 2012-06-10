#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include "globals.h"

#include "LabRender.h"
#include "LabComposer.h"

class StartWindow : public QMainWindow {
	Q_OBJECT

public:
	StartWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~StartWindow();

signals:
	void quitSignal();

public slots:
	void showSlot();

private slots:
	void ShowRender();
	void ShowComposer();
};

#endif