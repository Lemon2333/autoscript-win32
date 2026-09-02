# autoscript-win32

一個使用 C 開發的 Windows 自動化腳本引擎。

## 專案簡介

`autoscript-win32` 是一個實驗性的 Windows 自動化腳本引擎。

目前以簡單、可逐步擴充的架構為目標，包含：

- lexer
- parser
- interpreter
- 內建函式
- 基於 Win32 的插件式自動化能力

## 目前狀態

> 開發中。

目前重點：

- 基本腳本執行
- 語言核心結構
- 初步自動化 API

## 預計功能

- [ ] 變數賦值
- [ ] 運算式與比較運算
- [ ] `If / Else / End If`
- [ ] `While / End While`
- [ ] 內建函式
- [ ] Win32 視窗控制
- [ ] 鍵盤與滑鼠自動化
- [ ] 更完整的錯誤訊息

## 專案結構

```text
src/
include/
examples/
```
## 建置方式
使用 Visual Studio 開啟專案並執行，執行時帶入腳本檔案路徑。

範例參數：

```text
examples\demo.as
```
## 腳本範例
```vb
hwnd = Plugin.Window.Find(0, "Notepad")

If hwnd <> 0 Then
    SayString("window found")
Else
    SayString("window not found")
End If
```
## 開發規劃
- 強化語言核心
- 加入真正的 Win32 自動化能力
- 拆分平台相關模組
- 擴充內建函式與插件 API
## 授權
TBD
