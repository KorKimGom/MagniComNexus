#ifndef DOCKSERVER_H
#define DOCKSERVER_H

#include <QDockWidget>

namespace Ui { class DockServer; }

class DockServer : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockServer(QWidget *parent = nullptr);
    ~DockServer();

private:
    Ui::DockServer *ui;
};

#endif // DOCKSERVER_H
