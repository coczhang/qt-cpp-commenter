# Example Prompt

请使用 `qt-cpp-commenter`，帮我给下面的 Qt/C++ 代码添加专业中文注释。

要求：

1. 保留原有代码逻辑。
2. 不要逐行添加废话注释。
3. 类、public 函数、signals、slots 使用 Doxygen 风格。
4. 对 QObject 生命周期、信号槽、QThread、WebSocket、文件分片、UUID、JSON 协议字段重点注释。
5. 如果发现明显问题，请在代码后单独列出，不要直接改逻辑。
