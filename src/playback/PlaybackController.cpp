#include "PlaybackController.h"

#include <algorithm>

PlaybackController::PlaybackController(QObject *parent)
    : QObject(parent)
{
}

int PlaybackController::currentIndex() const
{
    return m_currentIndex;
}

qreal PlaybackController::position() const
{
    return m_position;
}

int PlaybackController::currentPositionIndex() const
{
    if (m_currentIndex < 0 ||
        m_currentIndex >= m_positionIndexByPacket.size()) {
        return -1;
    }

    return m_positionIndexByPacket.at(m_currentIndex);
}

bool PlaybackController::hasPosition() const
{
    return currentPositionIndex() >= 0;
}

double PlaybackController::latitude() const
{
    const int index = currentPositionIndex();

    if (index < 0) {
        return 0.0;
    }

    return m_positions.at(index).latitude;
}

double PlaybackController::longitude() const
{
    const int index = currentPositionIndex();

    if (index < 0) {
        return 0.0;
    }

    return m_positions.at(index).longitude;
}

double PlaybackController::altitude() const
{
    const int index = currentPositionIndex();

    if (index < 0) {
        return 0.0;
    }

    return m_positions.at(index).altitudeMeters;
}

double PlaybackController::heading() const
{
    const int index = currentPositionIndex();

    if (index < 0) {
        return 0.0;
    }

    return m_positions.at(index).headingDegrees;
}

bool PlaybackController::hasRoute() const
{
    return !m_positions.isEmpty();
}

QGeoCoordinate PlaybackController::initialCoordinate() const
{
    if (m_positions.isEmpty()) {
        return {};
    }

    const GeoPosition& position =
        m_positions.first();

    return QGeoCoordinate(
        position.latitude,
        position.longitude,
        position.altitudeMeters
        );
}

QVariantList PlaybackController::traveledPath() const
{
    const int positionIndex =
        currentPositionIndex();

    if (positionIndex < 0) {
        return {};
    }

    return m_routePath.mid(
        0,
        positionIndex + 1
        );
}

QString PlaybackController::currentTime() const
{
    if (m_timestamps.isEmpty()) {
        return QStringLiteral("00:00");
    }

    const quint64 elapsed =
        m_timestamps.at(m_currentIndex)
        - m_timestamps.first();

    return formatTime(elapsed);
}

QString PlaybackController::duration() const
{
    if (m_timestamps.size() < 2) {
        return QStringLiteral("00:00");
    }

    const quint64 total =
        m_timestamps.last()
        - m_timestamps.first();

    return formatTime(total);
}

int PlaybackController::activePacket() const
{
    if (m_timestamps.isEmpty()) {
        return 0;
    }

    return m_currentIndex + 1;
}

void PlaybackController::setEntries(
    const QVector<LogEntry>& entries)
{
    m_timestamps.clear();
    m_positions.clear();
    m_positionIndexByPacket.clear();
    m_routePath.clear();

    m_timestamps.reserve(entries.size());
    m_positionIndexByPacket.reserve(entries.size());

    int lastPositionIndex = -1;

    for (const LogEntry& entry : entries) {
        m_timestamps.append(
            entry.timestampUs()
            );

        const auto position =
            entry.position();

        if (position.has_value()) {
            const GeoPosition& geoPosition =
                position.value();

            m_positions.append(
                geoPosition
                );

            m_routePath.append(
                QVariant::fromValue(
                    QGeoCoordinate(
                        geoPosition.latitude,
                        geoPosition.longitude,
                        geoPosition.altitudeMeters
                        )
                    )
                );

            lastPositionIndex =
                m_positions.size() - 1;
        }

        m_positionIndexByPacket.append(
            lastPositionIndex
            );
    }

    m_currentIndex = 0;
    m_position = 0.0;

    emit currentIndexChanged();
    emit positionChanged();
    emit durationChanged();

    emit routeChanged();
    emit currentPositionChanged();
}

void PlaybackController::seek(qreal position)
{
    if (m_timestamps.isEmpty()) {
        return;
    }

    position = qBound(
        0.0,
        position,
        1.0
        );

    const quint64 first =
        m_timestamps.first();

    const quint64 last =
        m_timestamps.last();

    if (last <= first) {
        return;
    }

    const quint64 target =
        first
        + static_cast<quint64>(
            (last - first) * position
            );

    const auto it = std::lower_bound(
        m_timestamps.cbegin(),
        m_timestamps.cend(),
        target
        );

    int newIndex;

    if (it == m_timestamps.cend()) {
        newIndex =
            m_timestamps.size() - 1;
    } else {
        newIndex =
            static_cast<int>(
                std::distance(
                    m_timestamps.cbegin(),
                    it
                    )
                );
    }

    const bool hasIndexChanged =
        newIndex != m_currentIndex;

    const bool hasPlaybackPositionChanged =
        !qFuzzyCompare(
            m_position,
            position
            );

    const int previousPositionIndex =
        currentPositionIndex();

    m_currentIndex = newIndex;
    m_position = position;

    const int newPositionIndex =
        currentPositionIndex();

    if (hasIndexChanged) {
        emit currentIndexChanged();
    }

    if (previousPositionIndex !=
        newPositionIndex) {

        emit currentPositionChanged();
    }

    if (hasPlaybackPositionChanged) {
        emit positionChanged();
    }
}

QString PlaybackController::formatTime(
    quint64 microseconds) const
{
    const quint64 totalMilliseconds =
        microseconds / 1000;

    const quint64 totalSeconds =
        totalMilliseconds / 1000;

    const quint64 hours =
        totalSeconds / 3600;

    const quint64 minutes =
        (totalSeconds / 60) % 60;

    const quint64 seconds =
        totalSeconds % 60;

    const quint64 milliseconds =
        totalMilliseconds % 1000;

    if (hours > 0) {
        return QStringLiteral(
                   "%1:%2:%3.%4"
                   )
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
            .arg(milliseconds, 3, 10, QChar('0'));
    }

    return QStringLiteral(
               "%1:%2.%3"
               )
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(milliseconds, 3, 10, QChar('0'));
}