Attribute VB_Name = "Drawing"
Option Explicit

Sub draw_topology()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

' Suppression de la feuille existante et création de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("Topology Draw").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True
 
 'déclaration des variables
 
 Dim NewSheet As Worksheet
 Dim VRange_topology As Range
 Dim pointeur1, pointeur2, pointeur3 As Long
 
 'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "Topology Draw"
 
 'creation de la range de topology

ThisWorkbook.Worksheets(2).Name = "topology"
ThisWorkbook.Worksheets("topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_topology = ActiveCell.CurrentRegion

For pointeur1 = 1 To VRange_topology.Rows.count
    ThisWorkbook.Worksheets("topology").Cells(VRange_topology.Rows.count + 1, pointeur1).Value = "EXTEND CURRENT REGION"
Next pointeur1

 'maj de la range de topology

ThisWorkbook.Worksheets("topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_topology = ActiveCell.CurrentRegion

For pointeur1 = 1 To VRange_topology.Rows.count
    ThisWorkbook.Worksheets("topology").Cells(VRange_topology.Rows.count, pointeur1).Value = ""
Next pointeur1

'traitement

'declaration & initialisation des variables

Dim list_calc(100, 100) As String
Dim pos_calc_x(100), pos_calc_y(100) As Single
Dim pos_calc_res(100) As Boolean

For pointeur1 = 0 To 100
    For pointeur2 = 0 To 100
        list_calc(pointeur2, pointeur1) = ""
    Next pointeur2
    pos_calc_x(pointeur1) = 0
    pos_calc_y(pointeur1) = 0
    pos_calc_res(pointeur1) = False
Next pointeur1

'balayage des lignes de la topologie
For pointeur1 = 1 To VRange_topology.Rows.count
    'balayage des colonnes de la topologie
    For pointeur2 = 2 To VRange_topology.Columns.count
        'on sauvegarde le connecteur d'un calculateur à un réseau dans un tableau à deux dimensions
        If (ThisWorkbook.Worksheets("topology").Cells(pointeur1, pointeur2).Value = "X") Then
            list_calc(pointeur2 - 2, pointeur1 - 1) = ThisWorkbook.Worksheets("topology").Cells(pointeur1, 1).Value
        End If
    Next pointeur2
Next pointeur1

'redeclaration & initialisation de variables

Dim pos_bx, pos_by, pos_ex, pos_ey, width, height, increment_x, increment_y As Single
Dim myDocument As Variant
Dim increment, inc_color, inc_next_con, inc_next_con2, count, R, G, B, count2, count3 As Integer
Dim objShp As Shape
Dim verrouillage, verrouillage2, verrouillage3 As Boolean


pos_bx = 120 / 10
pos_by = 240 / 10
pos_ex = (3500 * 2) / 18
pos_ey = 240 / 10

width = (120 * 2) / 8
height = 60 / 6

increment_x = (pos_bx + width)
increment_y = (pos_by + height)

increment = 1
inc_color = 3
inc_next_con = 30 / 10
inc_next_con2 = 0
verrouillage = False
verrouillage2 = False
verrouillage3 = False
count = 0
count2 = 0
count3 = 0

'balayage des colonnes de la topologie
For pointeur1 = 0 To VRange_topology.Columns.count
    
    verrouillage = True

    'balayage des lignes de la topologie
    For pointeur2 = 0 To VRange_topology.Rows.count
        'si on a un nouveau calculateur avec un connecteur sur le réseau "i" alors...
        If ((list_calc(pointeur1, pointeur2) <> "") And (ThisWorkbook.Worksheets("topology").Cells(pointeur2 + 1, 1).Interior.Color <> RGB(255, 0, 0))) Then
                        
            'on créer un carré qui content son nom
            Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
            'si on à la place sur la première ligne alors...
            If (count < 7) Then
                'on créer le carré et on incrémente les attributs pour le prochain
                Set objShp = myDocument.Shapes.AddShape(msoTextOrientationHorizontal, increment_x, increment_y, width, height)
                'on retient sa position
                pos_calc_x(pointeur2 + 1) = increment_x
                pos_calc_y(pointeur2 + 1) = increment_y
                'ici on incremente les attibuts
                objShp.DrawingObject.Text = list_calc(pointeur1, pointeur2)
                objShp.DrawingObject.Font.Size = 40 / 7
                objShp.DrawingObject.HorizontalAlignment = xlLeft
                objShp.DrawingObject.VerticalAlignment = xlCenter
                'objShp.DrawingObject.Interior.ColorIndex = inc_color
                
                If (verrouillage3 = False) Then
                    'on génère un nombre aléatoire pour déterminer la couleur du carré
                    Randomize
                    R = Int(255 * Rnd)
                    G = Int(255 * Rnd)
                    B = Int(255 * Rnd)
                    verrouillage3 = True
                End If
                
                'gestion de la couleur de police
                If ((R < 127) And (G < 127) And (B < 127)) Then
                    objShp.DrawingObject.Font.Color = RGB(255, 255, 255)
                Else
                    objShp.DrawingObject.Font.Color = RGB(0, 0, 0)
                End If
                
                objShp.DrawingObject.Interior.Color = RGB(R, G, B)
                objShp.DrawingObject.Border.Color = RGB(R, G, B)
                ThisWorkbook.Worksheets("topology").Cells(pointeur2 + 1, 1).Interior.Color = RGB(255, 0, 0)
                increment_x = increment_x + (240 * 2) / 10
                
                'et on créé une ligne pour le connecter à son réseau
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(increment_x - (240 * 2) / 10 + width / 2, increment_y, increment_x - (240 * 2) / 10 + width / 2, pos_ey).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(R, G, B)
                    .Weight = 1
                End With
                count = count + 1
            'sinon si on a plus la place alors...
            ElseIf (count > 6) Then
                'on revient au début de la ligne
                If (verrouillage2 = False) Then
                    increment_x = 36
                    verrouillage2 = True
                End If
                'on créé le carré
                Set objShp = myDocument.Shapes.AddShape(msoTextOrientationHorizontal, increment_x, increment_y + 300 / 10, width, height)
                'on incrémente de la même manière les attributs pour la nouvelle ligne
                pos_calc_x(pointeur2 + 1) = increment_x
                pos_calc_y(pointeur2 + 1) = increment_y
                pos_calc_res(pointeur2 + 1) = True
                
                objShp.DrawingObject.Text = list_calc(pointeur1, pointeur2)
                objShp.DrawingObject.Font.Size = 40 / 7
                objShp.DrawingObject.HorizontalAlignment = xlLeft
                objShp.DrawingObject.VerticalAlignment = xlCenter
                'objShp.DrawingObject.Interior.ColorIndex = inc_color
                
                If (verrouillage3 = False) Then
                    'on génère un nombre aléatoire pour déterminer la couleur du carré
                    Randomize
                    R = Int(255 * Rnd)
                    G = Int(255 * Rnd)
                    B = Int(255 * Rnd)
                    verrouillage3 = True
                End If
                
                'gestion de la couleur de police suivant le fond
                If ((R < 127) And (G < 127) And (B < 127)) Then
                    objShp.DrawingObject.Font.Color = RGB(255, 255, 255)
                Else
                    objShp.DrawingObject.Font.Color = RGB(0, 0, 0)
                End If
                
                objShp.DrawingObject.Interior.Color = RGB(R, G, B)
                objShp.DrawingObject.Border.Color = RGB(R, G, B)
                ThisWorkbook.Worksheets("topology").Cells(pointeur2 + 1, 1).Interior.Color = RGB(255, 0, 0)
                increment_x = increment_x + (240 * 2) / 10
                
                'et on crée une ligne pour le connecter à la nouvelle ligne de son réseau
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(increment_x - (240 * 2) / 10 + width / 2, increment_y + 300 / 10, increment_x - (240 * 2) / 10 + width / 2, pos_ey + 300 / 10).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(R, G, B)
                    .Weight = 1
                End With
                count = count + 1
            End If
        
        'sinon si on a un ANCIEN calculateur avec un connecteur sur le réseau "i" alors...
        ElseIf ((list_calc(pointeur1, pointeur2) <> "") And (ThisWorkbook.Worksheets("topology").Cells(pointeur2 + 1, 1).Interior.Color = RGB(255, 0, 0))) Then
            'si le calculateur est sur la première ligne du réseau alors...
            If (pos_calc_res(pointeur2 + 1) = False) Then
                'et une ligne pour le connecter à son réseau
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(pos_calc_x(pointeur2 + 1) + width / 2, pos_calc_y(pointeur2 + 1) + height, pos_calc_x(pointeur2 + 1) + width / 2, pos_calc_y(pointeur2 + 1) + height + inc_next_con).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(0, 0, 0)
                    .Weight = 1
                End With
                
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(pos_calc_x(pointeur2 + 1) + width / 2, pos_calc_y(pointeur2 + 1) + height + inc_next_con, pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2, pos_calc_y(pointeur2 + 1) + height + inc_next_con).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(0, 0, 0)
                    .Weight = 1
                End With
                
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2, pos_calc_y(pointeur2 + 1) + height + inc_next_con, pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2, pos_ey).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(0, 0, 0)
                    .Weight = 1
                End With
                
                Set objShp = myDocument.Shapes.AddShape(msoShapeOval, pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2 - 2, pos_ey - 2, 4, 4)
                objShp.DrawingObject.Interior.Color = RGB(0, 0, 0)
                objShp.DrawingObject.Border.Color = RGB(0, 0, 0)
                
            'si il est sur la 2ème ligne alors on s'adapte pour créer les fils du 2ème connecteur
            Else
                'et une ligne pour le connecter à son réseau
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(pos_calc_x(pointeur2 + 1) + width / 2, pos_calc_y(pointeur2 + 1) + height + 300 / 10, pos_calc_x(pointeur2 + 1) + width / 2, pos_calc_y(pointeur2 + 1) + height + inc_next_con + 300 / 10).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(0, 0, 0)
                    .Weight = 1
                End With
                
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(pos_calc_x(pointeur2 + 1) + width / 2, pos_calc_y(pointeur2 + 1) + height + inc_next_con + 300 / 10, pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2, pos_calc_y(pointeur2 + 1) + height + inc_next_con + 300 / 10).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(0, 0, 0)
                    .Weight = 1
                End With
                
                Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
                With myDocument.Shapes.AddLine(pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2, pos_calc_y(pointeur2 + 1) + height + inc_next_con + 300 / 10, pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2, pos_ey + 300 / 10).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(0, 0, 0)
                    .Weight = 1
                End With
                
                Set objShp = myDocument.Shapes.AddShape(msoShapeOval, pos_calc_x(pointeur2 + 1) + width / 2 - 240 / 10 - inc_next_con2 - 2, pos_ey - 2, 4, 4)
                objShp.DrawingObject.Interior.Color = RGB(0, 0, 0)
                objShp.DrawingObject.Border.Color = RGB(0, 0, 0)
                
            End If
            
        End If
        
        'on va créer une ligne pour le réseau
        If (ThisWorkbook.Worksheets("topology").Cells(pointeur2 + 1, pointeur1 + 2).Value = "X") Then
            verrouillage = False
            count2 = count2 + 1
            'on regarde que ce reseau n'a as que des connecteurs secondaire
            For pointeur3 = 1 To pointeur1 + 1
                If (ThisWorkbook.Worksheets("topology").Cells(pointeur2 + 1, pointeur3).Value = "X") Then
                    count3 = count3 + 1
                    Exit For
                End If
            Next pointeur3
        End If
        

    Next pointeur2
    
    verrouillage2 = False
    verrouillage3 = False
    
    'on crée la ligne pour le réseau
    If (verrouillage = False) Then
        'position si on est sur la première ligne...
        If (count < 7) Then
            Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
            'si on a un réseau normal...
            If (count2 <> count3) Then
                With myDocument.Shapes.AddLine(pos_bx, pos_by, pos_ex, pos_ey).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(R, G, B)
                    .Weight = 2
                    
                End With
                count2 = 0
                count3 = 0
            'si on a un réseau qui ne contient que des connecteurs secondaires alors c'est un autre réseau
            Else
                Randomize
                R = Int(255 * Rnd)
                G = Int(255 * Rnd)
                B = Int(255 * Rnd)
            
                With myDocument.Shapes.AddLine(pos_bx, pos_by, pos_ex, pos_ey).Line
                    '.DashStyle = msoLineDashDotDot
                    .ForeColor.RGB = RGB(R, G, B)
                    .Weight = 2
                    
                End With
                count2 = 0
                count3 = 0
            End If
        
            'on incrémente les positions des reseaux et calculateurs pour éviter les superpositions
            pos_by = pos_by + 300 / 10
            pos_ey = pos_ey + 300 / 10
            
            increment_y = pos_by + height
            increment_x = pos_bx + width
            'inc_color = inc_color + 1
            inc_next_con = 30 / 10
            count = 0
            
        'sinon si on est sur la 2ème ligne...
        ElseIf (count > 6) Then
        
            '1er réseau comme as préc.
            Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
            With myDocument.Shapes.AddLine(pos_bx, pos_by, pos_ex, pos_ey).Line
                '.DashStyle = msoLineDashDotDot
                .ForeColor.RGB = RGB(R, G, B)
                .Weight = 2
                
            End With
            
            'connection entre les deux
            Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
            With myDocument.Shapes.AddLine(pos_ex, pos_ey, pos_ex, pos_ey + 300 / 10).Line
                '.DashStyle = msoLineDashDotDot
                .ForeColor.RGB = RGB(R, G, B)
                .Weight = 2
                
            End With
            
            'on incrémente les positions des reseaux et calculateurs pour éviter les superpositions
            pos_by = pos_by + 300 / 10
            pos_ey = pos_ey + 300 / 10
            
            increment_y = pos_by + height
            increment_x = pos_bx + width
            'inc_color = inc_color + 1
            inc_next_con = 30 / 10
            'count = 0
            
            '2ème partie
            Set myDocument = ThisWorkbook.Worksheets("Topology Draw")
            With myDocument.Shapes.AddLine(pos_bx, pos_by, pos_ex, pos_ey).Line
                '.DashStyle = msoLineDashDotDot
                .ForeColor.RGB = RGB(R, G, B)
                .Weight = 2
                
            End With
            
            'on incrémente les positions des reseaux et calculateurs pour éviter les superpositions
            pos_by = pos_by + 300 / 10
            pos_ey = pos_ey + 300 / 10
            
            increment_y = pos_by + height
            increment_x = pos_bx + width
            inc_color = inc_color + 1
            inc_next_con = 30 / 10
            count = 0
        
        End If
    End If

Next pointeur1

For pointeur1 = 1 To VRange_topology.Rows.count
    ThisWorkbook.Worksheets("topology").Cells(pointeur1, 1).Interior.Color = RGB(255, 255, 255)
Next pointeur1
         

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub
