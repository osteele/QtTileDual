#include <math.h>
#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>

#include "tilingwidget.h"
#include "board.h"

TilingWidget::TilingWidget()
    : timerId(0), board(*new Board(10,10))
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    // TODO select this based on window size
    // TODO rescale board when window changes
    scene->setSceneRect(-25, -40, 540, 550);
    scene->addItem(&board);
    setScene(scene);
    // copied from elasticnodes
    // TODO figure out what these do, and which are necessary
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    //scale(qreal(0.8), qreal(0.8));
    setMinimumSize(600, 600);
    setWindowTitle(tr("Tile"));

    board.setCellStates();

    timerId = startTimer(500);
}

void TilingWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void TilingWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    board.updateCellStates();
}

// Mostly copied from the elasticnodes example.
void TilingWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
	painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
	painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Click on a cell to change its type."));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}

// Copied from the elasticnodes example.
void TilingWidget::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
