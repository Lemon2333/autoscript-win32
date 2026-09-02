hwnd = Plugin.Window.Find(0, "Notepad")

If hwnd <> 0 Then
    SayString("window found")
Else
    SayString("window not found")
End If