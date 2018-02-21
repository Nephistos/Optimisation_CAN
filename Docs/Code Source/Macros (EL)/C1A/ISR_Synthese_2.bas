Attribute VB_Name = "ISR_Synthese_2"
Option Explicit

'objectif : mettre en corrélation ISR synthese et ISR applied afin de créer les triplés
'paramètre/consommateur/update time puis replacer ces triplés dans Param grâce à
'Correct Correct_Period_Message_Set

Sub correct_isr_synthese_2()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'PART2 : on complète les consommateurs réels pour chaque isr

'déclaration & initialisation des variable

Dim VRange_isr_synthese, VRange_isr_applied As Range
Dim VRange_isr_synthese2 As Variant
Dim VRange_isr_applied2 As Variant
Dim pointeur0, pointeur1, pointeur2, pointeur3, pointeur4 As Long

For pointeur0 = 36 To 40
    ThisWorkbook.Worksheets("ISR_Synthese").Cells(1, pointeur0).Value = "EXTEND CURRENT REGION"
Next pointeur0

'création de la plage isr synthese

ThisWorkbook.Worksheets("ISR_Synthese").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_isr_synthese = ActiveCell.CurrentRegion
VRange_isr_synthese2 = VRange_isr_synthese.Value

'création de la plage isr applied

ThisWorkbook.Worksheets("C1A Applied Q1 2017").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_isr_applied = ActiveCell.CurrentRegion
VRange_isr_applied2 = VRange_isr_applied.Value

'comparaison des ISR et recopie du consommateur

'balayage des lignes de isr_synthese
For pointeur1 = 2 To VRange_isr_synthese.Rows.Count
    'balayage des lignes de isr applied
    For pointeur2 = 2 To VRange_isr_applied.Rows.Count
        'si on a une isr identique on recopie le consommateur de isr applied vers isr synthese
        If (CStr(VRange_isr_applied2(pointeur2, 1)) = CStr(VRange_isr_synthese2(pointeur1, 1))) Then
            VRange_isr_synthese2(pointeur1, 39) = VRange_isr_applied2(pointeur2, 6)
            Exit For
        End If
    Next pointeur2
Next pointeur1

'traitement des cas ou l'isr n'existe soit pas du tout soit seulement dans isr synthese

'balayage des lignes de isr synthese
For pointeur3 = 2 To VRange_isr_synthese.Rows.Count
    'si la case est vide c'est que l'isr n'a pas été retrouvé
    If (VRange_isr_synthese2(pointeur3, 39) = "") Then
        VRange_isr_synthese2(pointeur3, 39) = VRange_isr_synthese2(pointeur3, 6)
    End If
Next pointeur3

'MsgBox NombreDimensions(VRange_isr_synthese2)

For pointeur4 = 2 To VRange_isr_synthese.Rows.Count
    ThisWorkbook.Worksheets("ISR_Synthese").Cells(pointeur4, 39).Value = VRange_isr_synthese2(pointeur4, 39)
    ThisWorkbook.Worksheets("ISR_Synthese").Cells(pointeur4, 40).Value = VRange_isr_synthese2(pointeur4, 40)
Next pointeur4

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub

Function NombreDimensions(monTableau As Variant)
Dim i As Integer
Dim monRetour As Variant

On Error Resume Next
Do
i = i + 1
monRetour = LBound(monTableau, i)
Loop Until Err <> 0
On Error GoTo 0
NombreDimensions = i - 1
End Function
