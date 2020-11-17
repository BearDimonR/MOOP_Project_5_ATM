#include <QApplication>
#include "Utility/utilities.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include "ATM/atm.h"
#include "mainwindow.h"
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // опция 1
    ATMSelector selector;
    ATMSelectorWidget widget(&selector);
    widget.show();
    // при выборе банкомата, атмселектор запустит банкомат
    QObject::connect(&widget, &ATMSelectorWidget::atm_selected, &MainWindow::startMainWindow);
    // опция 2
    // запустить банкомат самому
    // MainWindow::startMainWindow(1);
    return a.exec();
}

