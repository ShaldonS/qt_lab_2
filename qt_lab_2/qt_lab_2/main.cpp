#pragma once
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w(NULL);
    QGLFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 5);
    format.setProfile(QGLFormat::CoreProfile);
    // must be called before the widget or its parent window gets shown
    w.setFormat(format);
    w.LoadData("data/FOURDIX-1.bin");
    w.show();
    return a.exec();

}
