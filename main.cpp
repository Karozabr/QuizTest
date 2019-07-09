#include "startwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication Quiz(argc, argv);
    StartWindow StartW;
     StartW.show();

    return Quiz.exec();
}
