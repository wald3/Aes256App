#include <thread>
#include <chrono>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "passwordwindow.h"

#include <qdebug.h>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>

#include "FileAES.h"
#include "Aes256.h"

using namespace std::chrono_literals;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->statusBar()->hide();
    this->ui->progressBar->setVisible(false);
    disableButtons(false, false);
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

    if (!path.isEmpty())
    {
        this->filePath = path;
        this->ui->fileNameLabel->setText("File name : " + path);

        std::string _path = this->filePath.toUtf8().constData();
        int pos = _path.find(".aes");
        bool isEncryptedFile = pos > 0 && pos < _path.size() ? true : false;

        if (isEncryptedFile) {
            disableButtons(false, true);
        }
        else {
            disableButtons(true, false);
        }
    }
}

void MainWindow::disableButtons(bool isEncrypt, bool isDecrypt)
{
    this->ui->encryptBtn->setEnabled(isEncrypt);
    this->ui->DecryptBtn->setEnabled(isDecrypt);
}

void MainWindow::setFromEnable(bool isDisabled)
{
    this->ui->DecryptBtn->setEnabled(isDisabled);
    this->ui->encryptBtn->setEnabled(isDisabled);
    this->ui->chooseFileBtn->setEnabled(isDisabled);
}

void MainWindow::fillProgressBar()
{
    this->password->close();
    this->show();

    setFromEnable(false);
    this->ui->progressBar->setVisible(true);
    this->ui->progressBar->setValue(0);

    for (int i = 0; i < 101; i++)
    {
        this->ui->progressBar->setValue(i);
        std::this_thread::sleep_for(50ms);
    }

    this->ui->progressBar->setVisible(false);
    setFromEnable(true);
    disableButtons(false, false);
}

void MainWindow::recieveData(bool isEncrypt, ByteArray key)
{
    if (!key.empty()) {

        //ByteArray key_example = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

        if (isEncrypt) {
            qDebug() << "enctrypt Ok";
            auto answer = QMessageBox::question(this, "Are you sure want to encrypt the file?",
                "\nAfter encryption file will be permanently"
                " encrypted, you can only decrypt it with the"
                " same MODE and PASSWORD!");
            if (answer == QMessageBox::Yes)
            {
                encrypt(key, this->filePath.toUtf8().constData());
                fillProgressBar();
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
                fillProgressBar();
            }
        }
        
        this->filePath = "";
        this->ui->fileNameLabel->setText("File name : ");

    }
}

void MainWindow::encrypt(ByteArray key, std::string path)
{
    ByteArray txt, enc, dec;
    FileAES file(path), file2(path+".aes");

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