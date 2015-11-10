# avr.sdk

# Introduction

avr.sdk est un Software Development Kit contenant les librairies et les outils
utilisés pour mes réalisations personnelles.

La documentation en ligne du SDK est accessible [ici](http://zcool85.github.io/avr.sdk/index.html).

# Templates
## Template Makefile

Un template de fichier Makefile ([Makefile.template](templates/Makefile.template)) est disponible dans le répertoire templates. Pour l'utiliser, il suffit de le copier dans le répertoire des sources à compiler (en renommant le fichier `Makefile`) et de modifier l'entête du fichier déclarant les variables suivantes :
- MCU
- HZ
- PROGRAMMER_MCU
- TARGET_NAME
- INC
- LIBS
- OPTLEVEL
- AVRDUDE_PROGRAMMERID
- AVRDUDE_PORT

## Template Xcode

Un template complet pour Xcode (v4 à v7) est diponible dans le répertoire templates/Xcode.

Avant d'installer ce template, il convient de modifier les répertoires de recherche des headers par défaut de avr-gcc. Pour ce faire, il faut lancer la commande suivante : `echo | avr-cpp -v` et récupérer les chemins indiqués sous la ligne `#include <...> search starts here:`. Ces chemins doivent être ajoutés à la clef `HEADER_SEARCH_PATHS` dans le fichier `templates/Xcode/Microcontroller/AVR.xctemplate/TemplateInfo.plist` :

````XML
...
<key>HEADER_SEARCH_PATHS</key>
<array>
    <string>/usr/local/lib/gcc/avr/4.9.2/include</string>
    <string>/usr/local/lib/gcc/avr/4.9.2/include-fixed</string>
    <string>/usr/local/lib/gcc/avr/4.9.2/../../../../avr/include</string>
</array>
...
````

Puis, pour installer ce template, il suffit de copier le contenu du répertoire `templates/Xcode` dans le répertoire des templates Xcode.

Pour ce faire, il suffit dans un premier temps de créer le répertoire des templates de projet pour Xcode :

`mkdir -p ~/Library/Developer/Xcode/Templates/Project\ Template`

Et copier le template dans le répertoire tout juste créé :

`cp -R templates/Xcode/* ~/Library/Developer/Xcode/Templates/Project\ Template/.`.

Le template apparaitra dans la liste des templates à l'ouverture d'Xcode.

# TODO

- [ ] template Xcode : Revoir le template du makefile
- [ ] Faire fonctionner l'API RFM pour les modules alpha
- [ ] Présenter l'organisation des répertoires dans la documentation
- [ ] Compléter l'API I2C avec un mode de fonctionnement par interruptions
- [ ] Faire une API correcte pour le module FM RFM12B
- [ ] Faire une API correcte pour le module RFID MFRC522
- [ ] Ajouter une API pour gérer le mode SPI en esclave
- [ ] Ajouter une API UART
- [ ] template Xcode : Ajout de l'argument -Wc99-extensions pour la target 'Index' (GCC_C_LANGUAGE_STANDARD=GNU99)
- [ ] template Xcode : Correction des deux erreurs pour la target 'Index'
- [ ] template Xcode : Trouver une solution pour activer le contrôle de la documentation embarquée dans le code
