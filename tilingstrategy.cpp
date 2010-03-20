#include <QDebug>
#include "tilingstrategy.h"

const QList<TilingStrategy*> TilingStrategy::Strategies = TilingStrategy::createStrategies();

const QList<TilingStrategy*> TilingStrategy::createStrategies()
{
    QList<TilingStrategy*> strategies;

    strategies << new TilingStrategy("Rectangular", 0);
    strategies << new TilingStrategy("Diagonal", 1);
    strategies << new TilingStrategy("Mixed", 2);

    return strategies;
}
