#include "Application/application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application app;
    app.show();
    return a.exec();
}
