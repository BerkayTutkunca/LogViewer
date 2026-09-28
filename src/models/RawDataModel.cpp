#include "RawDataModel.h"

RawDataModel::RawDataModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int RawDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_entries.size();
}

QVariant RawDataModel::data(
    const QModelIndex &index,
    int role
    ) const
{
    if (!index.isValid()
        || index.row() < 0
        || index.row() >= m_entries.size()) {
        return {};
    }

    const LogEntry &entry = m_entries.at(index.row());

    switch (role) {
    case TimestampRole:
        return formatTimestamp(entry.timestampUs());

    case MessageNameRole:
        return entry.messageName();

    case SourceInfoRole:
        return QStringLiteral("SYS %1 / COMP %2")
            .arg(static_cast<int>(entry.systemId()))
            .arg(static_cast<int>(entry.componentId()));

    case PayloadRole:
        return entry.payload();

    default:
        return {};
    }
}

QHash<int, QByteArray> RawDataModel::roleNames() const
{
    return {
        { TimestampRole, "timestamp" },
        { MessageNameRole, "messageName" },
        { SourceInfoRole, "sourceInfo" },
        { PayloadRole, "payload" }
    };
}

void RawDataModel::setEntries(const QVector<LogEntry> &entries)
{
    beginResetModel();

    m_entries = entries;

    endResetModel();
}

QString RawDataModel::formatTimestamp(quint64 timestampUs) const
{
    if (m_entries.isEmpty()) {
        return {};
    }

    const quint64 firstTimestampUs =
        m_entries.first().timestampUs();

    const quint64 elapsedUs =
        timestampUs >= firstTimestampUs
            ? timestampUs - firstTimestampUs
            : 0;

    const quint64 hours =
        elapsedUs / 3600000000ULL;

    const quint64 minutes =
        (elapsedUs / 60000000ULL) % 60;

    const quint64 seconds =
        (elapsedUs / 1000000ULL) % 60;

    const quint64 milliseconds =
        (elapsedUs / 1000ULL) % 1000;

    return QStringLiteral("%1:%2:%3.%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(milliseconds, 3, 10, QChar('0'));
}