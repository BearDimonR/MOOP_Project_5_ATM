#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ATM/Model/atmcard2.h"
#include "ATM/atm.h"
#include "ATM/Socket/atmselectorsocket.h"
#include "ATM/Model/atmparams.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include "ATM/clienterror.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    atm_(Q_NULLPTR),
    ui(new Ui::MainWindow)

{
    // TODO classic
    //qDebug() << "pidar";

    ui->setupUi(this);

    ui->lineEdit_cardNum->setInputMask("9999-9999-9999-9999");

    ui->lineEdit_anotherCardNum->setInputMask("9999-9999-9999-9999");

    ui->lineEdit_telephoneNum->setInputMask("999-999-99-99");

    ui->lineEdit_enterSum->setInputMask("9999999");//поставити обмеження на картку

    //ui->lineEdit_PIN->setInputMask("9999");//поставити обмеження на ПІН в чотири символи
    ui->lineEdit_PIN->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_attemptNum->setInputMask("9")
            ;
    ui->lineEdit_changePIN->setInputMask("9999");
    ui->lineEdit_repeatChangePIN->setInputMask("9999");
}

// запускає вікно з банкоматом з айді таким
// перевірка чи вже запущений має бути
// видавати помилки іф запущений
// ДОДАТИ ВИХІД З БАНКОМАТУ!
void MainWindow::activate(size_t id)
{
    if (atm_ != Q_NULLPTR)
        qFatal("%s", QString(ClientError("MainWindow already started",
                       ClientError::CLIENT_ERROR, atm_->bankName())).toLatin1().constData());
    atm_ = new ATM(id);
    // connect to atmStarted
    connect(atm_, SIGNAL(atmStarted()), this, SLOT(successStart()));
    // після того як запустився банкомат коннектити можна і решту сигналів до потрібних функцій
}

// банкомат запущений, можна тут коннектити до чого треба і тп і тд
void MainWindow::successStart()
{
    this->show();
    qDebug() << "successfull start!";

    // тестування запитів
    connect(atm_, SIGNAL(cardInserted()), this, SLOT(testInsert()));
    atm_->insertCard("1111111111111111");
}

void MainWindow::testInsert()
{
    qDebug() << "Insert card successfull";
    connect(atm_, SIGNAL(pinSuccess()), this, SLOT(testPinSuccess()));
    connect(atm_, SIGNAL(pinValidated(const size_t)), this, SLOT(testPinValidated(const size_t)));
    atm_->validatePin(1211);
}

void MainWindow::testPinSuccess()
{
   qDebug() << "Pin successfull";
   connect(atm_, SIGNAL(cardFree()), this, SLOT(testFree()));
   atm_->freeCard();
}

void MainWindow::testPinValidated(const size_t i)
{
    qDebug() << "Pin try: " << i;
    atm_->validatePin(1111);
}

void MainWindow::testFree()
{
    qDebug() << "Free card successfull";
}

// функція бере екземпляр Мейнвіндов і активує його з айдішкою
void MainWindow::startMainWindow(const size_t id)
{
    static MainWindow inst;
    inst.activate(id);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onInsert()
{
    qDebug() << "insertCard: " << atm_->card()->bal();
}


//page 0 -- insert card
void MainWindow::on_insertButton_page0_clicked()
{
    // TODO atm_->insertCard("1111111111111111");
    // відправити номер картки
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


void MainWindow::on_lineEdit_cardNum_editingFinished()

{
    connect(atm_,SIGNAL(cardInserted()), this, SLOT(insertCard(ui->lineEdit_cardNum->text())));

    //перевірити якщо відповідь від сервера ОК то --> виконаты дії нижче
    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText("Ваша картка була успішно вставлена");
    msgBox.setStandardButtons(QMessageBox::Ok);
    ui->mainStackedWidget->setCurrentIndex(2);

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



//сделать проверку пина
//сделать скрытие пина под *
void MainWindow::on_lineEdit_PIN_editingFinished()//проверка пина
{
  //якщо пін правильний то переходимо в головне меню картки
    connect(atm_,SIGNAL(pinSuccess()), this, SLOT());
    ui->mainStackedWidget->setCurrentIndex(1);

    //Якщо пін не правильний то червоний екран, повідомлення про помилку --> перехід на сторінку вставити картку
    ui->page2_pin->setStyleSheet("background-color: rgb(252, 1, 7);");
    QMessageBox msgBox;
    msgBox.setWindowTitle("Обмеження роботи з карткою");
    msgBox.setText("Нажаль Ви 3 рази ввели неправильний PIN \n"
                   "Робота з даною карткою закінчена" );
    msgBox.setStandardButtons(QMessageBox::Ok);
    ui->mainStackedWidget->setCurrentIndex(0);

}




//page 1 -- main card menu
void MainWindow::on_cashTransfersButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(9);
}

void MainWindow::on_cardBalanceButton_clicked()
{

    connect(atm_,SIGNAL(balChecked()), this, SLOT(checkBal()));
    QMessageBox msgBox;
    msgBox.setWindowTitle("Баланс вашої картки");
    msgBox.setText("Баланс на вашій картці = " + atm_->card()->bal());
    msgBox.setStandardButtons(QMessageBox::Ok);

}

void MainWindow::on_cardSettingsButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(7);
}

void MainWindow::on_cashWithdrawalButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(5);
}

