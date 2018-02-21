Attribute VB_Name = "Frame"
Option Explicit

'objectif : création du fichier frame et recopie des bonnes informations
'au bon endroit

Sub create_frame()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

 ' Suppression de la feuille existante et création de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("Frame").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True
 
 'declaration des variables

Dim NewSheet As Worksheet
Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5, pointeur6, pointeur7 As Long
Dim counteur(100), counteur2(100), i, memory As Integer
Dim VRange_frame_list, VRange_connection, VRange_frame_synthese, VRange_frame, VRange_ident, VRange_periode, VRange_size, VRange_conso, VRange_calc As Range

'initialisation du compteur de connecteur

For i = 0 To 100
    counteur(i) = 0
    counteur2(i) = 0
Next i

'creation de la range du tableau frame synthesis

ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Activate
ActiveSheet.Cells(6, 1).Activate
Set VRange_frame_synthese = ActiveCell.CurrentRegion

'creation des ranges des frames puis identificateurs puis periodes puis size puis conso et calc...

Set VRange_frame_list = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Range(Cells(2, 1), Cells(VRange_frame_synthese.Rows.Count, 1))

Set VRange_ident = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Range(Cells(2, 2), Cells(VRange_frame_synthese.Rows.Count, 2))

Set VRange_periode = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Range(Cells(2, 6), Cells(VRange_frame_synthese.Rows.Count, 6))

Set VRange_size = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Range(Cells(2, 4), Cells(VRange_frame_synthese.Rows.Count, 4))

Set VRange_conso = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Range(Cells(2, 10), Cells(VRange_frame_synthese.Rows.Count, VRange_frame_synthese.Columns.Count))

Set VRange_calc = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Range(Cells(1, 10), Cells(1, VRange_frame_synthese.Columns.Count))

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "Frame"
Sheets("Frame").Move Before:=Sheets(1)
ThisWorkbook.Worksheets("Frame").Tab.ColorIndex = 3

'ecriture des elements fixes

ThisWorkbook.Worksheets("Frame").Cells(1, 1).Value = "Frame Name"
ThisWorkbook.Worksheets("Frame").Cells(1, 2).Value = "Identifier (hex)"
ThisWorkbook.Worksheets("Frame").Cells(1, 3).Value = "Period (ms)"
ThisWorkbook.Worksheets("Frame").Cells(1, 4).Value = "Size (Bytes)"
ThisWorkbook.Worksheets("Frame").Cells(1, 5).Value = "Frame Load (%)"

'remplissage ligne ECU

VRange_calc.Copy Destination:=ThisWorkbook.Worksheets("Frame").Cells(1, 6)

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'creation de la range de connection

ThisWorkbook.Worksheets("Connection").Activate
ActiveSheet.Cells(3, 2).Activate
Set VRange_connection = ActiveCell.CurrentRegion

'remplissage ligne network et nombre connecteurs

ThisWorkbook.Worksheets("Frame").Cells(2, 1).Value = "Total Connector number"
ThisWorkbook.Worksheets("Frame").Cells(3, 1).Value = "FD Connector number"
ThisWorkbook.Worksheets("Frame").Cells(4, 1).Value = "Nb Max Frames Bridge"

'balayage des calculateurs de frame
For pointeur2 = 6 To VRange_frame.Columns.Count
    'balayage des calculateurs de connection
    For pointeur3 = 3 To VRange_connection.Rows.Count + 3
        'si les calculateurs sont les mêmes...
        If (ThisWorkbook.Worksheets("Frame").Cells(1, pointeur2).Value = ThisWorkbook.Worksheets("Connection").Cells(pointeur3, 2).Value) Then
            'alors on balaye les colonnes de connection pour...
            For pointeur4 = 3 To VRange_connection.Columns.Count
                'pour compter le nombre de connecteur (si il y a une "x il y a un connetceur)
                If (ThisWorkbook.Worksheets("Connection").Cells(pointeur3, pointeur4).Value = "x") Then
                    counteur(pointeur2 - 6) = counteur(pointeur2 - 6) + 1
                    'si le réseau en question est CAN-FD alors on compte...
                    If (Right(ThisWorkbook.Worksheets("Connection").Cells(2, pointeur4).Value, 2) = "FD") Then
                        'un connecteur FD supplémentaire
                        counteur2(pointeur2 - 6) = counteur2(pointeur2 - 6) + 1
                    End If
                End If
            Next pointeur4
        End If
    Next pointeur3
    'on écrit le nb de connecteurs
    ThisWorkbook.Worksheets("Frame").Cells(2, pointeur2).Value = counteur(pointeur2 - 6)
    ThisWorkbook.Worksheets("Frame").Cells(3, pointeur2).Value = counteur2(pointeur2 - 6)
    ThisWorkbook.Worksheets("Frame").Cells(4, pointeur2).Value = 1000
Next pointeur2

ThisWorkbook.Worksheets("Frame").Cells(1, VRange_frame.Columns.Count + 2).Value = "fixed routing"
memory = VRange_frame.Columns.Count + 2

'remplissage des network

