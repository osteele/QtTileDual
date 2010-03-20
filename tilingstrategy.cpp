#include <QDebug>
#include <QtScript>

#include "tilingstrategy.h"

const QList<TilingStrategy*> TilingStrategy::Strategies = TilingStrategy::createStrategies();

const QList<TilingStrategy*> TilingStrategy::createStrategies()
{
    QList<TilingStrategy*> strategies;

    strategies << new TilingStrategy("Rectangular", "0");
    strategies << new TilingStrategy("Diagonal", "1");
    strategies << new TilingStrategy("Mixed", "i^j");

    return strategies;
}

int TilingStrategy::evaluateAt(int row, int col) const
{
    static QScriptEngine engine;
    QScriptValue rowValue(&engine, row);
    QScriptValue colValue(&engine, col);
    engine.globalObject().setProperty("i", rowValue);
    engine.globalObject().setProperty("j", colValue);
    QScriptValue result = engine.evaluate(expression);
    return result.toInteger();
}
