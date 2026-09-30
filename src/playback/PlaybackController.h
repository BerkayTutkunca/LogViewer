#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QGeoCoordinate>
#include <QObject>
#include <QVariantList>
#include <QVector>

#include "domain/LogEntry.h"

class PlaybackController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(qreal position READ position NOTIFY positionChanged)

    Q_PROPERTY(QString currentTime READ currentTime NOTIFY currentIndexChanged)
    Q_PROPERTY(QString duration READ duration NOTIFY durationChanged)

    Q_PROPERTY(int activePacket READ activePacket NOTIFY currentIndexChanged)

    Q_PROPERTY(bool hasPosition READ hasPosition NOTIFY currentPositionChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY currentPositionChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY currentPositionChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY currentPositionChanged)
    Q_PROPERTY(double heading READ heading NOTIFY currentPositionChanged)

    Q_PROPERTY(QVariantList traveledPath READ traveledPath NOTIFY currentPositionChanged)

    Q_PROPERTY(bool hasRoute READ hasRoute NOTIFY routeChanged)
    Q_PROPERTY(QGeoCoordinate initialCoordinate READ initialCoordinate NOTIFY routeChanged)

public:
    explicit PlaybackController(QObject* parent = nullptr);

    int currentIndex() const;
    qreal position() const;

    QString currentTime() const;
    QString duration() const;

    int activePacket() const;

    bool hasPosition() const;
    double latitude() const;
    double longitude() const;
    double altitude() const;
    double heading() const;

    QVariantList traveledPath() const;

    bool hasRoute() const;
    QGeoCoordinate initialCoordinate() const;

    void setEntries(const QVector<LogEntry>& entries);

    Q_INVOKABLE void seek(qreal position);

signals:
    void currentIndexChanged();
    void positionChanged();
    void durationChanged();

    void currentPositionChanged();
    void routeChanged();

private:
    QString formatTime(quint64 microseconds) const;
    int currentPositionIndex() const;

    QVector<quint64> m_timestamps;
    QVector<GeoPosition> m_positions;
    QVector<int> m_positionIndexByPacket;

    QVariantList m_routePath;

    int m_currentIndex {0};
    qreal m_position {0.0};
};

#endif // PLAYBACKCONTROLLER_H