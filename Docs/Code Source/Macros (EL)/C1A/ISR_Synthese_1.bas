Attribute VB_Name = "ISR_Synthese_1"
Option Explicit

'objectif : corriger les périodes de toutes les ISRs

Sub correct_isr_synthese_1()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'PART 1 : correction des update time de isr synthese

 Dim VRange_isr_synthese As Range
 Dim Caractere As Integer, i As Integer, Chaine As Variant
 
 'creation de la plage de donnée
 
ThisWorkbook.Worksheets("ISR_Synthese").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_isr_synthese = ActiveCell.CurrentRegion

'suppression des non numériques

    'balayage des lignes de isr synthese
     For i = 2 To VRange_isr_synthese.Rows.Count
         'reinitialisation de notre variable intermédiaire
         Chaine = ""
         'balayage de tous les caracères d'une cellule
         For Caractere = 1 To Len(ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 29))
             'si detection des numeriques...
             If IsNumeric(Mid(ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 29), Caractere, 1)) Then
                'stockage des numeriques dans la variable intermediaire
                Chaine = Chaine & Mid(ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 29), Caractere, 1)
             'sinon si on a un espace on en tient pas compte
             ElseIf (Mid(ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 29), Caractere, 1) = " ") Then Caractere = Caractere
             'sinon (autre que numeriques et espace...) on met notre compteur de caractère à la taille maximale de la cellule
             Else: Caractere = Len(ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 29))
             End If
         Next Caractere
         'recopie dans une autre colonne
         ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 40).Formula = "'" & Chaine
         'si on a un event on met 0 dans l'autre colonne (cf notice SW d'optimisation CAN)
         If ((ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 40).Value = "") And (ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 29).Value <> "")) Then
            ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 40).Value = "0"
        End If
        'ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 40).Value = CDbl(ThisWorkbook.Worksheets("ISR_Synthese").Cells(i, 40).Value)
     Next i
     
     
'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True
     
End Sub

