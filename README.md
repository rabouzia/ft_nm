## FT_NM


- [ ] clean code
- [ ] add bonus
- [x] fix leak
- [x] test with other binary

ft_nm
ft_nm est une réimplémentation en C de la commande UNIX nm, utilisée pour afficher la table des symboles d’un fichier objet ELF (Executable and Linkable Format). Ce projet fait partie des exercices de bas niveau pour comprendre le fonctionnement interne des binaires et des formats ELF.

📌 Objectif
Reproduire le comportement principal de la commande nm GNU, en supportant :

les formats ELF 32 bits et 64 bits,

l’analyse des sections .symtab et .strtab,

l’affichage des symboles définis et indéfinis avec leurs :

adresses,

types (T, U, B, etc.),

noms.

⚙️ Fonctionnalités
Prise en charge des options :

-a : affiche tous les symboles (même ceux utilisés par les debuggers),

-g : affiche uniquement les symboles globaux,

-u : affiche uniquement les symboles indéfinis,

-p : désactive le tri par ordre alphabétique,

-r : inverse l’ordre du tri.

Lecture manuelle du fichier ELF :

parsing de l’en-tête ELF,

parsing des sections et de la table des symboles,

gestion fine des offsets et pointeurs,

filtrage de symboles inutiles (comme __PRETTY_FUNCTION__.*).

🧠 Ce que j’ai appris
Manipulation bas niveau des fichiers binaires ELF.

Compréhension des structures ELF (Elf32_Ehdr, Elf64_Shdr, etc.).

Manipulation des bits, cast mémoire, tri personnalisé.

Analyse et comparaison de la sortie avec l’outil nm.

🧪 Test
La sortie de ./ft_nm est comparée à celle de la commande système nm pour s’assurer de la conformité. Les différences de sortie sont minimisées en excluant les symboles non pertinents ou absents selon les options.

