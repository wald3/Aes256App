#pragma once
#ifndef PASSWORDWINDOW_H
#define PASSWORDWINDOW_H

#include <QDialog>

typedef std::vector<unsigned char> ByteArray;

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
    void sendData(bool isEncrypt, ByteArray str);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    ByteArray str2ByteArray(QString s);

private:
    Ui::passwordWindow* ui;
    bool isEncrypt;
};

#endif // PASSWORDWINDOW_H