hwnd = Plugin.Window.Find(0, "未命名 - 記事本")

If hwnd <> 0 Then
    SayString("window found")
Else
    SayString("window not found")
End If