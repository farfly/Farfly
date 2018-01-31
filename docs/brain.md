## 程序是和QQ一样的可常驻系统通知栏 建议参考Qt example的 Systray例子
1. WriteLog
- 可配置用户名密码
- 右键点击WriteLog弹出dialog;
- dialog上有可输入字段，包括上报日期/工日数/原地待工工日/40字以内的工作描述等字段。用户点击submit按钮提交。
- dialog靠上有`启动自动`复选框， 勾选则激活 自动启动周期设置字段供用户编辑。
- 如果用户已经配置了`启动自动`则按照用户设置的周期去重复执行.

```
设计上建议将集成google的cef控件，以cef控件解析页面的方式实现WriteLog。
即用户点击WriteLog菜单后弹出对话框实际是嵌入了一个cef控件，cef控件加载html/css/js来实现WriteLog全部功能
```
   
2. 邮件助手
- 收到新邮件给出提醒：可选tips气泡提醒或3D提醒。3d提醒，例如有新邮件到来，则全屏显示飘来一封信件，并且根据信件的紧急程度有不同的信件logo
- 用户可点击打开第三方邮件管理软件，例如Outlook，foxmail等进一步查看信件内容。
- 可自定义自动发送任务

3. 软件名字： zxvf.f.  farfly Farfly logo 可以为一只狗
