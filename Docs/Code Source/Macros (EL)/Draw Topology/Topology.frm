VERSION 5.00
Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} Topology 
   Caption         =   "Topology"
   ClientHeight    =   1080
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5520
   OleObjectBlob   =   "Topology.frx":0000
   StartUpPosition =   1  'CenterOwner
End
Attribute VB_Name = "Topology"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Choice_Click()

'désactivation
Application.DisplayAlerts = False
    
'on va chercher le fichier
fd = Application.GetOpenFilename("Excel Files (*.csv), *.csv")

'déclaration des variables
Dim wkA As Workbook, wkB As Workbook
Dim chemin As String, fichier As String
 
'classeur A qui contient la macro
Set wkA = ThisWorkbook
 
'ouvre le fichier B
Workbooks.Open fd

'met en variable le classeur B
Set wkB = ActiveWorkbook

'range de la topologie
wkB.Worksheets(1).Activate
ActiveSheet.Cells(1, 1).Activate
Set VRange_topology = ActiveCell.CurrentRegion
 
'copie la feuille "feuil1" du classeur B avant la feuille 1 dans le classeur A
wkB.Sheets(1).Select
Sheets(1).Copy After:=Workbooks("Draw_Topology.xlsm").Sheets(1)

'on supprime la feuille initiale
wkA.Worksheets(1).Activate
wkA.Sheets(1).Delete

'convertisseur csv --> xlsm

     For i = 1 To VRange_topology.Rows.count
         'reinitialisation de notre variable intermédiaire
         Chaine = ""
         count = 0
         
         'balayage de tous les caracères d'une cellule
         For Caractere = 1 To Len(ThisWorkbook.Worksheets(1).Cells(i, 1))
            '!= ; & != X ==> on stock
            If ((Mid(ThisWorkbook.Worksheets(1).Cells(i, 1), Caractere, 1) <> ";") And (Mid(ThisWorkbook.Worksheets(1).Cells(i, 1), Caractere, 1) <> "X")) Then
                Chaine = Chaine & Mid(ThisWorkbook.Worksheets(1).Cells(i, 1), Caractere, 1)
            End If
             'si on a un ; alors un saute une colonne
             If (Mid(ThisWorkbook.Worksheets(1).Cells(i, 1), Caractere, 1) = ";") Then
                count = count + 1
            'si ona un X alros on le nothifie sur son réseau
            ElseIf (Mid(ThisWorkbook.Worksheets(1).Cells(i, 1), Caractere, 1) = "X") Then
                'Count = Count + 1
                ThisWorkbook.Worksheets(1).Cells(i, count + 1) = "X"
             End If
             
          Next Caractere
         
         'recopie dans une autre colonne
         ThisWorkbook.Worksheets(1).Cells(i, 1).Formula = "'" & Chaine
         
    Next i

wkB.Close False 'ferme et enregistre le classeur B

Application.DisplayAlerts = True

Call Drawing.draw_topology

End Sub



Private Sub Export_Click()

Application.DisplayAlerts = False

'declaration des variables
Dim LaDate As String, LeParcours As String, LeRep As String

'initialisation desvariables
LaDate = Format(Date, "yyyymmdd")
LeParcours = Range("A1").Value
ThisWorkbook.Worksheets("Topology Draw").Activate

Set objShell = CreateObject("Shell.Application")
Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
Set oFolderItem = objFolder.Items.Item
On Error Resume Next
LeRep = oFolderItem.Path & "\"
On Error GoTo 0

ActiveSheet.ExportAsFixedFormat Type:=xlTypePDF, Filename:= _
    LeRep & "Topologie" & ".pdf", Quality:= _
    xlQualityStandard, IncludeDocProperties:=True, IgnorePrintAreas:=False, _
    From:=1, to:=1, OpenAfterPublish:=False
    
'suppression de la feuille
ThisWorkbook.Worksheets(1).Delete

Application.DisplayAlerts = True

End Sub

Private Sub Leave_Click()

'désactivation

Application.Calculation = xlCalculationManual
Application.ScreenUpdating = False
Application.EnableEvents = False
Application.DisplayStatusBar = False
ActiveSheet.DisplayPageBreaks = False
Application.DisplayAlerts = False
On Error Resume Next


    'Dim Classeur As Workbook
    'For Each Classeur In Workbooks
    '    If Classeur.Name <> ThisWorkbook.Name Then
    '        Classeur.Close SaveChanges:=False
    '    End If
    'Next Classeur
    'ThisWorkbook.Close False
    Application.Quit
    
On Error GoTo 0

End Sub

Private Sub RefEdit1_BeforeDragOver(Cancel As Boolean, ByVal Data As MSForms.DataObject, ByVal x As stdole.OLE_XPOS_CONTAINER, ByVal y As stdole.OLE_YPOS_CONTAINER, ByVal DragState As MSForms.fmDragState, Effect As MSForms.fmDropEffect, ByVal Shift As Integer)

End Sub

Private Sub UserForm_Click()

End Sub
