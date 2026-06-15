---
name: qt-cpp-commenter
description: Add, improve, and standardize professional Chinese comments for Qt/C++ codebases, especially Qt 6 QWidget projects, signals/slots, QObject classes, WebSocket/file-transfer modules, threading, models, and maintainable Doxygen documentation.
---

# qt-cpp-commenter

## Purpose

Use this skill when the user asks Codex to add comments, improve existing comments, document APIs, explain code intent, or standardize documentation for Qt/C++ source code.

This skill is optimized for Qt 6 C++ projects, especially QWidget desktop applications, but it also applies to QtCore, QtNetwork, QtWebSockets, QtConcurrent, QThread worker patterns, model/view code, and general C++ modules used inside Qt projects.

## Default Output Language

Default comments are written in Simplified Chinese.

Use English comments only when the user explicitly requests English, the repository already consistently uses English comments, or the surrounding file style makes English clearly more appropriate.

## Core Behavior

When commenting Qt/C++ code:

1. Preserve the original code behavior.
2. Preserve formatting and style as much as possible.
3. Do not refactor code unless the user explicitly asks for refactoring.
4. Add comments that explain intent, responsibility, constraints, and maintenance notes.
5. Avoid noisy comments that merely restate syntax.
6. Prefer concise, professional, maintainable comments.
7. Respect existing project style when it is clear.
8. Correct misleading or stale comments when updating comments.
9. If the code contains likely bugs, mention them separately after the commented code or in a review note. Do not silently change logic.

## Comment Density

Use moderate comment density by default.

Add comments for:

- Public classes and public APIs
- Non-trivial private helper functions
- Qt signals and slots
- Important member variables
- Ownership and lifetime rules
- Threading boundaries
- Protocol fields and message formats
- File chunking or upload/download logic
- UUID/task correlation logic
- Error handling decisions
- Non-obvious UI state transitions
- Complex condition branches
- Performance-sensitive logic

Avoid comments for:

- Obvious variable assignments
- Basic C++ syntax
- Simple getters/setters
- Trivial constructors with no special behavior
- Obvious signal-slot connections unless they encode business intent
- Comments like `i++; // i 加 1`

## Comment Style

Use these styles unless the repository already has a consistent alternative:

- `/** ... */` Doxygen comments for classes, public functions, important protected/private functions, signals, and slots.
- `//` comments for short local notes and implementation details.
- Keep comments close to the code they describe.
- Do not add large banners unless the file already uses them.
- Do not overuse decorative separators.

## Doxygen Rules

For class comments, explain the class role and responsibility.

```cpp
/**
 * @brief 文件上传窗口
 *
 * 负责文件选择、上传任务创建、上传进度展示以及上传状态更新。
 * 该类只处理界面交互逻辑，具体网络传输由 FileUploadClient 负责。
 */
class FileUploadWidget : public QWidget
{
    Q_OBJECT
};
```

For function comments, include `@brief`, `@param`, and `@return` when useful.

```cpp
/**
 * @brief 开始上传指定文件
 * @param filePath 本地文件路径
 * @param fileUuid 当前文件上传任务的唯一标识
 * @return 上传任务是否成功启动
 */
bool startUpload(const QString &filePath, const QString &fileUuid);
```

For signals, explain when the signal is emitted.

```cpp
signals:
    /**
     * @brief 文件上传进度变化时发出
     * @param fileUuid 文件唯一标识
     * @param bytesSent 已发送字节数
     * @param bytesTotal 文件总字节数
     */
    void uploadProgressChanged(const QString &fileUuid,
                               qint64 bytesSent,
                               qint64 bytesTotal);
```

For slots, explain what event or signal triggers the slot.

```cpp
private slots:
    /**
     * @brief 响应“选择文件”按钮点击事件
     *
     * 打开本地文件选择对话框，并将用户选择的文件加入待上传列表。
     */
    void onSelectFileClicked();
```

## Qt-Specific Guidance

### QObject Ownership

When code depends on QObject parent-child ownership, comment the ownership rule.

```cpp
// 以当前窗口作为父对象，交由 Qt 对象树管理生命周期
m_socket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
```

### Signal-Slot Connections

Do not comment every `connect()` mechanically. Add comments only when the connection expresses workflow or business behavior.

Good:

```cpp
// WebSocket 连接建立后，立即发送文件元信息，服务端据此创建上传任务
connect(m_socket, &QWebSocket::connected,
        this, &FileUploadClient::sendFileMeta);
```

Avoid:

```cpp
connect(button, &QPushButton::clicked, this, &Widget::onClicked); // 连接 clicked 信号
```

### Threading

For QThread, worker objects, queued connections, and cross-thread signals, comment thread ownership and execution context.

```cpp
// Worker 移动到后台线程后，其槽函数将在 workerThread 所在线程执行
m_worker->moveToThread(m_workerThread);
```

### File Upload / Download

For file transfer code, clarify:

- Chunk size
- Offset/progress calculation
- Retry behavior
- UUID/task relation
- Metadata/data/end message order
- Whether a UUID is per file or per chunk

```cpp
// 同一个文件上传过程中必须复用同一个 UUID，服务端通过它关联所有分片
const QString fileUuid = task.uuid;
```

### JSON Protocols

For QJsonObject/QJsonDocument protocol messages, document required fields.

```cpp
// 上传元信息消息：服务端根据 type、uuid、fileName、fileSize 初始化接收任务
QJsonObject message{
    {"type", "file_meta"},
    {"uuid", fileUuid},
    {"fileName", fileName},
    {"fileSize", fileSize}
};
```

### UI State

For QWidget UI logic, comment state transitions rather than every widget operation.

```cpp
// 上传过程中禁用文件选择，避免用户修改正在发送的任务列表
ui->selectFileButton->setEnabled(false);
```

## Output Format

When the user provides code and asks for comments:

1. Return the full commented code unless the user asks for a patch/diff only.
2. Keep code fences with the correct language tag, usually `cpp`, `h`, or `cmake`.
3. After the code, add a short note only if there are assumptions, warnings, or likely bugs.
4. Do not add unrelated explanations.

When operating inside a repository:

1. Inspect surrounding files to infer comment language and style.
2. Modify only requested files unless broader changes are required and explicitly requested.
3. Keep public API comments in header files where possible.
4. Put implementation details in `.cpp` files.
5. Avoid duplicating identical long comments in both header and implementation.

## Review Checklist

Before finalizing commented code, verify:

- Comments do not contradict code behavior.
- Comments are not too verbose for simple logic.
- Public APIs have useful Doxygen comments.
- Qt signals mention emission timing.
- Qt slots mention trigger source.
- Ownership/lifetime is clear for QObject pointers.
- Threading comments distinguish caller thread and execution thread where relevant.
- File-transfer comments clarify UUID, chunk, progress, and protocol behavior.
- Existing useful comments were preserved.

## Example Task Prompt

The user may say:

> 请使用 qt-cpp-commenter，帮我给下面这段 Qt 6 QWidget 代码添加专业中文注释。保留原有逻辑，不要逐行废话注释，类、函数、信号槽使用 Doxygen 风格。

Apply this skill and return the commented code.
