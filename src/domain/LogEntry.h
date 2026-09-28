#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QString>
#include <QtGlobal>
#include <optional>

struct GeoPosition
{
    double latitude {};
    double longitude {};
    double altitudeMeters {};
};

class LogEntry
{
public:
    LogEntry() = default;

    LogEntry(
        quint64 timestampUs,
        quint32 messageId,
        quint8 systemId,
        quint8 componentId,
        const QString& messageName,
        const QString& payload
        );

    quint64 timestampUs() const;
    quint32 messageId() const;
    quint8 systemId() const;
    quint8 componentId() const;

    QString messageName() const;
    QString payload() const;

    std::optional<GeoPosition> position() const;
    void setPosition(const GeoPosition& position);


private:
    quint64 m_timestampUs {};
    quint32 m_messageId {};
    quint8 m_systemId {};
    quint8 m_componentId {};

    QString m_messageName;
    QString m_payload;

    std::optional<GeoPosition> m_position;
};

#endif // LOGENTRY_H