Attribute VB_Name = "Calc_without_frame"
Sub test1()

'declaration des variables

Dim VRange_frame As Range
Dim pointeur1, pointeur2 As Long

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'traitement

For pointeur2 = 6 To VRange_frame.Columns.Count
    For pointeur1 = 5 To VRange_frame.Rows.Count
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur2).Value <> "") Then
            Exit For
        ElseIf (pointeur1 = VRange_frame.Rows.Count) Then
            ThisWorkbook.Worksheets("Frame").Cells(1, pointeur2).Interior.Color = RGB(255, 0, 0)
        End If
    Next pointeur1
Next pointeur2
        

End Sub
