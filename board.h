#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsItem>

class Cell;

/*! An instance of this class represents a partition of a rectangular subset of the plane into polygons, where each polygon
  * is either a rectangle within a grid, or one of two right triangles that comprises such a rectangle. These polygons are
  * accessible via the cell() method.
  */
class BoardModel : public QGraphicsItemGroup
{
public:
    static const QColor GridColor;
    static const QColor DualColor;

    const int rows;
    const int cols;
    int cellWidth;
    int cellHeight;

    BoardModel(int rows, int cols, int cellWidth, int cellHeight);
    //! Retrieves the cell at row, col. The row or column may be outside the board, in which case this method
    //! returns null.
    Cell* cell(int row, int col) const;
    /// Initialize the cell states to an interesting configuration.
    void setCellStates(int strategy=0);

private:
    QList<Cell*> cellList;
};

/*! Renders the graph and the dual graph of a board model.
  */
class BoardView: public QGraphicsItemGroup
{
public:
    static const QColor GridColor;
    static const QColor DualColor;

    const int rows;
    const int cols;
    int cellWidth;
    int cellHeight;

    BoardView(int rows, int cols);
    //! Retrieves the cell at row, col. The row or column may be outside the board, in which case this method
    //! returns null.
    Cell* cell(int row, int col) const { return model->cell(row, col); }
    /// Initialize the cell states to an interesting configuration.
    void setCellStates(int strategy=0);
    /// Update the board state.  This is used during animation.
    void updateCellStates();

    //! 0=focus on the grid; 1=focus on the dual
    void setDualFocus(qreal);

    // QGraphicsItem API
    QRectF boundingRect() const { return QRectF(0, 0, model->cols * model->cellWidth, model->rows * model->cellHeight); }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    // QGraphicsItem API
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    BoardModel *model;
    class BoardGraphView* boardGraphView;
    class BoardDualView* boardDualView;
    Cell* lastCell; //!< Dragging will use this

    void paintDual(QPainter*);
};

//! Responsible for drawing the (non-dual) graph
// TODO BoardGraphview and BoardDualView should share a superclass
class BoardGraphView : public QGraphicsItem
{
public:
    BoardGraphView(const BoardModel* model);

    // QGraphicsItem API
    QRectF boundingRect() const { return QRectF(0, 0, model->cols * model->cellWidth, model->rows * model->cellHeight); }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    friend class BoardView;
    const BoardModel* model;
    class QGraphicsBlurEffect* effect;
};

//! Responsible for drawing the dual
class BoardDualView : public QGraphicsItem
{
public:
    BoardDualView(const BoardModel* model);

    // QGraphicsItem API
    QRectF boundingRect() const { return QRectF(0, 0, model->cols * model->cellWidth, model->rows * model->cellHeight); }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    friend class BoardView;
    const BoardModel* model;
    class QGraphicsBlurEffect* effect;
};

#endif // BOARD_H
