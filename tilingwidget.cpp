#include <math.h>
#include <QDebug>
#include <QGraphicsScene>
#include <QtGui>
#include <QSignalMapper>

#include "boardview.h"
#include "tilingwidget.h"
#include "tilingstrategy.h"

MainWidget::MainWidget()
{
    TilingWidget* tilingWidget = new TilingWidget;

    QHBoxLayout *layout = new QHBoxLayout;
    // add the focus radio button group
    {
        QRadioButton *focusGrid = new QRadioButton(tr("Grid"));
        connect(focusGrid, SIGNAL(toggled(bool)), tilingWidget , SLOT(onFocusGrid(bool)));
        QRadioButton *focusDual = new QRadioButton(tr("Dual"));
        connect(focusDual, SIGNAL(toggled(bool)), tilingWidget , SLOT(onFocusDual(bool)));
        focusDual->setChecked(true);
        QButtonGroup *bg = new QButtonGroup;
        bg->addButton(focusGrid);
        bg->addButton(focusDual);
        QVBoxLayout *layo = new QVBoxLayout;
        layo->addWidget(focusGrid);
        layo->addWidget(focusDual);
        QGroupBox *groupBox = new QGroupBox(tr("Focus"));
        groupBox->setLayout(layo);
        layout->addWidget(groupBox);
    }

    QCheckBox *animateButton = new QCheckBox(tr("Animate"));
    connect(animateButton, SIGNAL(toggled(bool)), tilingWidget , SLOT(setAnimationState(bool)));
    animateButton->setChecked(true);
    layout->addWidget(animateButton);

    // add the tiling strategies
    {
        QHBoxLayout *layo = new QHBoxLayout;
        foreach (TilingStrategy *strategy, TilingStrategy::Strategies) {
            strategy->setWidget(tilingWidget );
            QPushButton *button = new QPushButton(strategy->name, this);
            connect(button, SIGNAL(clicked()), strategy, SLOT(apply()));
            layo->addWidget(button);
        }
        QGroupBox *groupBox = new QGroupBox(tr("Patterns"));
        groupBox->setLayout(layo);
        layout->addWidget(groupBox);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tilingWidget);
    mainLayout->addLayout(layout);

    setWindowTitle(tr("Tile"));
    setLayout(mainLayout);
}

TilingWidget::TilingWidget()
    : timerId(0), boardView(new BoardView(10,10))
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    // TODO select this based on window size
    // TODO rescale board when window changes
    scene->setSceneRect(-25, -30, 540, 550);
    scene->addItem(boardView);
    setScene(scene);
    // copied from elasticnodes
    // TODO figure out what these do, and which are necessary
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    //scale(qreal(0.8), qreal(0.8));
    setMinimumSize(600, 650);
}

void TilingWidget::onFocusGrid(bool)
{
    boardView->setDualFocus(0);
}

void TilingWidget::onFocusDual(bool)
{
    boardView->setDualFocus(1);
}

void TilingWidget::setAnimationState(bool enable)
{
    if (enable) {
        if (!timerId) timerId = startTimer(500);
    } else if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

void TilingWidget::setPattern(const class TilingStrategy& strategy) {
    boardView->setPattern(strategy);
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
    boardView->updateCellStates();
}

// Mostly copied from the elasticnodes example.
void TilingWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
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
