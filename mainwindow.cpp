#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ATM/Model/atmcard2.h"
#include "ATM/Socket/atmselectorsocket.h"
#include "ATM/Model/atmparams.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include "ATM/clienterror.h"
#include "ATM/atm.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    pin(""),
    atm_(Q_NULLPTR),
    ui(new Ui::MainWindow)

{
    // TODO classic

    ui->setupUi(this);

    ui->lineEdit_cardNum->setInputMask("9999-9999-9999-9999");

    ui->lineEdit_anotherCardNum->setInputMask("9999-9999-9999-9999");

    ui->lineEdit_telephoneNum->setInputMask("999-999-99-99");

    ui->lineEdit_enterSum->setInputMask("9999999");//поставити обмеження на картку

    ui->lineEdit_PIN->setInputMask("XXXX");//поставити обмеження на ПІН в чотири символи

    ui->lineEdit_attemptNum->setInputMask("9");
    //ui->lineEdit_attemptNum->setReadOnly()

    ui->lineEdit_changePIN->setInputMask("9999");

    ui->mainStackedWidget->setCurrentIndex(0);
}

//----------------------------------------------------


// запускає вікно з банкоматом з айді таким
// перевірка чи вже запущений має бути
// видавати помилки іф запущений
// ДОДАТИ ВИХІД З БАНКОМАТУ!
void MainWindow::activate(size_t id)
{
    if (atm_ != Q_NULLPTR)
        delete atm_;
    atm_ = new ATM(id);
    // connect to atmStarted
    connect(atm_, SIGNAL(atmStarted()), this, SLOT(successStart()));
    // після того як запустився банкомат коннектити можна і решту сигналів до потрібних функцій
}

// банкомат запущений, можна тут коннектити до чого треба і тп і тд
void MainWindow::successStart()
{

    this->show();
    ATMSelectorWidget::hideSelector();

    qDebug() << "successfull start!";

    // вивід помилок
    connect(atm_, SIGNAL(errorOccured(const QString&)), this, SLOT(showError(const QString&)));


    //вставлення картки
    connect(atm_,SIGNAL(cardInserted()),this,SLOT(onSuccessCardInsertion()));

    //робота з пінкодом
    connect(atm_,SIGNAL(pinSuccess()), this, SLOT(onSuccessPIN()));
    connect(atm_,SIGNAL(pinValidated(const size_t)), this, SLOT(onWrongPIN(const size_t)));


    //перевірити баланс по карті
    connect(atm_,SIGNAL(balChecked()),this,SLOT(onBalCheckedAnswer()));


    //зняти гроші
    //connect(atm_,SIGNAL(cashTaken(const long money)),this,SLOT(onSuccessCashTaken(const long money)));
    connect(atm_,&ATM::cashTaken,this,&MainWindow::onSuccessCashTaken);

    //перерахувати гроші
    connect(atm_,SIGNAL(cashSend()),this,SLOT(onSuccessCashSend()));

    //змінити пінкод
    connect(atm_,SIGNAL(pinChanged()),this,SLOT(onSuccessPINchange()));


    //закинчити роботу з карткою -- дістати картку
    connect(atm_,SIGNAL(cardFree()),this, SLOT(onSuccessFreeCard()));

    //тестування запитів
    //        connect(atm_, SIGNAL(cardInserted()), this, SLOT(testInsert()));
    //        connect(atm_, SIGNAL(pinSuccess()), this, SLOT(testPinSuccess()));
    //        connect(atm_, SIGNAL(pinValidated(const size_t)), this, SLOT(testPinValidated(const size_t)));
    //        connect(atm_, SIGNAL(balChecked()), this, SLOT(testCheckBal()));
    //        connect(atm_, SIGNAL(pinChanged()), this, SLOT(testChangePin()));
    //        connect(atm_, SIGNAL(cardFree()), this, SLOT(testFree()));
    //        connect(atm_, SIGNAL(cashSend()), this, SLOT(testOnSend()));
    //        connect(atm_, SIGNAL(cardFree()), this, SLOT(testFree()));
    //        atm_->insertCard("1111111111111111");
}

void MainWindow::showError(const QString & er)
{
    QMessageBox::critical(this, tr("ATMSelectorWidget"), er);
}


void MainWindow::testInsert()
{
    qDebug() << "Insert card successfull";

    atm_->validatePin(1211);
}

void MainWindow::testPinSuccess()
{
    qDebug() << "Pin successfull";
    atm_->changePin(1111);
}

