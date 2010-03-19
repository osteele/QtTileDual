#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "cell.h"
#include "board.h"

Cell::Cell(Board& board, int row, int col)
    : board(board), row(row), col(col)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

// FIXME better names
int Cell::dxs[] = {0, 0, -1, 1};
int Cell::dys[] = {0, 0, -1, 1};

Cell* Cell::neighbor(Direction dir)
{
    // this order must match the order of the members of Direction
    //static int[] dys = {-1, 1, 0, 0};
    return board.cell(row + dys[dir], col + dxs[dir]);
}

QRectF Cell::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  23 + adjust, 23 + adjust);
}

QPainterPath Cell::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

// TODO add lines between nodes.
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
}

QVariant Cell::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
