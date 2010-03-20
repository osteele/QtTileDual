#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "cell.h"
#include "boardmodel.h"

Cell::Cell(const BoardModel& board, int row, int col)
    : state(Square), board(board), row(row), col(col)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

// FIXME better names
const int Cell::dxs[] = {0, 0, -1, 1};
const int Cell::dys[] = {0, 0, -1, 1};

Cell* Cell::neighbor(Direction dir) const
{
    // this order must match the order of the members of Direction
    //static int[] dys = {-1, 1, 0, 0};
    return board.cell(row + dys[dir], col + dxs[dir]);
}

bool Cell::isDivided() const
{
    return state != Square;
}

struct SubPoly {
    const Cell::Direction hdir;
    const Cell::Direction vdir;
    const int dx;
    const int dy;
    SubPoly(int dx, int dy)
        : hdir(dx == -1 ? Cell::Left : Cell::Right),
          vdir(dy == -1 ? Cell::Up : Cell::Down),
          dx(dx), dy(dy)
    {}
    bool touches(Cell::Direction dir) const
    {
        if (dx == 0 && dy == 0) return true;
        return hdir == dir || vdir == dir;
    }
};

const SubPoly TotalSquare(0, 0);
const SubPoly TopLeft(-1, -1);
const SubPoly TopRight(1, -1);
const SubPoly BottomLeft(-1, 1);
const SubPoly BottomRight(1, 1);

static const SubPoly& findPoly(Cell::CellState state, Cell::Direction dir)
{
    if (state == Cell::Square) return TotalSquare;
    const SubPoly& first = state == Cell::BendDexter ? TopRight : TopLeft;
    const SubPoly& second = state == Cell::BendDexter ? BottomLeft : BottomRight;
    return first.touches(dir) ? first : second;
}

QPointF Cell::center(Direction dir) const
{
    // Center of the square cell. This is adjusted based on the cell type and direction.
    float x = board.cellWidth / 2;
    float y = board.cellHeight / 2;
    // Displacement from center of square towards center of triangle.
    float dx = board.cellWidth / 4;
    float dy = board.cellHeight / 4;
    const SubPoly& which = findPoly(state, dir);
    x += which.dx * dx;
    y += which.dy * dy;
    return topLeft() + QPointF(x, y);
}

QPointF Cell::topLeft() const
{
    return QPointF(col * board.cellWidth, row * board.cellHeight);
}

QRectF Cell::boundingRect() const
{
    QPointF tl = topLeft();
    return QRectF(tl, tl + QPointF(board.cellWidth, board.cellHeight));
}

// TODO draw the lines between nodes here?
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    //exit(0);
    //painter->setPen(QPen(Qt::red, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->setBrush(QBrush(Qt::red, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->fillRect(boundingRect(), Qt::red);
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    qDebug() << "down";
}

void Cell::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
    qDebug() << "move";
}

void Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    qDebug() << "up";
}


