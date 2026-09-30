#ifndef TLOGPARSER_H
#define TLOGPARSER_H

#include <QString>
#include <QVector>

#include "domain/LogEntry.h"

class TlogParser
{
public:
    bool parse(
        const QString& filePath,
        QVector<LogEntry>& entries,
        QString& errorMessage
        ) const;
};

#endif // TLOGPARSER_H