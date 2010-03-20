#include <QDebug.h>
#include <QtGui/QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsBlurEffect>

#include "osutils.h"
#include "board.h"
#include "cell.h"

/*
 * Model
 */

BoardModel::BoardModel(int rows, int cols, int cellWidth, int cellHeight)
    : rows(rows), cols(cols),
      cellWidth(cellWidth), cellHeight(cellHeight)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            cellList << new Cell(*this, row, col);
        }
    }
}

Cell* BoardModel::cell(int row, int col) const
{
    if (0 <= row && row < rows &&
        0 <= col && col < cols) {
        return cellList[row * cols + col];
    } else
        return 0;
}

void BoardModel::setCellStates(int strategy)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int state = 0;
            switch (strategy) {
            case 0: state = 0; break;
            case 1: state = 1; break;
            case 2: state = i^j; break;
            }
            cell(i, j)->setState(state % Cell::StateCount);
        }
    }
}


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
    model->setCellStates();
    setDualFocus(1);
}

void BoardView::setDualFocus(qreal s)
{
    boardGraphView->effect->setBlurRadius(1 + 5 * s);
    boardDualView->effect->setBlurRadius(1 + (1 - s) * 5);
}

void BoardView::setCellStates(int strategy)
{
    model->setCellStates(strategy);
    boardGraphView->update();
    boardDualView->update();
}

void BoardView::updateCellStates()
{
    Cell& cell = *model->cell(qrand() % rows, qrand() % cols);
    cell.setState((cell.state + 1) % Cell::StateCount);
    update(); // TODO perf update cell and neighbors
    boardGraphView->update();
    boardDualView->update();
}

void BoardView::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void BoardView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int col = event->pos().x() / cellWidth;
    int row = event->pos().y() / cellHeight;
    Cell* cell = this->cell(row, col);
    if (cell && cell != this->lastCell) {
        cell->setState((cell->state + 1) % Cell::StateCount);
        update(); // TODO perf update cell and neighbors
    }
    this->lastCell = 0;
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
