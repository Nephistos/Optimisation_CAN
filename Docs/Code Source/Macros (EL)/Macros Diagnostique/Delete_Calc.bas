Attribute VB_Name = "Delete_Calc"
Option Explicit

Sub test7()

'declaration des variables

Dim VRange_frame, VRange_mat_rout, VRange_topology, VRange_user_topology As Range
Dim pointeur0, pointeur1, pointeur2, pointeur3, pointeur4 As Long
Dim count, max, sel_row, sel_col As Integer
Dim sel_val As String

count = 0
max = 0

sel_col = Selection(1).Column
sel_row = Selection(1).Row
sel_val = Selection(1).Value

'creation de la range de frame

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

'creation de la range de mat rout

Workbooks("Mat Rout.csv").Worksheets("Mat Rout").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_mat_rout = ActiveCell.CurrentRegion

'creation de la range de topology

Workbooks("Topology.csv").Worksheets("Topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_topology = ActiveCell.CurrentRegion

'creation de la range de user topology

Workbooks("User Topology.csv").Worksheets("User Topology").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_user_topology = ActiveCell.CurrentRegion

'traitement

'on balaye les lignes de frame
For pointeur1 = 5 To VRange_frame.Rows.count
    'si on croise des T on les compte
    If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, sel_col) = "T") Then
        count = count + 1
        'en sauvegardant tjrs la plus haute valeur
        If (max < count) Then
            max = count
        End If
    'sinon on reinitialise
    Else
        count = 0
    End If
Next pointeur1

'on répète autant de fois qu'il y a de trames consécutives T
For pointeur0 = 1 To max
    'on balaye les lignes de frame
    For pointeur1 = 5 To VRange_frame.Rows.count
        'si on trouve un T alors...
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur1, sel_col) = "T") Then
            'on balaye les lignes de mat rout
            For pointeur2 = 2 To VRange_mat_rout.Rows.count
                'si on trouve la même trame alors...
                If (Workbooks("Mat Rout.csv").Worksheets("Mat Rout").Cells(pointeur2, 1).Value = ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Value) Then
                    'on la supprime dans mat rout en ligne et en colonne puis on sort
                    Workbooks("Mat Rout.csv").Worksheets("Mat Rout").Cells(pointeur2, 1).EntireRow.Delete
                    Workbooks("Mat Rout.csv").Worksheets("Mat Rout").Cells(1, pointeur2).EntireColumn.Delete
                    ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).EntireRow.Delete
                    Exit For
                End If
            Next pointeur2
        End If
    Next pointeur1
Next pointeur0
    
'on balaye les lignes de topology
For pointeur3 = 1 To VRange_topology.Rows.count
    'si on trouve le calc selectionné alors...
    If (Workbooks("Topology.csv").Worksheets("Topology").Cells(pointeur3, 1).Value = sel_val) Then
        'on le supprime puis on sort
        Workbooks("Topology.csv").Worksheets("Topology").Cells(pointeur3, 1).EntireRow.Delete
        Exit For
    End If
Next pointeur3

'on balaye les lignes de user topology
For pointeur4 = 2 To VRange_user_topology.Rows.count
    'si on trouve le calc selectionne alors...
    If (Workbooks("User Topology.csv").Worksheets("User Topology").Cells(pointeur4, 1).Value = sel_val) Then
        'on l supprime en ligne et en colonne puis on sort
        Workbooks("User Topology.csv").Worksheets("User Topology").Cells(pointeur4, 1).EntireRow.Delete
        Workbooks("User Topology.csv").Worksheets("User Topology").Cells(1, pointeur4).EntireColumn.Delete
        Exit For
    End If
Next pointeur4

'on finit par supprimer la colonne du calc selectionné dans frame
ThisWorkbook.Worksheets("Frame").Cells(1, sel_col).EntireColumn.Delete

End Sub
