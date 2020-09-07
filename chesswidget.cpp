#include "chesswidget.h"
#include "chesslogic.h"
#include "ui_chesswidget.h"

#include <QtGui>
#include <QtWidgets>
#include <QPixmap>
#include <QVector>

ChessWidget::ChessWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ChessWidget)
{
    ui->setupUi(this);

    QGridLayout *topLayout = new QGridLayout(this);
    QFrame *boardFrame = new QFrame(this);
    boardFrame->setFrameShadow(QFrame::Raised);
    boardFrame->setFrameShape(QFrame::Panel);
//    boardFrame->setAccessibleDescription(QString("frame"));
    boardFrame->setLineWidth(4);            // 4 pixel wide frame around the board
    boardFrame->setMidLineWidth(1);        // 4 pixel wide frame around the board
//    boardFrame->setFrameRect(QRect(0,0,64,8*64));
    boardFrame->setFixedSize(64*8+2*boardFrame->lineWidth()+20,
                             64*8+2*boardFrame->lineWidth()+20);
    // board is 8 times square size wide plus frame width

    QVBoxLayout *buttons = new QVBoxLayout();
    QPushButton *newGame = new QPushButton(QString("New game"), this);
    QPushButton *undoMove = new QPushButton(QString("Undo move"), this);
    QPushButton *quit = new QPushButton(QString("Quit game"), this);
    //    QPushButton *newGame = new QPushButton("New game", this);
    QPushButton *moveList = new QPushButton("Move list", this);
    //    QPushButton *memTest = new QPushButton("New game", this);

    buttons->addWidget(newGame);
    buttons->addWidget(undoMove);
    buttons->addWidget(quit);
    buttons->addWidget(moveList);

    buttons->setGeometry(QRect(
                             this->width()-newGame->width(), // left
                             0,                             // int top, alignt top
                             newGame->width(),             // int width,
                             buttons->count()*newGame->height()));// int height);



//    buttons->setGeometry(QRect(550, 0, 100,500));






    connect(newGame, SIGNAL(clicked()), this,  SLOT(_newGame()));
    //connect(undoMove, SIGNAL(clicked()), this,  SLOT(close()));
    connect(quit, SIGNAL(clicked()), this,  SLOT(close()));

    game = new ChessLogic();
    game->board = QVector<ROW_V>(8,ROW_V(8));
    game->moveHistory = new MoveHistory();
    grid = new QGridLayout(boardFrame); // place grid in board frame

    createBoard(*grid);
    grid->setSpacing(0);

    topLayout->addWidget(boardFrame, 0, 0); // (Qt::Alignment) 0x022
    topLayout->addLayout(buttons, 0, 1);

    game->initNewGame();

    refreshImages();
    qDebug() << "chess widget created at:" << this;
 //   qDebug();
 //   t->setPixmap(game->getPiece(0,0)->img);


}

ChessWidget::~ChessWidget()
{
      //  delete game;
    delete ui;

}

void ChessWidget::_newGame()
{
   game->resetGame();
   refreshImages();
}

void ChessWidget::createBoard(QGridLayout &grid)
{
    for(int y=8; y > 0; y--)            // y=8 as addWidget references from 1
    {
        for(int x=1; x <= 8; x++)
        {
            Sqr *temp = createSqr(this, x, y);

            grid.addWidget(temp, y, x); // (widget, row, col)
            game->board[8-y][x-1]=temp;
        }                               // temp drops out of scope here
    }
    return (void)NULL;
}

Sqr * ChessWidget::createSqr(ChessWidget *parent, int x, int y)
{
    //qDebug() << "parent()=" << this->parentWidget();
    Sqr *temp = new Sqr(this);
    temp->setFixedSize(64, 64);

    QPalette tempPalette(temp->palette()) ;
    if((y+x)%2 != 0)
    {   // Set black square color dark brown
        tempPalette.setColor(QPalette::Background,
                             QColor(125, 57, 57));

    } else
    {   // Set white square color light brown
        tempPalette.setColor(QPalette::Background,
                             QColor(207, 184, 184));
    }
    temp->setPalette(tempPalette);
    temp->setAutoFillBackground(true);
    temp->show();
    temp->setFile(x);              // Define board "up side down" as in first
    temp->setRank(9-y);            // vector to represent bottom rank.
    temp->setEnabled(true);        // Enable mouse events.
    return temp;
}

void ChessWidget::refreshImages()
{
    qDebug() << "refreshImages()";

    for(int y=7; y >= 0; y--)
    {
        for(int x=0; x < 8; x++)
        {
            game->board[y][x]->refreshImg();
        }
    }
}

