#include "chesslogic.h"
#include "chesswidget.h"
#include <QObject>
#include <QVector>
#include <QtDebug>
#include <QPixmap>
#include <QDebug>

ChessLogic::ChessLogic()
{
    qDebug() << "new logic created at " << this;

}

/*
ChessLogic::~ChessLogic()
{
    qDebug() << '1';
    //delete images;
    moveHistory->clear(); // Can't leave these for Qt object handling
    qDebug() << '2';
    delete moveHistory;
    qDebug() << '3';
    deleteAllPieces();
    qDebug() << '4';
}
*/

/*
QVector<QVector<Sqr *> > * ChessLogic::getBoard()
{
    return (QVector<QVector<Sqr *> >*)&board;
}

*/

void ChessLogic::initNewGame()
{
    qDebug() << "initNewGame() " << this;
//    initBoard();
    resetGame();

}

int ChessLogic::movePiece(Move m)
{
    int validity = 0; // init just in case
    if((validity = m.isValid(board)))
    {
        move(m);
    }
    return validity;
}


void ChessLogic::deletePiece(Sqr* s)
{
    s->deletePiece();
    return (void)NULL;
}

void ChessLogic::deleteAllPieces()
{
    for(int y=7; y >= 0; y--)
    {
        for(int x=0; x < 8; x++)
        {
            if(board[y][x]->piece_ptr() != NULL)
            {
                //qDebug() << x << ' '<<y;
                deletePiece(board.at(y).at(x));
                // board[y][x]=(Piece*)NULL; //deletePiece(board[y][x]);
            }
        }
    }
    qDebug() << "deleteAllPieces() finished";
}

void ChessLogic::initBoard() // never called
{
    qDebug() << "initBoard() " << this;
    qDebug() << board;

    qDebug() << "board00" <<board[0][0];
    qDebug() << "board.at0.at0" << board.at(0).at(0);
    qDebug() << "board7.size()" << board[7].size();

    for(int y=7; y >= 0; y--)
    {

        for(int x=0; x < 8; x++)
        {

            //board[y][]->=NULL;
        }
    }
    return (void)NULL;
}
/*
Sqr* ChessLogic::getPiece(int x, int y)
{
    qDebug() << "getPiece() *";
    return board[x][y];
}
*/

void ChessLogic::resetGame()
{
    deleteAllPieces();
    moveHistory->clear();

    board[0][0]->setPiece(new Piece(WHITE, ROOK)  );
    board[0][1]->setPiece(new Piece(WHITE, KNIGHT));
    board[0][2]->setPiece(new Piece(WHITE, BISHOP));
    board[0][3]->setPiece(new Piece(WHITE, QUEEN) );
    board[0][4]->setPiece(new Piece(WHITE, KING)  );
    board[0][5]->setPiece(new Piece(WHITE, BISHOP));
    board[0][6]->setPiece(new Piece(WHITE, KNIGHT));
    board[0][7]->setPiece(new Piece(WHITE, ROOK)  );

    board[7][0]->setPiece(new Piece(BLACK, ROOK)  );
    board[7][1]->setPiece(new Piece(BLACK, KNIGHT));
    board[7][2]->setPiece(new Piece(BLACK, BISHOP));
    board[7][3]->setPiece(new Piece(BLACK, QUEEN) );
    board[7][4]->setPiece(new Piece(BLACK, KING)  );
    board[7][5]->setPiece(new Piece(BLACK, BISHOP));
    board[7][6]->setPiece(new Piece(BLACK, KNIGHT));
    board[7][7]->setPiece(new Piece(BLACK, ROOK)  );

    for(int y=0; y < 8; y++)
    {
        board[1][y]->setPiece(new Piece(WHITE, PAWN));
        board[6][y]->setPiece(new Piece(BLACK, PAWN));
    }
    qDebug() << "resetGame() finished";

}



void ChessLogic::move(Move m)
{
    board[m.toRank-1][m.toFile-1]->setPiece(
            board[m.fromRank-1][m.fromFile-1]->piece_ptr());

    board[m.fromRank-1][m.fromFile-1]->clearSqr();
}


// MOVE PUBLIC
Move::Move(): fromFile(0), fromRank(0),
    toFile(0), toRank(0), next(NULL), previous(NULL) {}
Move::~Move()
{
//     previous->next=NULL; // no, no. pointer is reapplied with delete
}

Move::Move(const Move &m):
    fromFile(m.fromFile),
    fromRank(m.fromRank),
    toFile(m.toFile),
    toRank(m.toRank),
    next(NULL),
    previous(NULL), piece(m.piece)
{

}

Move::Move(int fromX, int fromY, int toX, int toY):
    fromFile(fromX), fromRank(fromY), toFile(toX), toRank(toY),
    next(NULL), previous(NULL){}


Move& Move::operator=(const Move &m)
{
    return *this;
}

int Move::ownPiece()
{
    return 0;
}

MoveHistory::MoveHistory():first(NULL), last(NULL), moveCount(0)
{
    qDebug()<< "history luotu " << this;
}

MoveHistory::~MoveHistory()
{
    qDebug()<< "history tuhottu " << this;

}

void MoveHistory::addMove(Move m)
{
    Move *temp = new Move(m);

    if(first==NULL)
    {
        first = temp;
        last  = temp;
    }
    else
    {
        temp->previous=last;
        last->next=temp;
        last = temp;
    }
    moveCount++;
}

int MoveHistory::pop_backMove()
{
    if(first==NULL)
    {
        return 0;
    } else
    {

        if(first==last)
        {
            delete first;
            first=NULL;
            last=NULL;
        }
        else
        {
            Move *temp=last;
            last=last->previous;
            last->next=NULL;
            delete temp;
        }

    }
    moveCount--;
    return 1;

}

void MoveHistory::clear()
{

    while(pop_backMove() != 0); // uhh...?
}


bool Move::resultsInCheck(QVector<ROW_V> &board)
{
    return false;
}

bool Move::isValid(QVector<ROW_V> &board)
{
    bool validity = false;
    int deltaFile = toFile - fromFile;
    int deltaRank = toRank - fromRank;
    // first test delta zero move
    if(fromFile == toFile && fromRank == toRank)
    {
        return validity;
    }

    switch(piece->color)
    {
    case WHITE:
        switch(piece->type)
        {
        case BLANK:
            qDebug("You are trying to move a BLANK piece?!?!");
            break;
        case PAWN:
            if(((abs(toRank-fromRank)) == (abs(toFile - fromFile)) == 1) &&
                    freePath(board))
            {
                return true;
            }
            else if(fromRank == 2)
            {
                if((toFile == fromFile) && (toRank-fromRank < 3)
                        && freePath(board) && !resultsInCheck(board))
                {
                    qDebug() << "return true from white";
                    return true;
                }
            }
            else if((toRank - fromRank == 1) && (fromFile == toFile)
                    && freePath(board) && !resultsInCheck(board))
            {
                return true;
            }
            else
            {
                qDebug() << "the pwan goes: weird shit, dude. ";
                return false;
            }
            break;
        case KNIGHT:
            if( (freePath(board) && !resultsInCheck(board) &&
               ((abs(toFile-fromFile) == 2 && abs(toRank-fromRank) == 1)) ||
               ((abs(toFile-fromFile) == 1 && abs(toRank-fromRank) == 2))))
            {
                return true;
            }
            break;
        case BISHOP:
            if(abs(deltaFile) == abs(deltaRank) && (deltaRank) != 0
                    && !resultsInCheck(board) && freePath(board)   )
            {
                return true;
            }
            break;
        case ROOK:
            if((( (deltaFile) != 0 && (deltaRank) == 0) ||
                ( (deltaFile) == 0 && (deltaRank) != 0))
                    && !resultsInCheck(board) && freePath(board))
            {
                return true;
            }
            break;
        case QUEEN:
            if( ( abs(toFile-fromFile) == abs(toRank-fromRank) ||
                   (toFile-fromFile  == 0) || (toRank-fromRank == 0 ) )
                    && freePath(board) && !resultsInCheck(board))
            {
                return true;
            }
            break;
        case KING:
            if( abs(fromFile-toFile) <= 1 && abs(toRank-fromRank) <= 1
                && !resultsInCheck(board) && freePath(board) )
            {
                return true;
            }

            break;
        case JEDI:
            qDebug("HOLY SHIT! You have a jedi in your ranks!");
            break;

        }
            break;
    case BLACK:
        switch(piece->type)
        {
        case BLANK:
            qDebug("You are trying to move a BLANK piece?!?!");
            break;
        case PAWN:
            if(abs(fromRank-toRank) == abs(toFile - fromFile) == 1 &&
                    freePath(board))
            {
                return true;
            }
            else if(fromRank == 7) // black pawn from rank 7
            {
                if((toFile == fromFile) && (toRank-fromRank > -3)
                        && freePath(board) && !resultsInCheck(board))
                {
                    return true;
                }
            }
            else if((toRank - fromRank == -1) && (fromFile == toFile)
                    && freePath(board) && !resultsInCheck(board))
            {
                return true;
            }
            else
            {
                qDebug() << "the pwan goes: weird shit, dude. ";
                return false;
            }
            break;
        case KNIGHT:
            if( freePath(board) && !resultsInCheck(board) &&
                    (abs(toFile-fromFile) == 2 && abs(toRank-fromRank) == 1 ||
                     abs(toFile-fromFile) == 1 && abs(toRank-fromRank) == 2))
            {
                return true;
            }
            break;
        case BISHOP:
            if(abs(fromFile-toFile) == abs(fromRank-toRank) && (fromRank-toRank)!=0
                    && !resultsInCheck(board) && freePath(board)   )
            {
                return true;
            }
            break;
        case ROOK:
            if( ( (fromFile-toFile)!=0 && (fromRank-toRank)==0) ||
                ( (fromFile-toFile)==0 && (fromRank-toRank)!=0)
                    && !resultsInCheck(board) && freePath(board))
            {
                return true;

            }
            break;
        case QUEEN:
            if( ( abs(toFile-fromFile) == abs(toRank-fromRank) ||
                   (toFile-fromFile  == 0) || (toRank-fromRank == 0 ) )
                    && freePath(board) && !resultsInCheck(board))
            {
                return true;
            }
            break;
        case KING:
            if(abs(fromFile-toFile) <= 1 && abs(toRank-fromRank) <= 1
                && !resultsInCheck(board) && freePath(board) )
            {
                return true;
            }

            break;
        case JEDI:
            qDebug("HOLY SHIT! You have a jedi in your ranks!");
            break;
        }
        break;
    }

    return validity;
}

int Move::sign(int x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;

}

int Move::greater(int x, int y) // fit for move delta comparison only
{
    if (abs(x) >= abs(y)) return abs(x);
    if (abs(y) > abs(x)) return abs(y);
    return 0;

}

bool Move::traceLine(QVector<ROW_V> &board)
{
    int deltaFile = toFile - fromFile;
    int deltaRank = toRank - fromRank;
    qDebug() << "greater (or equal)" << greater(deltaFile, deltaRank);

    for (int i=1; i <= greater(deltaFile, deltaRank)-1; i++)
    {
        if( board[(fromRank+sign(deltaRank)*i)-1]
                [(fromFile+sign(deltaFile)*i)-1]
                ->piece_ptr() != NULL  )
        {
            qDebug() << "line obstructed";
            return false;
        }
    }
    if(board[toRank-1][toFile-1]->piece_ptr()!=NULL)
    {
        if(board[toRank-1][toFile-1]->piece_ptr()->color
                == piece->color)
        {
            qDebug() << "capturing same colour";
            return false;
        }
    }
    return true;
}

bool Move::freePath(QVector<ROW_V> &board)
{
    int deltaFile=toFile-fromFile;
    int deltaRank=toRank-fromRank;
   // int repeat; // not used atm

    switch(piece->color)
    {
    case WHITE:
        switch(piece->type)
        {
        case BLANK:
            qDebug("You are trying to move a BLANK piece?!?!");
            break;
        case PAWN:
            if(abs(toFile-fromFile) == 1 && (toRank-fromRank == 1)) // diagonal capture
            {
                if(board[toRank-1][toFile-1]->piece_ptr() != NULL)
                {
                    if(board[toRank-1][toFile-1]->piece_ptr()->color
                            != piece->color)
                    {
                        qDebug() << "white capping opponent color";
                        return true;
                    } else
                    {
                        qDebug() << "white pawn capping white piece";
                        return false;
                    }
                }
            } else
                if(fromRank==2)
                {
                    if( (deltaFile == 0) && deltaRank < 3 )
                    {
                        for(int i=1; i <= ( abs(deltaRank) ); i++)
                        {
                            if( board[fromRank+i-1][toFile-1]->piece_ptr() != NULL )
                            {
                                qDebug() << "targer square occupied";
                                return false; // target square occupied
                            }
                        }
                        return true; // target square free
                    }
                }
                else if (deltaRank == 1 && deltaFile == 0) // one step move
                {
                    if( board[toRank-1][toFile-1]->piece_ptr() != NULL )
                    {
                        qDebug() << "targer square occupied";
                        return false; // occupied
                    } else return true; // free
                } else
                {
                    qDebug() << "weird shit, dude. ";
                    return false;
                }
            break;

        case KNIGHT:
                if(board[toRank-1][toFile-1]->piece_ptr() != NULL)
                {
                    if(board[toRank-1][toFile-1]->piece_ptr()->color
                            == piece->color  )
                    {
                        qDebug() << "white KNIGHT capturing white piece";
                        return false;
                    }
                }
                return true;
            break;
        case BISHOP:
            return traceLine(board);
            break;
        case ROOK:
            return traceLine(board);
            break;
        case QUEEN:
            return traceLine(board);
            break;
        case KING:
            return traceLine(board);
            break;
        case JEDI:
            qDebug("HOLY SHIT! You have a jedi in your ranks!");
            break;
        }
    break; // break case WHITE:
    case BLACK:
        switch(piece->type)
        {
        case BLANK:
            qDebug("You are trying to move a BLANK piece?!?!");
            break;
        case PAWN:
            if(abs(deltaFile) == 1 && (deltaRank == -1)) // diagonal capture
            {
                if(board[toRank-1][toFile-1]->piece_ptr() != NULL)
                {
                    if(board[toRank-1][toFile-1]->piece_ptr()->color
                            != piece->color)
                    {
                        qDebug() << "black capping opponent color";
                        return true; // capping opponen color
                    } else
                    {
                        qDebug() << "black pawn capping blakc piece";
                        return false;
                    }
                }
            } else
                if(fromRank==7)
                {
                    if( (deltaFile == 0) && deltaRank > -3 )
                    {
                        for(int i=1; i <= ( abs(deltaRank) ); i++)
                        {
                            if( board[fromRank-i-1][toFile-1]->piece_ptr() != NULL )
                            {
                                qDebug() << "targer square occupied";
                                return false; // target square occupied
                            }
                        }
                        return true; // target square free
                    }
                } else
                    if (deltaRank == -1 && deltaFile == 0) // one step move
                    {
                        if( board[toRank-1][toFile-1]->piece_ptr() != NULL )
                        {
                            qDebug() << "targer square occupied";
                            return false; // occupied
                        } else return true; // free
                    } else
                    {
                        qDebug() << "black pwn goes: weird shit, dude. ";
                        return false;
                    }
            break;

        case KNIGHT:
                if(board[toRank-1][toFile-1]->piece_ptr() != NULL)
                {
                    if(board[toRank-1][toFile-1]->piece_ptr()->color
                            == piece->color  )
                    {
                        qDebug() << "black KNIGHT capturing black piece";
                        return false;
                    }
                }
                return true;
            break;
        case BISHOP:
            for (int i=1; i <= (abs(deltaRank)-1); i++)
            {
                if( board[(fromRank+sign(deltaRank)*i)-1]
                        [(fromFile+sign(deltaFile)*i)-1]
                        ->piece_ptr() != NULL  )
                {
                    return false;
                }
            }
            if(board[toRank-1][toFile-1]->piece_ptr()!=NULL)
            {
                if(board[toRank-1][toFile-1]->piece_ptr()->color
                        == piece->color)
                {
                    qDebug() << "black bishop capturing black piece";
                    return false;
                }
            }
            return true;
            break;
        case ROOK:

            if( deltaRank == deltaFile ) // to double check
            {
                qDebug() << "something in moving rook has gone wrong";
                qDebug() << deltaFile << deltaRank;
                return false;
            }
            if(board[toRank-1][toFile-1]->piece_ptr() != NULL)
            {
                if(board[toRank-1][toFile-1]->piece_ptr()->color
                        == piece->color)
                {
                    return false;
                    qDebug() << "black rook capturing black piece";
                }
            }
            // here targetSqr is determined as either null of black
            for(int i=0; i < abs(deltaFile+deltaRank)-1; i++)
            {
                if(board[8-(fromRank+sign(deltaRank)*i)]
                        [   fromFile+sign(deltaFile)*i-1]->piece_ptr() != NULL)
                    return false;
            }
            return true;
            break;
        case QUEEN:

            return traceLine(board);
            break;
        case KING:
            if(board[toRank-1][toFile-1]->piece_ptr() != NULL) //
            {
                if(board[toRank-1][toFile-1]->piece_ptr()->color
                        == piece->color)
                {
                    qDebug() << "black king  capturing black piece";
                    return false;
                }
            } else return true;

        break;
        case JEDI:
            qDebug("HOLY SHIT! You have a jedi in your ranks!");
            break;


        }
            break;
    }
    return 0;
}





Piece::Piece(Color c, Type t) : color(c),type(t)
{
     //   qDebug() << c << t << "luotu"<< this;
    defineImage();

  //  qDebug() << "ja image kans" << getImage();
}


Piece::Piece(Color c, Type t, QPixmap i) : color(c), type(t), img(i)
{
    qDebug() << c << t << "luotu imagella"<< this;
}

QPixmap Piece::getImage()
{
    return img;

}

Piece::~Piece()
{
    qDebug() << color << type << "tuhottu"<< this;

    // delete img;
}

void Piece::defineImage()
{
    if(color==WHITE)
    {
        switch (type)
        {
        case BLANK:
            qDebug("white BLANK. the ip should not be here in the first place");
            break;
        case PAWN:
            img=QPixmap (QString(":/icons/WP.png"));
            break;
        case KNIGHT:
            img=QPixmap (QString(":/icons/WN.png"));
            break;
        case BISHOP:
            img=QPixmap (QString(":/icons/WB.png"));
            break;
        case ROOK:
            img=QPixmap (QString(":/icons/WR.png"));
            break;
        case QUEEN:
            img=QPixmap (QString(":/icons/WQ.png"));
            break;
        case KING:
            img=QPixmap (QString(":/icons/WK.png"));
            break;
        case JEDI:
            qDebug("A jedi!");
            break;
        }
    } else
    {
        switch (type)
        {
        case BLANK:
            qDebug("black BLANK. the ip should no be here in the first place");
            break;
        case PAWN:
            img=QPixmap (QString(":/icons/BP.png"));
            break;
        case KNIGHT:
            img=QPixmap (QString(":/icons/BN.png"));
            break;
        case BISHOP:
            img=QPixmap (QString(":/icons/BB.png"));
            break;
        case ROOK:
            img=QPixmap (QString(":/icons/BR.png"));
            break;
        case QUEEN:
            img=QPixmap (QString(":/icons/BQ.png"));
            break;
        case KING:
            img=QPixmap (QString(":/icons/BK.png"));
            break;
        case JEDI:
            qDebug("A black jedi!");
            break;
        }
    }
}