void MainWindow::testPinValidated(const size_t i)
{
    qDebug() << "Pin try: " << i;
    atm_->validatePin(1111);
}

void MainWindow::testChangePin()
{
    qDebug() << "Pin changed successfull";
    atm_->checkBal();
}

void MainWindow::testCheckBal()
{
    qDebug() << "Bal check successfull: " << atm_->card()->bal();
    atm_->sendToCard("1234123412341234", 100);
}

void MainWindow::testFree()
{
    qDebug() << "Free card successfull";
    atm_->takeCash(100);
}

void MainWindow::testOnTakeCash(const long money)
{
    qDebug() << "takeCash: " << money;
    atm_->freeCard();
}

void MainWindow::testOnSendCash()
{
    qDebug() << "sendedCash: " << atm_->card()->bal();
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
    atm_->insertCard(ui->lineEdit_cardNum->text().remove(QChar('-')));
}

void MainWindow::on_backButton_page4_clicked()
{

    ui->mainStackedWidget->setCurrentIndex(0);
    ui->lineEdit_cardNum->clear();
}


void MainWindow::onSuccessCardInsertion()
{
    qDebug() << "Card inserted successfully";
    //відповідь від сервера ОК то --> виконаты дії нижче
    ui->lineEdit_cardNum->clear();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText("Ваша картка була успішно вставлена!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    // тут треба якись таймаут, або чекати на відповідь клієнта, бо вікно ховається
    ui->mainStackedWidget->setCurrentIndex(2);
}



//page 2 enter PIN after card insertion (2d index page)
void MainWindow::on_num1_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("1");
}

void MainWindow::on_num2_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("2");
}

void MainWindow::on_num3_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("3");
}

void MainWindow::on_num4_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("4");
}

void MainWindow::on_num5_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("5");
}

void MainWindow::on_num6_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("6");
}

void MainWindow::on_num7_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("7");
}

void MainWindow::on_num8_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("8");
}

void MainWindow::on_num9_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("9");
}

void MainWindow::on_num0_2_clicked()
{
    if(pin.size() == 4)
        return;
    ui->lineEdit_PIN->insert("*");
    pin.append("0");
}

// не вызывается при нажатии
void MainWindow::on_clearOneclicked()
{
    ui->lineEdit_PIN->backspace();
}

// а эта вызывается
void MainWindow::on_clearOne_2_clicked()
{
    ui->lineEdit_PIN->backspace();
    pin.chop(1);
}


// ощичает строку
void MainWindow::on_clearButton_page2_clicked()
{
    ui->lineEdit_PIN->clear();
    pin.clear();
}


void MainWindow::on_backButton_page2_clicked()
{
    // освободить карту надо
    ui->mainStackedWidget->setCurrentIndex(4);
    atm_->freeCard();
}


//сделать проверку пина
//сделать скрытие пина под *
void MainWindow::on_okButton_page2_clicked()
{
    // if (checkPIN(ui->lineEdit_PIN)){

    atm_->validatePin(pin.toUInt());
    //        emit(atm_->pinValidated(3));
    //        return;
    // }
}


void MainWindow::onSuccessPIN()
{
    //якщо пін правильний то переходимо в головне меню картки
    ui->lineEdit_PIN->clear();

    qDebug()<<"PIN input success";
    ui->mainStackedWidget->setCurrentIndex(1);

}

void MainWindow::onWrongPIN(const size_t attempts)
{
    //if ((attempts<3)&& (attempts>0))
    //pin.clear();
    ui->lineEdit_PIN->clear();
    ui->lineEdit_attemptNum->setText(QString::number (attempts));

    if (attempts==0){  //Якщо пін не правильний 3 рази то червоний екран, повідомлення про помилку --> перехід на сторінку вставити картку

        ui->page2_pin->setStyleSheet("background-color: rgb(252, 1, 7);");
        ui->lineEdit_PIN->clear();

        QMessageBox msgBox;
        msgBox.setWindowTitle("Обмеження роботи з карткою");
        msgBox.setText("Нажаль Ви 3 рази ввели неправильний PIN :( \n"
                       "Робота з даною карткою закінчена." );
        msgBox.setIconPixmap(QPixmap(":/imgs/img/unnamed.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        atm_->freeCard();
        ui->mainStackedWidget->setCurrentIndex(0);
        ui->page2_pin->setStyleSheet("background-color: rgb(255, 234, 189);");

    }
}



//page 1 -- main card menu
void MainWindow::on_cashTransfersButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(9);
}

void MainWindow::on_cardBalanceButton_clicked()
{
    atm_->checkBal();
}

void MainWindow::onBalCheckedAnswer()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Баланс вашої картки");
    msgBox.setText("Баланс на вашій картці = " + QString::number(atm_->card()->bal())+" boobliks");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/580b57fcd9996e24bc43c395.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

}


