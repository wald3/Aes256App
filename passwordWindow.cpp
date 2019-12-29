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
    connect(this, SIGNAL(sendData(bool, QString)), parent, SLOT(recieveData(bool, QString)));
    connect(this, SIGNAL(sendData(bool, QString)), parent, SLOT(show()));
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

void passwordWindow::on_pushButton_clicked()
{
    if (ui->password->text() == ui->passwordConfirmed->text())
    {
        emit sendData(this->isEncrypt, ui->password->text());
        this->close();
    }
    else {
        QMessageBox::warning(this, "Error", "Passwords aren`t equals!");
    }
}

void passwordWindow::on_pushButton_2_clicked()
{
    emit sendData(this->isEncrypt, nullptr);
    this->close();
}
