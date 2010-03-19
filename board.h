#ifndef BOARD_H
#define BOARD_H

#include <QtGui/QGraphicsView>

class Cell;
//#include "cell.h"

class Board
{
public:
    const int rows;
    const int cols;

    Board(int rows, int cols);
    Cell* cell(int row, int col);
private:
    QList<Cell *> cellList;
};

#endif // BOARD_H
