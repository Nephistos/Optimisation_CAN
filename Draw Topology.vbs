Option Explicit

On Error Resume Next
ExempleMacroExcel
 
Sub ExempleMacroExcel() 
 
  Dim ApplicationExcel 
  Dim ClasseurExcel
  Dim Fichier
  Dim fso
  Dim path
  Dim nom
 
  Set ApplicationExcel = CreateObject("Excel.Application") 
  Set fso = CreateObject("Scripting.FileSystemObject")

  path = fso.GetParentFolderName(wscript.ScriptFullName) & "\Excel\"
  'Msgbox path
  nom = "Draw_Topology"
  Fichier = path & nom
  'Msgbox Fichier

  If FichierExiste(Fichier) = True Then
  MsgBox "Le fichier " & Fichier & " est introuvable !"
  WScript.Quit
  End If

  Set ClasseurExcel = ApplicationExcel.Workbooks.Open(Fichier) 
  
  ApplicationExcel.Visible = False   'les actions seront visibles. Pour tout lancer en arrière-plan, remplacer True par False

  ApplicationExcel.Run "Menu.launch_menu" 'va lancer la macro "MacroTest1"

  'ApplicationExcel.Quit 
 
  Set ClasseurExcel = Nothing 
  Set ApplicationExcel = Nothing

  'quitte VBS
  WScript.Quit
 
End Sub

  'Test d'existence d'un fichier
  Function FichierExiste(Fichier)
  Dim FSO
  Set FSO = CreateObject("Scripting.FileSystemObject")
  FichierExiste = FSO.FileExists(Fichier)
  Set FSO = Nothing
  End Function