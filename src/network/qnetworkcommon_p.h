#ifndef QNETWORKCOMMON_H
#define QNETWORKCOMMON_H

#include "qsharedpointer.h"

Q_DECLARE_METATYPE(QSharedPointer<char>)

QT_BEGIN_NAMESPACE

static void downloadBufferDeleter(char *ptr)
{
    delete[] ptr;
}

QT_END_NAMESPACE

#endif // QNETWORKCOMMON_H
