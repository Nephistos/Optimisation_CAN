Attribute VB_Name = "Correct_R_Message_Set"
Option Explicit

'objectif : corriger les paramètre ASIL de param qui sont actuellement dans la r table

Sub correct_r_table()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'déclaration des variables

 Dim VRange_r_table, VRange_param As Range
 Dim Caractere, Caractere2 As Integer
 Dim i, j As Integer
 Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5 As Long
 Dim current_frame As String
 Dim verrouillage As Boolean
 Dim memory As Integer
 
 'initialisation des variables
 
 current_frame = ""
 verrouillage = False
 memory = 0

 
 'creation des plages de données
 
ThisWorkbook.Worksheets("Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_param = ActiveCell.CurrentRegion
 
ThisWorkbook.Worksheets("R_Table").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_r_table = ActiveCell.CurrentRegion

'detection des CRC et Clocks

    'balayage des lignes de r_table
     For i = 2 To VRange_r_table.Rows.Count
        DoEvents
         'balayage de tous les caracères d'une cellule
         For Caractere = 1 To Len(ThisWorkbook.Worksheets("R_Table").Cells(i, 5))
             'si detection de CRC...
             If (UCase(Mid(ThisWorkbook.Worksheets("R_Table").Cells(i, 5), Caractere, 3)) = "CRC") Then
                'on balaye les calculateurs
                For pointeur1 = 9 To VRange_r_table.Columns.Count
                    'si il y a un R alors on le remplace par l'update time de la trame
                    If (ThisWorkbook.Worksheets("R_Table").Cells(i, pointeur1).Value = "R") Then
                        ThisWorkbook.Worksheets("R_Table").Cells(i, pointeur1).Value = ThisWorkbook.Worksheets("R_Table").Cells(i, 4).Value
                        ThisWorkbook.Worksheets("R_Table").Cells(i, 5).Interior.Color = RGB(0, 255, 0)
                    End If
                Next pointeur1
             End If
         Next Caractere
    Next i
    
    'balayage des lignes de r_table
     For j = 2 To VRange_r_table.Rows.Count
        DoEvents
         'balayage de tous les caracères d'une cellule
         For Caractere2 = 1 To Len(ThisWorkbook.Worksheets("R_Table").Cells(j, 5))
             'si detection de Clock...
             If (UCase(Mid(ThisWorkbook.Worksheets("R_Table").Cells(j, 5), Caractere2, 5)) = "CLOCK") Then
                'on balaye les calculateurs
                For pointeur2 = 9 To VRange_r_table.Columns.Count
                    'si il y a un R alors on le remplace par l'update time de la trame
                    If (ThisWorkbook.Worksheets("R_Table").Cells(j, pointeur2).Value = "R") Then
                        ThisWorkbook.Worksheets("R_Table").Cells(j, pointeur2).Value = ThisWorkbook.Worksheets("R_Table").Cells(j, 4).Value
                        ThisWorkbook.Worksheets("R_Table").Cells(j, 5).Interior.Color = RGB(0, 255, 0)
                    End If
                Next pointeur2
             End If
         Next Caractere2
    Next j
    
'copie des lignes CRC et clock dans Param (avec le bon update time)
     
    For pointeur3 = 2 To VRange_r_table.Rows.Count
        DoEvents
        If (ThisWorkbook.Worksheets("R_Table").Cells(pointeur3, 5).Interior.Color = RGB(0, 255, 0)) Then
            Worksheets("R_Table").Cells(pointeur3, 5).EntireRow.Copy Destination:=Worksheets("Param").Cells(Worksheets("R_Table").Cells(pointeur3, 70).Value, 1)
            Worksheets("Param").Cells(Worksheets("R_Table").Cells(pointeur3, 70).Value, 8).Value = 1
        End If
    Next pointeur3
    
'On fixe les CRC & Clock si il ne sont déja plus dans la R_Table
    
    For pointeur5 = 3 To VRange_param.Rows.Count
        DoEvents
        If (ThisWorkbook.Worksheets("Param").Cells(pointeur5, 5).Interior.Color = RGB(0, 255, 0)) Then
            ThisWorkbook.Worksheets("Param").Cells(pointeur5, 8).Value = "1"
        End If
    Next pointeur5
    
    
'On fixe tous les paramètres des trames qui contiennent un ou plusieurs ASILs
    
    'balayage des lignes de param
    For pointeur4 = 3 To VRange_param.Rows.Count
        DoEvents
        'si on rencontre un paramtre fixé précédemment hors current frame on definit sa frame comme current frame et on active le verrouillage
        If ((ThisWorkbook.Worksheets("Param").Cells(pointeur4, 8).Value = "1") And (ThisWorkbook.Worksheets("Param").Cells(pointeur4, 2).Value <> current_frame) And (verrouillage = False)) Then
            current_frame = ThisWorkbook.Worksheets("Param").Cells(pointeur4, 2).Value
            verrouillage = True
            memory = pointeur4
            pointeur4 = 3
        End If
        'si on est dans la current frame on verrouille chacun de ses parametres
        If (ThisWorkbook.Worksheets("Param").Cells(pointeur4, 2).Value = current_frame) Then
            ThisWorkbook.Worksheets("Param").Cells(pointeur4, 8).Value = "1"
            'on deverrouille lorsqu'on est revenu au parametre initial
            If (pointeur4 = memory) Then
                verrouillage = False
            End If
        End If
    Next pointeur4
    
    'on supprime la colonne 70 de param
     ThisWorkbook.Worksheets("Param").Cells(1, 70).EntireColumn.Delete
     
'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True
                  
End Sub

'objectif : recopier les lignes de r table dans param une fois celle-ci corrigée

Sub post_correction_manuel()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'déclaration des variables

Dim pointeur3 As Long
Dim VRange_r_table As Range

'création de la range de R Table

ThisWorkbook.Worksheets("R_Table").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_r_table = ActiveCell.CurrentRegion

'balayage des lignes de r table
For pointeur3 = 2 To VRange_r_table.Rows.Count
        'recopie des lignes corrigés dans param
        Worksheets("R_Table").Cells(pointeur3, 5).EntireRow.Copy Destination:=Worksheets("Param").Cells(Worksheets("R_Table").Cells(pointeur3, 70).Value, 1)
Next pointeur3

'on supprime la colonne 70 de param
ThisWorkbook.Worksheets("Param").Cells(1, 70).EntireColumn.Delete

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub

'objectif : corriger automatiquement les lignes de R table puis les recopier dans param

Sub correct_auto_R_table()

'déclaration des variables

Dim VRange_r_table As Range
Dim pointeur1, pointeur2 As Long

'création de la range de r table

ThisWorkbook.Worksheets("R_Table").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_r_table = ActiveCell.CurrentRegion

'balayage des calculateurs de r table
For pointeur1 = 9 To VRange_r_table.Columns.Count
    'balayage des lignes de r table
    For pointeur2 = 2 To VRange_r_table.Rows.Count
        'si un calculateur/paramètre n'a pas d'update time alors...
        If (ThisWorkbook.Worksheets("R_Table").Cells(pointeur2, pointeur1).Value = "R") Then
            'l'update time de sa trame devient également le sien
            ThisWorkbook.Worksheets("R_Table").Cells(pointeur2, pointeur1).Value = ThisWorkbook.Worksheets("R_Table").Cells(pointeur2, 4).Value
        End If
    Next pointeur2
Next pointeur1

End Sub

'objectif : inutilisée car redondante avec r table (fonctionne avec export_isr_period)
'exporter la table de toutes les isrs sans update time une fois corrigées manuellement

Sub export_isr_period()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'déclaration des variables

Dim VRange_isr_synthese, VRange_miss_period As Range
Dim pointeur1 As Long

'creation de la range d'isr synthese

ThisWorkbook.Worksheets("ISR_Synthese").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_isr_synthese = ActiveCell.CurrentRegion

'creation de la range de miss period param

ThisWorkbook.Worksheets("Miss Period Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_miss_period = ActiveCell.CurrentRegion

'balayage de miss period param
For pointeur1 = 2 To VRange_miss_period.Rows.Count
    ThisWorkbook.Worksheets("Miss Period Param").Cells(pointeur1, 1).EntireRow.Copy Destination:=ThisWorkbook.Worksheets("ISR_Synthese").Cells(ThisWorkbook.Worksheets("Miss Period Param").Cells(pointeur1, 70).Value, 1)
Next pointeur1

'on supprime la colonne 70 de ISR Synthese
ThisWorkbook.Worksheets("ISR_Synthese").Cells(1, 70).EntireColumn.Delete

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub

