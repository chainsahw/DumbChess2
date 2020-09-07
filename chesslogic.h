#ifndef CHESSLOGIC_H
#define CHESSLOGIC_H

#include <QObject>
#include <QVector>
#include "chesswidget.h"

class ChessLogic;
class Move;
class MoveHistory;
class Images;
class Piece;
class Sqr;


typedef QVector<Sqr *> ROW_V;

//typedef QList<Piece *> ROW_L;

enum Color{
    WHITE, BLACK
};

enum Type{
    BLANK, // zero reserved just in case
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    JEDI
};

enum File{
   ZERO, A, B, C, D, E, F, G, H
};



class Piece : public QObject
{
    Q_OBJECT

public:

    Color color;
    Type type;
    QPixmap img;

    Piece(Color, Type);
    Piece(Color, Type, QPixmap);
    ~Piece();

    void defineImage();
    QPixmap getImage();
};

class ChessLogic : public QObject
{
    Q_OBJECT

public:
    ChessLogic();
  //  ~ChessLogic();

    void initNewGame();
    void initBoard();
    int movePiece(Move);


    static QVector<QVector<Sqr *> > *getBoard();
    QVector<ROW_V> board;// == QVector<QVector<Piece*> >(8,QVector<Piece*>(8));
    Sqr *getPiece(int x, int y);
    void  deletePiece(Sqr*);
    void deleteAllPieces();
    void resetGame();

    MoveHistory *moveHistory;

public slots:


private:

    void move(Move);

};


class Move  : public QObject
{

    Q_OBJECT

public:

    int fromFile;
    int fromRank;
    int toFile;
    int toRank;
    Move *next;
    Move *previous;
    Piece *piece;



    Move();
    Move(const Move &);             // terror from deep copy
    Move(int, int, int, int);
    ~Move();
    int sign(int);
    int greater(int, int);


    Move& operator=(const Move &m); // override to enable self reference
    bool isValid(QVector<ROW_V > &);

private:
    bool resultsInCheck(QVector<ROW_V> &board);
    //    int outsideBoard(); // not gonna happen in widget
    bool freePath(QVector<ROW_V> &board);
    int ownPiece();
    bool traceLine(QVector<ROW_V> &board);

};


class MoveHistory : public QObject
{
    Q_OBJECT

public:
    MoveHistory();
    ~MoveHistory();

    void addMove(Move m);
    int  pop_backMove();
    void clear();

private:

    Move *first;
    Move *last;
    int moveCount;

};

#endif // CHESSLOGIC_H


/**



*/
