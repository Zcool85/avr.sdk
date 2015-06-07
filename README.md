# avr.sdk

# Introduction

avr.sdk est un Software Development Kit contenant les librairies et les outils
utilisés pour mes réalisations personnelles.

Documentation en ligne accessible [ici](http://zcool85.github.io/avr.sdk/index.html)

# Template Makefile

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

# TODO

- [ ] Faire fonctionner l'API RFM pour les modules alpha
- [ ] Présenter l'organisation des répertoires dans la documentation
- [ ] Faire une API correcte pour le protocol TWI (ou I2C pour les intimes)
- [ ] Faire une API correcte pour le module FM RFM12B
- [ ] Faire une API correcte pour le module RFID MFRC522
