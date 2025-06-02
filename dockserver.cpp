#include "dockserver.h"
#include "ui_dockserver.h"

DockServer::DockServer(QWidget *parent):
    QDockWidget(parent),
    ui(new Ui::DockServer)
{
    ui->setupUi(this);
}

DockServer::~DockServer()
{
    delete ui;
}