For pointeur5 = 3 To VRange_connection.Columns.Count
    ThisWorkbook.Worksheets("Frame").Cells(1, pointeur5 + VRange_frame.Columns.Count).Value = ThisWorkbook.Worksheets("Connection").Cells(2, pointeur5)
    If (Right(ThisWorkbook.Worksheets("Frame").Cells(1, pointeur5 + VRange_frame.Columns.Count).Value, 2) = "FD") Then
        ThisWorkbook.Worksheets("Frame").Cells(2, pointeur5 + VRange_frame.Columns.Count).Value = "1"
        ThisWorkbook.Worksheets("Frame").Cells(4, pointeur5 + VRange_frame.Columns.Count).Value = "2000000"
        ThisWorkbook.Worksheets("Frame").Cells(3, pointeur5 + VRange_frame.Columns.Count).Value = "0.7"
    Else: ThisWorkbook.Worksheets("Frame").Cells(2, pointeur5 + VRange_frame.Columns.Count).Value = "0"
          ThisWorkbook.Worksheets("Frame").Cells(4, pointeur5 + VRange_frame.Columns.Count).Value = "500000"
          ThisWorkbook.Worksheets("Frame").Cells(3, pointeur5 + VRange_frame.Columns.Count).Value = "1"
    End If
Next pointeur5

'copie des trames et identificateurs puis periodes puis conso

VRange_frame_list.Copy Destination:=ThisWorkbook.Worksheets("Frame").Cells(5, 1)

VRange_ident.Copy Destination:=ThisWorkbook.Worksheets("Frame").Cells(5, 2)

VRange_periode.Copy Destination:=ThisWorkbook.Worksheets("Frame").Cells(5, 3)

VRange_size.Copy Destination:=ThisWorkbook.Worksheets("Frame").Cells(5, 4)

VRange_conso.Copy Destination:=ThisWorkbook.Worksheets("Frame").Cells(5, 6)


'---

'maj de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

' recopie des émetteur et consommateur des container FD

'balayage des lignes de frame (ligne étudiée)
For pointeur1 = 5 To VRange_frame.Rows.Count
    DoEvents
    'rebalayage ... (comparaison avec les autres lignes)
    For pointeur2 = 5 To VRange_frame.Rows.Count
        'si on trouve le container FD de la trame étudiée alors...
        If (Left(ThisWorkbook.Worksheets("Frame").Cells(pointeur2, 1).Value, Len(ThisWorkbook.Worksheets("Frame").Cells(pointeur2, 1).Value)) = (Left(ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Value, Len(ThisWorkbook.Worksheets("Frame").Cells(pointeur2, 1).Value)) & "_FD")) Then
            'on balaye les colonnes de frame
            For pointeur3 = 6 To VRange_frame.Columns.Count
                'si le container émet ou reçoit la trame alors on recopie les émetteurs et consommateurs
                If (ThisWorkbook.Worksheets("Frame").Cells(pointeur2, pointeur3).Value = "R") Then
                    ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur3).Value = ThisWorkbook.Worksheets("Frame").Cells(pointeur2, pointeur3).Value
                ElseIf (ThisWorkbook.Worksheets("Frame").Cells(pointeur2, pointeur3).Value = "T") Then
                    ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur3).Value = ThisWorkbook.Worksheets("Frame").Cells(pointeur2, pointeur3).Value
                End If
            Next pointeur3
        End If
    Next pointeur2
Next pointeur1

'---

'élimination des lignes sans periode et des trames container FD

For pointeur6 = 3 To VRange_frame_synthese.Rows.Count + 6 - 1
    If (ThisWorkbook.Worksheets("Frame").Cells(pointeur6, 3).Value = "-") Then
        ThisWorkbook.Worksheets("Frame").Cells(pointeur6, 3).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Frame").Cells(pointeur6, 3).Value = "0") Then
        ThisWorkbook.Worksheets("Frame").Cells(pointeur6, 3).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (UCase(Right(ThisWorkbook.Worksheets("Frame").Cells(pointeur6, 1).Value, 2)) = "FD") Then
        ThisWorkbook.Worksheets("Frame").Cells(pointeur6, 3).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    End If
Next pointeur6

'modification de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'remplissage de la charge & fixed routing = 0

For pointeur1 = 5 To VRange_frame.Rows.Count
        ThisWorkbook.Worksheets("Frame").Cells(pointeur1, memory).Value = 0
        ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 5).Value = (((34 + 8 * ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 4).Value) * 1.1 + 13) * (1 / (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 3).Value * 10 ^ -3))) / 500000
Next pointeur1

'suppression des bridges

For pointeur7 = 1 To VRange_frame.Columns.Count
    If (Right(UCase(ThisWorkbook.Worksheets("Frame").Cells(1, pointeur7).Value), 6) = "BRIDGE") Then
        ThisWorkbook.Worksheets("Frame").Cells(1, pointeur7).EntireColumn.Delete
    End If
Next pointeur7

'clear format

Worksheets("Frame").Range(Cells(1, 1), Cells(500, 100)).ClearFormats

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub
