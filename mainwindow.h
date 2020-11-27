#pragma once

#include <QMainWindow>
#include "ATM/Model/atmcard2.h"
#include "ATM/Socket/atmselectorsocket.h"
#include "ATM/Model/atmparams.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include <QLineEdit>
#include "ATM/atm.h"

class ATM;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

private:
    QString pin_;
    size_t sum_;
    ATM* atm_;
    Ui::MainWindow *ui_;

    explicit MainWindow(QWidget *parent = nullptr);

    void activate(size_t);

public:
    static void startMainWindow(const size_t);
    virtual ~MainWindow();

private slots:

    void successStart();
    void showError(const QString&);

    void onSuccessCardInsertion();

    void onSuccessPIN();
    void onWrongPIN(const size_t);


    void onBalCheckedAnswer();

    void onSuccessCashTaken(const long money);
    void onSuccessCashSend();

    void onSuccessPINchange();

    void onSuccessFreeCard();

   //-----------------Utilities----------------------
    QLineEdit* chooseSelectedLineEdit(QLineEdit* line1, QLineEdit* line2);
    void checkSum(size_t);
    void errorMsg(QLineEdit * );

    //--------------------Buttons------------------------

    void on_insertButton_page0_clicked();
    void on_insertButton_page0_2_clicked();
    void on_selectorButton_clicked();

    //-------------Input card number page-----------------------
    void on_num1_4_clicked();

    void on_num2_4_clicked();

    void on_num3_4_clicked();

    void on_num4_4_clicked();

    void on_num5_4_clicked();

    void on_num6_4_clicked();

    void on_num7_4_clicked();

    void on_num8_4_clicked();

    void on_num9_4_clicked();

    void on_num0_4_clicked();

    void on_clearOne_4_clicked();

    void on_clearButton_page4_clicked();

    void on_okButton_page4_clicked();

    void on_backButton_page4_clicked();

    //---------------PIN input page---------------------

    void on_num1_2_clicked();

    void on_num2_2_clicked();

    void on_num3_2_clicked();

    void on_num4_2_clicked();

    void on_num5_2_clicked();

    void on_num6_2_clicked();

    void on_num7_2_clicked();

    void on_num8_2_clicked();

    void on_num9_2_clicked();

    void on_num0_2_clicked();

    //void on_clearOneclicked();

    void on_backButton_page2_clicked();


    void on_okButton_page2_clicked();
    void on_clearOne_2_clicked();
    void on_clearButton_page2_clicked();

    //----------------QR page-------------------

    void on_clearButton_page3_clicked();

    void onSuccessQr();


    //----------------Main menu page-------------------

    void on_cashTransfersButton_clicked();

    void on_cardBalanceButton_clicked();

    void on_cardSettingsButton_clicked();

    void on_cashWithdrawalButton_clicked();

    void on_Button_freeCard_clicked();

    //---------------Settings page---------------------

    void on_Button_changePin_clicked();

    void on_Button_otherATMs_clicked();

    void on_backButton_page7_clicked();

    //-----------------Sum selection page-------------------------

    void on_Button_20grn_clicked();

    void on_Button_50grn_clicked();

    void on_Button_100grn_clicked();

    void on_Button_200grn_clicked();

    void on_Button_500grn_clicked();

    void on_Button_1000grn_clicked();

    void on_backButton_page5_clicked();

    void on_Button_AnySum_clicked();


    //----------------Another sum input page----------------------
    void on_num1_6_clicked();

    void on_num2_6_clicked();

    void on_num3_6_clicked();

    void on_num4_6_clicked();

    void on_num5_6_clicked();

    void on_num6_6_clicked();

    void on_num7_6_clicked();

    void on_num8_6_clicked();

    void on_num9_6_clicked();

    void on_num00_6_clicked();

    void on_num0_6_clicked();

    void on_clearOne_6_clicked();

    void on_clearButton_page6_clicked();

    void on_backButton_page6_clicked();

    void on_okButton_page6_clicked();


 //---------------page 8 -- Change PIN------------------------
    void on_num1_8_clicked();

    void on_num2_8_clicked();

    void on_num3_8_clicked();

    void on_num4_8_clicked();

    void on_num5_8_clicked();

    void on_num6_8_clicked();

    void on_num7_8_clicked();

    void on_num8_8_clicked();

    void on_num9_8_clicked();

    void on_num0_8_clicked();

    void on_clearOne_8_clicked();
    void on_OKButton_page8_clicked();
    void on_clearButton_page8_clicked();
    void on_backButton_page8_clicked();


 //---------------page 9 -- Choose sum for transfer ------------------------
    void on_num1_9_clicked();
    void on_num2_9_clicked();
    void on_num3_9_clicked();
    void on_num4_9_clicked();
    void on_num5_9_clicked();
    void on_num6_9_clicked();
    void on_num7_9_clicked();
    void on_num8_9_clicked();
    void on_num9_9_clicked();
    void on_num0_9_clicked();
    void on_clearOne_9_clicked();
    void on_clearButton_page9_clicked();
    void on_backButton_page9_clicked();
    void on_okButton_page9_clicked();



    void on_Button_20grn_12_clicked();
    void on_Button_50grn_10_clicked();
    void on_Button_100grn_10_clicked();
    void on_Button_200grn_10_clicked();
    void on_Button_500grn_10_clicked();
    void on_Button_1000grn_10_clicked();
    void on_backButton_page10_clicked();
    void on_Button_AnySum_10_clicked();



 //---------------page 11 -- Enter another sum to transfer to another card ------------------------
    void on_num1_11_clicked();
    void on_num2_11_clicked();
    void on_num3_11_clicked();
    void on_num4_11_clicked();
    void on_num5_11_clicked();
    void on_num6_11_clicked();
    void on_num7_11_clicked();
    void on_num8_11_clicked();
    void on_num9_11_clicked();
    void on_num00_11_clicked();
    void on_num0_11_clicked();
    void on_clearOne_11_clicked();
    void on_clearButton_page11_clicked();
    void on_backButton_page11_clicked();
    void on_okButton_page11_clicked();


};

