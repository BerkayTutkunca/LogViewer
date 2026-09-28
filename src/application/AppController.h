#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QAbstractItemModel>
#include <QObject>
#include <QUrl>
#include <QVector>

#include "domain/LogEntry.h"
#include "playback/PlaybackController.h"

class TlogParser;
class RawDataModel;

class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(
        QAbstractItemModel* rawDataModel
            READ rawDataModel
                CONSTANT
        )

    Q_PROPERTY(
        PlaybackController* playbackController
            READ playbackController
                CONSTANT
        )

public:
    explicit AppController(QObject *parent = nullptr);

    Q_INVOKABLE void loadLog(const QUrl& fileUrl);

    QAbstractItemModel* rawDataModel() const;
    PlaybackController* playbackController() const;

signals:
    void loadStarted();
    void loadSucceeded();
    void loadFailed(const QString& errorMessage);

private:
    TlogParser* m_tlogParser;
    RawDataModel* m_rawDataModel;
    PlaybackController* m_playbackController;

    QVector<LogEntry> m_entries;
};

#endif // APPCONTROLLER_H