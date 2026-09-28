#ifndef TLOGPARSER_H
#define TLOGPARSER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "domain/LogEntry.h"

class TlogParser : public QObject
{
    Q_OBJECT

public:
    explicit TlogParser(QObject* parent = nullptr);

    bool parse(
        const QString& filePath,
        QVector<LogEntry>& entries,
        QString& errorMessage
        ) const;
};

#endif // TLOGPARSER_H