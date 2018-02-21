Attribute VB_Name = "Frame_empty_calc"
Option Explicit

Sub delete_bad_calc()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'suppresion des calculateurs qui n'émette pas et ne reçoive pas

'déclaration des variabes
Dim not_ok As Boolean
Dim VRange_frame As Range
Dim pointeur1, pointeur2, max As Long
Dim increment, increment_max As Integer

not_ok = False
increment = 0

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

max = VRange_frame.Columns.Count

'1er passage : detection

'balayage des colonnes de frame
For pointeur1 = 6 To max
    DoEvents
    'reinitialisation de la variable
    not_ok = False
    'balayage des lignes de frame
    For pointeur2 = 5 To VRange_frame.Rows.Count
        'si on croise une trame emise ou recu alors...
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur2, pointeur1).Value <> "") Then
            'on sort de la boucle
            not_ok = True
            Exit For
        End If
    Next pointeur2
    'si on n'en a pas trouvé alors...
    If (not_ok = False) Then
        'on supprime le calculateur
        ThisWorkbook.Worksheets("Frame").Cells(1, pointeur1).Interior.Color = RGB(255, 0, 0)
        increment = increment + 1
        If (increment_max < increment) Then
            increment_max = increment
        End If
    Else
        increment = 1
   End If
Next pointeur1

'maj de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

max = VRange_frame.Columns.Count

'2ème passage : suppression

For pointeur1 = 1 To increment_max
    For pointeur2 = 6 To max
        If (ThisWorkbook.Worksheets("Frame").Cells(1, pointeur2).Interior.Color = RGB(255, 0, 0)) Then
            ThisWorkbook.Worksheets("Frame").Cells(1, pointeur2).EntireColumn.Delete
            max = max - 1
        End If
    Next pointeur2
Next pointeur1

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub
