#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "cell.h"
#include "board.h"

Cell::Cell(Board& board, int row, int col)
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

int Cell::centerCount() const
{
    return state == Square ? 1 : 2;
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

// Return the center of the polygon that touches the dir border of the cell.
// If the cell is a square, this is the center of the square. Otherwise it's
// the center of one of the two triangles that subdivide the square, where the
// triangles depend on the cell state, and which one depends on dir.
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

// TODO add lines between nodes.
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
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
    exit(1);
    QGraphicsItem::mousePressEvent(event);
}

void Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
