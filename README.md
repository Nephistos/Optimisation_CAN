<h1>Partie 1 :</h1>

<h2>Optimisation</h2>

Développement d'une application assurant la compatibilité entre un outil d'optimisation et ses fichiers d'entrée.

<h2>Contexte</h2>

Durant mon apprentissage chez Renault, j'ai été amené à travailler autour d'un logiciel d'optimisation. Cet outil est capable d'optimiser conjointement le synoptique et le routage emprunté par les trames. Ce logiciel est compatible avec les réseaux CAN et CAN-FD.

Suite à l'introduction de la technologie de GateWay dans nos véhicules, le format des messageries (fichiers d'entrée de l'outil d'optimisation) a été en grande partie remanié. De fait, l'outil n'était plus capable d'importer les informations à optimiser.

<h2>Objectif</h2>

Mon rôle autour de cet outil a été de développer une application (VisualBasic) capable de réorganiser les fichiers de messageries dans le format exigé par l'outil d'optimisation.

<h2>Performances</h2>

Mon application est capable de générer des fichiers d'entrée compatibles avec l'outil d'optimisation, depuis n'importe quelle version des messageries C1A et C1AHS et pour chacun des modes de fonctionnement de l'outil.

<h2>Fonctionnement</h2>

•	L'utilisateur lance un VBScript qui va lui demander de sélectionner une messagerie à optimiser. Le VBScript ouvre le classeur *.xlsm correspondant puis appel la macro "Menu" qu'il contient.
•	A son tour, la macro "Menu" appel l'interface graphique qui va proposer à l'utilisateur le mode de fonctionnement pour lequel il veux générer des fichiers d'entrée.
•	Après que l'utilisateur ai sélectionner le mode de fonctionement qu'il compte utiliser à l'aide de l'interface graphique, cette dernière appel la main-macro qui va effectuer le traitement adéquat et exporter les fichiers.

---

<h1>Partie 2 :</h1>

<h2>Synoptique</h2>

Développement d'une application de schématisation des synoptiques CAN & CAN-FD.

<h2>Contexte</h2>

Suite à des travaux autour d'un logiciel d'optimisation, j'ai constaté que ce dernier retournais un fichier de résultat du synoptique sous forme de tableau.

<h2>Objectif</h2>

<h2>La lecture d'un synoptique sous forme de tableau étant peu chaleureuse, j'ai entrepris de déveloper une application (VisualBasic) capable de lire le tableau retourné par l'outil d'optimisation et de générer un schéma en conséquence.

<h2>Performances</h2>

Mon application est capable de traiter tous les fichiers de résultat du synoptique retournés par l'outil.

<h2>Fonctionnement</h2>

•	L'utilisateur lance un VBScript qui va ouvrir un classeur *.xlsm prédéfini.
•	A l'ouverture de ce dernier, une interface graphique sera appelée via une macro prédéfinie.
•	L'Interface permet à l'utilisateur d'importer n'importe quelle fichier de résultat du synoptique.
•	Une fois un fichier importé, la macro de traitement se déclenche automatiquement afin de schématiser le tableau.
•	Lorsque l'opération est terminée, l'utilisateur peut demander l'export *.pdf de son schéma et choisir la localisation de l'enregistrement.

