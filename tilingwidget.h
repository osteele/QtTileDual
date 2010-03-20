#ifndef TILINGWIDGET_H
#define TILINGWIDGET_H

#include <QtGui/QGraphicsView>

#include "board.h"

class TilingWidget : public QGraphicsView
{
    Q_OBJECT

public:
    TilingWidget();

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

public slots:
    void setLayout0() {board.setCellStates(0);}
    void setLayout1() {board.setCellStates(1);}
    void setLayout2() {board.setCellStates(2);}

private:
    int timerId;
    Board& board;
};

#endif
