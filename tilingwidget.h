#ifndef TILINGWIDGET_H
#define TILINGWIDGET_H

#include <QtGui/QGraphicsView>

class BoardView;

class TilingWidget : public QGraphicsView
{
    Q_OBJECT

public:
    TilingWidget();
    void applyBoardFunction(int n);

public slots:
    void onFocusGrid(bool);
    void onFocusDual(bool);

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    BoardView* boardView;
};

#endif
