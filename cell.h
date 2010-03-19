#ifndef CELL_H
#define CELL_H

#include <QGraphicsItem>
#include <QList>

class TilingWidget;
class Board;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Cell : public QGraphicsItem
{
public:
    enum CellState { Square, BendSinister, BendDexter };
    // if you change this, change the statics in Cell::neighbor
    enum Direction { Up, Down, Left, Right };

    CellState state;

    Cell(Board&, int row, int col);

    // topology
    void setState(CellState state) { this->state = state; }
    void setState(int state) { setState(CellState(state)); }
    Cell* neighbor(Direction) const;
    int centerCount() const;

    // geometry
    QPointF topLeft() const;
    QPointF center(Direction) const;

    // QGraphicsItem API
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QCursor cursor() const { return Qt::PointingHandCursor; }
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
private:
    //QList<Edge *> edgeList;
    QPointF newPos;
    //TilingWidget& widget;
    Board& board;
    int row;
    int col;

    static const int dxs[];
    static const int dys[];
};

#endif
