# qt-cpp-commenter

`qt-cpp-commenter` 是一个适用于 Codex 的 Skill 项目，用于给 Qt/C++ 项目添加专业、清晰、可维护的中文代码注释。

## 适用场景

- Qt 6 / QWidget 项目代码注释
- QObject / QWidget / QThread / QWebSocket 代码说明
- 信号与槽注释规范化
- 文件上传、WebSocket、UUID、JSON 协议代码注释
- C++ 头文件 API 文档补充
- 项目已有注释优化

## 项目结构

```text
qt-cpp-commenter/
├── SKILL.md
├── README.md
├── references/
│   ├── qt_comment_guidelines.md
│   └── qt_cpp_terms.md
├── examples/
│   ├── before_file_upload_client.cpp
│   ├── after_file_upload_client.cpp
│   └── example_prompt.md
├── scripts/
│   └── check_skill.py
└── assets/
    └── templates/
        └── doxygen_function_template.md
```

## 安装/使用

将整个 `qt-cpp-commenter` 文件夹放入 Codex 支持的 Skills 目录，或者在支持 Skill 上传的界面中上传该文件夹/压缩包。

Codex 的 Skills 通常以一个包含 `SKILL.md` 的目录为核心；`SKILL.md` 需要包含 `name` 和 `description` 元数据。

在项目根目录中运行以下 PowerShell 命令，可将该 Skill 安装到当前用户的 Codex Skills 目录：

```powershell
Copy-Item `
  -LiteralPath (Get-Location).Path `
  -Destination (Join-Path $HOME ".codex\skills\qt-cpp-commenter") `
  -Recurse
```

安装后可以运行以下命令检查 `SKILL.md` 是否存在：

```powershell
Test-Path (Join-Path $HOME ".codex\skills\qt-cpp-commenter\SKILL.md")
```

命令输出 `True` 表示安装成功。安装完成后需要重启 Codex，使其加载新 Skill。

如果目标目录已经存在，请先确认其中是否包含需要保留的修改，避免直接复制导致新旧文件混合。

## 推荐调用方式

```text
请使用 qt-cpp-commenter，帮我给 src/FileUploadClient.cpp 和 include/FileUploadClient.h 添加专业中文注释。
要求：
1. 保留原有代码逻辑
2. 不要逐行废话注释
3. 类、函数、信号槽使用 Doxygen 风格
4. 对 WebSocket、文件分片、UUID 关联逻辑重点注释
```

## 检查 Skill 文件

```bash
python scripts/check_skill.py
```
