//------------------------------------------------------------------------------
///                                                                             
///@file demo.cpp                                                               
///                                                                             
///@brief   Demonstrate that the Qt build is working along with linking         
///         against the datr.open.source code.                                  
///                                                                             
///@version 2020-09-08  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Pull in any of the datr.open.source code files.                             
//------------------------------------------------------------------------------

#include <QApplication>
#include <QLabel>
#include <QString>

int main(int argc, char* argv[])
{
    //--------------------------------------------------------------------------
    //  Initialize the Qt application.                                          
    //--------------------------------------------------------------------------
    QApplication app(argc, argv);

    //--------------------------------------------------------------------------
    //  Create a Qt widget, display it, and execute the Qt application.         
    //--------------------------------------------------------------------------
    QLabel  label("Test");

    label.show();

    return app.exec();
}

