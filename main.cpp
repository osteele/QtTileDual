#include <QDebug>
#include <QtGui>

#include "tilingwidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    MainWidget widget;
    widget.show();
    return app.exec();
}
