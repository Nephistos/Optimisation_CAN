Attribute VB_Name = "Frame_bri"
Option Explicit

Sub frame_bri()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'declaration des variables

Dim VRange_frame, VRange_network_path As Range
Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5, pointeur6, pointeur7 As Long
Dim increment, max As Integer
Dim found As Boolean
Dim Caractere, Chaine As Variant

increment = 0
found = False

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

ThisWorkbook.Worksheets("Network Path").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_network_path = ActiveCell.CurrentRegion

'traitement en 2 passages

'balayage des lignes de frame
For pointeur1 = 5 To VRange_frame.Rows.Count
    DoEvents
    found = False
    'balayage des colonnes de frame
    For pointeur2 = 6 To VRange_frame.Columns.Count
        'si on trouve l'émetteur alors...
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur2).Value = "T") Then
            'c'est bon on peut quitter cette ligne
            found = True
            Exit For
        'sinon...
        ElseIf (pointeur2 = VRange_frame.Columns.Count) Then
            Chaine = ""
            
            'balayage des lignes de network path
            For pointeur3 = 2 To VRange_network_path.Rows.Count
                'si on retrouve la même trame alors...
                If (UCase(ThisWorkbook.Worksheets("Network Path").Cells(pointeur3, 1).Value) = UCase(ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Value)) Then
                    'balayage des colonnes de frame
                    For pointeur4 = 6 To VRange_frame.Columns.Count
                        'si on retrouve le même calculateur alors...
                        If (UCase(ThisWorkbook.Worksheets("Frame").Cells(1, pointeur4).Value) = UCase(ThisWorkbook.Worksheets("Network Path").Cells(pointeur3, 3).Value)) Then
                            'on le nothifie d'un "T"
                            ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur4).Value = "T"
                            found = True
                            Exit For
                        End If
                    Next pointeur4
                'sinon...
                ElseIf ((pointeur3 = VRange_network_path.Rows.Count) And (found = False)) Then
                    'balayage de tous les caracères d'une cellule
                    For Caractere = 1 To Len(ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1))
                        'si detection des "_"
                        If (Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1), Caractere, 1) <> "_") Then
                            'stockage des numeriques dans la variable intermediaire
                            Chaine = Chaine & Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1), Caractere, 1)
                        'sinon (= "_") on met notre compteur de caractère à la taille maximale de la cellule
                        Else: Caractere = Len(ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1))
                        End If
                    Next Caractere
                    'rebalayage des colonnes de frame
                    For pointeur5 = 6 To VRange_frame.Columns.Count
                        'si on trouve le vrai émetteur (au lieu du bridge) alors...
                        If (UCase(ThisWorkbook.Worksheets("Frame").Cells(1, pointeur5).Value) = UCase(Chaine)) Then
                            'on met  un "T"
                            ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur5).Value = "T"
                            found = True
                            Exit For
                        'sinon...
                        ElseIf ((pointeur5 = VRange_frame.Columns.Count) And (found = False)) Then
                            'en dernier recour on supprime la ligne
                            ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)
                            Exit For
                        End If
                    Next pointeur5
                
                End If
            Next pointeur3
        
        End If
    Next pointeur2
    
    If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)) Then
        increment = increment + 1
    End If
    If (increment > max) Then
        max = increment
    End If
    If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Interior.Color <> RGB(255, 0, 0)) Then
        increment = 0
    End If
    
Next pointeur1

For pointeur7 = 1 To max

'balayage des lignes de frame
For pointeur1 = 5 To VRange_frame.Rows.Count
    If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Interior.Color = RGB(255, 0, 0)) Then
        ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).EntireRow.Delete
    End If
Next pointeur1

Next pointeur7

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True
        

End Sub
