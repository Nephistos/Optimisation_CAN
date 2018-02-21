Attribute VB_Name = "Param_Transmitter"
Sub test3()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'declaration des variables

Dim VRange_param As Range
Dim pointeur1, pointeur2 As Long
Dim increment, max As Integer
Dim is_ok As Boolean

increment = 0
is_ok = False

'creation de la range de frame

ThisWorkbook.Worksheets("Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_param = ActiveCell.CurrentRegion

'traitement en 2 passages

'1er passage : detection

For pointeur1 = 5 To VRange_param.Rows.Count
    is_ok = False
    For pointeur2 = 6 To VRange_param.Columns.Count
        If (ThisWorkbook.Worksheets("Param").Cells(pointeur1, 1).Value = ThisWorkbook.Worksheets("Param").Cells(1, pointeur2).Value) Then
            is_ok = True
            Exit For
        End If
    Next pointeur2
    If (is_ok = False) Then
        'ThisWorkbook.Worksheets("Param").Cells(VRange_param.Rows.Count + 2 + increment, 1).Value = pointeur1
        ThisWorkbook.Worksheets("Param").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)
        increment = increment + 1
    Else
        If (max < increment) Then
            max = increment
        End If
        increment = 1
    End If
Next pointeur1

'2eme passage : suppression

'traitement

'on passage le nombre de fois qu'il y a des lignes fausses consecutives
For pointeur2 = 1 To max
    'balayage des lignes de param
    For pointeur1 = 5 To VRange_param.Rows.Count
        'on supprime la ligne si elle a été détectée
        If (ThisWorkbook.Worksheets("Param").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)) Then
            ThisWorkbook.Worksheets("Param").Cells(pointeur1, 1).EntireRow.Delete
        End If
    Next pointeur1
Next pointeur2

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub

