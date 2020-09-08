//------------------------------------------------------------------------------
///@file fileapp.cpp
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE
///
///@version 2020-08-25  PN     Update documentation to DATR standards.
///@version 2020-08-19  PN     Changed filename to fileapp. Added
///                            QCoreApplication setters.
///@version 2020-08-12  PN     Update include file paths.
///@version 2020-08-11  PN     File creation
//------------------------------------------------------------------------------

#include "lib_qt_pathfield.h"

#include <iostream>
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    std::cout << "Starting Appliction" << std::endl << std::endl;
    QApplication app(argc, argv);

//------------------------------------------------------------------------------
// Developers that use pathfield class will need to set these fields
//------------------------------------------------------------------------------
    QCoreApplication::setOrganizationName("DATR");
    QCoreApplication::setOrganizationDomain("navy.mil");
    QCoreApplication::setApplicationName("FileApp");

    lib::qt::PathField path_field("file", nullptr);
    path_field.show();

    return app.exec();
}
