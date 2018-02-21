Attribute VB_Name = "Menu"
Public Declare Function GetTickCount& Lib "kernel32" ()

'A mettre au d�but dans la macro "lourde"
Dim PctDone As Single

'macro qui se d�clenche  � l'ouverture du classeur
Sub auto_open()

ProgressBar.Show 0

 Set mabarre = CommandBars.Add(Name:="Cr�er les fichiers pour l'optimisation", temporary:=True)
 With mabarre
 .Position = 4
 .Left = 100
 .Top = 100
 .Visible = True
 With .Controls.Add(Type:=msoControlPopup)
 .Caption = "Cr�er les fichiers pour l'optimisation"
 
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
 
   Set mabarre3 = CommandBars.Add(Name:="Reg�n�rer CAN Message Set", temporary:=True)
 With mabarre3
 .Position = 4
 .Left = 100
 .Top = 100
 .Visible = True
 With .Controls.Add(Type:=msoControlPopup)
 .Caption = "Reg�n�rer CAN Message Set"
 
   With .Controls.Add(Type:=msoControlButton)
 .Caption = "Reg�n�rer CAN Message Set"
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
 
 'd�claration des variables
 
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

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer les fichiers", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
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
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'chemin de user topology

ThisWorkbook.Worksheets("User Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer User Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("User Topology").Name
nom = "User Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'chemin de topology

ThisWorkbook.Worksheets("Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Topology").Name
nom = "Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'chemin de mat rout

ThisWorkbook.Worksheets("Mat Rout").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer Mat Rout.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Mat Rout").Name
nom = "Mat Rout" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

End Sub
 
  Sub group_pre_param()
 
 Dim D�part As Double, arriv�e As Double, i As Long, Dur�e As Double
Dim mn As Long, ms As Long, sd As Long, tps As String
D�part = GetTickCount&
 
 Call ISR_Synthese_1.correct_isr_synthese_1
 Call ISR_Synthese_2.correct_isr_synthese_2
 Call Correct_Period_Message_Set.copy_period_to_message_set
 
 arriv�e = GetTickCount&
Dur�e = arriv�e - D�part
mn = Int(Dur�e / 1000 / 60)
sd = Int((Dur�e / 1000) - (mn * 60))
ms = Dur�e - (sd * 1000) - (mn * 1000 * 60)
'Formatage #:##:###
tps = mn & ":" & Right("00" & sd, 2) & ":" & Right("000" & ms, 3)
'MsgBox "Rebuild R Table : " & tps
MsgBox "Cr�ation de Param : " & tps
  
 End Sub
 
 Sub group_param()
 
Dim D�part As Double, arriv�e1, arriv�e2, arriv�e3, arriv�e4, arriv�e5, arriv�e6, arriv�e7 As Double, i As Long, Dur�e As Double
Dim mn As Long, ms As Long, sd As Long, tps As String
D�part = GetTickCount&

' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0.05
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
  
 Call Param.create_param
 
' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0.1
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

 Call Build_R_Table.Detect_R_from_Param
 
' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0.4
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
 
 Call Correct_R_Message_Set.correct_r_table
 
' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0.8
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
 
 Call Build_R_Table.Detect_R_from_Param
 
 ' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 1
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone
 
 'Unload ProgressBar
 
arriv�e7 = GetTickCount&
Dur�e = arriv�e7 - D�part
mn = Int(Dur�e / 1000 / 60)
sd = Int((Dur�e / 1000) - (mn * 60))
ms = Dur�e - (sd * 1000) - (mn * 1000 * 60)
'Formatage #:##:###
tps = mn & ":" & Right("00" & sd, 2) & ":" & Right("000" & ms, 3)
'MsgBox "Cr�ation de Param : " & tps

 End Sub
 
 'objectif : recopier les lignes de r table dans param puis maj r table
 
 Sub group_manuel()
 
 'd�clarations des variables
 
  Dim chemin As String
Dim objShell As Object, objFolder As Object, oFolderItem As Object
Dim NomClient As String
 
 Call Correct_R_Message_Set.post_correction_manuel
 Call Build_R_Table.Detect_R_from_Param
 
 'cr�ation de param.csv
 
 ThisWorkbook.Worksheets("Param").Copy

Chx = MsgBox("Veuiller choisir un chemin o� enregistrer les fichiers", vbYesNo + vbQuestion, "mDF XLpages.com")
If Chx = vbYes Then
Set objShell = CreateObject("Shell.Application")
Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
Set oFolderItem = objFolder.Items.Item
chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Param").Name
nom = "Param" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'cr�ation de topology.csv

ThisWorkbook.Worksheets("Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Topology").Name
nom = "Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'cr�ation de user topology

ThisWorkbook.Worksheets("User Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer User Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("User Topology").Name
nom = "User Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

Else
CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
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
 
  'cr�ation de param.csv
 
 ThisWorkbook.Worksheets("Param").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer les fichiers", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
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
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'cr�ation de topology.csv

ThisWorkbook.Worksheets("Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("Topology").Name
nom = "Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'cr�ation de user topology

ThisWorkbook.Worksheets("User Topology").Copy

'Chx = MsgBox("Veuiller choisir un chemin o� enregistrer User Topology.csv", vbYesNo + vbQuestion, "mDF XLpages.com")
'If Chx = vbYes Then
'Set objShell = CreateObject("Shell.Application")
'Set objFolder = objShell.BrowseForFolder(&H0&, "Choisir un r�pertoire pour l'enregistrement du fichier", &H1&)
'Set oFolderItem = objFolder.Items.Item
'chemin = oFolderItem.Path & "\"
NomClient = ThisWorkbook.Worksheets("User Topology").Name
nom = "User Topology" 'InputBox("Indiquer le nom du fichier :")
ActiveWorkbook.SaveAs fileName:=chemin & nom & ".csv", FileFormat:=xlCSV, local:=True
'MsgBox chemin & nom & ".csv"
'ActiveWorkbook.Close SaveChanges:=False
ThisWorkbook.Activate

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If

'Else
'CreateObject("Wscript.shell").Popup "Le fichier n'a pas �t� sauvegard�... ", vbExclamation
'ActiveWorkbook.Close SaveChanges:=False
'ThisWorkbook.Activate

'End If
 
 End Sub
 
 'objectif : inutilis� car redondant avec r table
 'exporter les isr sans update time corrig�es puis maj r table
 
 Sub group_period()
 
 Call Correct_R_Message_Set.export_isr_period
 Call Build_R_Table.build_miss_isr_period_table
 
 End Sub

'objectif : inutilis�
'ouvre la macro associ� � un bouton

Sub ouvrir()
    UserForm_demo.Show
End Sub

Sub group_frame()

Dim D�part As Double, arriv�e7 As Double, i As Long, Dur�e As Double
Dim mn As Long, ms As Long, sd As Long, tps As String
D�part = GetTickCount&

'ProgressBar.Show 0

' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

Call Frame.create_frame

' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0.75
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

Call copy_rout.copy_rout

' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0.9
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

Call frame_bri.frame_bri
Call Frame_empty_calc.delete_bad_calc

' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 1
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

'Unload ProgressBar

arriv�e7 = GetTickCount&
Dur�e = arriv�e7 - D�part
mn = Int(Dur�e / 1000 / 60)
sd = Int((Dur�e / 1000) - (mn * 60))
ms = Dur�e - (sd * 1000) - (mn * 1000 * 60)
'Formatage #:##:###
tps = mn & ":" & Right("00" & sd, 2) & ":" & Right("000" & ms, 3)
'MsgBox "Cr�ation de Frame : " & tps

End Sub

Sub launch_menu()

ProgressBar.Show 0
' Mise � jour du pourcentage (It�ration / par le nombre max de lignes)
PctDone = 0
' Appelle de la sous-routine qui met � jour la progressBar sur base du PctDone
UpdateProgressBar PctDone

End Sub

'Code de la sous-routine
Sub UpdateProgressBar(PctDone As Single)
    With ProgressBar

        ' Mise � jour du label.
        .FrameProgress.Caption = Format(PctDone, "0%")

        ' Afin de param�trer la fin de la progressBar par rapport au frame
        .LabelProgress.Width = PctDone * _
            (.FrameProgress.Width - 20)
    End With

    ' DoEvents autorisant au UserForm de ce mettre � jour
    DoEvents
End Sub


