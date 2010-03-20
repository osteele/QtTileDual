#ifndef TILINGSTRATEGY_H
#define TILINGSTRATEGY_H

#include "tilingwidget.h"

class TilingStrategy : public QObject {
    Q_OBJECT

public:
    TilingStrategy(TilingWidget& widget, int n) : widget(widget), n(n) {}

public slots:
    void setLayout() {
        widget.applyBoardFunction(n);
    }

private:
    TilingWidget& widget;
    int n;
};

#endif // TILINGSTRATEGY_H
