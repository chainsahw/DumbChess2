#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QVector>
#include <QImage>
#include <QtGui>
#include <QtWidgets>
#include <QImage>
#include <QVector>
#include "chesslogic.h"


namespace Ui {
    class ChessWidget;
}

class Images;
class Piece;
class ChessLogic;
class Sqr;
class ChessWidget;
//enum Column;

//typedef QVector<Piece*> ROW_V;
//typedef QList<Piece *> ROW_L;

// using namespace Images;

class ChessWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ChessWidget(QWidget *parent = 0);
    ~ChessWidget();

    void createBoard(QGridLayout &);

    QGridLayout *grid;

    void refreshImages();
    Sqr         *draggedPiece;
    Sqr         *createSqr(ChessWidget *parent, int x, int y);
    //Move        *move;
    ChessLogic  *game;

private:
    Ui::ChessWidget *ui;

    QVBoxLayout *buttons;
    QPushButton *newGame;
    QPushButton *undoMove;
    QPushButton *quit;



//        Piece *board[8][8];

//signals:


public slots:


    void _newGame();

};

class Sqr : public QLabel
{
    Q_OBJECT

    int file;
    int rank;
    QPixmap *img;
    Piece *piece;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);


public:
    Sqr(QWidget *parent);
    Sqr(Sqr *parent);
    Sqr(Sqr &sqr);
    ~Sqr();

    int getFile();
    int getRank();
    void setFile(int);
    void setRank(int);
    Sqr *draggedSqr;

    void setPiece(Piece*);
    void clearSqr();
    Piece * piece_ptr();
    QPixmap * getImg();
    void refreshImg();
    void deletePiece();
    void removePixmap();
    Sqr * childAt(const QPoint & p);
    Sqr & operator=(Sqr &);

};

class Icon : public Sqr
{


};

#endif // CHESSWIDGET_H

