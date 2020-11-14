#include <QApplication>
#include "Utility/utilities.h"
#include "ATMSelector/atmselector.h"
#include "ATMSelector/atmselectorwidget.h"
#include "ATM/atm.h"
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ATMSelector selector;
    ATMSelectorWidget widget(&selector);
    widget.show();

    //qDebug() << Utility::UtilityError("", Utility::UtilityError::ErrorCodes::FILE_ERROR);


    //  selector.refreshATMParams();




    //TODO add alternate launch ATM
    return a.exec();
}

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w(0);
//     w.show();
//     return a.exec();
// }
