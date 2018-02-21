Attribute VB_Name = "Mat_Rout"
Option Explicit

'objectif : création de la feuille de contraintes de routage

Sub create_mat_rout()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

 ' Suppression de la feuille existante et création de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("Mat Rout").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True
 
 'declaration des variables

Dim NewSheet As Worksheet
Dim VRange_frame_synthese, VRange_frame_list, VRange_mat_rout, VRange_frame As Range
Dim pointeur1, pointeur2 As Long
Dim increment As Integer

increment = 2

 'creation de la plage de donnée de frame synthese
 
ThisWorkbook.Worksheets("Frame synthesis").Activate
ActiveSheet.Cells(6, 1).Activate
Set VRange_frame_synthese = ActiveCell.CurrentRegion

 'creation de la plage de donnée de frame
 
ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "Mat Rout"
Sheets("Mat Rout").Move Before:=Sheets(2)
ThisWorkbook.Worksheets("Mat Rout").Tab.ColorIndex = 3

'creation de la range des frame

ThisWorkbook.Worksheets("Frame synthesis").Activate
Set VRange_frame_list = ThisWorkbook.Worksheets("Frame synthesis").Range(Cells(6, 1), Cells(VRange_frame_synthese.Rows.Count + 6, 1))

'recopie des frames verticales

'initialement => sans frame_bri
'For pointeur2 = 2 To VRange_frame_list.Rows.Count + 1
'    If ((ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Cells(pointeur2, 6) <> "-") And (ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Cells(pointeur2, 6) <> "0") And (UCase(Right(ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Cells(pointeur2, 1), 2)) <> "FD")) Then
'        ThisWorkbook.Worksheets("Mat Rout").Cells(increment, 1).Value = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Cells(pointeur2, 1)
'       increment = increment + 1
'     End If
'Next pointeur2

'avec frame bri
For pointeur2 = 5 To VRange_frame.Rows.Count
    ThisWorkbook.Worksheets("Mat Rout").Cells(pointeur2 - 3, 1).Value = ThisWorkbook.Worksheets("Frame").Cells(pointeur2, 1).Value
Next pointeur2

'creation de la range de mat rout

ThisWorkbook.Worksheets("Mat Rout").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_mat_rout = ActiveCell.CurrentRegion

'recopie des frames horizontales

For pointeur1 = 2 To VRange_mat_rout.Rows.Count
    ThisWorkbook.Worksheets("Mat Rout").Cells(1, pointeur1).Value = ThisWorkbook.Worksheets("Mat Rout").Cells(pointeur1, 1)
Next pointeur1

'clear format

Worksheets("Mat Rout").Range(Cells(1, 1), Cells(VRange_mat_rout.Rows.Count, VRange_mat_rout.Columns.Count)).ClearFormats

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True


End Sub


