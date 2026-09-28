#ifndef RAWDATAMODEL_H
#define RAWDATAMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "domain/LogEntry.h"

class RawDataModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        TimestampRole = Qt::UserRole + 1,
        MessageNameRole,
        SourceInfoRole,
        PayloadRole
    };

    explicit RawDataModel(QObject *parent = nullptr);

    int rowCount(
        const QModelIndex &parent = QModelIndex()
        ) const override;

    QVariant data(
        const QModelIndex &index,
        int role = Qt::DisplayRole
        ) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setEntries(const QVector<LogEntry> &entries);

private:
    QString formatTimestamp(quint64 timestampUs) const;

    QVector<LogEntry> m_entries;
};

#endif // RAWDATAMODEL_H