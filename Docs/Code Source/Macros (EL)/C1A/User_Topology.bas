Attribute VB_Name = "User_Topology"
Option Explicit

'objectif : création de la feuille de contraintes topologiques

Sub create_user_topology()

Dim tps As Integer

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

 ' Suppression de la feuille existante et création de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("User Topology").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True
 
 'declaration des variables

Dim NewSheet As Worksheet
Dim VRange_user_topology, VRange_user_topology2, VRange_connection, VRange_frame_synthese As Range
Dim pointeur1, pointeur2, pointeur4, pointeur5 As Long
Dim is_ok As Boolean

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "User Topology"
Sheets("User Topology").Move Before:=Sheets(2)
ThisWorkbook.Worksheets("User Topology").Tab.ColorIndex = 3

'création de la range de frame synthese

ThisWorkbook.Worksheets("Frame synthesis").Activate
ActiveSheet.Cells(6, 2).Activate
Set VRange_frame_synthese = ActiveCell.CurrentRegion

'création de la range de connection

ThisWorkbook.Worksheets("Connection").Activate
ActiveSheet.Cells(3, 2).Activate
Set VRange_connection = ActiveCell.CurrentRegion

'creation de la range des calculateurs de connection
   
ThisWorkbook.Worksheets("Connection").Activate
Set VRange_user_topology = ThisWorkbook.Worksheets("Connection").Range(Cells(3, 2), Cells(VRange_connection.Rows.Count, 2))

'copie des calculateurs verticalement dans user topology

ThisWorkbook.Worksheets("User Topology").Activate
VRange_user_topology.Copy Destination:=ThisWorkbook.Worksheets("User Topology").Cells(2, 1)

'suppression des mauvais calculateurs en 2 passages

Dim increment, max As Integer
Dim pointeur6 As Long

'balayage des lignes de user topology
For pointeur4 = 2 To VRange_user_topology.Rows.Count
    is_ok = False
    'balayage des colonnes de frame synthese
    For pointeur5 = 8 To VRange_frame_synthese.Columns.Count
        'si on trouve que les calculateurs étudiés sont identitiques alors...
        If (ThisWorkbook.Worksheets("User Topology").Cells(pointeur4, 1).Value = ThisWorkbook.Worksheets("Frame synthesis").Cells(5, pointeur5).Value) Then
            'pas de soucis => on passe au suivant
            is_ok = True
            Exit For
        'sinon si on ne retrouve pas le calculateur étudié alors...
        ElseIf ((is_ok = False) And (pointeur5 = VRange_frame_synthese.Columns.Count)) Then
            'on supprime la ligne car ce calculateur ne consomme/produit aucune trame
            ThisWorkbook.Worksheets("User Topology").Cells(pointeur4, 1).Interior.Color = RGB(254, 0, 0)
            is_ok = True
        End If
    Next pointeur5
    
    If (ThisWorkbook.Worksheets("User Topology").Cells(pointeur4, 1).Interior.Color = RGB(254, 0, 0)) Then
        increment = increment + 1
    End If
    If (increment > max) Then
        max = increment
    End If
    If (ThisWorkbook.Worksheets("User Topology").Cells(pointeur4, 1).Interior.Color <> RGB(254, 0, 0)) Then
        increment = 0
    End If
    
Next pointeur4

For pointeur6 = 1 To max
    
'balayage des lignes de user topology
For pointeur4 = 2 To VRange_user_topology.Rows.Count
    If (ThisWorkbook.Worksheets("User Topology").Cells(pointeur4, 1).Interior.Color = RGB(254, 0, 0)) Then
        ThisWorkbook.Worksheets("User Topology").Cells(pointeur4, 1).EntireRow.Delete
    End If
Next pointeur4

Next pointeur6

'maj de la range de user topology

ThisWorkbook.Worksheets("User Topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_user_topology2 = ActiveCell.CurrentRegion

'suppresion des bridges verticaux

For pointeur1 = 2 To VRange_user_topology2.Rows.Count
    If (UCase(Right(ThisWorkbook.Worksheets("User Topology").Cells(pointeur1, 1).Value, 3)) = "BRI") Then
        ThisWorkbook.Worksheets("User Topology").Cells(pointeur1, 1).EntireRow.Delete
    End If
Next pointeur1

'copie des calculateurs horizontalement dans user topology

'balayage des lignes de user topologie puis...
For pointeur1 = 2 To VRange_user_topology.Rows.Count
    'recopie des calculateurs
    ThisWorkbook.Worksheets("User Topology").Cells(1, pointeur1).Value = ThisWorkbook.Worksheets("User Topology").Cells(pointeur1, 1)
Next pointeur1

'maj de la range de user topology

ThisWorkbook.Worksheets("User Topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_user_topology2 = ActiveCell.CurrentRegion

'copie des reseaux verticalement dans user topology

For pointeur2 = 3 To VRange_connection.Columns.Count
    ThisWorkbook.Worksheets("User Topology").Cells(pointeur2 + VRange_user_topology2.Rows.Count - 1, 1).Value = ThisWorkbook.Worksheets("Connection").Cells(2, pointeur2)
Next pointeur2

'clear format

Worksheets("User Topology").Range(Cells(1, 1), Cells(VRange_user_topology2.Rows.Count + 100, VRange_user_topology2.Columns.Count)).ClearFormats

'---

'declaration des variables

Dim VRange_user_topology_reseaux As Range
Dim pointeur3 As Long

'creation de la range de user topology reseaux

ThisWorkbook.Worksheets("User Topology").Activate
ActiveSheet.Cells(VRange_user_topology2.Rows.Count + 2, 1).Activate
Set VRange_user_topology_reseaux = ActiveCell.CurrentRegion

'on rempli user topology par des 0
Range(Cells(2, 2), Cells(VRange_user_topology.Rows.Count - 1, VRange_user_topology2.Columns.Count)) = "0"
Range(Cells(VRange_user_topology2.Rows.Count + 2, 2), Cells(VRange_user_topology2.Rows.Count + 2 + VRange_user_topology_reseaux.Rows.Count - 1, VRange_user_topology2.Columns.Count)) = "0"
                              
  'balayage des colonnes de user topology
  For pointeur1 = 2 To VRange_user_topology2.Columns.Count
    'balayage des lignes de connection
    For pointeur2 = 3 To VRange_connection.Rows.Count
        'si on a le même calculateur dans les 2 feuilles alors ...
        If (ThisWorkbook.Worksheets("User Topology").Cells(1, pointeur1).Value = ThisWorkbook.Worksheets("Connection").Cells(pointeur2, 2).Value) Then
            'on balaye les colonnes de connection pour...
            For pointeur3 = 3 To VRange_connection.Columns.Rows.Count
                'trouver les reseaux auquel il est connecté
                If (ThisWorkbook.Worksheets("connection").Cells(pointeur2, pointeur3).Value = "x") Then
                    'alors on le notifie dans user topology afin de conserver la topologie initiale
                    ThisWorkbook.Worksheets("User Topology").Cells(VRange_user_topology2.Rows.Count + 2 + pointeur3 - 3, pointeur1).Value = "1"
                End If
            Next pointeur3
        End If
    Next pointeur2
Next pointeur1
                
'---
                
'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True

End Sub

