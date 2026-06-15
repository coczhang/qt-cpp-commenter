#include "FileUploadClient.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

/**
 * @brief 启动指定文件的上传流程
 * @param filePath 本地待上传文件路径
 * @param fileUuid 当前文件上传任务的唯一标识
 *
 * 该函数负责打开本地文件、发送文件元信息，并开始发送第一个文件分片。
 * 同一个文件上传过程必须保持 fileUuid 不变，服务端通过该值关联后续所有分片。
 */
void FileUploadClient::startUpload(const QString &filePath, const QString &fileUuid)
{
    m_file.setFileName(filePath);
    m_fileUuid = fileUuid;

    // 文件无法打开时立即终止上传，避免后续发送空数据或错误进度
    if (!m_file.open(QIODevice::ReadOnly)) {
        emit uploadFailed(fileUuid, tr("Failed to open file"));
        return;
    }

    QFileInfo info(filePath);

    // 上传元信息消息：服务端根据 type、uuid、fileName、fileSize 创建接收任务
    QJsonObject meta;
    meta.insert("type", "file_meta");
    meta.insert("uuid", fileUuid);
    meta.insert("fileName", info.fileName());
    meta.insert("fileSize", info.size());

    m_socket->sendTextMessage(QString::fromUtf8(QJsonDocument(meta).toJson(QJsonDocument::Compact)));

    // 元信息发送后开始发送二进制文件分片
    sendNextChunk();
}

/**
 * @brief 发送下一个文件分片
 *
 * 从当前打开的文件中按固定分片大小读取数据，并通过 WebSocket 发送。
 * 当读取结果为空时，表示文件已经全部发送完成。
 */
void FileUploadClient::sendNextChunk()
{
    // 文件未打开时无法继续读取分片
    if (!m_file.isOpen()) {
        return;
    }

    // 按固定分片大小读取下一段文件内容
    QByteArray data = m_file.read(m_chunkSize);

    // 读取结果为空表示文件内容已经发送完毕
    if (data.isEmpty()) {
        emit uploadFinished(m_fileUuid);
        m_file.close();
        return;
    }

    // 使用二进制消息发送文件分片，文本消息仅用于协议控制信息
    m_socket->sendBinaryMessage(data);

    // 使用当前文件偏移量作为已发送字节数，用于界面进度展示
    emit uploadProgress(m_fileUuid, m_file.pos(), m_file.size());
}
