#include "RawDataModel.h"
#include <utility>

namespace
{

constexpr quint64 UsPerMillisecond = 1'000;
constexpr quint64 UsPerSecond = 1'000'000;
constexpr quint64 UsPerMinute = 60 * UsPerSecond;
constexpr quint64 UsPerHour = 60 * UsPerMinute;

} // namespace

RawDataModel::RawDataModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int RawDataModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return static_cast<int>(m_entries.size());
}

QVariant RawDataModel::data(
    const QModelIndex& index,
    int role
    ) const
{
    if (!index.isValid()) {
        return {};
    }

    const int row = index.row();

    if (row < 0 || row >= m_entries.size()) {
        return {};
    }

    const LogEntry& entry = m_entries.at(row);

    switch (role) {
    case TimestampRole:
        return formatTimestamp(entry.timestampUs());

    case MessageNameRole:
        return entry.messageName();

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
        { PayloadRole, "payload" }
    };
}

void RawDataModel::setEntries(QVector<LogEntry> entries)
{
    beginResetModel();

    m_entries = std::move(entries);

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
        elapsedUs / UsPerHour;

    const quint64 minutes =
        (elapsedUs / UsPerMinute) % 60;

    const quint64 seconds =
        (elapsedUs / UsPerSecond) % 60;

    const quint64 milliseconds =
        (elapsedUs / UsPerMillisecond) % 1000;

    return QStringLiteral("%1:%2:%3.%4")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(milliseconds, 3, 10, QChar('0'));
}