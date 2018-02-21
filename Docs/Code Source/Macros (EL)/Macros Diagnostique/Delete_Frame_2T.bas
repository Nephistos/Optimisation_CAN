Attribute VB_Name = "Delete_Frame_2T"
Sub test6()

'declaration des variables

Dim VRange_frame As Range
Dim pointeur1, pointeur2 As Long
Dim already_t As Boolean

already_t = False

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'traitement

For pointeur1 = 5 To VRange_frame.Rows.Count
    already_t = False
    For pointeur2 = 6 To VRange_frame.Columns.Count
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur2).Value = "T") Then
            If (already_t = True) Then
                ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)
                ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).EntireRow.Delete
            End If
            already_t = True
        End If
    Next pointeur2
Next pointeur1

End Sub
