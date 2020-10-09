//------------------------------------------------------------------------------                                                                         
///@file demo.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///@brief   Demonstrate a prototype for Qt Mdi window.
///                                                                             
///@version 2020-10-07  PN     File creation.
//------------------------------------------------------------------------------

#include "lib_qt_mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    //--------------------------------------------------------------------------
    //  Initialize the Qt application.                                          
    //--------------------------------------------------------------------------
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("DATR");
    QCoreApplication::setOrganizationDomain("navy.mil");
    QCoreApplication::setApplicationName("MdiWindowPrototype");

    lib::qt::MainWindow main_window(nullptr);
    main_window.show();

    return app.exec();
}

