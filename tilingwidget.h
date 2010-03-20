#ifndef TILINGWIDGET_H
#define TILINGWIDGET_H

#include <QtGui/QGraphicsView>

class BoardView;

class TilingWidget : public QGraphicsView
{
    Q_OBJECT

public:
    TilingWidget();
    void setPattern(const class TilingStrategy& strategy);

public slots:
    void onFocusGrid(bool);
    void onFocusDual(bool);
    void setAnimationState(bool);

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    BoardView* boardView;
};

class MainWidget : public QGraphicsView
{
    Q_OBJECT

public:
    MainWidget();
};

#endif
