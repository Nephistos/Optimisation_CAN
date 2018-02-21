Attribute VB_Name = "Param"
Option Explicit

'objectif : création du fichier param et recopie des bonnes informations
'au bon endroit

Sub create_param()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

 ' Suppression de la feuille existante et création de la nouvelle
 Application.DisplayAlerts = False
 On Error Resume Next
 ThisWorkbook.Worksheets("Param").Delete
 On Error GoTo 0
 Application.DisplayAlerts = True
 
 'declaration des variables

Dim NewSheet As Worksheet
Dim pointeur1, pointeur2, pointeur3, pointeur4, pointeur5, pointeur6, pointeur7, pointeur8 As Long
Dim counteur(100), i As Integer
Dim pointeur10, pointeur11, pointeur12, pointeur13, pointeur14, pointeur15 As Long
Dim VRange_frame_synthesis, VRange_connection, VRange_message_set, VRange_frame, VRange_param, VRange_size, VRange_calc, VRange_consum, VRange_network_path, VRange_param2 As Range
Dim Caractere, Chaine As Variant

'initialisation du compteur de connecteur

For i = 0 To 100
    counteur(i) = 0
Next i

'creation de la range de frame synthesis

ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame_synthesis = ActiveCell.CurrentRegion

'creation de la range du tableau network path

ThisWorkbook.Worksheets("Network Path").Activate
ActiveSheet.Cells(2, 1).Activate
Set VRange_network_path = ActiveCell.CurrentRegion

'creation de la range de connection

ThisWorkbook.Worksheets("Connection").Activate
ActiveSheet.Cells(3, 2).Activate
Set VRange_connection = ActiveCell.CurrentRegion

'creation de la range du tableau can message set

ThisWorkbook.Worksheets("Message List (FD+HS) all CAN").Activate
ActiveSheet.Cells(2, 1).Activate
Set VRange_message_set = ActiveCell.CurrentRegion

'creation des ranges des identificateurs puis size puis conso et calc

Set VRange_frame = ThisWorkbook.Worksheets("Message List (FD+HS) all CAN").Range(Cells(2, 2), Cells(VRange_message_set.Rows.Count, 2))

Set VRange_param = ThisWorkbook.Worksheets("Message List (FD+HS) all CAN").Range(Cells(2, 1), Cells(VRange_message_set.Rows.Count, 1))

Set VRange_size = ThisWorkbook.Worksheets("Message List (FD+HS) all CAN").Range(Cells(2, 16), Cells(VRange_message_set.Rows.Count, 16))

Set VRange_calc = ThisWorkbook.Worksheets("Message List (FD+HS) all CAN").Range(Cells(1, 25), Cells(1, VRange_message_set.Columns.Count))

Set VRange_consum = ThisWorkbook.Worksheets("Message List (FD+HS) all CAN").Range(Cells(2, 25), Cells(VRange_message_set.Rows.Count, VRange_message_set.Columns.Count))

'creation de la nouvelle feuille

Set NewSheet = ThisWorkbook.Worksheets.Add
NewSheet.Name = "Param"
Sheets("Param").Move Before:=Sheets(2)
ThisWorkbook.Worksheets("Param").Tab.ColorIndex = 3


'ecriture des elements fixes

ThisWorkbook.Worksheets("Param").Cells(1, 1).Value = "Source"
ThisWorkbook.Worksheets("Param").Cells(1, 2).Value = "Frame"
ThisWorkbook.Worksheets("Param").Cells(1, 3).Value = "Fix Frame"
ThisWorkbook.Worksheets("Param").Cells(1, 4).Value = "Frame Period"
ThisWorkbook.Worksheets("Param").Cells(1, 5).Value = "Parameter"
ThisWorkbook.Worksheets("Param").Cells(1, 6).Value = "Size (bits)"
ThisWorkbook.Worksheets("Param").Cells(1, 7).Value = "Charge (bps)"
ThisWorkbook.Worksheets("Param").Cells(1, 8).Value = "Fix param in frame"


'remplissage ligne ECU

VRange_calc.Copy Destination:=ThisWorkbook.Worksheets("Param").Cells(1, 9)

'creation de la range de param

