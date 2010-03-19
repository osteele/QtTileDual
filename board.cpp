#include "board.h"
#include "cell.h"

Board::Board(int rows, int cols)
    : rows(rows), cols(cols)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cellList << new Cell(*this, row, col);
        }
    }
}

Cell* Board::cell(int row, int col)
{
    if (0 <= row && row <= rows &&
        0 <= col && col <= cols) {
        return cellList[row * cols + col];
    } else
        return 0;
}

