Attribute VB_Name = "Frame_without_R"
Sub test3()

'declaration des variables

Dim VRange_frame As Range
Dim pointeur1, pointeur2 As Long

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'traitement

For pointeur1 = 5 To VRange_frame.Rows.Count
    For pointeur2 = 6 To VRange_frame.Columns.Count
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur2).Value = "R") Then
            Exit For
        ElseIf (pointeur2 = VRange_frame.Columns.Count) Then
            ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)
        End If
    Next pointeur2
Next pointeur1
        
End Sub
