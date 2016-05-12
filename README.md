Impl�mentation MD5
==================
Ce r�pertoire contient une impl�mentation de l'algorithme de hachage MD5 dans le cardre de mon projet scientifique de cours de math�matique discr�te. Pour avoir une version lisible de ce fichier et pour avoir plus d'informations, consultez ce lien : [https://github.com/TheNiceGuy/md5](https://github.com/TheNiceGuy/md5).

Structure du projet
-------------------
- Le dossier `./bin` contient l'executable `md5.exe` et une impl�mentation populaire de l'algorithme t�l�charg�e en ligne `md5deep.exe`.
- Le dossier `./doc` contient la documentation du standard MD5 ainsi que le travail th�orique sur le projet scientifique.
- Le dossier `./src` contient le code source du programme. Afin de le compiler, il faut les outils de d�veloppement Linux (`gcc`, `make`, `mingw`, etc). Il n'est pas n�c�ssaire de le compiler, car l'executable est fournie.
- Le dossier `./.git` et le fichier `.gitignore` peuvent �tre ignor�s, ils contiennent de l'information reli� � mon avancement du projet, la m�me chose qui peut �tre r�cup�r�e � cette addresse : [https://github.com/TheNiceGuy/md5](https://github.com/TheNiceGuy/md5).
- Les fichiers `makefile.win32` et `makefile.linux` contiennent les d�finitions pour le logiciel make afin compiler pour Windows ou Linux. Ils peuvent �tre ignor�s.

Scripts
-------
Il y plusieurs scripts. Vous pouvez consulter les petits scripts en les ouvrants avec bloc-note. Le script `md5_fichier.bat` va chercher certains fichiers dans ce dossier afin de les hacher avec le programme pour ensuite comparer le r�sultat avec l'impl�mentation t�l�charg�e en ligne.

Le script `md5_string.bat` demande un string � l'utilisateur pour ensuite le hacher avec mon programme. Vous pouvez comparer le r�sultat du string avec une impl�mentation en ligne comme : [http://www.md5.cz/](http://www.md5.cz/).

Finalement, le script `md5_help.bat` ne fait qu'afficher l'aide pour utiliser le programme � la ligne de commande. Il permet ensuite d'executer des commandes si n�cessaires.
