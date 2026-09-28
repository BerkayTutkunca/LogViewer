#include "TlogParser.h"

#include <QByteArray>
#include <QFile>
#include <QtEndian>
#include <cstring>
#include <common/mavlink.h>

namespace
{

QString messageNameForId(quint32 messageId)
{
    const mavlink_message_info_t* info =
        mavlink_get_message_info_by_id(messageId);

    if (info == nullptr) {
        return QStringLiteral("MSG_%1").arg(messageId);
    }

    return QString::fromLatin1(info->name);
}

int mavlinkTypeSize(mavlink_message_type_t type)
{
    switch (type) {
    case MAVLINK_TYPE_CHAR:
    case MAVLINK_TYPE_UINT8_T:
    case MAVLINK_TYPE_INT8_T:
        return 1;

    case MAVLINK_TYPE_UINT16_T:
    case MAVLINK_TYPE_INT16_T:
        return 2;

    case MAVLINK_TYPE_UINT32_T:
    case MAVLINK_TYPE_INT32_T:
    case MAVLINK_TYPE_FLOAT:
        return 4;

    case MAVLINK_TYPE_UINT64_T:
    case MAVLINK_TYPE_INT64_T:
    case MAVLINK_TYPE_DOUBLE:
        return 8;

    default:
        return 1;
    }
}
QString scalarValueToString(
    const mavlink_message_t& message,
    mavlink_message_type_t type,
    uint8_t offset)
{
    switch (type) {
    case MAVLINK_TYPE_CHAR:
        return QString(QChar::fromLatin1(
            _MAV_RETURN_char(&message, offset)
            ));

    case MAVLINK_TYPE_UINT8_T:
        return QString::number(
            _MAV_RETURN_uint8_t(&message, offset)
            );

    case MAVLINK_TYPE_INT8_T:
        return QString::number(
            static_cast<int>(
                _MAV_RETURN_int8_t(&message, offset)
                )
            );

    case MAVLINK_TYPE_UINT16_T:
        return QString::number(
            _MAV_RETURN_uint16_t(&message, offset)
            );

    case MAVLINK_TYPE_INT16_T:
        return QString::number(
            _MAV_RETURN_int16_t(&message, offset)
            );

    case MAVLINK_TYPE_UINT32_T:
        return QString::number(
            _MAV_RETURN_uint32_t(&message, offset)
            );

    case MAVLINK_TYPE_INT32_T:
        return QString::number(
            _MAV_RETURN_int32_t(&message, offset)
            );

    case MAVLINK_TYPE_UINT64_T:
        return QString::number(
            _MAV_RETURN_uint64_t(&message, offset)
            );

    case MAVLINK_TYPE_INT64_T:
        return QString::number(
            _MAV_RETURN_int64_t(&message, offset)
            );

    case MAVLINK_TYPE_FLOAT:
        return QString::number(
            _MAV_RETURN_float(&message, offset),
            'g',
            6
            );

    case MAVLINK_TYPE_DOUBLE:
        return QString::number(
            _MAV_RETURN_double(&message, offset),
            'g',
            10
            );

    default:
        return QStringLiteral("?");
    }
}
QString arrayValueToString(
    const mavlink_message_t& message,
    const mavlink_field_info_t& field)
{
    constexpr unsigned int maxDisplayedElements = 12;

    const unsigned int elementCount =
        qMin(field.array_length, maxDisplayedElements);

    const int elementSize =
        mavlinkTypeSize(field.type);

    QStringList values;

    for (unsigned int i = 0; i < elementCount; ++i) {
        const auto offset = static_cast<uint8_t>(
            field.wire_offset + (i * elementSize)
            );

        values.append(
            scalarValueToString(
                message,
                field.type,
                offset
                )
            );
    }

    if (field.array_length > maxDisplayedElements) {
        values.append(QStringLiteral("..."));
    }

    return QStringLiteral("[%1]")
        .arg(values.join(QStringLiteral(", ")));
}
QString charArrayToString(
    const mavlink_message_t& message,
    const mavlink_field_info_t& field)
{
    QByteArray value(
        _MAV_PAYLOAD(&message) + field.wire_offset,
        field.array_length
        );

    const int nullIndex = value.indexOf('\0');

    if (nullIndex >= 0) {
        value.truncate(nullIndex);
    }

    bool printable = true;

    for (const char ch : value) {
        const unsigned char byte =
            static_cast<unsigned char>(ch);

        if (byte < 32 || byte > 126) {
            printable = false;
            break;
        }
    }

    if (printable) {
        return QStringLiteral("\"%1\"")
        .arg(QString::fromLatin1(value));
    }

    return QString::fromLatin1(
        value.left(16).toHex(' ')
        );
}
QString payloadToString(const mavlink_message_t& message)
{
    const mavlink_message_info_t* info =
        mavlink_get_message_info(&message);

    if (info == nullptr) {
        return QStringLiteral("Unknown MAVLink message");
    }

    mavlink_message_t safeMessage = message;

    if (safeMessage.len < MAVLINK_MAX_PAYLOAD_LEN) {
        std::memset(
            _MAV_PAYLOAD_NON_CONST(&safeMessage)
                + safeMessage.len,
            0,
            MAVLINK_MAX_PAYLOAD_LEN
                - safeMessage.len
            );
    }

    QStringList fields;

    fields.reserve(
        static_cast<qsizetype>(info->num_fields)
        );

    for (unsigned int i = 0;
         i < info->num_fields;
         ++i) {

        const mavlink_field_info_t& field =
            info->fields[i];

        QString value;

        if (field.array_length == 0) {
            value = scalarValueToString(
                safeMessage,
                field.type,
                static_cast<uint8_t>(
                    field.wire_offset
                    )
                );
        }
        else if (field.type == MAVLINK_TYPE_CHAR) {
            value = charArrayToString(
                safeMessage,
                field
                );
        }
        else {
            value = arrayValueToString(
                safeMessage,
                field
                );
        }

        fields.append(
            QStringLiteral("%1: %2")
                .arg(
                    QString::fromLatin1(field.name),
                    value
                    )
            );
    }

    return fields.join(QStringLiteral(", "));
}
} // namespace

