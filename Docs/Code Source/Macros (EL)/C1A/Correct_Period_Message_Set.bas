Attribute VB_Name = "Correct_Period_Message_Set"
Option Explicit

'objectif : copier les périodes d'isr corrigées dans isr synthese dans CAN Message Set afin de
'les replacer au bon endroit

Sub copy_period_to_message_set()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'déclaration des variables

Dim VRange_can_set_message, VRange_can_set_message_calc, VRange_isr_synthese As Range
Dim VRange_can_set_message2, VRange_can_set_message_calc2, VRange_isr_synthese2 As Variant
Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5, pointeur6 As Long

'on définit la range de ISR synthese

ThisWorkbook.Worksheets("ISR_Synthese").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_isr_synthese = ActiveCell.CurrentRegion
VRange_isr_synthese2 = VRange_isr_synthese.Value

'on définit la range générale puis des calculateurs

ThisWorkbook.Worksheets("CAN Message Set").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_can_set_message = ActiveCell.CurrentRegion
VRange_can_set_message2 = VRange_can_set_message.Value

'comparaison des noms des parametres pour chaque consommateur

'STEP 1 : on remplase SI POSSIBLE par un update time

'balayage des colonnes de calculateurs de CAN message set
For pointeur2 = 23 To VRange_can_set_message.Columns.Count
    'balayage de toutes les lignes de CAN message set
    For pointeur1 = 2 To VRange_can_set_message.Rows.Count
        'si on a des T on remplace par un vide avant la prochaine condition (prochain if)
        If (VRange_can_set_message2(pointeur1, pointeur2) = "T") Then
            VRange_can_set_message2(pointeur1, pointeur2) = ""
        End If
        'si on a un R ou un update time...
        If (VRange_can_set_message2(pointeur1, pointeur2) <> "") Then
            ' balayage des lignes de isr synthese
            For pointeur3 = 2 To VRange_isr_synthese.Rows.Count
                'si on a le meme parametre (tout en maj) avec le meme consommateur dans CAN message set et isr synthese
                If ((UCase(VRange_can_set_message2(pointeur1, 1)) = UCase(VRange_isr_synthese2(pointeur3, 8))) And (UCase(VRange_can_set_message2(1, pointeur2)) = UCase(VRange_isr_synthese2(pointeur3, 39)))) Then 'And (Worksheets("ISR_Synthese").Cells(pointeur3, 40).Value <> "")) Then
                    'si possible on remplace par un update time plus petit
                    If ((VRange_can_set_message2(pointeur1, pointeur2) <> "") And (VRange_isr_synthese2(pointeur3, 40) < VRange_can_set_message2(pointeur1, pointeur2)) And (VRange_isr_synthese2(pointeur3, 40) <> "") And (VRange_isr_synthese2(pointeur3, 40) <> "0") And (VRange_isr_synthese2(pointeur3, 40) <> "1")) Then
                        VRange_can_set_message2(pointeur1, pointeur2) = VRange_isr_synthese2(pointeur3, 40)
                    End If
                End If
            Next pointeur3
        End If
    Next pointeur1
Next pointeur2

'STEP 2 : on remplace SI POSSIBLE par un 0 => event

'balayage des colonnes de calculateurs de CAN message set
For pointeur2 = 23 To VRange_can_set_message.Columns.Count
    'balayage de toutes les lignes de CAN message set
    For pointeur1 = 2 To VRange_can_set_message.Rows.Count
        'si on a des T on remplace par un vide avant la prochaine condition (prochain if)
        If (VRange_can_set_message2(pointeur1, pointeur2) = "T") Then
            VRange_can_set_message2(pointeur1, pointeur2) = ""
        End If
        'si on a un R ou un update time...
        If (VRange_can_set_message2(pointeur1, pointeur2) <> "") Then
            ' balayage des lignes de isr synthese
            For pointeur3 = 2 To VRange_isr_synthese.Rows.Count
                'si on a le meme parametre (tout en maj) avec le meme consommateur dans CAN message set et isr synthese
                If ((UCase(VRange_can_set_message2(pointeur1, 1)) = UCase(VRange_isr_synthese2(pointeur3, 8))) And (UCase(VRange_can_set_message2(1, pointeur2)) = UCase(VRange_isr_synthese2(pointeur3, 39)))) Then 'And (Worksheets("ISR_Synthese").Cells(pointeur3, 40).Value <> "")) Then
                    'si possible on remplace par un event
                    If ((VRange_can_set_message2(pointeur1, pointeur2) = "R") And (VRange_isr_synthese2(pointeur3, 40) = "0")) Then
                        VRange_can_set_message2(pointeur1, pointeur2) = VRange_isr_synthese2(pointeur3, 40)
                    End If
                End If
            Next pointeur3
        End If
    Next pointeur1
Next pointeur2

'réecriture finale

For pointeur5 = 23 To VRange_can_set_message.Columns.Count
    For pointeur6 = 2 To VRange_can_set_message.Rows.Count
        ThisWorkbook.Worksheets("CAN Message Set").Cells(pointeur6, pointeur5).Value = VRange_can_set_message2(pointeur6, pointeur5)
    Next pointeur6
Next pointeur5

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub
