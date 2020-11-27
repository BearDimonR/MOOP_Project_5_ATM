#include <QApplication>
#include "Utility/utilities.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include "ATM/atm.h"
#include "mainwindow.h"
#include <QDebug>
#include "mainwindow.h"

#include <bqrcodegen.h>
#include <bqrdefaultimagefactory.h>
#include <bqrimagefactory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // опция 1
    // при выборе банкомата, атмселектор запустит банкомат


    ATMSelectorWidget::startSelector();
    // опция 2
    // запустить банкомат самому
    // MainWindow::startMainWindow(1);
    return a.exec();
}

