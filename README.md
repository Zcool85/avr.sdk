# avr.sdk

# Introduction

avr.sdk est un Software Development Kit contenant les librairies et les outils
utilisés pour mes réalisations personnelles.

Documentation en ligne accessible [ici](http://zcool85.github.io/avr.sdk/index.html)

# Templates
## Template Makefile

Un template de fichier Makefile ([Makefile.template](templates/Makefile.template)) est disponible dnas le répertoire templates. Pour l'utiliser, il suffit de le copier dans le répertoire des sources à compiler (en renommant le fichier `Makefile`) et de modifier l'entête du fichier déclarant les variables suivantes :
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

Un template complet pour Xcode (v4 à v7) est diponible dans le répertoire templates/Xcode. Pour installer ce template, il suffit de copier le contenu du répertoire `templates/Xcode` dans le répertoire des templates Xcode.

Pour ce faire, il suffit dans un premier temps de créer le répertoire des templates de projet pour Xcode :
`mkdir -p ~/Library/Developer/Xcode/Templates/Project\ Template`
Et copier le template dans le répertoire tout juste créé :
`cp -R templates/Xcode/* ~/Library/Developer/Xcode/Templates/Project\ Template/.`.

Le template apparaitra dans la liste des templates à l'ouverture d'Xcode.

# TODO

- [ ] Faire fonctionner l'API RFM pour les modules alpha
- [ ] Présenter l'organisation des répertoires dans la documentation
- [ ] Compléter l'API I2C avec un mode de fonctionnement par interruptions
- [ ] Faire une API correcte pour le module FM RFM12B
- [ ] Faire une API correcte pour le module RFID MFRC522
- [ ] Ajouter une API pour gérer le mode SPI en esclave
- [ ] Ajouter une API UART
