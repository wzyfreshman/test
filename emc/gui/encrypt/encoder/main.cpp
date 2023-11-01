#include <QApplication>
#include <QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec *code = QTextCodec::codecForName("utf-8");
    //QTextCodec::setCodecForTr(code );
    QTextCodec::setCodecForLocale(code );
    //QTextCodec::setCodecForCStrings(code );
    w.setExcutePath(argv[0]);
    w.show();
    return a.exec();
}
