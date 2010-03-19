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

    Cell(Board&, int row, int col);

    int type() const { return Type; }
    Cell* neighbor(Direction);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

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
    CellState state;

    static int dxs[];
    static int dys[];
};

#endif
