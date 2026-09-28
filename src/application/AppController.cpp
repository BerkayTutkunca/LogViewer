#include "AppController.h"

#include "infrastructure/mavlink/TlogParser.h"
#include "models/RawDataModel.h"
#include "playback/PlaybackController.h"
#include <QFileInfo>

AppController::AppController(QObject *parent)
    : QObject(parent),
    m_tlogParser(new TlogParser(this)),
    m_rawDataModel(new RawDataModel(this)),
    m_playbackController(new PlaybackController(this))
{
}

QAbstractItemModel* AppController::rawDataModel() const
{
    return m_rawDataModel;
}

PlaybackController* AppController::playbackController() const
{
    return m_playbackController;
}

void AppController::loadLog(const QUrl& fileUrl)
{
    const QString filePath = fileUrl.toLocalFile();

    if (filePath.isEmpty()) {
        emit loadFailed(QStringLiteral("Geçersiz dosya yolu."));
        return;
    }

    const QFileInfo fileInfo(filePath);

    if (!fileInfo.exists() || !fileInfo.isFile()) {
        emit loadFailed(QStringLiteral("Dosya bulunamadı."));
        return;
    }

    if (fileInfo.suffix().compare(
            QStringLiteral("tlog"),
            Qt::CaseInsensitive) != 0) {

        emit loadFailed(
            QStringLiteral("Seçilen dosya .tlog formatında değil.")
            );
        return;
    }

    emit loadStarted();

    QVector<LogEntry> entries;
    QString errorMessage;

    if (!m_tlogParser->parse(
            filePath,
            entries,
            errorMessage)) {

        emit loadFailed(errorMessage);
        return;
    }

    m_entries = entries;

    m_rawDataModel->setEntries(m_entries);
    m_playbackController->setEntries(m_entries);

    emit loadSucceeded();
}