Sqr::Sqr(QWidget *parent) : QLabel(parent), piece(NULL)
{
    qDebug() << "Sqr default const at" << this;
}

Sqr::~Sqr()
{
    qDebug() << "sqr destructed  "<< this;
}

Sqr::Sqr(Sqr *parent) : QLabel(parent), piece(NULL)
{
    setGeometry(parent->geometry());
}

Sqr & Sqr::operator =(Sqr &sqr)
{
    this->img = new QPixmap(*sqr.getImg());
    setPiece(NULL);
    qDebug() << "new copy const at:" << this;
    return *this;
}

Piece * Sqr::piece_ptr()
{
    return (Piece*)piece;
}

void Sqr::setPiece(Piece *p)
{
    piece=p;
}

void Sqr::clearSqr(){
    piece = (Piece*)NULL;
    // removePixmap();
}

void Sqr::refreshImg()
{
    if(piece != NULL)
    {
        img=&(piece->img);
        setPixmap(*img);
    } else
    {
        img=NULL;
        setPixmap(QPixmap()); // set null pixmap
    }

}

void Sqr::deletePiece()
{
    if (piece!=NULL)
    {
        delete piece;
        this->clearSqr();
    }
}

QPixmap* Sqr::getImg()
{
    return img;
}
/*
Sqr * Sqr::childAt(const QPoint & p)
{
    Sqr *temp=

}
*/

void Sqr::setFile(int x)
{
    this->file=x;
}

void Sqr::setRank(int y)
{
   this->rank=y;
}

void Sqr::removePixmap()
{
    setPixmap(QPixmap());
}

int Sqr::getFile() {return file;}
int Sqr::getRank() {return rank;}

void Sqr::mousePressEvent(QMouseEvent *e)
{
   //  e->accept();                            // redundant yet here
   // qDebug() << "mouse event accepted";
    setAccessibleName(QString("clicked"));
    removePixmap();

    //qDebug() << "pixmap removed";

    qDebug() << "grabbed"<< this << e->x() << e->y() << "mouse globalpos" << e->globalPos();
    qDebug() << "grabbed"<< this->getFile() << this->getRank();

    draggedSqr = new Sqr(this);
    draggedSqr->draggedSqr=draggedSqr; // set _ptr to oneself ?
    qDebug() << "created new sqr:" << draggedSqr;
    draggedSqr->setAccessibleName(QString("dragged")); // help debugging

    draggedSqr->setPiece(this->piece_ptr());
    draggedSqr->refreshImg();
    draggedSqr->setAutoFillBackground(false); // make background tranparent
    draggedSqr->setParent(parentWidget()); // now parent = qwidget::chesswidget?
//    parentWidget()->parentWidget()->
    draggedSqr->draggedSqr=NULL; // just to make debugger more readable
    draggedSqr->show();
    qDebug() << draggedSqr->geometry();
 //   qDebug() << "  press sqr:"<<this;
//    qDebug() << "parent:"<< parentWidget();
//    qDebug() << "parent parent:"<< parentWidget()->parentWidget();


}

void Sqr::mouseReleaseEvent(QMouseEvent *e)
{

    draggedSqr->hide();
    if (parentWidget()->childAt(mapToParent(e->pos())) != NULL)
    {
    qDebug() << "on release" << this << "child at:"<< parentWidget()->childAt(mapToParent(e->pos()));
    Sqr *targetSqr =(Sqr*)(parentWidget()->childAt(mapToParent(e->pos())));
    qDebug() << "on release" << this << "over:"
             << targetSqr->getFile()<<targetSqr->getRank();

    qDebug() << "parent:" << parentWidget();
    qDebug() << "parent parent :" << parentWidget()->parentWidget();


    if (this->piece_ptr() != NULL)
    {
        Move *move = new Move();            // create move object to be fed into the
        move->fromFile=this->getFile();     // chess machine from hell
        move->fromRank=this->getRank();
        move->toFile = targetSqr->getFile();
        move->toRank = targetSqr->getRank();
        move->piece = this->piece_ptr();

        // feed the beast
        ((ChessWidget*)(parentWidget()->parentWidget()))->
                game->movePiece(*move);
    }
    targetSqr->refreshImg();

    }
    delete draggedSqr;                  // dispose of the evidence
    refreshImg();
}

void Sqr::mouseMoveEvent(QMouseEvent *e)
{

    draggedSqr->move(mapToParent(e->pos()));
   // qDebug() << "   move  sqr:"<<this << e->pos();

}


