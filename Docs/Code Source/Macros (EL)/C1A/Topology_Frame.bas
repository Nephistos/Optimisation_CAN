Attribute VB_Name = "Topology_Frame"
Option Explicit

'objectif : création de la feuille détaillant la topologie initiale

Sub create_topology()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

 ' Suppression de la feuille existante et création de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("Topology").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True
 
 'declaration ds variables

Dim NewSheet As Worksheet
Dim VRange_topology, VRange_topology2, VRange_connection, VRange_frame As Range
Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5 As Long
Dim is_ok As Boolean

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "Topology"
Sheets("Topology").Move Before:=Sheets(2)
ThisWorkbook.Worksheets("Topology").Tab.ColorIndex = 3

'creation de la range de frame synthese

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'creation de la range de connection

ThisWorkbook.Worksheets("Connection").Activate
ActiveSheet.Cells(3, 2).Activate
Set VRange_connection = ActiveCell.CurrentRegion

'creation de la range de la topologie initiale
   
ThisWorkbook.Worksheets("Connection").Activate
Set VRange_topology = ThisWorkbook.Worksheets("Connection").Range(Cells(3, 2), Cells(VRange_connection.Rows.Count, VRange_connection.Columns.Count))

'on copie le contenue de connection pour le transmettre en tant que topologie initiale

ThisWorkbook.Worksheets("Topology").Activate
VRange_topology.Copy Destination:=ThisWorkbook.Worksheets("Topology").Cells(1, 1)

'creation de la range de topology

ThisWorkbook.Worksheets("Topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_topology2 = ActiveCell.CurrentRegion

'extension de la region

For pointeur3 = 1 To VRange_connection.Columns.Count
    ThisWorkbook.Worksheets("Topology").Cells(VRange_topology2.Rows.Count + 1, pointeur3).Value = "EXTEND CURRENT REGION"
Next pointeur3
    
'maj de la range de topology

ThisWorkbook.Worksheets("Topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_topology2 = ActiveCell.CurrentRegion

'mise en majuscule des "x"

'balayage des lignes de calculateurs
For pointeur1 = 1 To VRange_topology2.Rows.Count
    'balayage des colonnes de subnet
    For pointeur2 = 2 To VRange_topology2.Columns.Count
        'si il y a un "x" alors on le met en majuscule
        If (ThisWorkbook.Worksheets("Topology").Cells(pointeur1, pointeur2).Value = "x") Then
            ThisWorkbook.Worksheets("Topology").Cells(pointeur1, pointeur2).Value = UCase(ThisWorkbook.Worksheets("Topology").Cells(pointeur1, pointeur2))
        End If
    Next pointeur2
Next pointeur1

'clear format

Worksheets("Topology").Range(Cells(1, 1), Cells(VRange_topology2.Rows.Count + 100, VRange_topology2.Columns.Count)).ClearFormats
ThisWorkbook.Worksheets("Topology").Cells(VRange_topology2.Rows.Count, pointeur3).EntireRow.Delete

'suppression des mauvais calculateurs en 2 passages

Dim increment, max As Integer
Dim pointeur6 As Long

'balayage des lignes de topology
For pointeur4 = 1 To VRange_topology2.Rows.Count
    is_ok = False
    'balayage des colonnes de frame synthese
    For pointeur5 = 6 To VRange_frame.Columns.Count
        'si on trouve que les calculateurs étudiés sont identitiques alors...
        If (ThisWorkbook.Worksheets("Topology").Cells(pointeur4, 1).Value = ThisWorkbook.Worksheets("Frame").Cells(1, pointeur5).Value) Then
            'pas de soucis => on passe au suivant
            is_ok = True
            Exit For
        'sinon si on ne retrouve pas le calculateur étudié alors...
        ElseIf ((is_ok = False) And (pointeur5 = VRange_frame.Columns.Count)) Then
            'on supprime la ligne car ce calculateur ne consomme/produit aucune trame
            ThisWorkbook.Worksheets("Topology").Cells(pointeur4, 1).Interior.Color = RGB(255, 0, 0)
            is_ok = True
        End If
    Next pointeur5
    
    If (ThisWorkbook.Worksheets("Topology").Cells(pointeur4, 1).Interior.Color = RGB(255, 0, 0)) Then
        increment = increment + 1
    End If
    If (increment > max) Then
        max = increment
    End If
    If (ThisWorkbook.Worksheets("Topology").Cells(pointeur4, 1).Interior.Color <> RGB(255, 0, 0)) Then
        increment = 0
    End If
    
Next pointeur4

For pointeur6 = 1 To max

'balayage des lignes de user topology
For pointeur4 = 2 To VRange_topology2.Rows.Count
    If (ThisWorkbook.Worksheets("Topology").Cells(pointeur4, 1).Interior.Color = RGB(255, 0, 0)) Then
        ThisWorkbook.Worksheets("Topology").Cells(pointeur4, 1).EntireRow.Delete
    End If
Next pointeur4

Next pointeur6

'suppresion des bridges

For pointeur1 = 1 To VRange_topology2.Rows.Count
    If (UCase(Right(ThisWorkbook.Worksheets("Topology").Cells(pointeur1, 1).Value, 3)) = "BRI") Then
        ThisWorkbook.Worksheets("Topology").Cells(pointeur1, 1).EntireRow.Delete
    End If
Next pointeur1

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub
