#include "mainwindow.h"
#include "ui_mainwindow.h"
<<<<<<< Updated upstream
=======
#include <QMessageBox>
#include "ATM/Model/atmcard2.h"
#include "ATM/atm.h"
#include "ATM/Socket/atmselectorsocket.h"
#include "ATM/Model/atmparams.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"

>>>>>>> Stashed changes

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
<<<<<<< Updated upstream
=======
    //ui->mainStackedWidget->addWidget()
    ui->lineEdit_cardNum->setInputMask("9999-9999-9999-9999");
    ui->lineEdit_anotherCardNum->setInputMask("9999-9999-9999-9999");
    ui->lineEdit_PIN->setInputMask("9999");
    ui->lineEdit_attemptNum->setInputMask("9");
>>>>>>> Stashed changes
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_insertButton_page0_clicked()
{
<<<<<<< Updated upstream
     ui->mainStackedWidget->setCurrentIndex(1);
}
=======
    ui->mainStackedWidget->setCurrentIndex(4);
}

//page 2 -- enter card number after insertion
void MainWindow::on_num1_4_clicked()
{
    ui->lineEdit_cardNum->insert("1");
}

void MainWindow::on_num2_4_clicked()
{
    ui->lineEdit_cardNum->insert("2");
}

void MainWindow::on_num3_4_clicked()
{
    ui->lineEdit_cardNum->insert("3");
}

void MainWindow::on_num4_4_clicked()
{
    ui->lineEdit_cardNum->insert("4");
}

void MainWindow::on_num5_4_clicked()
{
    ui->lineEdit_cardNum->insert("5");
}

void MainWindow::on_num6_4_clicked()
{
    ui->lineEdit_cardNum->insert("6");
}

void MainWindow::on_num7_4_clicked()
{
    ui->lineEdit_cardNum->insert("7");
}

void MainWindow::on_num8_4_clicked()
{
    ui->lineEdit_cardNum->insert("8");
}

void MainWindow::on_num9_4_clicked()
{
    ui->lineEdit_cardNum->insert("9");
}

void MainWindow::on_num0_4_clicked()
{
    ui->lineEdit_cardNum->insert("0");
}

void MainWindow::on_clearOne_4_clicked()
{
    ui->lineEdit_cardNum->backspace();
}

void MainWindow::on_clearButton_page4_clicked()
{
    ui->lineEdit_cardNum->clear();
}

void MainWindow::on_okButton_page4_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_backButton_page4_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(0);
}


//page enter PIN after card insertion (3d page)
void MainWindow::on_num1_2_clicked()
{
    ui->lineEdit_PIN->insert("1");
}

void MainWindow::on_num2_2_clicked()
{
    ui->lineEdit_PIN->insert("2");
}

void MainWindow::on_num3_2_clicked()
{
    ui->lineEdit_PIN->insert("3");
}

void MainWindow::on_num4_2_clicked()
{
    ui->lineEdit_PIN->insert("4");
}

void MainWindow::on_num5_2_clicked()
{
    ui->lineEdit_PIN->insert("5");
}

void MainWindow::on_num6_2_clicked()
{
    ui->lineEdit_PIN->insert("6");
}

void MainWindow::on_num7_2_clicked()
{
    ui->lineEdit_PIN->insert("7");
}

void MainWindow::on_num8_2_clicked()
{
    ui->lineEdit_PIN->insert("8");
}

void MainWindow::on_num9_2_clicked()
{
    ui->lineEdit_PIN->insert("9");
}

void MainWindow::on_num0_2_clicked()
{
    ui->lineEdit_PIN->insert("0");
}

void MainWindow::on_clearOne_clicked()
{
    ui->lineEdit_PIN->backspace();
}

void MainWindow::on_backButton_page2_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_okButton_page2_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}
//сделать проверку пина
//сделать скрытие пина под *
void MainWindow::on_lineEdit_PIN_editingFinished()//проаерка пина
{
    //connect()
}



//page 4 -- main card menu
void MainWindow::on_cashTransfersButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_cardBalanceButton_clicked()
{


//        QMessageBox::information(this,"Баланс по картці", ATM::checkBal() );
//        QMessageBox msgBox;
//        msgBox.setText("The document has been modified.");
}
//--------------------------------------------------------


void MainWindow::on_cardSettingsButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(7);
}



void MainWindow::on_cashWithdrawalButton_clicked()
{
     ui->mainStackedWidget->setCurrentIndex(5);
}


//page 7 -- settings
void MainWindow::on_Button_changePin_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_Button_otherATMs_clicked()
{

}
>>>>>>> Stashed changes
