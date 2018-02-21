Attribute VB_Name = "Menu"
Public Declare Function GetTickCount& Lib "kernel32" ()

'A mettre au début dans la macro "lourde"
Dim PctDone As Single

'macro qui se déclenche  à l'ouverture du classeur
Sub auto_open()

ProgressBar.Show 0

 Set mabarre = CommandBars.Add(Name:="Créer les fichiers pour l'optimisation", temporary:=True)
 With mabarre
 .Position = 4
 .Left = 100
 .Top = 100
 .Visible = True
 With .Controls.Add(Type:=msoControlPopup)
 .Caption = "Créer les fichiers pour l'optimisation"
 
 With .Controls.Add(Type:=msoControlButton)
 .Caption = "Optimisation sans mise en trame (~10sec)"
 .OnAction = "group_without_met2"
 End With
 
 With .Controls.Add(Type:=msoControlButton)
 .Caption = "Optimisation avec mise en trame (~1min)"
 .OnAction = "group_with_met"
 End With

 End With

 End With
 
 Set mabarre2 = CommandBars.Add(Name:="Corriger les fichiers pour l'optimisation", temporary:=True)
 With mabarre2
 .Position = 4
 .Left = 100
 .Top = 100
 .Visible = True
 With .Controls.Add(Type:=msoControlPopup)
 .Caption = "Corriger les fichiers pour l'optimisation"
 
 With .Controls.Add(Type:=msoControlButton)
 .Caption = "Correction Automatique de Param"
 .OnAction = "group_auto"
 End With
 
With .Controls.Add(Type:=msoControlButton)
 .Caption = "Correction Manuelle de Param"
 .OnAction = "group_manuel"
 End With

 End With

 End With
 
   Set mabarre3 = CommandBars.Add(Name:="Regénérer CAN Message Set", temporary:=True)
 With mabarre3
 .Position = 4
 .Left = 100
 .Top = 100
 .Visible = True
 With .Controls.Add(Type:=msoControlPopup)
 .Caption = "Regénérer CAN Message Set"
 
   With .Controls.Add(Type:=msoControlButton)
 .Caption = "Regénérer CAN Message Set"
 .OnAction = "group_pre_param"
 End With

 End With

 End With

 End Sub
 
 Sub group_with_met()
 
Call group_param
Call User_Topology.create_user_topology
Call Topology.create_topology
 
 End Sub
 
 Sub group_without_met()
 
 'déclaration des variables
 
 Dim chemin As String
Dim objShell As Object, objFolder As Object, oFolderItem As Object
Dim NomClient As String
 
Call group_frame
Call User_Topology_Frame.create_user_topology
Call Topology_Frame.create_topology
Call create_mat_rout
'Call group_without_met_export

End Sub

Sub group_without_met2()

Call group_without_met
Call group_without_met_export

End Sub

Sub group_without_met_export()

'chemin de frame

ThisWorkbook.Worksheets("Frame").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer les fichiers", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"

Dim RepParent As String, RepInitial As String
 
ChDir (ThisWorkbook.Path)
RepInitial = CurDir(ThisWorkbook.Path)
ChDir ".."
RepParent = CurDir(ThisWorkbook.Path)

chemin = RepParent & "\DataNew" & "\"
NomClient = ThisWorkbook.Worksheets("Frame").Name
nom = "Frame" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'chemin de user topology

