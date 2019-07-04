#include "startwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication Quiz(argc, argv);
    StartWindow MainW;
     MainW.show();

    return Quiz.exec();
}
