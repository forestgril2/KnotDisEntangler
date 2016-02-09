#include <QtGui/QApplication>
#include "KnotDisentangler2.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    KnotDisentangler2 knotdisentangler2;
    knotdisentangler2.show();
    return app.exec();
}
