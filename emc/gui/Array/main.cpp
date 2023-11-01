#include "array.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Array w;
    w.show();
    
    return a.exec();
}
