#include "qdebug.h"
#include "qlibraryinfo.h"

int main() {
    qDebug() << QLibraryInfo::buildKey();
}
