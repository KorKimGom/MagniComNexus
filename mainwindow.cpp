#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupMenus();

    // 메뉴 클릭 시를 감지하기 위해 이벤트 필터 등록
    ui->menuServer->installEventFilter(this);
    ui->menuClient->installEventFilter(this);

    // 초기 위젯 백업 (처음에 비워낼 때 필요)
    lServer = ui->lServer;
    lClient = ui->lClient;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dockServer;
    delete dockClient;
}

void MainWindow::setupMenus()
{
    QList<QAction*> actions = ui->menuProtocols->actions();
    QList<QAction*> toRemove;
    bool foundNetwork = false;

    for (QAction *action : actions)
    {
        if (action == ui->menuNetwork->menuAction())
        {
            foundNetwork = true;
            continue;
        }

        if (foundNetwork && action != ui->actionExit)
            toRemove.append(action);
    }

    for (QAction *action : toRemove)
        ui->menuProtocols->removeAction(action);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (watched == ui->menuServer)
        {
            onMenuServerClicked();
            return true;
        }
        else if (watched == ui->menuClient)
        {
            onMenuClientClicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::showDockInLayout(QWidget *dockWidget, QWidget *targetPanel, QWidget *&backupWidget)
{
    content = dockWidget->findChild<QWidget*>("dockWidgetContents");
    if (!content) return;

    layout = targetPanel->layout();
    if (!layout) return;

    if (backupWidget && layout->indexOf(backupWidget) != -1)    // 기존 백업 위젯 제거
    {
        layout->removeWidget(backupWidget);
        backupWidget->deleteLater();
        backupWidget = nullptr;
    }

    prevParent = content->parentWidget();    // 이전 부모에서 제거 (다른 패널에서 사용했을 수 있음)
    if (prevParent && prevParent != targetPanel)
    {
        QLayout *oldLayout = prevParent->layout();
        if (oldLayout) oldLayout->removeWidget(content);
    }

    content->setParent(targetPanel);    // 부모 설정 및 중복 방지 후 삽입

    if (layout->indexOf(content) == -1)
        layout->addWidget(content);
}

void MainWindow::onMenuServerClicked()
{
    if (!dockServer)
        dockServer = new DockServer(this);

    showDockInLayout(dockServer, ui->pSever, lServer);
    ui->stackedWidget->setCurrentWidget(ui->pSever);
}

void MainWindow::onMenuClientClicked()
{
    if (!dockClient)
        dockClient = new DockClient(this);

    showDockInLayout(dockClient, ui->pClient, lClient);
    ui->stackedWidget->setCurrentWidget(ui->pClient);
}
