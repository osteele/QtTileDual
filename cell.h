#ifndef CELL_H
#define CELL_H

#include <QGraphicsItem>
#include <QCursor>

class BoardModel;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! A Cell represents one of the square subdivisions of a Board. It is either a single (square) polygon, or
//! two right triangles separated by a diagonal line from top left to bottom right (a "bend dexter"), or two triangles
//! two right triangles separated by a line from bottom left to top right (a "bend sinister"). The \c state field,
//! of type \c CellState, tells which.
class Cell : public QGraphicsItem
{
public:
    //! If and how the square is divided into polygons
    enum CellState { Square, BendSinister, BendDexter };
    static const int StateCount = 3;
    //! if you change this, change the statics in Cell::neighbor
    enum Direction { Up, Down, Left, Right };

    CellState state;

    Cell(const BoardModel&, int row, int col);

    // topology
    void setState(CellState state) { this->state = state; }
    void setState(int state) { setState(CellState(state)); }
    Cell* neighbor(Direction) const;
    bool isDivided() const; //!< true iff the cell represents two triangles (instead of a single square)

    // geometry
    QPointF topLeft() const;
    // Return the center of the polygon that touches the dir border of the cell.
    // If the cell is a square, this is the center of the square. Otherwise it's
    // the center of one of the two triangles that subdivide the square, where the
    // triangles depend on the cell state, and which one depends on dir.
    QPointF center(Direction) const;

    // QGraphicsItem API
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QCursor cursor() const { return Qt::PointingHandCursor; }
    
protected:
    // QGraphicsItem API
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    const BoardModel& board;
    int row;
    int col;

    // see the impl file
    static const int dxs[];
    static const int dys[];
};

#endif // CELL_H
