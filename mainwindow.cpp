#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupMenu();  // 메뉴 설정 실행
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenu()
{
    protocolsMenu = this->menuBar()->findChild<QMenu*>("menuProtocols");
    if (!protocolsMenu) return;

    menuNetwork = protocolsMenu->findChild<QMenu*>("menuNetwork");
    actionExit = protocolsMenu->findChild<QAction*>("actionExit");

    if (!actionExit)
    {
        actionExit = new QAction("Exit", this);
        actionExit->setObjectName("actionExit");
        protocolsMenu->addAction(actionExit);
    }

    bool foundNetwork = false;
    QList<QAction*> removeActions;

    const QList<QAction*> actionsList = protocolsMenu->actions();       // `qAsConst()` 대신 `const auto&` 사용하여 오류 방지
    for (const auto& act : actionsList)
    {
        if (act == menuNetwork->menuAction())
        {
            foundNetwork = true;
            continue;
        }
        else if (act == actionExit) break;
        else if (foundNetwork) removeActions.append(act);
    }

    for (const auto& act : removeActions)
        protocolsMenu->removeAction(act);

    if (menuNetwork)
        menuNetwork->addAction(actionExit);
}
