#ifndef OSUTILS_H
#define OSUTILS_H

#include <QColor>

namespace ColorUtils
{
    //! return a copy of \c basis, but with an override for alpha
    static QColor alpha(Qt::GlobalColor basis, qreal alpha)
    {
        QColor c(basis);
        c.setAlphaF(alpha);
        return c;
    }
};



#endif // OSUTILS_H