void MainWindow::on_FinishWorkButton_1_clicked()
{
    connect(atm_,SIGNAL(cardFree()), this, SLOT(freeCard()));
    ui->mainStackedWidget->setCurrentIndex(0);
}


//page 7 -- settings
void MainWindow::on_Button_changePin_clicked()//перехід на сторінку зміни піна
{
    ui->mainStackedWidget->setCurrentIndex(8);
}

void MainWindow::on_Button_otherATMs_clicked()//перехід на сторінку з вибором доступних атм
{
    ATMSelector selector;
    ATMSelectorWidget widget(&selector);
    widget.show();
}

void MainWindow::on_backButton_page7_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}


//page 5 Sum select
void MainWindow::on_Button_20grn_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
{
    moneyWithdrawProcess(20);

}

void MainWindow::on_Button_50grn_clicked()
{
    moneyWithdrawProcess(50);
}

void MainWindow::on_Button_100grn_clicked()
{
    moneyWithdrawProcess(100);
}

void MainWindow::on_Button_200grn_clicked()
{
    moneyWithdrawProcess(200);
}

void MainWindow::on_Button_500grn_clicked()
{
    moneyWithdrawProcess(500);
}

void MainWindow::on_Button_1000grn_clicked()
{
    moneyWithdrawProcess(1000);
}

void MainWindow::on_backButton_page5_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_Button_AnySum_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(6);
}


void MainWindow::moneyWithdrawProcess(const size_t& suma)
{
    connect(atm_,SIGNAL(cashSend()), this, SLOT(takeCash(suma)));
    connect(atm_,SIGNAL(balChecked()), this, SLOT(checkBal()));
    QMessageBox msgBox;
    msgBox.setWindowTitle("Баланс вашої картки");
    msgBox.setText("Баланс на вашій картці = " + atm_->card()->bal());
    msgBox.setStandardButtons(QMessageBox::Ok);
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
    //перевірка чи введена сума < суми що лежить на карті якщо так то
    moneyWithdrawProcess(ui->lineEdit_enterSum->text().toLong());
    //якщо ні, то виведення повідомлення про помилку і очищення поля
//    QMessageBox msgBox;
//    msgBox.setWindowTitle("Помилка");
//    msgBox.setText("Введена Вами сума більша за поточний баланс на вашій картці");
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    ui->lineEdit_enterSum->clear();
}



//page 3 QR ?????
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


//page 8 change PIN page
void MainWindow::on_num1_8_clicked()
{

}

void MainWindow::on_num2_8_clicked()
{

}

void MainWindow::on_num3_8_clicked()
{

}

void MainWindow::on_num4_8_clicked()
{

}

void MainWindow::on_num5_8_clicked()
{

}

void MainWindow::on_num6_8_clicked()
{

}

void MainWindow::on_num7_8_clicked()
{

}

void MainWindow::on_num8_8_clicked()
{

}

void MainWindow::on_num9_8_clicked()
{

}

void MainWindow::on_num0_8_clicked()
{

}

void MainWindow::on_clearOne_8_clicked()
{

}
