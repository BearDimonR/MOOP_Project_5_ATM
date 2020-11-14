#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ATM/Model/atmcard2.h"
#include "ATM/atm.h"
#include "ATM/Socket/atmselectorsocket.h"
#include "ATM/Model/atmparams.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"

//    card_(atm->card()),
//    par_(atm->par()),
//    atm_(atm)



MainWindow::MainWindow(const size_t id, QWidget *parent) :
    QMainWindow(parent),
    atm_(new ATM(id)),
    ui(new Ui::MainWindow)

{
    qDebug() << "pidar";

    ui->setupUi(this);
    //startSelector();
    //ui->mainStackedWidget->addWidget(ATMSelectorWidget::ui());

    ui->lineEdit_cardNum->setInputMask("9999-9999-9999-9999");
    ui->lineEdit_anotherCardNum->setInputMask("9999-9999-9999-9999");
    ui->lineEdit_telephoneNum->setInputMask("999-999-99-99");
    ui->lineEdit_enterSum->setInputMask("9999999");//поставити обмеження на картку
    //ui->lineEdit_PIN->setInputMask("9999");//поставити обмеження на ПІН в чотири символи
    ui->lineEdit_PIN->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_attemptNum->setInputMask("9");

}


//void MainWindow::startSelector()
//{
//    ATMSelector selector;
//    ATMSelectorWidget widget0(&selector);
//    ui->mainStackedWidget->insertWidget(8,&widget0);
//}

MainWindow::~MainWindow()
{
    delete ui;
}


//page 0 -- insert card
void MainWindow::on_insertButton_page0_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(4);
}

//page 4 -- enter card number after insertion
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



//page 2 enter PIN after card insertion (2d index page)
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

void MainWindow::on_clearOneclicked()
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
void MainWindow::on_lineEdit_PIN_editingFinished()//проверка пина
{
    //connect()
}




//page 1 -- main card menu
void MainWindow::on_cashTransfersButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_cardBalanceButton_clicked()
{
    //            QMessageBox::information(this, "Баланс по картці");
    //            QMessageBox msgBox;
    //            msgBox.setText("Баланс на вашій картці = " + ATM::checkBal());
}

void MainWindow::on_cardSettingsButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(7);
}

void MainWindow::on_cashWithdrawalButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(5);
}


//page 7 -- settings
void MainWindow::on_Button_changePin_clicked()//перехід на сторінку зміни піна
{
    ui->mainStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_Button_otherATMs_clicked()//перехід на сторінку з вибором доступних атм
{
    //ATMSelectorWidget atm;

    //ui->mainStackedWidget->setCurrentIndex(ui->mainStackedWidget->indexOf())
}

void MainWindow::on_backButton_page7_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}


//page 5 Sum select
void MainWindow::on_Button_20grn_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
{

}

void MainWindow::on_Button_50grn_clicked()
{

}

void MainWindow::on_Button_100grn_clicked()
{

}

void MainWindow::on_Button_200grn_clicked()
{

}

void MainWindow::on_Button_500grn_clicked()
{

}

void MainWindow::on_Button_1000grn_clicked()
{

}

void MainWindow::on_backButton_page5_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_Button_AnySum_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(6);
}

//page 6 Another sum input for take off
void MainWindow::on_num1_6_clicked()
{
    ui->lineEdit_enterSum->insert("1");
}

void MainWindow::on_num2_6_clicked()
{
    ui->lineEdit_enterSum->insert("2");
}

void MainWindow::on_num3_6_clicked()
{
    ui->lineEdit_enterSum->insert("3");
}

void MainWindow::on_num4_6_clicked()
{
    ui->lineEdit_enterSum->insert("4");
}

void MainWindow::on_num5_6_clicked()
{
    ui->lineEdit_enterSum->insert("5");
}

void MainWindow::on_num6_6_clicked()
{
    ui->lineEdit_enterSum->insert("6");
}

void MainWindow::on_num7_6_clicked()
{
    ui->lineEdit_enterSum->insert("7");
}

void MainWindow::on_num8_6_clicked()
{
    ui->lineEdit_enterSum->insert("8");
}

void MainWindow::on_num9_6_clicked()
{
    ui->lineEdit_enterSum->insert("9");
}

void MainWindow::on_num00_6_clicked()
{
    ui->lineEdit_enterSum->insert("00");
}

void MainWindow::on_num0_6_clicked()
{
    ui->lineEdit_enterSum->insert("0");
}

void MainWindow::on_clearOne_6_clicked()
{
    ui->lineEdit_enterSum->backspace();
}

void MainWindow::on_clearButton_page6_clicked()
{
    ui->lineEdit_enterSum->clear();
}

void MainWindow::on_backButton_page6_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(5);
}

void MainWindow::on_okButton_page6_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
{

}

//page 3 QR


void MainWindow::on_num1_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("1");
}

void MainWindow::on_num2_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("2");
}

void MainWindow::on_num3_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("3");
}

void MainWindow::on_num4_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("4");
}

void MainWindow::on_num5_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("5");
}

void MainWindow::on_num6_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("6");
}

void MainWindow::on_num7_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("7");
}

void MainWindow::on_num8_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("8");
}

void MainWindow::on_num9_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("9");
}

void MainWindow::on_num0_3_clicked()
{
    ui->lineEdit_telephoneNum->insert("0");
}

void MainWindow::on_clearOne_3_clicked()
{
    ui->lineEdit_telephoneNum->backspace();
}

void MainWindow::on_clearButton_page3_clicked()
{
    ui->lineEdit_telephoneNum->clear();
}

void MainWindow::on_okButton_page3_clicked()
{

}


