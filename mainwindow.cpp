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
    pin_(""),
    sum_(0),
    atm_(Q_NULLPTR),
    ui_(new Ui::MainWindow)

{
    ui_->setupUi(this);

    ui_->lineEdit_cardNum->setInputMask("9999-9999-9999-9999");
    ui_->lineEdit_cardNum->setReadOnly(true);

    ui_->lineEdit_anotherCardNum->setInputMask("9999-9999-9999-9999");

    ui_->lineEdit_enterSum->setInputMask("9999999");//поставити обмеження на картку

    ui_->lineEdit_PIN->setInputMask("XXXX");//поставити обмеження на ПІН в чотири символи

    ui_->lineEdit_attemptNum->setInputMask("9");

    ui_->lineEdit_changePIN->setInputMask("9999");
    ui_->lineEdit_repeatChangePIN->setInputMask("9999");

    ui_->mainStackedWidget->setCurrentIndex(0);
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
    connect(atm_,&ATM::cashTaken,this,&MainWindow::onSuccessCashTaken);

    //перерахувати гроші
    connect(atm_,SIGNAL(cashSend()),this,SLOT(onSuccessCashSend()));

    //змінити пінкод
    connect(atm_,SIGNAL(pinChanged()),this,SLOT(onSuccessPINchange()));


    //закінчити роботу з карткою -- дістати картку
    connect(atm_,SIGNAL(cardFree()),this, SLOT(onSuccessFreeCard()));

    //qr code
    QPixmap map(atm_->qrcode());
    map = map.scaled(ui_->label_3_2->size());
    ui_->label_3_2->clear();
    ui_->label_3_2->setPixmap(map);

}

void MainWindow::showError(const QString & er)
{
    QMessageBox::critical(this, tr("ATMSelectorWidget"), er);
}

// функція бере екземпляр Мейнвіндов і активує його з айдішкою
void MainWindow::startMainWindow(const size_t id)
{
    static MainWindow inst;
    inst.activate(id);
}

MainWindow::~MainWindow()
{
    delete ui_;
    if(atm_ != Q_NULLPTR)
        delete atm_;

}


void MainWindow::on_selectorButton_clicked()
{
    hide();
    delete atm_;
    atm_ = Q_NULLPTR;
    pin_.clear();
    sum_ = 0;
    ATMSelectorWidget::startSelector();
}



//page 0 -- insert card
void MainWindow::on_insertButton_page0_clicked()
{

    ui_->lineEdit_attemptNum->setText("3");
    ui_->mainStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_insertButton_page0_2_clicked()
{
    //qr code login
    // чекати на вхід
    connect(atm_,SIGNAL(qrSuccess()),this, SLOT(onSuccessQr()));
    ui_->mainStackedWidget->setCurrentIndex(3);
}


//page 4 -- enter card number after insertion
void MainWindow::on_num1_4_clicked()
{
    ui_->lineEdit_cardNum->insert("1");
}

void MainWindow::on_num2_4_clicked()
{
    ui_->lineEdit_cardNum->insert("2");
}

void MainWindow::on_num3_4_clicked()
{
    ui_->lineEdit_cardNum->insert("3");
}

void MainWindow::on_num4_4_clicked()
{
    ui_->lineEdit_cardNum->insert("4");
}

void MainWindow::on_num5_4_clicked()
{
    ui_->lineEdit_cardNum->insert("5");
}

void MainWindow::on_num6_4_clicked()
{
    ui_->lineEdit_cardNum->insert("6");
}

void MainWindow::on_num7_4_clicked()
{
    ui_->lineEdit_cardNum->insert("7");
}

void MainWindow::on_num8_4_clicked()
{
    ui_->lineEdit_cardNum->insert("8");
}

void MainWindow::on_num9_4_clicked()
{
    ui_->lineEdit_cardNum->insert("9");
}

void MainWindow::on_num0_4_clicked()
{
    ui_->lineEdit_cardNum->insert("0");
}

void MainWindow::on_clearOne_4_clicked()
{
    ui_->lineEdit_cardNum->backspace();
}

void MainWindow::on_clearButton_page4_clicked()
{
    ui_->lineEdit_cardNum->clear();
}

void MainWindow::on_okButton_page4_clicked()
{
    atm_->insertCard(ui_->lineEdit_cardNum->text().remove(QChar('-')));
}

void MainWindow::on_backButton_page4_clicked()
{

    ui_->mainStackedWidget->setCurrentIndex(0);
    ui_->lineEdit_cardNum->clear();
}


void MainWindow::onSuccessCardInsertion()
{
    qDebug() << "Card inserted successfully";
    //відповідь від сервера ОК то --> виконаты дії нижче
    ui_->lineEdit_cardNum->clear();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText("Ваша картка була успішно вставлена!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    ui_->mainStackedWidget->setCurrentIndex(2);
}


//page 3 - qr

void MainWindow::on_clearButton_page3_clicked()
{
    // перестати чекати на вхід
    disconnect(atm_,SIGNAL(qrSuccess()),this, SLOT(onSuccessQr()));
    ui_->mainStackedWidget->setCurrentIndex(0);
}

void MainWindow::onSuccessQr()
{
    // зайти в атм
    qDebug() << "QR login success";

    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText("QR вхід був успішним!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    ui_->mainStackedWidget->setCurrentIndex(2);
}



//page 2 enter PIN after card insertion (2d index page)
void MainWindow::on_num1_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("1");
}

void MainWindow::on_num2_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("2");
}

void MainWindow::on_num3_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("3");
}

void MainWindow::on_num4_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("4");
}

void MainWindow::on_num5_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("5");
}

void MainWindow::on_num6_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("6");
}

void MainWindow::on_num7_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("7");
}

void MainWindow::on_num8_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("8");
}

void MainWindow::on_num9_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("9");
}

void MainWindow::on_num0_2_clicked()
{
    if(pin_.size() == 4)
        return;
    ui_->lineEdit_PIN->insert("*");
    pin_.append("0");
}



void MainWindow::on_clearOne_2_clicked()
{
    ui_->lineEdit_PIN->backspace();
    pin_.chop(1);
}



void MainWindow::on_clearButton_page2_clicked()
{
    ui_->lineEdit_PIN->clear();
    pin_.clear();
}


void MainWindow::on_backButton_page2_clicked()
{
    ui_->lineEdit_attemptNum->setText("3");
    pin_.clear();
    ui_->lineEdit_PIN->clear();
    atm_->freeCard();
}


void MainWindow::on_okButton_page2_clicked()
{

    atm_->validatePin(pin_.toUInt());

}


void MainWindow::onSuccessPIN()
{
    //якщо пін правильний то переходимо в головне меню картки
    ui_->lineEdit_PIN->clear();

    qDebug()<<"PIN input success";
    ui_->mainStackedWidget->setCurrentIndex(1);
    ui_->lineEdit_attemptNum->setText("3");

}

void MainWindow::onWrongPIN(const size_t attempts)
{

    ui_->lineEdit_PIN->clear();
    pin_.clear();
    ui_->lineEdit_attemptNum->setText(QString::number (attempts));

    if (attempts==0){  //Якщо пін не правильний 3 рази то червоний екран, повідомлення про помилку --> перехід на сторінку вставити картку

        ui_->page2_pin->setStyleSheet("background-color: rgb(252, 1, 7);");
        ui_->lineEdit_PIN->clear();

        QMessageBox msgBox;
        msgBox.setWindowTitle("Обмеження роботи з карткою");
        msgBox.setText("Нажаль Ви 3 рази ввели неправильний PIN :( \n"
                       "Робота з даною карткою закінчена." );
        msgBox.setIconPixmap(QPixmap(":/imgs/img/unnamed.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        ui_->page2_pin->setStyleSheet("background-color: rgb(255, 234, 189);");

        atm_->freeCard();

    }
}



//page 1 -- main card menu
void MainWindow::on_cashTransfersButton_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(9);
}

void MainWindow::on_cardBalanceButton_clicked()
{
    atm_->checkBal();
}

void MainWindow::onBalCheckedAnswer()
{


    switch (ui_->mainStackedWidget->currentIndex()) {
    case 1: {

        QMessageBox msgBox;
        msgBox.setWindowTitle("Баланс вашої картки");
        msgBox.setText("Баланс на вашій картці = " + QString::number(atm_->card()->bal())
                       +" boobliks."
                       +"\nКомісія для зняття: " + QString::number(atm_->withdrawInterest()) + " %"
                       +"\nКомісія для переводу: " + QString::number(atm_->transactInterest())+ " %");
        msgBox.setIconPixmap(QPixmap(":/imgs/img/580b57fcd9996e24bc43c395.png"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        break;
    }
    case 5:
    case 6:{
        //перевірка чи введена сума < суми що лежить на карті якщо так то
        // атм кард може бути null, якщо я до цього не перевіряв баланс
        if (static_cast<long>(sum_)<=atm_->card()->bal()&& (static_cast<long>(sum_)!=0))
            atm_->takeCash(sum_);
        else  errorMsg(ui_->lineEdit_enterSum);
        break;
    }
    case 10:
    case 11:{
        if (static_cast<long>(sum_)<=atm_->card()->bal())
             atm_->sendToCard(ui_->lineEdit_anotherCardNum->text().remove(QChar('-')),sum_);

        else errorMsg(ui_->lineEdit_enterSumForTransfer_11);
        break;
        }

    }


    }

void MainWindow::errorMsg(QLineEdit *line)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Помилка");
    msgBox.setText("Введена Вами сума більша за поточний баланс на вашій картці");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/unnamed.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    line->clear();
}


void MainWindow::on_cardSettingsButton_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(7);
}

void MainWindow::on_cashWithdrawalButton_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(5);
}


void MainWindow::onSuccessFreeCard()
{
    qDebug() << "Card was free";

    ui_->mainStackedWidget->setCurrentIndex(0);
}


void MainWindow::on_Button_freeCard_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Дістати картку", "Ви впевнені що хочете завершити роботу з карткою?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        atm_->freeCard();
        pin_.clear();}
}




//page 7 -- settings
void MainWindow::on_Button_changePin_clicked()//перехід на сторінку зміни піна
{
    ui_->mainStackedWidget->setCurrentIndex(8);
}

void MainWindow::on_Button_otherATMs_clicked()//перехід на сторінку з вибором доступних атм
{
    ATMSelectorWidget::startSelector();
    this->close();
}

void MainWindow::on_backButton_page7_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(1);
}



//page 5 Sum select
void MainWindow::checkSum(size_t sum)
{

    sum_ = sum;
    atm_->checkBal();

}


void MainWindow::takeCashQuestion(size_t sum, size_t commis_percent)
{
    size_t newsum = sum+(sum*commis_percent/100);
    QMessageBox msgBox;
    msgBox.setWindowTitle("Помилка");
    msgBox.setText("Впевнені що хочете зняти " + QString::number(sum) + " boobliks ?\n"
                                                                        "Майте на увазі з Вас зніметься комісія рівна: " + QString::number(commis_percent)  + " %");

    msgBox.setIconPixmap(QPixmap(":/imgs/img/thinking-speaking-emoji-draper-inc-blog-site-4.png"));
    msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);


    if(msgBox.exec() == QMessageBox::Yes){
        qDebug() << "Yes was clicked";
        checkSum(newsum);
    }else {
      // do something else
        msgBox.close();
    }


}


void MainWindow::on_Button_20grn_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
{

   takeCashQuestion(20,atm_->withdrawInterest());

}
void MainWindow::on_Button_50grn_clicked()
{
    takeCashQuestion(50,atm_->withdrawInterest());
}

void MainWindow::on_Button_100grn_clicked()
{
    takeCashQuestion(100,atm_->withdrawInterest());
}

void MainWindow::on_Button_200grn_clicked()
{
    takeCashQuestion(200,atm_->withdrawInterest());
}

void MainWindow::on_Button_500grn_clicked()
{
    takeCashQuestion(500,atm_->withdrawInterest());
}

void MainWindow::on_Button_1000grn_clicked()
{
    takeCashQuestion(1000,atm_->withdrawInterest());
}

void MainWindow::on_backButton_page5_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_Button_AnySum_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(6);
}


void MainWindow::onSuccessCashTaken(const long money)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Гроші знято!");
    msgBox.setText("Введена Вами сума була успішно знята з картки! \n"
                   "Баланс на вашій картці = " + QString::number(atm_->card()->bal())+" boobliks.\n"
                                                                                      "Було знято: " + QString::number(money) +" boobliks.");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    qDebug() << "Cash was taken successfully";

}




//page 6 Another sum input for take off
void MainWindow::on_num1_6_clicked()
{
    ui_->lineEdit_enterSum->insert("1");
}

void MainWindow::on_num2_6_clicked()
{
    ui_->lineEdit_enterSum->insert("2");
}

void MainWindow::on_num3_6_clicked()
{
    ui_->lineEdit_enterSum->insert("3");
}

void MainWindow::on_num4_6_clicked()
{
    ui_->lineEdit_enterSum->insert("4");
}

void MainWindow::on_num5_6_clicked()
{
    ui_->lineEdit_enterSum->insert("5");
}

void MainWindow::on_num6_6_clicked()
{
    ui_->lineEdit_enterSum->insert("6");
}

void MainWindow::on_num7_6_clicked()
{
    ui_->lineEdit_enterSum->insert("7");
}

void MainWindow::on_num8_6_clicked()
{
    ui_->lineEdit_enterSum->insert("8");
}

void MainWindow::on_num9_6_clicked()
{
    ui_->lineEdit_enterSum->insert("9");
}

void MainWindow::on_num00_6_clicked()
{
    ui_->lineEdit_enterSum->insert("00");
}

void MainWindow::on_num0_6_clicked()
{
    ui_->lineEdit_enterSum->insert("0");
}

void MainWindow::on_clearOne_6_clicked()
{
    ui_->lineEdit_enterSum->backspace();
}

void MainWindow::on_clearButton_page6_clicked()
{
    ui_->lineEdit_enterSum->clear();
}

void MainWindow::on_backButton_page6_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(5);
    ui_->lineEdit_enterSum->clear();
}

void MainWindow::on_okButton_page6_clicked()//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
{

    takeCashQuestion(ui_->lineEdit_enterSum->text().toULong(),atm_->withdrawInterest());

}


//page 8 -- change PIN page
QLineEdit* MainWindow::chooseSelectedLineEdit(QLineEdit* line1,QLineEdit* line2)
{
    if (line1->hasFocus()) return line1;
    else return line2;
}

void MainWindow::on_num1_8_clicked()
{

    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("1");
}

void MainWindow::on_num2_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("2");
}

void MainWindow::on_num3_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("3");
}

void MainWindow::on_num4_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("4");
}

void MainWindow::on_num5_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("5");
}

void MainWindow::on_num6_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("6");
}

void MainWindow::on_num7_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("7");
}

void MainWindow::on_num8_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("8");
}

void MainWindow::on_num9_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("9");
}

void MainWindow::on_num0_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->insert("0");
}

void MainWindow::on_clearOne_8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->backspace();
}

void MainWindow::on_clearButton_page8_clicked()
{
    chooseSelectedLineEdit(ui_->lineEdit_changePIN,ui_->lineEdit_repeatChangePIN)->clear();
}

void MainWindow::on_backButton_page8_clicked()
{

    ui_->lineEdit_changePIN->clear();
    ui_->lineEdit_repeatChangePIN->clear();
    ui_->mainStackedWidget->setCurrentIndex(7);

}


void MainWindow::on_OKButton_page8_clicked()
{
    atm_->changePin(ui_->lineEdit_changePIN->text().toUInt());
    ui_->lineEdit_changePIN->clear();
    ui_->lineEdit_repeatChangePIN->clear();
}