ThisWorkbook.Worksheets("Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_param2 = ActiveCell.CurrentRegion

'remplissage ligne network et nombre connecteurs

ThisWorkbook.Worksheets("Param").Cells(2, 1).Value = "Connector number"

'balayage des calclateurs de param
For pointeur2 = 9 To VRange_param2.Columns.Count
    DoEvents
    'balayage des calculateurs de connection
    For pointeur3 = 3 To VRange_connection.Rows.Count
        'si on a le même calculateurs dans param et connection alors...
        If (ThisWorkbook.Worksheets("Param").Cells(1, pointeur2).Value = ThisWorkbook.Worksheets("Connection").Cells(pointeur3, 2).Value) Then
            'on balaye les colonnes de connection pour...
            For pointeur4 = 3 To VRange_connection.Columns.Count
                'compter les connecteurs (si il y a un "x" il y a un connecteur)
                If (ThisWorkbook.Worksheets("Connection").Cells(pointeur3, pointeur4).Value = "x") Then
                    counteur(pointeur2 - 9) = counteur(pointeur2 - 9) + 1
                End If
            Next pointeur4
        End If
    Next pointeur3
    ThisWorkbook.Worksheets("Param").Cells(2, pointeur2).Value = counteur(pointeur2 - 9)
Next pointeur2

'copie des trames et periodes et taille source charge

VRange_frame.Copy Destination:=ThisWorkbook.Worksheets("Param").Cells(3, 2)

'--- trouver la périodes des trames

Dim verrouillage As Boolean
verrouillage = False

For pointeur2 = 3 To VRange_message_set.Rows.Count + 3
    DoEvents
    For pointeur3 = 2 To VRange_frame_synthesis.Rows.Count
        If (UCase(ThisWorkbook.Worksheets("Param").Cells(pointeur2, 2).Value) <> UCase(ThisWorkbook.Worksheets("Param").Cells(pointeur2 - 1, 2).Value)) Then
            verrouillage = False
        End If
        If (verrouillage = True) Then
            ThisWorkbook.Worksheets("Param").Cells(pointeur2, 4).Value = ThisWorkbook.Worksheets("Param").Cells(pointeur2 - 1, 4).Value
            Exit For
        End If
        If ((UCase(ThisWorkbook.Worksheets("Param").Cells(pointeur2, 2).Value) = UCase(ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Cells(pointeur3, 1).Value)) And (verrouillage = False)) Then
            ThisWorkbook.Worksheets("Param").Cells(pointeur2, 4).Value = ThisWorkbook.Worksheets("Frame Synthesis (FD+HS) all CAN").Cells(pointeur3, 6).Value
            verrouillage = True
            Exit For
        End If
    Next pointeur3
Next pointeur2

'---

VRange_param.Copy Destination:=ThisWorkbook.Worksheets("Param").Cells(3, 5)

VRange_size.Copy Destination:=ThisWorkbook.Worksheets("Param").Cells(3, 6)

VRange_consum.Copy Destination:=ThisWorkbook.Worksheets("Param").Cells(3, 9)

'élimination des lignes sans periode

'elimination des lignes avec update time (de la frame) "-" et des trames container FD

For pointeur6 = 3 To VRange_message_set.Rows.Count
    DoEvents
    If (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 4).Value = "-") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 4).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 4).Value = "0") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 4).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "**** fixed to zero ****") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "Reserved1bit") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "Reserved_1bit") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "Reserved_2bits") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "Reserved_3bits") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "Reserved_4bits") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).Value = "Reserved_5bits") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    ElseIf (UCase(Right(ThisWorkbook.Worksheets("Param").Cells(pointeur6, 2).Value, 2)) = "FD") Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur6, 5).EntireRow.Delete
        pointeur6 = pointeur6 - 1
    End If
Next pointeur6

'remplissage colonne charge

For pointeur1 = 3 To VRange_message_set.Rows.Count
     If (ThisWorkbook.Worksheets("Param").Cells(pointeur1, 4).Value <> 0) Then
        ThisWorkbook.Worksheets("Param").Cells(pointeur1, 7).Value = (ThisWorkbook.Worksheets("Param").Cells(pointeur1, 6).Value * (1 / (ThisWorkbook.Worksheets("Param").Cells(pointeur1, 4).Value * 10 ^ -3)))
     End If
Next pointeur1

'modification de la range de param

