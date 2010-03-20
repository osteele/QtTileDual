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
    const QString expression;

    TilingStrategy(QString name, QString expression) : name(name), expression(expression) {}

    // FIXME I don't like having to side-effect this after the instance is initialized. Would it be more idiomatic to use a QSignalMapper instead?
    void setWidget(TilingWidget* widget) { this->widget = widget; }

    int evaluateAt(int row, int col) const;

public slots:
    void apply() {
        widget->applyStrategy(*this);
    }

private:
    TilingWidget* widget;
    int n;

    static const QList<TilingStrategy*> createStrategies();
};

#endif // TILINGSTRATEGY_H