void MainWindow::on_cardSettingsButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(7);
}

void MainWindow::on_cashWithdrawalButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(5);
}


void MainWindow::onSuccessFreeCard()
{
    qDebug() << "Card was free";

    ui->mainStackedWidget->setCurrentIndex(0);


}


void MainWindow::on_Button_freeCard_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Дістати картку", "Ви впевнені що хочете завершити роботу з карткою?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        atm_->freeCard();
        pin.clear();}
}






//page 7 -- settings
void MainWindow::on_Button_changePin_clicked()//перехід на сторінку зміни піна
{
    ui->mainStackedWidget->setCurrentIndex(8);
}

void MainWindow::on_Button_otherATMs_clicked()//перехід на сторінку з вибором доступних атм
{
    ATMSelectorWidget::startSelector();
    this->close();
}

void MainWindow::on_backButton_page7_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}




//page 5 Sum select
void MainWindow::checkSumForTake(size_t sum)
{


    //перевірка чи введена сума < суми що лежить на карті якщо так то
    if (static_cast<long>(sum)<=atm_->card()->bal())
        atm_->takeCash(sum);
    //якщо ні, то виведення повідомлення про помилку і очищення поля
    else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Помилка");
        msgBox.setText("Введена Вами сума більша за поточний баланс на вашій картці");
        msgBox.setIconPixmap(QPixmap(":/imgs/img/unnamed.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        ui->lineEdit_enterSum->clear();
    }}


void MainWindow::on_Button_20grn_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
{
    checkSumForTake(20);
}
void MainWindow::on_Button_50grn_clicked()
{
    checkSumForTake(50);
}

void MainWindow::on_Button_100grn_clicked()
{
    checkSumForTake(100);
}

void MainWindow::on_Button_200grn_clicked()
{
    checkSumForTake(200);
}

void MainWindow::on_Button_500grn_clicked()
{
    checkSumForTake(500);
}

void MainWindow::on_Button_1000grn_clicked()
{
    checkSumForTake(1000);
}

void MainWindow::on_backButton_page5_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_Button_AnySum_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(6);
}


void MainWindow::onSuccessCashTaken(const long money)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Гроші знято!");
    msgBox.setText("Введена Вами сума була успішно знята з картки! \n"
                   "Баланс на вашій картці = " + QString::number(atm_->card()->bal())+" boobliks.");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    qDebug() << "Cash was taken successfully";

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

   //checkSumForTake(ui->lineEdit_enterSum->text().toULong());


    QString entered_sum = ui->lineEdit_enterSum->text();

    //перевірка чи введена сума < суми що лежить на карті якщо так то
    if (entered_sum.toUInt()<=atm_->card()->bal())
        atm_->takeCash(entered_sum.toUInt());
    //якщо ні, то виведення повідомлення про помилку і очищення поля
    else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Помилка");
        msgBox.setText("Введена Вами сума більша за поточний баланс на вашій картці");
        msgBox.setIconPixmap(QPixmap(":/imgs/img/unnamed.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        ui->lineEdit_enterSum->clear();

}}



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


//page 8 -- change PIN page
QLineEdit* MainWindow::chooseSelectedLineEdit(QLineEdit* line1,QLineEdit* line2)
{
    if (line1->hasFocus()) return line1;
    else return line2;
}

void MainWindow::on_num1_8_clicked()
{

    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("1");
}

void MainWindow::on_num2_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("2");
}

void MainWindow::on_num3_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("3");
}

void MainWindow::on_num4_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("4");
}

void MainWindow::on_num5_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("5");
}

void MainWindow::on_num6_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("6");
}

void MainWindow::on_num7_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("7");
}

void MainWindow::on_num8_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("8");
}

void MainWindow::on_num9_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("9");
}

void MainWindow::on_num0_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->insert("0");
}

void MainWindow::on_clearOne_8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->backspace();
}

void MainWindow::on_clearButton_page8_clicked()
{
    chooseSelectedLineEdit(ui->lineEdit_changePIN,ui->lineEdit_repeatChangePIN)->clear();
}

void MainWindow::on_backButton_page8_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(7);
}


void MainWindow::on_OKButton_page8_clicked()
{
    atm_->changePin(ui->lineEdit_changePIN->text().toUInt());
}


