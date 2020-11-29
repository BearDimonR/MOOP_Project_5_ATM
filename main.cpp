#include <QApplication>
#include "Utility/utilities.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include "ATM/atm.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // free map which was used to init static variables
    Utility::getInstance().freeInstance();
    qInstallMessageHandler(log);

	QApplication a(argc, argv);
    // опція 1
    // при виборі банкомата атмселектор запускає сам
    ATMSelectorWidget::startSelector();
    // опція 2
    // запустити банкомат самому (якщо він доступний)
    // MainWindow::startMainWindow(1);
    return a.exec();
}

