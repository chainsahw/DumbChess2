#include <QApplication>
#include "chesswidget.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    ChessWidget w;
    w.show();
    
    return a.exec();
}
