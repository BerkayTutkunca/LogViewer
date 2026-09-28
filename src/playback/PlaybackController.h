#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QObject>
#include <QVector>

#include "domain/LogEntry.h"

class PlaybackController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex
                   READ currentIndex
                       NOTIFY currentIndexChanged)

    Q_PROPERTY(qreal position
                   READ position
                       NOTIFY positionChanged)

    Q_PROPERTY(QString currentTime
                   READ currentTime
                       NOTIFY currentIndexChanged)

    Q_PROPERTY(QString duration
                   READ duration
                       NOTIFY durationChanged)

    Q_PROPERTY(int activePacket
                   READ activePacket
                       NOTIFY currentIndexChanged)

    Q_PROPERTY(
        bool hasPosition
            READ hasPosition
                NOTIFY currentPositionChanged
        )

    Q_PROPERTY(
        double latitude
            READ latitude
                NOTIFY currentPositionChanged
        )

    Q_PROPERTY(
        double longitude
            READ longitude
                NOTIFY currentPositionChanged
        )

    Q_PROPERTY(
        double altitude
            READ altitude
                NOTIFY currentPositionChanged
        )

public:
    explicit PlaybackController(QObject *parent = nullptr);

    int currentIndex() const;
    qreal position() const;

    QString currentTime() const;
    QString duration() const;

    int activePacket() const;

    bool hasPosition() const;

    double latitude() const;
    double longitude() const;
    double altitude() const;

    void setEntries(const QVector<LogEntry>& entries);

    Q_INVOKABLE void seek(qreal position);

signals:
    void currentIndexChanged();
    void positionChanged();
    void durationChanged();
    void currentPositionChanged();

private:
    QString formatTime(quint64 microseconds) const;

    QVector<quint64> m_timestamps;

    int m_currentIndex {0};
    qreal m_position {0.0};

    QVector<GeoPosition> m_positions;
    QVector<int> m_positionIndexByPacket;

    int currentPositionIndex() const;
};

#endif // PLAYBACKCONTROLLER_H