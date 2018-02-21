Attribute VB_Name = "Compare_Frame_Files"
Sub test4()

'declarations des variables

Dim VRange_frame_to, VRange_frame_from As Range
Dim pointeur1, pointeur2, pointeur3, pointeur4 As Long

'creation de la range de frame to (à étudier)

ThisWorkbook.Worksheets("Frame to").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_to = ActiveCell.CurrentRegion

'creation de la range de frame from (qui marche, et, que l'on compare)

ThisWorkbook.Worksheets("Frame from").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_from = ActiveCell.CurrentRegion

'traitement

'balayage des lignes du frame à étudier
For pointeur1 = 5 To VRange_frame_to.Rows.Count
    'balayage des lignes du bon frame à comparer
    For pointeur2 = 5 To VRange_frame_from.Rows.Count
        'si on retrouve la ligne alors...
        If (ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, 1).Value = ThisWorkbook.Worksheets("Frame from").Cells(pointeur2, 1).Value) Then
            'on passe à la suivante
            Exit For
        'sinon...
        ElseIf (pointeur2 = VRange_frame_from.Rows.Count) Then
            'd'une part on la marque puis...
            ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)
            'ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, 1).EntireRow.Delete
            'on balaye les calculateur qui l'émette ou la consomme afin de...
            For pointeur3 = 6 To VRange_frame_to.Columns.Count
                If (ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, pointeur3).Value <> "") Then
                    ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, pointeur3).Interior.Color = RGB(255, 0, 0)
                    'les observer à leurs tour (les calculateur) pour vori si il ne vont pas être vide
                    For pointeur4 = 5 To VRange_frame_to.Rows.Count
                        'si ce n'est pas le cas...
                        If ((ThisWorkbook.Worksheets("Frame to").Cells(pointeur4, pointeur3).Value <> "") And (ThisWorkbook.Worksheets("Frame to").Cells(pointeur4, pointeur3).Interior.Color <> RGB(255, 0, 0))) Then
                            'on passe à la lgine suivante
                            Exit For
                        'sinon...
                        ElseIf (pointeur4 = VRange_frame_to.Rows.Count) Then
                            'on marque ce alculateur car il posera problème lors de l'optimisation
                            ThisWorkbook.Worksheets("Frame to").Cells(1, pointeur3).Interior.Color = RGB(255, 0, 0)
                        End If
                    Next pointeur4
                End If
            Next pointeur3
        End If
    Next pointeur2
Next pointeur1
            

End Sub
