#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupMenu();  // 메뉴 설정 함수

private:
    Ui::MainWindow *ui;

    QMenu *protocolsMenu;
    QMenu *menuNetwork;
    QAction *actionExit;
    QList<QAction*> removeActions;
};
#endif // MAINWINDOW_H
