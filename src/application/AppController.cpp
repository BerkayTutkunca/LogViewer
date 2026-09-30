#include "AppController.h"

#include "domain/LogEntry.h"
#include "models/RawDataModel.h"

#include <QFileInfo>
#include <QVector>

#include <utility>

AppController::AppController(QObject* parent)
    : QObject(parent),
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
        emit loadFailed(
            QStringLiteral("Geçersiz dosya yolu.")
            );
        return;
    }

    const QFileInfo fileInfo(filePath);

    if (!fileInfo.exists() || !fileInfo.isFile()) {
        emit loadFailed(
            QStringLiteral("Dosya bulunamadı.")
            );
        return;
    }

    if (fileInfo.suffix().compare(
            QStringLiteral("tlog"),
            Qt::CaseInsensitive) != 0) {

        emit loadFailed(
            QStringLiteral(
                "Seçilen dosya .tlog formatında değil."
                )
            );
        return;
    }

    emit loadStarted();

    QVector<LogEntry> entries;
    QString errorMessage;

    if (!m_tlogParser.parse(
            filePath,
            entries,
            errorMessage)) {

        emit loadFailed(errorMessage);
        return;
    }

    m_playbackController->setEntries(entries);

    m_rawDataModel->setEntries(
        std::move(entries)
        );

    emit loadSucceeded();
}
