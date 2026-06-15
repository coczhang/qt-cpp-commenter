#include "FileUploadClient.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

void FileUploadClient::startUpload(const QString &filePath, const QString &fileUuid)
{
    m_file.setFileName(filePath);
    m_fileUuid = fileUuid;

    if (!m_file.open(QIODevice::ReadOnly)) {
        emit uploadFailed(fileUuid, tr("Failed to open file"));
        return;
    }

    QFileInfo info(filePath);
    QJsonObject meta;
    meta.insert("type", "file_meta");
    meta.insert("uuid", fileUuid);
    meta.insert("fileName", info.fileName());
    meta.insert("fileSize", info.size());

    m_socket->sendTextMessage(QString::fromUtf8(QJsonDocument(meta).toJson(QJsonDocument::Compact)));
    sendNextChunk();
}

void FileUploadClient::sendNextChunk()
{
    if (!m_file.isOpen()) {
        return;
    }

    QByteArray data = m_file.read(m_chunkSize);
    if (data.isEmpty()) {
        emit uploadFinished(m_fileUuid);
        m_file.close();
        return;
    }

    m_socket->sendBinaryMessage(data);
    emit uploadProgress(m_fileUuid, m_file.pos(), m_file.size());
}
