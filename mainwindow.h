#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QLayout>
#include <QWidget>

#include "dockserver.h"
#include "dockclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::MainWindow *ui;

    // Server/Client DockWidget
    DockServer *dockServer = nullptr;
    DockClient *dockClient = nullptr;

    // Dock 내부 콘텐츠를 안전하게 추출하는 함수
    QWidget* getDockContent(QWidget *dock);
    QWidget *lServer = nullptr;   // ui->lServer 백업용
    QWidget *lClient = nullptr;   // ui->lClient 백업용

    QWidget *content;
    QWidget *prevParent;
    QLayout *layout;

    void setupMenus();                  // 불필요한 메뉴 제거
    void showDockInLayout(QWidget *dockWidget, QWidget *targetPanel, QWidget *&backupWidget);
    void onMenuServerClicked();         // 메뉴 Server 클릭 시
    void onMenuClientClicked();         // 메뉴 Client 클릭 시
};

#endif // MAINWINDOW_H
