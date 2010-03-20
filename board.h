#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsItem>

class Cell;

class Board : public QGraphicsItem
{
public:
    const int rows;
    const int cols;
    int cellWidth;
    int cellHeight;

    Board(int rows, int cols);
    Cell* cell(int row, int col);
    void setCellStates();
    void updateCellStates();

    // QGraphicsItem API
    QRectF boundingRect() const { return QRectF(0, 0, cols * cellWidth, cols * cellHeight); }
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    static QColor GridColor;
    static QColor DualColor;

    QList<Cell*> cellList;
    Cell* lastCell;

    void paintGrid(QPainter*);
    void paintDual(QPainter*);
};

#endif // BOARD_H
