#ifndef DOCKCLIENT_H
#define DOCKCLIENT_H

#include <QDockWidget>

namespace Ui { class DockClient; }

class DockClient : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockClient(QWidget *parent = nullptr);
    ~DockClient();

private:
    Ui::DockClient *ui;
};

#endif // DOCKCLIENT_H
