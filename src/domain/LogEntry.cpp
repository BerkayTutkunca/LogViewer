#include "LogEntry.h"

LogEntry::LogEntry(
    quint64 timestampUs,
    quint32 messageId,
    quint8 systemId,
    quint8 componentId,
    const QString& messageName,
    const QString& payload
    )
    : m_timestampUs(timestampUs),
    m_messageId(messageId),
    m_systemId(systemId),
    m_componentId(componentId),
    m_messageName(messageName),
    m_payload(payload)
{
}

quint64 LogEntry::timestampUs() const
{
    return m_timestampUs;
}

quint32 LogEntry::messageId() const
{
    return m_messageId;
}

quint8 LogEntry::systemId() const
{
    return m_systemId;
}

quint8 LogEntry::componentId() const
{
    return m_componentId;
}

const QString& LogEntry::messageName() const
{
    return m_messageName;
}

const QString& LogEntry::payload() const
{
    return m_payload;
}

std::optional<GeoPosition> LogEntry::position() const
{
    return m_position;
}

void LogEntry::setPosition(const GeoPosition& position)
{
    m_position = position;
}