#ifndef TILINGSTRATEGY_H
#define TILINGSTRATEGY_H

#include "tilingwidget.h"
class Cell;

//! A TilingStrategy tells how to assign the cells of a board with cell states (whether to divide each cell
//! with a sinister bend, a dexter bend, or not at all).
class TilingStrategy : public QObject {
    Q_OBJECT

public:
    static const QList<TilingStrategy*> Strategies;

    const QString name;
    TilingStrategy(QString name, int n) : name(name), widget(widget), n(n) {}

    // FIXME I don't like having to side-effect this after the instance is initialized. Would it be more idiomatic to use a QSignalMapper instead?
    void setWidget(TilingWidget* widget) { this->widget = widget; }

public slots:
    void apply() {
        widget->applyBoardFunction(n);
    }

private:
    TilingWidget* widget;
    int n;

    static const QList<TilingStrategy*> createStrategies();
};

#endif // TILINGSTRATEGY_H
