#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "passwordwindow.h"

#include <qdebug.h>
#include "QFileDialog"
#include "QIcon"
#include "QMessageBox"

#include "FileAES.h"
#include "Aes256.h"

std::string DATA    = "C:/Users/Vladislav/Desktop/data.txt";
std::string ENCRYPT = "C:/Users/Vladislav/Desktop/encrypted.txt";
std::string DECRYPT = "C:/Users/Vladislav/Desktop/decrypted.txt";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->statusBar()->hide();
    this->ui->progressBar->setVisible(false);
    disableButtons(false, false);
}

void MainWindow::disableButtons(bool isEncrypt, bool isDecrypt)
{
    this->ui->encryptBtn->setEnabled(isEncrypt);
    this->ui->DecryptBtn->setEnabled(isDecrypt);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openPasswordWindow(bool b)
{
    password = new passwordWindow(b, this);
    password->show();
}

void MainWindow::on_encryptBtn_clicked()
{
    this->hide();
    openPasswordWindow(true);
}

void MainWindow::on_DecryptBtn_clicked()
{
    this->hide();
    openPasswordWindow(false);
}

void MainWindow::on_chooseFileBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose file", "C://");
    this->filePath = path;
    this->ui->fileNameLabel->setText("File name : " + path);

    std::string _path = this->filePath.toUtf8().constData();
    int pos = _path.find(".aes");
    bool isEncryptedFile = pos > 0 && pos < _path.size() ? true : false;

    if (isEncryptedFile){
        disableButtons(false, true);
    }
    else {
        disableButtons(true, false);
    }


}

void MainWindow::recieveData(bool isEncrypt, QString str)
{
    if (str != nullptr) {

        ByteArray key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

        if (isEncrypt) {
            qDebug() << "enctrypt Ok";
            auto answer = QMessageBox::question(this, "Are you sure want to encrypt the file?",
                "\nAfter encryption file will be permanently"
                " encrypted, you can only decrypt it with the"
                " same MODE and PASSWORD!");
            if (answer == QMessageBox::Yes)
            {
                encrypt(key, this->filePath.toUtf8().constData());
                disableButtons(false, false);
            }
        }
        else {
            qDebug() << "dectrypt Ok";
            auto answer = QMessageBox::question(this, "Are you sure want to decrypt the file?",
                "\nTo decrypt the file you must be confident"
                " that MODE and PASSWORD are the same as when it be crypted!");
            if (answer == QMessageBox::Yes)
            {
                decrypt(key, this->filePath.toUtf8().constData());
                disableButtons(false, false);
            }
        }
        this->filePath = "";
        this->ui->fileNameLabel->setText("File name : ");

    }
}

void MainWindow::encrypt(ByteArray key, std::string path)
{
    ByteArray txt, enc;
    FileAES file(path);

    txt = file.ReadFile();
    Aes256::encrypt(key, txt, enc);

    file.WriteToFile(enc, true);
}

void MainWindow::decrypt(ByteArray key, std::string path)
{
    ByteArray enc, dec;
    FileAES file(path);

    enc = file.ReadFile();
    Aes256::decrypt(key, enc, dec);

    file.WriteToFile(dec, false);
}