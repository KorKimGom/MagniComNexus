#include "dockclient.h"
#include "ui_dockclient.h"

DockClient::DockClient(QWidget *parent):
    QDockWidget(parent),
    ui(new Ui::DockClient)
{
    ui->setupUi(this);
}

DockClient::~DockClient()
{
    delete ui;
}
