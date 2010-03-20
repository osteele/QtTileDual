#include "boardmodel.h"
#include "cell.h"

BoardModel::BoardModel(int rows, int cols, int cellWidth, int cellHeight)
    : rows(rows), cols(cols),
      cellWidth(cellWidth), cellHeight(cellHeight)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cellList << new Cell(*this, row, col);
        }
    }
}

Cell* BoardModel::cell(int row, int col) const
{
    if (0 <= row && row < rows &&
        0 <= col && col < cols) {
        return cellList[row * cols + col];
    } else
        return 0;
}

void BoardModel::setCellStates(int strategy)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int state = 0;
            switch (strategy) {
            case 0: state = 0; break;
            case 1: state = 1; break;
            case 2: state = i^j; break;
            }
            cell(i, j)->setState(state % Cell::StateCount);
        }
    }
}
