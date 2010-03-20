#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QObject>
#include <QList>

class Cell;

/*! An instance of this class represents a partition of a rectangular subset of the plane into polygons, where each polygon
  * is either a rectangle within a grid, or one of two right triangles that comprises such a rectangle. These polygons are
  * accessible via the cell() method.
  */
class BoardModel : public QObject
{
public:
    const int rows;
    const int cols;
    int cellWidth;
    int cellHeight;

    BoardModel(int rows, int cols, int cellWidth, int cellHeight);

    //! Retrieves the cell at row, col. The row or column may be outside the board, in which case this method
    //! returns null.
    Cell* cell(int row, int col) const;

    void setPattern(const class TilingStrategy&);

private:
    QList<Cell*> cellList;
};

#endif // BOARDMODEL_H