void MainWindow::onSuccessPINchange()
{
    qDebug()<<"Pin was changed successfully";
    pin_.append(ui_->lineEdit_changePIN->text());
    QMessageBox msgBox;
    msgBox.setWindowTitle("Зміна PIN");
    msgBox.setText("Зміна PIN-коду пройшла успішно");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    ui_->lineEdit_changePIN->clear();
    ui_->lineEdit_repeatChangePIN->clear();
    ui_->mainStackedWidget->setCurrentIndex(7);
}


//перевести на іншу картку (сторінка 9) --  сторінка вводу картки
void MainWindow::on_num1_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("1");
}

void MainWindow::on_num2_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("2");
}

void MainWindow::on_num3_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("3");
}

void MainWindow::on_num4_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("4");
}

void MainWindow::on_num5_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("5");
}

void MainWindow::on_num6_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("6");
}

void MainWindow::on_num7_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("7");
}

void MainWindow::on_num8_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("8");
}

void MainWindow::on_num9_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("9");
}

void MainWindow::on_num0_9_clicked()
{
    ui_->lineEdit_anotherCardNum->insert("0");
}

void MainWindow::on_clearOne_9_clicked()
{
    ui_->lineEdit_anotherCardNum->backspace();
}

void MainWindow::on_clearButton_page9_clicked()
{
    ui_->lineEdit_anotherCardNum->clear();
}

void MainWindow::on_backButton_page9_clicked()
{
    ui_->lineEdit_anotherCardNum->clear();
    ui_->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_okButton_page9_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(10);
}


//вибір суми для переказу -- переказ на іншу картку сторінка 10
void MainWindow::onSuccessCashSend()
{
    qDebug()<<"Cash was transfer successfully";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Гроші успішно перераховано! ");
    msgBox.setText("Сума успішно перерахована! \n"
                   "Баланс на вашій картці = " + QString::number(atm_->card()->bal()) + " boobliks.");
    msgBox.setIconPixmap(QPixmap(":/imgs/img/kisspng-check-mark-bottle-material-green-tick-5ad25467123860.2792666715237336070746.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_Button_20grn_12_clicked()
{
    takeCashQuestion(20,atm_->transactInterest());
}

void MainWindow::on_Button_50grn_10_clicked()
{
    takeCashQuestion(50,atm_->transactInterest());
}

void MainWindow::on_Button_100grn_10_clicked()
{
    takeCashQuestion(100,atm_->transactInterest());
}

void MainWindow::on_Button_200grn_10_clicked()
{
    takeCashQuestion(200,atm_->transactInterest());
}

void MainWindow::on_Button_500grn_10_clicked()
{
    takeCashQuestion(500,atm_->transactInterest());
}

void MainWindow::on_Button_1000grn_10_clicked()
{
    takeCashQuestion(1000,atm_->transactInterest());
}

void MainWindow::on_backButton_page10_clicked()
{
    ui_->lineEdit_anotherCardNum->clear();
    ui_->mainStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_Button_AnySum_10_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(11);
}




//вибір іншої суми для переказу -- (переказ на іншу картку) сторінка 11
void MainWindow::on_num1_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("1");
}

void MainWindow::on_num2_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("2");
}

void MainWindow::on_num3_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("3");
}

void MainWindow::on_num4_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("4");
}

void MainWindow::on_num5_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("5");
}

void MainWindow::on_num6_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("6");
}

void MainWindow::on_num7_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("7");
}

void MainWindow::on_num8_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("8");
}

void MainWindow::on_num9_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("9");
}

void MainWindow::on_num00_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("00");
}

void MainWindow::on_num0_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->insert("0");
}

void MainWindow::on_clearOne_11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->backspace();
}

void MainWindow::on_clearButton_page11_clicked()
{
    ui_->lineEdit_enterSumForTransfer_11->clear();
}

void MainWindow::on_backButton_page11_clicked()
{
    ui_->mainStackedWidget->setCurrentIndex(10);
    ui_->lineEdit_enterSumForTransfer_11->clear();
}

//вивести повідомлення про те що гроші були успішно зняті і показати поточний баланс картки
void MainWindow::on_okButton_page11_clicked()
{
    takeCashQuestion(ui_->lineEdit_enterSumForTransfer_11->text().toULong(),atm_->transactInterest());
}
