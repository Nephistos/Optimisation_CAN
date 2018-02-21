Attribute VB_Name = "Compare_rout"
Sub test9()

'declarations des variables

Dim VRange_frame_to, VRange_frame_from, VRange_frame_calc As Range
Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5 As Long
Dim is_ok(100), is_ok2(100) As Boolean

For pointeur1 = 0 To 100
    is_ok(pointeur1) = False
    is_ok2(pointeur1) = False
Next pointeur1

'creation de la range de frame to (à étudier)

ThisWorkbook.Worksheets("Frame to").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_to = ActiveCell.CurrentRegion

'creation de la range des calculateurs

ThisWorkbook.Worksheets("Frame to").Activate
ActiveSheet.Cells(1, VRange_frame_to.Columns.Count + 3).Activate
Set VRange_frame_calc = ActiveCell.CurrentRegion

'creation de la range de frame from (qui marche, et, que l'on compare)

ThisWorkbook.Worksheets("Frame from").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_from = ActiveCell.CurrentRegion

'balayage des lignes de la frame à optimiser
For pointeur1 = 5 To VRange_frame_to.Rows.Count
    'réinitialisation de la varuable temporaire
    For pointeur4 = 0 To 100
        is_ok(pointeur4) = False
        is_ok2(pointeur4) = False
    Next pointeur4
    'balayage des lignes de la frame optimisé
    For pointeur2 = 5 To VRange_frame_from.Rows.Count
        'si on trouve la même trame alors...
        If (ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, 1).Value = ThisWorkbook.Worksheets("Frame from").Cells(pointeur2, 1).Value) Then
            'on balaye la partie calculateur afin de...
            For pointeur3 = VRange_frame_to.Columns.Count + 3 To VRange_frame_to.Columns.Count + 3 + VRange_frame_calc.Columns.Count - 2
                'on sauvegarde le routage de la trame à optimiser
                ThisWorkbook.Worksheets("Frame to").Cells(1, pointeur3).Interior.Color = RGB(0, 255, 0)
                If (ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, pointeur3).Value <> "0") Then
                    is_ok(pointeur3) = True
                End If
                'et celui de la trame optimisé
                ThisWorkbook.Worksheets("Frame from").Cells(1, pointeur3 - 2).Interior.Color = RGB(0, 255, 0)
                If (ThisWorkbook.Worksheets("Frame from").Cells(pointeur2, pointeur3 - 2).Value <> "0") Then
                    is_ok2(pointeur3) = True
                End If
            Next pointeur3
            'puis on compare les 2
            For pointeur5 = 0 To 100
                'si ils sont different on le nothifie
                If (is_ok(pointeur5) <> is_ok2(pointeur5)) Then
                    ThisWorkbook.Worksheets("Frame to").Cells(pointeur1, VRange_frame_to.Columns.Count + 2).Interior.Color = RGB(255, 0, 0)
                    ThisWorkbook.Worksheets("Frame from").Cells(pointeur2, VRange_frame_to.Columns.Count).Interior.Color = RGB(255, 0, 0)
                End If
            Next pointeur5
            
        End If
    Next pointeur2
Next pointeur1
        

End Sub
