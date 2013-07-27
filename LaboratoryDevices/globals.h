#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <QApplication>

#include <QCloseEvent>

#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <QToolButton>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

#include <QVBoxLayout>

#include <QMdiArea>
#include <QMdiSubWindow>

#include <QMessageBox>

#include <QFileDialog>
#include <QDir>

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QSignalMapper>

#include <QPluginLoader>

#include <QSplashScreen>

#include <QDesktopServices>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QStringList>
#include <QVector>

#define RUS QString::fromLocal8Bit

typedef QMap<QString, QVariant> DBRecord;
typedef QList<DBRecord> DBRecordList;

#endif