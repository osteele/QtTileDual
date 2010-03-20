#ifndef TILINGWIDGET_H
#define TILINGWIDGET_H

#include <QtGui/QGraphicsView>

class Board;
class TilingStrategy;

class TilingWidget : public QGraphicsView
{
    Q_OBJECT

public:
    TilingWidget();
    void applyBoardFunction(int n);
    void applyStrategy(const TilingStrategy&);

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    Board& board;
};

#endif
