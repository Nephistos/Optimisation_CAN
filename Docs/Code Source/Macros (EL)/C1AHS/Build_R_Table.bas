Attribute VB_Name = "Build_R_Table"
Option Explicit

'objectif : construire une table qui r�capitule tous les couples param�tre/consommateur
'sans update time afin de pouvoir les corriger

Sub Detect_R_from_Param()

'd�sactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'd�claration des variables

Dim VRange_param, VRange_param_calc, VRange_isr_synthese, VRange_r_table As Range
Dim pointeur1, pointeur2, pointeur3 As Long
Dim NewSheet As Worksheet
Dim increment As Integer

 ' Suppression de la feuille existante et cr�ation de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("R_Table").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "R_Table"
Sheets("R_Table").Move Before:=Sheets("Message List (FD+HS) all CAN")
ThisWorkbook.Worksheets("R_Table").Tab.ColorIndex = 41

'on d�finit la range g�n�rale puis des calculateurs

'range de param
ThisWorkbook.Worksheets("Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_param = ActiveCell.CurrentRegion

'range des calculateurs de param
Set VRange_param_calc = Worksheets("Param").Range(Cells(1, 9), Cells(VRange_param.Rows.Count, VRange_param.Columns.Count))

'recopie de la premiere ligne d'ent�te

Worksheets("Param").Cells(1, 1).EntireRow.Copy Destination:=Worksheets("R_Table").Cells(1, 1)

'comparaison des noms des parametres pour chaque consommateur

'initialisation index R_Table
increment = 2

'balayage des lignes de param
For pointeur1 = 2 To VRange_param.Rows.Count
    DoEvents
    'balayage des calculateurs
    For pointeur2 = 9 To VRange_param.Columns.Count
        'si on a un R qui est restant alors on recopie la ligne enti�re dans R_Table
        If ((Worksheets("Param").Cells(pointeur1, pointeur2).Value = "R") And (pointeur1 <> ThisWorkbook.Worksheets("R_Table").Cells(increment - 1, 100).Value)) Then
            Worksheets("Param").Cells(pointeur1, pointeur2).EntireRow.Copy Destination:=Worksheets("R_Table").Cells(increment, 1)
            'copie de la position de la ligne copi�e
            Worksheets("R_Table").Cells(increment, 100).Value = pointeur1
            'incrementation de l'index
            increment = increment + 1
            'si on a copier une ligne, on passe � la suivante en remettant les colonne � l'origine
            'pointeur1 = pointeur1 + 1
            'pointeur2 = 9
        End If
    Next pointeur2
Next pointeur1

'creation de la range de R Table

ThisWorkbook.Worksheets("R_Table").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_r_table = ActiveCell.CurrentRegion

'clear format

Worksheets("R_Table").Range(Cells(1, 1), Cells(VRange_r_table.Rows.Count, VRange_r_table.Columns.Count)).ClearFormats

'r�activation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub

'objectif : inutilis�e car redondante avec r table (fonctionne avec export_isr_period)
'construire une table de toutes les isr sans update time

Sub build_miss_isr_period_table()

'd�sactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False


'd�claration des variables

Dim VRange_isr_synthese, VRange_miss_period As Range
Dim pointeur1 As Long
Dim index As Integer
Dim NewSheet As Worksheet

index = 2

 ' Suppression de la feuille existante et cr�ation de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("Miss Period Param").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "Miss Period Param"
Sheets("Miss Period Param").Move Before:=Sheets("CAN Message Set")
ThisWorkbook.Worksheets("Miss Period Param").Tab.ColorIndex = 41

ThisWorkbook.Worksheets("ISR_Synthese").Cells(1, 1).EntireRow.Copy Destination:=ThisWorkbook.Worksheets("Miss Period Param").Cells(1, 1)

'creation de la d'isr synthese

ThisWorkbook.Worksheets("ISR_Synthese").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_isr_synthese = ActiveCell.CurrentRegion

'balayage de isr synthese
For pointeur1 = 2 To VRange_isr_synthese.Rows.Count
    If (ThisWorkbook.Worksheets("ISR_Synthese").Cells(pointeur1, 29).Value = "") Then
        ThisWorkbook.Worksheets("ISR_Synthese").Cells(pointeur1, 29).EntireRow.Copy Destination:=ThisWorkbook.Worksheets("Miss Period Param").Cells(index, 1)
        ThisWorkbook.Worksheets("Miss Period Param").Cells(index, 70).Value = pointeur1
        index = index + 1
    End If
Next pointeur1

'creation de miss period param

ThisWorkbook.Worksheets("Miss Period Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_miss_period = ActiveCell.CurrentRegion

'clear format

Worksheets("Miss Period Param").Range(Cells(1, 1), Cells(VRange_miss_period.Rows.Count, VRange_miss_period.Columns.Count)).ClearFormats

'r�activation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub
