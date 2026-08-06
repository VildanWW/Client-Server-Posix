#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <memory>
#include <QInputDialog>
#include <QSettings>
#include <QMessageBox>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>()) {
    ui->setupUi(this);

    client = new Client(this);

    connect(client, &Client::messageReceived, this, &MainWindow::onMessageReceived);
    connect(client, &Client::disconnected, this, &MainWindow::onClientDisconnected);

    connect(ui->sendButton, &QPushButton::pressed, this, &MainWindow::onSendButtonClicked);

    setSettingsUser();
}

MainWindow::~MainWindow() {}

bool MainWindow::setSettingsUser() {
    bool ok = false;
    QString userName = QInputDialog::getText(this, "Авторизация", "Введите Ваше имя для входа", QLineEdit::Normal, "", &ok);
    userName = userName.trimmed();

    if(!ok || userName.isEmpty()) {
        QMessageBox::warning(this, "Выход", "Без имени войти нельзя!");
        QCoreApplication::quit();
        return false;
    }

    if(client->connectToServer(ServerConfig::serverIp, userName)) {
        ui->chatTextEdit->append("<i style='color: green;'>Система: Соединение установлено!</i>");

        client->sendToData(DataType::TEXT, userName.toUtf8());
        client->sendToData(DataType::TEXT, QString("присоединился к чату!").toUtf8());
    }
    else {
        QMessageBox::critical(this, "Ошибка", "Сервер недоступен. Сначала запусти серверную часть!");
        QCoreApplication::quit();
        return false;
    }

    return true;
}

void MainWindow::onSendButtonClicked() {

    QString message = ui->messageLineEdit->text().trimmed();
    if(message.isEmpty()) return;

    if(client->sendToData(DataType::TEXT, message.toUtf8())) {
        appendMessageToChat("Вы", message, true);
        ui->messageLineEdit->clear();
    }
    else {
        ui->chatTextEdit->append("<i style='color: red;'>Система: Ошибка отправки.</i>");
    }
}

void MainWindow::onMessageReceived(DataType type, const QString &senderName, const QByteArray &data) {
    if(type == DataType::TEXT) {
        QString messageText = QString::fromUtf8(data);

        appendMessageToChat(senderName, messageText, false);
    }
}

void MainWindow::onClientDisconnected() {
    ui->chatTextEdit->append("<br><b style='color: red;'>Система: Связь с сервером разорвана.</b>");
}

void MainWindow::appendMessageToChat(const QString& sender, const QString& text, bool isMyMessage) {
    QString timeStr = QTime::currentTime().toString("HH:mm");
    QString html;

    if (isMyMessage) {
        html = QString(
                   "<div style='margin: 6px; text-align: right;'>"
                   "  <div style='display: inline-block; background-color: #EEFFDE; color: #000000; "
                   "              padding: 8px 14px; border-radius: 12px 12px 0px 12px; "
                   "              max-width: 70%; text-align: left; box-shadow: 0px 1px 1px rgba(0,0,0,0.1);'>"
                   "    <b>Вы:</b><br>%1"
                   "    <br><span style='color: #689f38; font-size: 10px; float: right; margin-top: 4px;'>%2</span>"
                   "  </div>"
                   "</div>"
                   ).arg(text.toHtmlEscaped(), timeStr);
    } else {
        html = QString(
                   "<div style='margin: 6px; text-align: left;'>"
                   "  <div style='display: inline-block; background-color: #FFFFFF; color: #000000; "
                   "              padding: 8px 14px; border-radius: 12px 12px 12px 0px; "
                   "              max-width: 70%; text-align: left; box-shadow: 0px 1px 1px rgba(0,0,0,0.1);'>"
                   "    <b style='color: #2b73b3;'>%1:</b><br>%2"
                   "    <br><span style='color: #8c8c8c; font-size: 10px; float: right; margin-top: 4px;'>%3</span>"
                   "  </div>"
                   "</div>"
                   ).arg(sender.toHtmlEscaped(), text.toHtmlEscaped(), timeStr);
    }
    ui->chatTextEdit->append(html);
}


