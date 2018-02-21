VERSION 5.00
Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} ProgressBar 
   Caption         =   "Optimisation"
   ClientHeight    =   7980
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   13485
   OleObjectBlob   =   "ProgressBar.frx":0000
   StartUpPosition =   1  'CenterOwner
End
Attribute VB_Name = "ProgressBar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


'A mettre au début dans la macro "lourde"
Dim PctDone As Single
Dim mode As String
Dim already_launch As Boolean

Private Sub Launch_Click()
    
    If (mode = "MODE1") Then
    
        'Trame.Visible = False
        'Param.Visible = False
    
        Call Menu.group_without_met_export
        mode = ""
        
    Else:
        
        Call Menu.group_auto
        mode = ""
        
    End If
    
End Sub

Private Sub Leave_Click()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False
Application.DisplayAlerts = False
On Error Resume Next


    'Dim Classeur As Workbook
    'For Each Classeur In Workbooks
    '    If Classeur.Name <> ThisWorkbook.Name Then
    '        Classeur.Close SaveChanges:=False
    '    End If
    'Next Classeur
    'ThisWorkbook.Close False
    Application.Quit
    
On Error GoTo 0

End Sub

Private Sub Param_Click()

Dim index As Integer
index = ListBox1.ListIndex + 1

If (ThisWorkbook.Worksheets("Param").Cells(index, 8).Value = "0") Then
    ThisWorkbook.Worksheets("Param").Cells(index, 8).Value = "1"
Else
    ThisWorkbook.Worksheets("Param").Cells(index, 8).Value = "0"
End If

End Sub

Private Sub Trame_Click()

Dim index As Integer
index = ListBox1.ListIndex + 1

If (ThisWorkbook.Worksheets("Frame").Cells(index, 63).Value = "0") Then
    ThisWorkbook.Worksheets("Frame").Cells(index, 63).Value = "1"
Else
    ThisWorkbook.Worksheets("Frame").Cells(index, 63).Value = "0"
End If

End Sub

Sub UserForm_Activate()

    already_launch = False
    mode = ""
    Me.StartUpPosition = 2
    ' Largeur de la progressBar à 0
    ProgressBar.LabelProgress.Width = 0
    'ProgressBar.Width = Application.Width
    'ProgressBar.Height = Application.Height
    'ProgressBar.Left = 0
    'ProgressBar.Top = 0
    
    Trame.Visible = False
    Param.Visible = False
    Launch.Visible = False
        
    ProgressBar.Show 0
    ' Mise à jour du pourcentage
    PctDone = 0
    ' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
    UpdateProgressBar PctDone


End Sub

Private Sub MODE1_Click()

    If (already_launch <> False) Then
        MsgBox "Erreur... vous avez essayez de générer 2x des instances de classeur portant le même nom. Veuillez relancer l'application."
        Call Leave_Click
    End If
    already_launch = True
    mode = "MODE1"
    'Param.Visible = False
    'Trame.Visible = True

    Call Menu.group_without_met
    ListBox1.rowSource = "Frame!A1:ZA300"
    ListBox1.ListIndex = 0
    'Call Menu.group_without_met_export
    
    Call Launch_Click
    
'---

    Dim fso As Object, Src$, Dest$, Fich$

    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Dim RepParent As String, RepInitial As String
 
    ChDir (ThisWorkbook.Path)
    RepInitial = CurDir(ThisWorkbook.Path)
    ChDir ".."
    RepParent = CurDir(ThisWorkbook.Path)
    
    Src = RepParent & "\CFG\C1A\MODE1" & "\"
    Dest = RepParent & "\"
    Fich$ = "cfg.txt"
    fso.CopyFile Src & Fich, Dest & Fich
    
'---
    
    ProgressBar.Show 0
    ' Mise à jour du pourcentage
    PctDone = 0
    ' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
    UpdateProgressBar PctDone
    
End Sub

Private Sub MODE2_Click()
    
    If (already_launch <> False) Then
        MsgBox "Erreur... vous avez essayez de générer 2x des instances de classeur portant le même nom. Veuillez relancer l'application."
        Call Leave_Click
    End If
    already_launch = True
    mode = "MODE2"
    'Trame.Visible = False
    'Param.Visible = True
    
    Call Menu.group_with_met
    ListBox1.rowSource = "Param!A1:ZA3000"
    ListBox1.ListIndex = 0
    'Call Menu.group_auto
    
    Call Launch_Click
    
    '---

    Dim fso As Object, Src$, Dest$, Fich$

    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Dim RepParent As String, RepInitial As String
 
    ChDir (ThisWorkbook.Path)
    RepInitial = CurDir(ThisWorkbook.Path)
    ChDir ".."
    RepParent = CurDir(ThisWorkbook.Path)
    
    Src = RepParent & "\CFG\C1A\MODE2" & "\"
    Dest = RepParent & "\"
    Fich$ = "cfg.txt"
    fso.CopyFile Src & Fich, Dest & Fich
    
'---
    
    ProgressBar.Show 0
    ' Mise à jour du pourcentage
    PctDone = 0
    ' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
    UpdateProgressBar PctDone
End Sub

Private Sub MODE3_Click()
    If (already_launch <> False) Then
        MsgBox "Erreur... vous avez essayez de générer 2x des instances de classeur portant le même nom. Veuillez relancer l'application."
        Call Leave_Click
    End If
    already_launch = True
    mode = "MODE1"
    'Param.Visible = False
    'Trame.Visible = True

    Call Menu.group_without_met
    ListBox1.rowSource = "Frame!A1:ZA300"
    ListBox1.ListIndex = 0
    'Call Menu.group_without_met_export
    
    Call Launch_Click
    
'---

    Dim fso As Object, Src$, Dest$, Fich$

    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Dim RepParent As String, RepInitial As String
 
    ChDir (ThisWorkbook.Path)
    RepInitial = CurDir(ThisWorkbook.Path)
    ChDir ".."
    RepParent = CurDir(ThisWorkbook.Path)
    
    Src = RepParent & "\CFG\C1A\MODE3" & "\"
    Dest = RepParent & "\"
    Fich$ = "cfg.txt"
    fso.CopyFile Src & Fich, Dest & Fich
    
'---
    
    
    ProgressBar.Show 0
    ' Mise à jour du pourcentage
    PctDone = 0
    ' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
    UpdateProgressBar PctDone
    
End Sub

'Code de la sous-routine
Sub UpdateProgressBar(PctDone As Single)
    With ProgressBar

        ' Mise à jour du label.
        .FrameProgress.Caption = Format(PctDone, "0%")

        ' Afin de paramétrer la fin de la progressBar par rapport au frame
        .LabelProgress.Width = PctDone * _
            (.FrameProgress.Width - 20)
    End With

    ' DoEvents autorisant au UserForm de ce mettre à jour
    DoEvents
End Sub