ThisWorkbook.Worksheets("Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_param2 = ActiveCell.CurrentRegion

'remplissage colonne source

Dim res_ok As Boolean
Dim mem_beg, mem_end As Integer
Dim Caractere2 As Integer, Chaine_beg, Chaine_voisin_end, Chaine_voisin_beg As Variant

'---

'balayage des lignes de param
For pointeur10 = 3 To VRange_message_set.Rows.Count + 1 'on peut utiliser la range de message set car elle a plus de lignes (cf suppresion au dessus)
    DoEvents
    'si on a la même frame que précédemment alors...
    If (ThisWorkbook.Worksheets("Param").Cells(pointeur10, 2).Value = ThisWorkbook.Worksheets("Param").Cells(pointeur10 - 1, 2).Value) Then
        'on recopie la source
        ThisWorkbook.Worksheets("Param").Cells(pointeur10, 1).Value = ThisWorkbook.Worksheets("Param").Cells(pointeur10 - 1, 1).Value
    Else
        'sinon on balaye les lignes de network path
        For pointeur11 = 2 To VRange_network_path.Rows.Count
            'si le nom de trame est le même dans param et network path alors on copie le transmitter
            If ((ThisWorkbook.Worksheets("Param").Cells(pointeur10, 2).Value = ThisWorkbook.Worksheets("Network Path").Cells(pointeur11, 1).Value) And (ThisWorkbook.Worksheets("Param").Cells(pointeur10, 1).Value = "")) Then
                'si on détecte un bridge alors...
                If (UCase(Right(ThisWorkbook.Worksheets("Network Path").Cells(pointeur11, 3).Value, 6)) = "BRIDGE") Then
                    'on ne garde que la partie calculateur
                    ThisWorkbook.Worksheets("Param").Cells(pointeur10, 1).Value = Left(ThisWorkbook.Worksheets("Network Path").Cells(pointeur11, 3).Value, Len(ThisWorkbook.Worksheets("Network Path").Cells(pointeur11, 2).Value) - 7)
                    Exit For
                Else
                    ThisWorkbook.Worksheets("Param").Cells(pointeur10, 1).Value = ThisWorkbook.Worksheets("Network Path").Cells(pointeur11, 3).Value
                End If
            End If
        Next pointeur11
    End If
Next pointeur10

'---

'rebalayge de param pour trouver les sources vierges
For pointeur14 = 3 To VRange_param2.Rows.Count + 1

    DoEvents
    'reinitialisation de notre variable intermédiaire
    Chaine_beg = ""
    Chaine_voisin_beg = ""
    Chaine_voisin_end = ""
    
    'si on trouve une source vierge alors...
    If (ThisWorkbook.Worksheets("Param").Cells(pointeur14, 1).Value = "") Then
        'on note ou elle commence
        mem_beg = pointeur14
        'on cherche ou elle se termine
        For pointeur13 = mem_beg To VRange_param2.Rows.Count + 1
            'si on trouve finalement une trame différente alors...
            If (ThisWorkbook.Worksheets("Param").Cells(pointeur13 + 1, 2).Value <> ThisWorkbook.Worksheets("Param").Cells(pointeur13, 2).Value) Then
                'on note ou la trame c'est terminée
                mem_end = pointeur13 - 1
                Exit For
            End If
        Next pointeur13
        
        'on regarde les trames voisines (début et fin de notre trame)
        
         'balayage de tous les caracères d'une cellule
         For Caractere2 = 1 To Len(ThisWorkbook.Worksheets("Param").Cells(mem_beg, 2))
             'si detection des "_"
             If (Mid(ThisWorkbook.Worksheets("Param").Cells(mem_beg, 2), Caractere2, 1) <> "_") Then
                'stockage des numeriques dans la variable intermediaire
                Chaine_beg = Chaine_beg & Mid(ThisWorkbook.Worksheets("Param").Cells(mem_beg, 2), Caractere2, 1)
             'sinon (= "_") on met notre compteur de caractère à la taille maximale de la cellule
             Else: Caractere2 = Len(ThisWorkbook.Worksheets("Param").Cells(mem_beg, 2))
             End If
         Next Caractere2
         
         'balayage de tous les caracères d'une cellule
         For Caractere2 = 1 To Len(ThisWorkbook.Worksheets("Param").Cells(mem_beg - 2, 2))
             'si detection des "_"
             If (Mid(ThisWorkbook.Worksheets("Param").Cells(mem_beg - 2, 2), Caractere2, 1) <> "_") Then
                'stockage des numeriques dans la variable intermediaire
                Chaine_voisin_beg = Chaine_voisin_beg & Mid(ThisWorkbook.Worksheets("Param").Cells(mem_beg - 2, 2), Caractere2, 1)
             'sinon (= "_") on met notre compteur de caractère à la taille maximale de la cellule
             Else: Caractere2 = Len(ThisWorkbook.Worksheets("Param").Cells(mem_beg - 2, 2))
             End If
         Next Caractere2
         
         'balayage de tous les caracères d'une cellule
         For Caractere2 = 1 To Len(ThisWorkbook.Worksheets("Param").Cells(mem_end + 2, 2))
             'si detection des "_"
             If (Mid(ThisWorkbook.Worksheets("Param").Cells(mem_end + 2, 2), Caractere2, 1) <> "_") Then
                'stockage des numeriques dans la variable intermediaire
                Chaine_voisin_end = Chaine_voisin_end & Mid(ThisWorkbook.Worksheets("Param").Cells(mem_end + 2, 2), Caractere2, 1)
             'sinon (= "_") on met notre compteur de caractère à la taille maximale de la cellule
             Else: Caractere2 = Len(ThisWorkbook.Worksheets("Param").Cells(mem_end + 2, 2))
             End If
         Next Caractere2
         
         'on compare les trames voisines à la notre
         
         'si on la trame voisine début est la même alors ...
         If (Chaine_beg = Chaine_voisin_beg) Then
            'nos trames prennent la même source
            For pointeur15 = mem_beg To mem_end + 1
                ThisWorkbook.Worksheets("Param").Cells(pointeur15, 1).Value = ThisWorkbook.Worksheets("Param").Cells(mem_beg - 2, 1).Value
                'MsgBox pointeur15 & " - " & ThisWorkbook.Worksheets("Param").Cells(mem_beg - 2, 1).Value & " - " & mem_beg - 2
            Next pointeur15
        'si on la trame voisine fin est la même alors ...
        ElseIf (Chaine_beg = Chaine_voisin_end) Then
            'nos trames prennent la même source
            For pointeur15 = mem_beg To mem_end + 1
                ThisWorkbook.Worksheets("Param").Cells(pointeur15, 1).Value = ThisWorkbook.Worksheets("Param").Cells(mem_end + 2, 1).Value
                'MsgBox pointeur15 & " - " & Chaine_beg & " - " & ThisWorkbook.Worksheets("Param").Cells(mem_end + 2, 1).Value & " - " & mem_end + 2 & " - " & Chaine_voisin_end
            Next pointeur15
        'sinon on garde le début de la chaine de la trame (qui souvent représente la source)
        Else:
            For pointeur15 = mem_beg To mem_end + 1
                ThisWorkbook.Worksheets("Param").Cells(pointeur15, 1).Formula = "'" & Chaine_beg
                'MsgBox pointeur15 & " - " & Chaine_beg
            Next pointeur15
        End If
    End If
Next pointeur14

'suppression des sources erronnées (messagerie)
Call Param_Transmitter.test3
            
'modification de la range de param

ThisWorkbook.Worksheets("Param").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_param2 = ActiveCell.CurrentRegion

'fix frame & fix param in = 0

For pointeur1 = 3 To VRange_param2.Rows.Count
    ThisWorkbook.Worksheets("Param").Cells(pointeur1, 3).Value = "0"
    ThisWorkbook.Worksheets("Param").Cells(pointeur1, 8).Value = "0"
Next pointeur1

'Passage des "," au "." pour la charge des paramètres

For pointeur7 = 3 To VRange_param2.Rows.Count
    DoEvents
    'reinitialisation de notre variable intermédiaire
    Chaine = ""
    For Caractere = 1 To Len(ThisWorkbook.Worksheets("Param").Cells(pointeur7, 7))
        'si detection d'une virgule alors...
        If (Mid(ThisWorkbook.Worksheets("Param").Cells(pointeur7, 7), Caractere, 1) <> ",") Then
            'stockage des numeriques dans la variable intermediaire
            Chaine = Chaine & Mid(ThisWorkbook.Worksheets("Param").Cells(pointeur7, 7), Caractere, 1)
        'sinon si on a une virgule alors...
        ElseIf (Mid(ThisWorkbook.Worksheets("Param").Cells(pointeur7, 7), Caractere, 1) = ",") Then
            Chaine = Chaine & "."
        End If
    Next Caractere
    'recopie à la place de...
    ThisWorkbook.Worksheets("Param").Cells(pointeur7, 7).Formula = "'" & Chaine
Next pointeur7

'suppression des bridges

For pointeur7 = 9 To VRange_param2.Columns.Count
    If (UCase(Right(ThisWorkbook.Worksheets("Param").Cells(1, pointeur7).Value, 7)) = "_BRIDGE") Then
        'MsgBox ThisWorkbook.Worksheets("Param").Cells(1, pointeur7).Value
        ThisWorkbook.Worksheets("Param").Cells(1, pointeur7).EntireColumn.Delete
    End If
Next pointeur7

'clear format

Worksheets("Param").Range(Cells(1, 1), Cells(VRange_param2.Rows.Count, VRange_param2.Columns.Count)).ClearFormats
ThisWorkbook.Worksheets("Param").Cells(VRange_param2.Rows.Count + 1, 2).Value = "InsertBefore1"
ThisWorkbook.Worksheets("Param").Cells(VRange_param2.Rows.Count + 1, 5).Value = "InsertBefore1"
ThisWorkbook.Worksheets("Param").Cells(VRange_param2.Rows.Count + 2, 2).Value = "InsertBefore2"
ThisWorkbook.Worksheets("Param").Cells(VRange_param2.Rows.Count + 2, 5).Value = "InsertBefore2"

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True


End Sub

