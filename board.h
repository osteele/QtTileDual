#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsItem>

class Cell;

/*! An instance of this class represents a partition of a rectangular subset of the plane into polygons, where each polygon
  * is either a rectangle within a grid, or one of two right triangles that comprises such a rectangle. These polygons are
  * accessible via the cell() method.
  *
  * This class handles both the model (it specifies the polygons), and the view (it knows how to draw them).
  */
class Board : public QGraphicsItem
{
public:
    const int rows;
    const int cols;
    int cellWidth;
    int cellHeight;

    Board(int rows, int cols);
    //! Retrieves the cell at row, col. The row or column may be outside the board, in which case this method
    //! returns null.
    Cell* cell(int row, int col);
    /// Initialize the cell states to an interesting configuration.
    void setCellStates();
    /// Update the board state.  This is used during animation.
    void updateCellStates();

    // QGraphicsItem API
    QRectF boundingRect() const { return QRectF(0, 0, cols * cellWidth, cols * cellHeight); }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    // QGraphicsItem API
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    static QColor GridColor;
    static QColor DualColor;

    QList<Cell*> cellList;
    Cell* lastCell; //!< Dragging will use this

    void paintGrid(QPainter*);
    void paintDual(QPainter*);
};

#endif // BOARD_H
