#pragma once
#ifndef PASSWORDWINDOW_H
#define PASSWORDWINDOW_H

#include <QDialog>

namespace Ui {
    class passwordWindow;
}

class passwordWindow : public QDialog
{
    Q_OBJECT

public:
    explicit passwordWindow(bool isEncrypt, QWidget* parent = nullptr);
    ~passwordWindow();

protected:
    void changeEvent(QEvent* e);

signals:
    void sendData(bool isEncrypt, QString str);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::passwordWindow* ui;
    bool isEncrypt;
};

#endif // PASSWORDWINDOW_H