ThisWorkbook.Worksheets("User Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer User Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("User Topology").Name
nom = "User Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'chemin de topology

ThisWorkbook.Worksheets("Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Topology").Name
nom = "Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'chemin de mat rout

ThisWorkbook.Worksheets("Mat Rout").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer Mat Rout.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Mat Rout").Name
nom = "Mat Rout" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

End Sub
 
  Sub group_pre_param()
 
 Dim Départ As Double, arrivée As Double, i As Long, Durée As Double
Dim mn As Long, ms As Long, sd As Long, tps As String
Départ = GetTickCount&
 
 Call ISR_Synthese_1.correct_isr_synthese_1
 Call ISR_Synthese_2.correct_isr_synthese_2
 Call Correct_Period_Message_Set.copy_period_to_message_set
 
 arrivée = GetTickCount&
Durée = arrivée - Départ
mn = Int(Durée / 1000 / 60)
sd = Int((Durée / 1000) - (mn * 60))
ms = Durée - (sd * 1000) - (mn * 1000 * 60)
'Formatage #:##:###
tps = mn & ":" & Right("00" & sd, 2) & ":" & Right("000" & ms, 3)
'MsgBox "Rebuild R Table : " & tps
MsgBox "Création de Param : " & tps
  
 End Sub
 
 Sub group_param()
 
Dim Départ As Double, arrivée1, arrivée2, arrivée3, arrivée4, arrivée5, arrivée6, arrivée7 As Double, i As Long, Durée As Double
Dim mn As Long, ms As Long, sd As Long, tps As String
Départ = GetTickCount&

' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0.05
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
  
 Call Param.create_param
 
' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0.1
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

 Call Build_R_Table.Detect_R_from_Param
 
' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0.4
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
 
 Call Correct_R_Message_Set.correct_r_table
 
' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0.8
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
 
 Call Build_R_Table.Detect_R_from_Param
 
 ' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 1
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
 
 'Unload ProgressBar
 
arrivée7 = GetTickCount&
Durée = arrivée7 - Départ
mn = Int(Durée / 1000 / 60)
sd = Int((Durée / 1000) - (mn * 60))
ms = Durée - (sd * 1000) - (mn * 1000 * 60)
'Formatage #:##:###
tps = mn & ":" & Right("00" & sd, 2) & ":" & Right("000" & ms, 3)
'MsgBox "Création de Param : " & tps

 End Sub
 
 'objectif : recopier les lignes de r table dans param puis maj r table
 
 Sub group_manuel()
 
 'déclarations des variables
 
  Dim chemin As String
Dim objShell As Object, objFolder As Object, oFolderItem As Object
Dim NomClient As String
 
 Call Correct_R_Message_Set.post_correction_manuel
 Call Build_R_Table.Detect_R_from_Param
 
 'création de param.csv
 
 ThisWorkbook.Worksheets("Param").Copy

Chx = MsgBox("Veuiller choisir un chemin où enregistrer les fichiers", vbYesNo + vbQuestion, "mDF XLpages.com")
If Chx = vbYes Then
Set objShell = CreateObject("Shell.Application")
Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
Set oFolderItem = objFolder.Items.Item
chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Param").Name
nom = "Param" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'création de topology.csv

ThisWorkbook.Worksheets("Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Topology").Name
nom = "Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'création de user topology

ThisWorkbook.Worksheets("User Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer User Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("User Topology").Name
nom = "User Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

Else
CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

End If
 
 End Sub
 
 'objectif : corriger automatiquement puis recopier les lignes de r table dans param puis maj r table
 
 Sub group_auto()
 
Dim chemin As String
Dim objShell As Object, objFolder As Object, oFolderItem As Object
Dim NomClient As String
 
 Call Correct_R_Message_Set.correct_auto_R_table
 Call Correct_R_Message_Set.post_correction_manuel
 Call Build_R_Table.Detect_R_from_Param
 
  'création de param.csv
 
 ThisWorkbook.Worksheets("Param").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer les fichiers", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item

Dim RepParent As String, RepInitial As String
 
ChDir (ThisWorkbook.Path)
RepInitial = CurDir(ThisWorkbook.Path)
ChDir ".."
RepParent = CurDir(ThisWorkbook.Path)

'chemin = oFolderItem.Path & "\"
chemin = RepParent & "\DataNew" & "\"
NomClient = ThisWorkbook.Worksheets("Param").Name
nom = "Param" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'création de topology.csv

ThisWorkbook.Worksheets("Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Topology").Name
nom = "Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'création de user topology

ThisWorkbook.Worksheets("User Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin où enregistrer User Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un répertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("User Topology").Name
nom = "User Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas été sauvegardé... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If
 
 End Sub
 
 'objectif : inutilisé car redondant avec r table
 'exporter les isr sans update time corrigées puis maj r table
 
 Sub group_period()
 
 Call Correct_R_Message_Set.export_isr_period
 Call Build_R_Table.build_miss_isr_period_table
 
 End Sub

'objectif : inutilisé
'ouvre la macro associé à un bouton

Sub ouvrir()
    UserForm_demo.Show
End Sub

Sub group_frame()

Dim Départ As Double, arrivée7 As Double, i As Long, Durée As Double
Dim mn As Long, ms As Long, sd As Long, tps As String
Départ = GetTickCount&

'ProgressBar.Show 0

' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

Call Frame.create_frame

' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0.75
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

Call copy_rout.copy_rout

' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0.9
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

Call frame_bri.frame_bri
Call Frame_empty_calc.delete_bad_calc

' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 1
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

'Unload ProgressBar

arrivée7 = GetTickCount&
Durée = arrivée7 - Départ
mn = Int(Durée / 1000 / 60)
sd = Int((Durée / 1000) - (mn * 60))
ms = Durée - (sd * 1000) - (mn * 1000 * 60)
'Formatage #:##:###
tps = mn & ":" & Right("00" & sd, 2) & ":" & Right("000" & ms, 3)
'MsgBox "Création de Frame : " & tps

End Sub

Sub launch_menu()

ProgressBar.Show 0
' Mise à jour du pourcentage (Itération / par le nombre max de lignes)
PctDone = 0
' Appelle de la sous-routine qui met à jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

End Sub

'Code de la sous-routine
Sub UpdateProgressBar(PctDone As Single)
    With ProgressBar

        ' Mise à jour du label.
        .FrameProgress.Caption = Format(PctDone, "0%")

        ' Afin de paramétrer la fin de la progressBar par rapport au frame
        .LabelProgress.Width = PctDone * _
            (.FrameProgress.Width - 20)
    End With

    ' DoEvents autorisant au UserForm de ce mettre à jour
    DoEvents
End Sub


