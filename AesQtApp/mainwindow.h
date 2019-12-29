#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "passwordwindow.h"

typedef std::vector<unsigned char> ByteArray;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void disableButtons(bool encrypt, bool decrypt);

    void openPasswordWindow(bool b);

    void on_encryptBtn_clicked();

    void on_chooseFileBtn_clicked();

    void encrypt(ByteArray key, std::string path);

    void decrypt(ByteArray key, std::string path);

    void on_DecryptBtn_clicked();

    void setFromEnable(bool isDisabled);

    void fillProgressBar();

public slots:
    void recieveData(bool isEncrypt, ByteArray str);

private:
    Ui::MainWindow* ui;
    passwordWindow* password;
    QString filePath;
};
#endif // MAINWINDOW_H
