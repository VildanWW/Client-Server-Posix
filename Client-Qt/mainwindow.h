#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    std::unique_ptr<Ui::MainWindow> ui;

    Client* client = nullptr;

    //void onMessageReceived(DataType type, const QByteArray& data);
    //void onClientDisconnected();
};
#endif // MAINWINDOW_H