void MainWindow::onSuccessPINchange()
{
    qDebug()<<"Pin was changed successfully";
    pin.append(ui->lineEdit_changePIN->text());
    QMessageBox msgBox;
    msgBox.setWindowTitle("Зміна PIN");
    msgBox.setText("Зміна PIN-коду пройшла успішно");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    ui->lineEdit_changePIN->clear();
    ui->lineEdit_repeatChangePIN->clear();
    ui->mainStackedWidget->setCurrentIndex(2);
}


//перевести на іншу картку (сторінка 9) --  сторинка вводу карткы
void MainWindow::on_num1_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("1");
}

void MainWindow::on_num2_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("2");
}

void MainWindow::on_num3_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("3");
}

void MainWindow::on_num4_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("4");
}

void MainWindow::on_num5_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("5");
}

void MainWindow::on_num6_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("6");
}

void MainWindow::on_num7_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("7");
}

void MainWindow::on_num8_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("8");
}

void MainWindow::on_num9_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("9");
}

void MainWindow::on_num0_9_clicked()
{
    ui->lineEdit_anotherCardNum->insert("0");
}

void MainWindow::on_clearOne_9_clicked()
{
    ui->lineEdit_anotherCardNum->backspace();
}

void MainWindow::on_clearButton_page9_clicked()
{
    ui->lineEdit_anotherCardNum->clear();
}

void MainWindow::on_backButton_page9_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_okButton_page9_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(10);
}


//вибір суми для переказу -- переказ на іншу картку сторінка 10

void MainWindow::checkSumForSend(size_t sum)
{

    //перевірка чи введена сума < суми що лежить на карті якщо так то
    if (static_cast<long>(sum)<=atm_->card()->bal())
        atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),sum);
    //якщо ні, то виведення повідомлення про помилку і очищення поля
    else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Помилка");
        msgBox.setText("Введена Вами сума більша за поточний баланс на вашій картці");
        msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

    }
}


void MainWindow::onSuccessCashSend()
{
    qDebug()<<"Cash was transfer successfully";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Гроші успішно перераховано! ");
    msgBox.setText("Сума успішно перерахована! \n"
                   "Баланс на вашій картці = " + QString::number(atm_->card()->bal()));
    msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_Button_20grn_12_clicked()
{
    checkSumForSend(20);
    //atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),20);
}

void MainWindow::on_Button_50grn_10_clicked()
{
    checkSumForSend(50);
    //atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),50);
}

void MainWindow::on_Button_100grn_10_clicked()
{
    checkSumForSend(100);
    //atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),100);
}

void MainWindow::on_Button_200grn_10_clicked()
{
    checkSumForSend(200);
    //atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),200);
}

void MainWindow::on_Button_500grn_10_clicked()
{
    checkSumForSend(500);
    //atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),500);
}

void MainWindow::on_Button_1000grn_10_clicked()
{
    checkSumForSend(1000);
    //atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),1000);
}

void MainWindow::on_backButton_page10_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(8);
}

void MainWindow::on_Button_AnySum_10_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(11);
}

//вибір іншої суми для переказу -- (переказ на іншу картку) сторінка 11

void MainWindow::on_num1_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("1");
}

void MainWindow::on_num2_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("2");
}

void MainWindow::on_num3_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("3");
}

void MainWindow::on_num4_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("4");
}

void MainWindow::on_num5_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("5");
}

void MainWindow::on_num6_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("6");
}

void MainWindow::on_num7_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("7");
}

void MainWindow::on_num8_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("8");
}

void MainWindow::on_num9_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("9");
}

void MainWindow::on_num00_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("00");
}

void MainWindow::on_num0_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->insert("0");
}

void MainWindow::on_clearOne_11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->backspace();
}

void MainWindow::on_clearButton_page11_clicked()
{
    ui->lineEdit_enterSumForTransfer_11->clear();
}

void MainWindow::on_backButton_page11_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(10);
}

void MainWindow::on_okButton_page11_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки

{
    QString entered_sum = ui->lineEdit_enterSumForTransfer_11->text();

    //перевірка чи введена сума < суми що лежить на карті якщо так то
    if (entered_sum.toUInt()<=atm_->card()->bal())
        atm_->sendToCard(ui->lineEdit_anotherCardNum->text(),entered_sum.toUInt());
    //якщо ні, то виведення повідомлення про помилку і очищення поля
    else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Помилка");
        msgBox.setText("Введена Вами сума більша за поточний баланс на вашій картці");
        msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        ui->lineEdit_enterSum->clear();
    }
}
