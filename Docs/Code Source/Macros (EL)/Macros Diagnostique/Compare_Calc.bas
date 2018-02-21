Attribute VB_Name = "Compare_Calc"
Sub test8()

'déclaration des variables

Dim VRange_frame_from, VRange_frame_to As Range
Dim increment As Integer
Dim pointeur1, pointeur2 As Long

increment = 0

'creation de la range de frame étudiée

ThisWorkbook.Worksheets("Frame to").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_to = ActiveCell.CurrentRegion

'creation de la range de frame comparé

ThisWorkbook.Worksheets("Frame from").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_from = ActiveCell.CurrentRegion

'traitement
For pointeur1 = 6 To VRange_frame_from.Columns.Count
    For pointeur2 = 6 To VRange_frame_to.Columns.Count
        If (ThisWorkbook.Worksheets("Frame to").Cells(1, pointeur2).Value = ThisWorkbook.Worksheets("Frame from").Cells(1, pointeur1).Value) Then
            Exit For
        ElseIf (pointeur2 = VRange_frame_to.Columns.Count) Then
            ThisWorkbook.Worksheets("Frame from").Cells(VRange_frame_from.Rows.Count + 2 + increment, 1).Value = ThisWorkbook.Worksheets("Frame from").Cells(1, pointeur1).Value
            increment = increment + 1
        End If
    Next pointeur2
Next pointeur1

End Sub

