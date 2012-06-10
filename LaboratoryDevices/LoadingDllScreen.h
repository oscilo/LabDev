#ifndef LOADINGDLLSCREEN_H
#define LOADINGDLLSCREEN_H

#include "globals.h"

class LoadingDllScreen : public QSplashScreen {
	Q_OBJECT

public:
	LoadingDllScreen(QWidget*);

public slots:
	void showMessage(const QString &message, int alignment = Qt::AlignLeft, const QColor &color = Qt::black);
	void clearMessage();

private:
	QLabel *lbl;
};

#endif