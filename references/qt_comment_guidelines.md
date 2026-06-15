# Qt/C++ 注释规范参考

## 基本原则

- 注释解释“为什么”和“设计意图”，不要机械复述“代码做了什么”。
- 公共 API 使用 Doxygen 风格。
- 实现细节使用简洁的 `//` 注释。
- 对 Qt 对象生命周期、线程、信号槽、协议字段、UI 状态变化进行重点注释。

## 推荐注释位置

### 头文件 `.h`

适合放：

- 类职责说明
- public/protected API 注释
- signals 注释
- public slots 注释
- 重要枚举、结构体、成员变量说明

### 源文件 `.cpp`

适合放：

- 函数内部复杂逻辑说明
- 关键分支说明
- 协议消息构造说明
- 线程切换说明
- 错误处理策略说明

## 不推荐的注释

```cpp
i++; // i 加 1
```

```cpp
connect(button, &QPushButton::clicked, this, &Widget::onClicked); // 连接信号槽
```

## 推荐的注释

```cpp
// 上传过程中禁用文件选择，避免任务列表在发送期间被修改
ui->selectFileButton->setEnabled(false);
```

```cpp
// 同一个文件上传过程中保持 UUID 不变，服务端用它关联所有分片
message.insert("uuid", task.uuid);
```