TlogParser::TlogParser(QObject* parent)
    : QObject(parent)
{
}

bool TlogParser::parse(
    const QString& filePath,
    QVector<LogEntry>& entries,
    QString& errorMessage
    ) const
{
    entries.clear();
    errorMessage.clear();

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        errorMessage =
            QStringLiteral("Dosya açılamadı: %1")
                .arg(file.errorString());

        return false;
    }

    mavlink_message_t message {};
    mavlink_status_t status {};

    while (!file.atEnd()) {

        const QByteArray timestampBytes = file.read(8);

        if (timestampBytes.isEmpty()) {
            break;
        }

        if (timestampBytes.size() != 8) {
            errorMessage =
                QStringLiteral("Geçersiz veya eksik TLOG timestamp.");

            return false;
        }

        const quint64 timestampUs =
            qFromBigEndian<quint64>(
                reinterpret_cast<const uchar*>(
                    timestampBytes.constData()
                    )
                );

        bool messageParsed = false;

        while (!file.atEnd()) {

            char rawByte {};

            if (file.read(&rawByte, 1) != 1) {
                break;
            }

            const auto byte =
                static_cast<quint8>(rawByte);

            if (mavlink_parse_char(
                    MAVLINK_COMM_0,
                    byte,
                    &message,
                    &status)) {

                LogEntry entry(
                    timestampUs,
                    message.msgid,
                    message.sysid,
                    message.compid,
                    messageNameForId(message.msgid),
                    payloadToString(message)
                    );

                if (message.msgid ==
                    MAVLINK_MSG_ID_GLOBAL_POSITION_INT) {

                    mavlink_global_position_int_t rawPosition {};

                    mavlink_msg_global_position_int_decode(
                        &message,
                        &rawPosition
                        );

                    GeoPosition position;

                    position.latitude =
                        rawPosition.lat / 1e7;

                    position.longitude =
                        rawPosition.lon / 1e7;

                    position.altitudeMeters =
                        rawPosition.alt / 1000.0;

                    position.headingDegrees =
                        rawPosition.hdg == UINT16_MAX
                            ? 0.0
                            : rawPosition.hdg / 100.0;

                    entry.setPosition(position);
                }

                entries.append(entry);

                messageParsed = true;

                break;
            }
        }

        if (!messageParsed) {
            errorMessage =
                QStringLiteral(
                    "Timestamp sonrasında geçerli MAVLink paketi bulunamadı."
                    );

            return false;
        }
    }

    if (entries.isEmpty()) {
        errorMessage =
            QStringLiteral(
                "Dosyada geçerli MAVLink paketi bulunamadı."
                );

        return false;
    }

    return true;
}