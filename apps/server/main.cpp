
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QFileInfo>
#include <QDateTime>

#include <QPluginLoader>
#include <QTranslator>
#include "ctkLog.h"
#include "mainwindow.h"

//Qt5通过以下来支持中文显示
#ifdef WITH_QT5
#ifdef _WIN32
#pragma  execution_character_set("utf-8")
#endif
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    //用于支持中文路径
    QTextCodec *codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);


    //Qt4通过以下两句来支持中文显示
#ifndef WITH_QT5
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif

    MainWindow wnd;
    wnd.show();
    return app.exec();

}
