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
#include "lib_bits_work.h"

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
    //  Call some datr.open.source code.                                        
    //--------------------------------------------------------------------------
    char memory[] { 0x55, 0xAA};

    QString message("Bit pattern: ");

    for (int i=0; i < sizeof(memory) * 8; ++i)
    {
        message += (lib::bits::work::isBitSet(&memory[0], i)) ? "1" : "0";
    }


    //--------------------------------------------------------------------------
    //  Create a Qt widget, display it, and execute the Qt application.         
    //--------------------------------------------------------------------------
    QLabel  label(message);

    label.show();

    return app.exec();
}

