Attribute VB_Name = "Make_Mat_Rout"
Sub test5()

'declaration des variables

Dim VRange_mat_rout As Range
Dim pointeur1 As Long

'creation de la range de mat rout

ThisWorkbook.Worksheets("Mat Rout").Activate
ActiveSheet.Cells(2, 1).Activate
Set VRange_mat_rout = ActiveCell.CurrentRegion

'traitement

For pointeur1 = 2 To VRange_mat_rout.Rows.Count + 1
    ThisWorkbook.Worksheets("Mat Rout").Cells(1, pointeur1).Value = ThisWorkbook.Worksheets("Mat Rout").Cells(pointeur1, 1).Value
Next pointeur1

End Sub
