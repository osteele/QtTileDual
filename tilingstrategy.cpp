#include <QDebug>
#include <QScriptEngine>

#include "tilingstrategy.h"

const QList<TilingStrategy*> TilingStrategy::Strategies = TilingStrategy::createStrategies();

const QList<TilingStrategy*> TilingStrategy::createStrategies()
{
    QList<TilingStrategy*> strategies;

    strategies << new TilingStrategy("Rectangular", "0");
    strategies << new TilingStrategy("Diagonal", "1");
    strategies << new TilingStrategy("Oddball", "i^j");

    return strategies;
}

int TilingStrategy::evaluateAt(int row, int col) const
{
    QScriptEngine engine;
    QScriptValue rowValue = QScriptValue(&engine, row);
    QScriptValue colValue = QScriptValue(&engine, col);
    engine.globalObject().setProperty("i", rowValue);
    engine.globalObject().setProperty("j", colValue);
    QScriptValue result = engine.evaluate("i + j");
    return result.toInteger();
}
