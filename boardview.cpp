#include <QDebug.h>
#include <QtGui/QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsBlurEffect>

#include "osutils.h"
#include "boardview.h"
#include "cell.h"

/*
 * View
 */

const QColor BoardView::GridColor = Qt::darkGray;
const QColor BoardView::DualColor = ColorUtils::alpha(Qt::blue, 0.75);

BoardView::BoardView(int rows, int cols)
    : rows(rows), cols(cols),
      cellWidth(50), cellHeight(50),
      model(new BoardModel(rows, cols, cellWidth, cellHeight)),
      boardGraphView(new BoardGraphView(model)),
      boardDualView(new BoardDualView(model)),
      lastCell(0)
{
    addToGroup(boardGraphView);
    addToGroup(boardDualView);
#if 0
    for (int i = 0; i < model->rows; i++) {
        for (int j = 0; j < model->rows; j++) {
            addToGroup(model->cell(i, j));
        }
    }
#endif
    setDualFocus(1);
}

void BoardView::setDualFocus(qreal s)
{
    boardGraphView->effect->setBlurRadius(1 + 5 * s);
    boardDualView->effect->setBlurRadius(1 + (1 - s) * 5);
}

void BoardView::updateCellStates()
{
    Cell& cell = *model->cell(qrand() % rows, qrand() % cols);
    cell.setState((cell.state + 1) % Cell::StateCount);
    update(); // TODO perf update cell and neighbors
}

void BoardView::setPattern(const TilingStrategy& strategy)
{
    model->setPattern(strategy);
    update();
}


void BoardView::update(const QRectF& rect)
{
    boardGraphView->update();
    boardDualView->update();
    QGraphicsItemGroup::update(rect);
}

void BoardView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int col = event->pos().x() / cellWidth;
    int row = event->pos().y() / cellHeight;
    Cell* cell = this->cell(row, col);
    qDebug() << "press " << col << ", " << row;
    if (cell && cell != this->lastCell) {
        qDebug() << "update";
        cell->setState((cell->state + 1) % Cell::StateCount);
        update();
    }
    this->lastCell = cell;
    QGraphicsItem::mousePressEvent(event);
}

// FIXME this is never called. Figure out the magic.
void BoardView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "move";
    mousePressEvent(event);
    QGraphicsItem::mouseMoveEvent(event);
}

void BoardView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "up";
    this->lastCell = 0;
    QGraphicsItem::mouseReleaseEvent(event);
}

/*
 * Grid view
 */

BoardGraphView::BoardGraphView(const BoardModel* model) : model(model), effect(new QGraphicsBlurEffect)
{
    setGraphicsEffect(effect);
}

void BoardGraphView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    int cols = model->cols;
    int rows = model->rows;
    int cellWidth = model->cellWidth;
    int cellHeight = model->cellHeight;

    QPainterPath path;
    // vertical lines
    for (int col = 0; col <= cols; col++) {
        path.moveTo(col * cellWidth, 0);
        path.lineTo(col * cellWidth, rows * cellHeight);
    }
    // horizontal lines
    for (int row = 0; row <= rows; row++) {
        path.moveTo(0, row * cellHeight);
        path.lineTo(cols * cellWidth, row * cellHeight);
    }
    // dividers
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            const Cell& cell = *model->cell(row, col);
            QPointF topLeft = cell.topLeft();
            switch (cell.state) {
            case Cell::Square:
                break;
            case Cell::BendDexter:
                path.moveTo(topLeft);
                path.lineTo(topLeft + QPointF(cellWidth, cellHeight));
                break;
            case Cell::BendSinister:
                path.moveTo(topLeft + QPointF(0, cellHeight));
                path.lineTo(topLeft + QPointF(cellWidth, 0));
                break;
            }
        }
    }
    painter->setPen(QPen(BoardView::GridColor, 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(path);
}

/*
 * Dual view
 */

BoardDualView::BoardDualView(const BoardModel* model) : model(model), effect(new QGraphicsBlurEffect)
{
    setGraphicsEffect(effect);
}

void BoardDualView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    int cols = model->cols;
    int rows = model->rows;

    QPainterPath path;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Cell& cell = *model->cell(row, col);
            if (cell.isDivided()) {
                // could just as well use Left and Right
                path.moveTo(cell.center(cell.Up));
                path.lineTo(cell.center(cell.Down));
            }
            if (row+1 < rows) {
                path.moveTo(cell.center(cell.Down));
                path.lineTo(model->cell(row+1, col)->center(cell.Up));
            }
            if (col+1 < cols) {
                path.moveTo(cell.center(cell.Right));
                path.lineTo(model->cell(row, col+1)->center(cell.Left));
            }
        }
    }
    painter->setPen(QPen(BoardView::DualColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(path);
}
