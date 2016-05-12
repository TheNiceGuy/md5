Implémentation MD5
==================
Ce répertoire contient une implémentation de l'algorithme de hachage MD5 dans le cardre de mon projet scientifique de cours de mathématique discrète. Pour avoir une version lisible de ce fichier et pour avoir plus d'informations, consultez ce lien : [https://github.com/TheNiceGuy/md5](https://github.com/TheNiceGuy/md5).

Structure du projet
-------------------
- Le dossier `./bin` contient l'executable `md5.exe` et une implémentation populaire de l'algorithme téléchargée en ligne `md5deep.exe`.
- Le dossier `./doc` contient la documentation du standard MD5 ainsi que le travail théorique sur le projet scientifique.
- Le dossier `./src` contient le code source du programme. Afin de le compiler, il faut les outils de développement Linux (`gcc`, `make`, `mingw`, etc). Il n'est pas nécéssaire de le compiler, car l'executable est fournie.
- Le dossier `./.git` et le fichier `.gitignore` peuvent être ignorés, ils contiennent de l'information relié à mon avancement du projet, la même chose qui peut être récupérée à cette addresse : [https://github.com/TheNiceGuy/md5](https://github.com/TheNiceGuy/md5).
- Les fichiers `makefile.win32` et `makefile.linux` contiennent les définitions pour le logiciel make afin compiler pour Windows ou Linux. Ils peuvent être ignorés.

Scripts
-------
Il y plusieurs scripts. Vous pouvez consulter les petits scripts en les ouvrants avec bloc-note. Le script `md5_fichier.bat` va chercher certains fichiers dans ce dossier afin de les hacher avec le programme pour ensuite comparer le résultat avec l'implémentation téléchargée en ligne.

Le script `md5_string.bat` demande un string à l'utilisateur pour ensuite le hacher avec mon programme. Vous pouvez comparer le résultat du string avec une implémentation en ligne comme : [http://www.md5.cz/](http://www.md5.cz/).

Finalement, le script `md5_help.bat` ne fait qu'afficher l'aide pour utiliser le programme à la ligne de commande. Il permet ensuite d'executer des commandes si nécessaires.
