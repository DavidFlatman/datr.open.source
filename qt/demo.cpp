#include "lib_bits_work.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QLabel  label("Hello World!");

    label.show();

    char memory[] { 0x01, 0xFF, 0x02 };

    if (lib::bits::work::isBitSet(&memory[0], 2))
    {
        std::cout << "set" << std::endl;
    }

    return app.exec();
}

