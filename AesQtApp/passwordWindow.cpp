#include "passwordwindow.h"
#include "ui_passwordwindow.h"
#include "mainwindow.h"

#include "QMessageBox"

passwordWindow::passwordWindow(bool _isEncrypt, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::passwordWindow)
{
    this->isEncrypt = _isEncrypt;
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
    ui->passwordConfirmed->setEchoMode(QLineEdit::Password);
    connect(this, SIGNAL(sendData(bool, ByteArray)), parent, SLOT(recieveData(bool, ByteArray)));
    connect(this, SIGNAL(sendData(bool, ByteArray)), parent, SLOT(show()));
}

passwordWindow::~passwordWindow()
{
    delete ui;
}

void passwordWindow::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

ByteArray passwordWindow::str2ByteArray(QString s)
{
    ByteArray bytes;
    std::string str = s.toUtf8().constData();
    std::copy(str.begin(), str.end(), std::back_inserter(bytes));

    return bytes;
}

void passwordWindow::on_pushButton_clicked()
{
    if (ui->password->text() == ui->passwordConfirmed->text())
    {
        auto bytes = str2ByteArray(ui->password->text());
        emit sendData(this->isEncrypt, bytes);
        this->close();
    }
    else {
        QMessageBox::warning(this, "Error", "Passwords aren`t equals!");
    }
}

void passwordWindow::on_pushButton_2_clicked()
{
    emit sendData(this->isEncrypt, {});
    this->close();
}
