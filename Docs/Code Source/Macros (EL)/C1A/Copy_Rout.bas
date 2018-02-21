Attribute VB_Name = "Copy_Rout"

'objectif : remettre en forme le format du routage de chaque trame pour qu'il corresponde à
'celui des fichiers d'entrée

Sub copy_rout()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False

'déclaration des variables

Dim VRange_rout_av, VRange_frame, VRange_connection As Range
Dim pointeur0, pointeur1, pointeur2, pointeur3, pointeur4, pointeur5, pointeur6, pointeur7 As Long
Dim reseaux(20), verrouillage As Boolean
Dim current_frame As String
Dim Caractere, memory As Integer, Chaine As Variant

'initialisation des variables

For pointeur0 = 0 To 20
    reseaux(pointeur0) = False
Next pointeur0

'création des ranges puis maj

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

memory = VRange_frame.Columns.Count + 1
ThisWorkbook.Worksheets("Frame").Cells(2, memory).Value = "EXTEND CURRENT REGION"

ThisWorkbook.Worksheets("Network Path").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_rout_av = ActiveCell.CurrentRegion

ThisWorkbook.Worksheets("Frame").Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_frame = ActiveCell.CurrentRegion

ThisWorkbook.Worksheets("Connection").Activate
ActiveSheet.Cells(3, 2).Activate
Set VRange_connection = ActiveCell.CurrentRegion

'Détection du routage et recopie au bon format

'balayage des lignes de frame
For pointeur1 = 5 To VRange_frame.Rows.Count
    DoEvents
    'initialisation du tableau de flag
    For pointeur0 = 0 To VRange_connection.Columns.Count - 1
        reseaux(pointeur0) = False
    Next pointeur0
    'initialisation de la frame actuelle
    current_frame = ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Value
    
    'balayage du routage pré optimisation
    For pointeur2 = 2 To VRange_rout_av.Rows.Count
        'si on tombe sur la current frame alors...
        If (ThisWorkbook.Worksheets("Network Path").Cells(pointeur2, 1).Value = ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 1).Value) Then
            'on se bloque dessus
            verrouillage = True
            'on balaye les réseaux...
            For pointeur3 = 4 To VRange_connection.Columns.Count + 4 - 1
                'si la frame passe sur un réseau alors...
                If (ThisWorkbook.Worksheets("Network Path").Cells(pointeur2, pointeur3).Value = "1") Then
                    'on le note dans un tableau
                    reseaux(pointeur3 - 4) = True
                'idem
                ElseIf (ThisWorkbook.Worksheets("Network Path").Cells(pointeur2, pointeur3).Value = "2") Then
                    reseaux(pointeur3 - 4) = True
                End If
            Next pointeur3
        'si on tombe sur une autre trame alors nous avons fini d'étudier la précédente donc...
        ElseIf (verrouillage = True) Then
            'on peut dévérouiller notre trame pour passer à la suivante
            verrouillage = False
            Exit For
        End If
    Next pointeur2
    
    'balayage des réseaux de frame
    For pointeur4 = VRange_frame.Columns.Count - VRange_connection.Columns.Count + 1 + 2 To VRange_frame.Columns.Count
        'si la trame actuelle passe sur un réseau alors...
        If (reseaux(pointeur4 - VRange_frame.Columns.Count + VRange_connection.Columns.Count - 1 - 2) = True) Then
            'on impute sa charge au réseau
            ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur4).Value = ThisWorkbook.Worksheets("Frame").Cells(pointeur1, 5).Value
        Else:
            ThisWorkbook.Worksheets("Frame").Cells(pointeur1, pointeur4).Value = "0"
        End If
    Next pointeur4
Next pointeur1

'on annule l'extend current region
ThisWorkbook.Worksheets("Frame").Cells(2, memory).Value = ""

'Passage des "," au "." pour la charge du routage

For pointeur5 = VRange_frame.Columns.Count - VRange_connection.Columns.Count + 1 + 2 To VRange_frame.Columns.Count
    DoEvents
    For pointeur6 = 3 To VRange_frame.Rows.Count
        'reinitialisation de notre variable intermédiaire
        Chaine = ""
        'balayage de tous les caracères d'une cellule
        If (ThisWorkbook.Worksheets("Frame").Cells(pointeur6, pointeur5) > 0) Then
            For Caractere = 1 To Len(ThisWorkbook.Worksheets("Frame").Cells(pointeur6, pointeur5))
                'si detection d'une virgule alors...
                If (Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur6, pointeur5), Caractere, 1) <> ",") Then
                    'stockage des numeriques dans la variable intermediaire
                    Chaine = Chaine & Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur6, pointeur5), Caractere, 1)
                'sinon si on a une virgule alors...
                ElseIf (Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur6, pointeur5), Caractere, 1) = ",") Then
                    Chaine = Chaine & "."
                End If
            Next Caractere
        Else: Chaine = "0"
        End If
        'recopie à la place de...
        ThisWorkbook.Worksheets("Frame").Cells(pointeur6, pointeur5).Formula = "'" & Chaine
    Next pointeur6
Next pointeur5

'Passage des "," au "." pour la charge des trames

For pointeur7 = 3 To VRange_frame.Rows.Count
    DoEvents
    'reinitialisation de notre variable intermédiaire
    Chaine = ""
    For Caractere = 1 To Len(ThisWorkbook.Worksheets("Frame").Cells(pointeur7, 5))
        'si detection d'une virgule alors...
        If (Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur7, 5), Caractere, 1) <> ",") Then
            'stockage des numeriques dans la variable intermediaire
            Chaine = Chaine & Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur7, 5), Caractere, 1)
        'sinon si on a une virgule alors...
        ElseIf (Mid(ThisWorkbook.Worksheets("Frame").Cells(pointeur7, 5), Caractere, 1) = ",") Then
            Chaine = Chaine & "."
        End If
    Next Caractere
    'recopie à la place de...
    ThisWorkbook.Worksheets("Frame").Cells(pointeur7, 5).Formula = "'" & Chaine
Next pointeur7

'réactivation
                
Application.Calculation = xlCalculationAutomatic
Application.ScreenUpdating = True
Application.EnableEvents = True
Application.DisplayStatusBar = True
ActiveSheet.DisplayPageBreaks = True


End Sub
