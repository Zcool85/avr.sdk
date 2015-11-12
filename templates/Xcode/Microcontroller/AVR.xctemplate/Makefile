# This file is part of ___PACKAGENAME___.
#
# ___PACKAGENAME___ is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ___PACKAGENAME___ is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ___PACKAGENAME___.  If not, see <http://www.gnu.org/licenses/>.
#
# ___COPYRIGHT___

# TODO : Supprimer les références circulaires

#===============================================================================
#=========================== Configuration générale ============================
#===============================================================================

#-------------------------------------------------------------------------------
# Outils
#-------------------------------------------------------------------------------

# Définition des commandes et outils de compilation
SHELL                   := sh
CC                      := avr-gcc
OBJCOPY                 := avr-objcopy
OBJDUMP                 := avr-objdump
SIZE                    := avr-size
NM                      := avr-nm
AVRDUDE                 := avrdude
REMOVE                  := rm -f
REMOVEDIR               := rmdir
COPY                    := cp
WINSHELL                := cmd      # TODO : A remplacer par la version *nux


#-------------------------------------------------------------------------------
# Configuration du microcontrolleur
#-------------------------------------------------------------------------------

# MCU
#     Cf. http://www.nongnu.org/avr-libc/user-manual/using_tools.html
MCU                     := atmega324p    # TODO : ___VARIABLE_MCU___

# Fréquence du microcontrolleur
#     Cette variable est utilisée pour définir le symbol F_CPU dans tous les
#     fichiers sources. Il est inutile de suffixer la valeur par 'UL' car
#     cela est fait automatiquement par ce fichier Makefile
F_CPU                   := 16000000# TODO : ___VARIABLE_F_CPU___



#-------------------------------------------------------------------------------
# Configuration avrdude
#-------------------------------------------------------------------------------

# Définit le type de microcontrolleur à programmer pour avrdude
#    Cf. avrdude -c?
AVRDUDE_PROGRAMMER      := avrispmkII     # TODO : ___VARIABLE_PROGRAMMER___

# Port de connexion avrdude à utiliser pour programmer le microcontrolleur
#     com1 -> Port série
#     lpt1 -> Port parallèle
#     usb  -> Port usb
AVRDUDE_PORT            := usb   # TODO : ___VARIABLE_SERIAL_PORT___

# NOTE : Liste des microcontrolleurs programmable via avrdude : avrdude -p?

#-------------------------------------------------------------------------------
# Configuration de la compilation
#-------------------------------------------------------------------------------

# Format du fichier HEX
#     srec, ihex ou binary
FORMAT                  := ihex

# Fichier cible (sans extension)
TARGET                  := main

# Liste des répertoires complémentaires pour la recherche des fichiers d'entête
#     Chaque répertoire doit être séparé par un espace. Pour les répertoires
#     contenant des espaces, entourer le chemin par des doubles quotes.
EXTRAINCDIRS            :=

# Liste des fichiers assembleurs à ajouter au projet
#     Tous ces fichiers doivent être suffixés par un .S majucule !
#     Les fichiers se terminant pas un .s minuscule ne seront pas utilisés
#     pour la compilation et seront supprimés si 'make clean' est utilisé.
ASRC                    :=

# Niveau d'optimisation
#     0 -> Pas d'optimisation
#     1 .. 3 -> Niveau faible .. Niveau haut (NOTE : 3 n'est pas toujours le meilleur niveau d'optimisation. Cf. avr-libc FAQ.)
#     s -> Optimiser la taille
OPT                     := s

# Fromat des symbols de débogage
#     Les formats par défaut d'avr-gcc sont dwarf-2 (par défaut) ou stabs
#     AVR Studio 4.10 nécessite dwarf-2.
#     AVR COFF ou COFF étendu nécessitent le format stabs, et une exécution d'avr-objcopy
DEBUG                   := stabs

# Répertoire de compilation
#     Tous les fichiers binaires seront dans ce répertoires
OBJDIR                  := build


#-------------------------------------------------------------------------------
# Configuration des outils de déboggage
#-------------------------------------------------------------------------------

# Configuration pour simulavr : Fréquence du microcontrolleur
DEBUG_MFREQ             := $(F_CPU)

# Choix du debug_ui
#     gdb
#     insight
DEBUG_UI                := insight

# Choix de l'outil de simulation
#     simulavr
#     avarice
DEBUG_BACKEND           := avarice

# Fichier d'initialisation de GDB
GDBINIT_FILE            := __avr_gdbinit

# Configuration avarice : Périphérique JTAG
JTAG_DEV                := /dev/com1

# Port de débogage pour la communication entre GDB / avarice / simulavr
DEBUG_PORT              := 4242

# Hôte de déboggage pour la communication entre GDB / avarice / simulavr
#    Normalement la valeur est toujours localhost à moins d'utliser un autre
#    ordinateur pour effectuer un déboggage
DEBUG_HOST              := localhost



#===============================================================================
#============================= Configuration avancée ===========================
#===============================================================================


#-------------------------------------------------------------------------------
# Configuration avr-gcc
#-------------------------------------------------------------------------------

# Liste des fichiers source à compiler (les dépendances sont générées automatiquement)
SRC                     := $(wildcard *.c)

# Conformité au standard C
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard
#     gnu99 = c99 plus GCC extensions
CSTANDARD               := c99

# Options de compilation
#     -g*        -> Informations de déboggage
#     -DF_CPU    -> Définition du symbol F_CPU
#     -O*        -> Niveau d'optimisation
#     -f...      -> Tuning (Cf. GCC manual and avr-libc documentation)
#     -Wall...   -> Niveau d'alerte
#     -Wa,...    -> Commande envoyé à l'assembleur
#     -adhlns... -> Création d'un listing assembleur
#     -I...      -> Répertoire de recherche des fichiers d'entête
CFLAGS                  := -g$(DEBUG)
CFLAGS                  += -DF_CPU=$(F_CPU)UL
CFLAGS                  += -O$(OPT)
CFLAGS                  += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS                  += -Wall -Wstrict-prototypes
CFLAGS                  += -Wa,-adhlns=$(addprefix $(OBJDIR)/,$(<:.c=.lst))
CFLAGS                  += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS                  += -std=$(CSTANDARD)


#-------------------------------------------------------------------------------
# Configuration assembleur
#-------------------------------------------------------------------------------

# Options assembleur
#  -Wa,...             -> Indique à avr-gcc de passer le reste de la ligne de
#                         commande à l'assmebleur
#  -ahlms              -> Création d'un listing
#  -gstabs             -> Insertion des informations numéro de ligne et nom
#                         de fichier dans les fichiers assembleurs
#                         (Obligatoire pour les fichiers COFF Cf. avr-libc docs)
#  -listing-cont-lines -> Définit le nombre de ligne continue maximum de
#                         dump hexadécimal à afficher pour une seule ligne
#                         de cide source
ASFLAGS                 := -Wa,-adhlns=$(addprefix $(OBJDIR)/,$(<:.S=.lst)),-gstabs,--listing-cont-lines=100


#-------------------------------------------------------------------------------
# Configuration des librairies
#-------------------------------------------------------------------------------

# Version printf minimaliste
PRINTF_LIB_MIN          := -Wl,-u,vfprintf -lprintf_min

# Version de printf avec gestion des float (nécessite MATH_LIB = -lm)
PRINTF_LIB_FLOAT        := -Wl,-u,vfprintf -lprintf_flt

# Choix de la vesrion du printf à utiliser (Blanc pour la version standard)
PRINTF_LIB              :=
#PRINTF_LIB              := $(PRINTF_LIB_MIN)
#PRINTF_LIB              := $(PRINTF_LIB_FLOAT)


# Version minimaliste de scanf
SCANF_LIB_MIN           := -Wl,-u,vfscanf -lscanf_min

# Version de scanf avec gestion des float (nécessite MATH_LIB = -lm)
SCANF_LIB_FLOAT         := -Wl,-u,vfscanf -lscanf_flt

# Choix de la vesrion de scanf à utiliser (Blanc pour la version standard)
SCANF_LIB               :=
#SCANF_LIB               := $(SCANF_LIB_MIN)
#SCANF_LIB               := $(SCANF_LIB_FLOAT)

# librairie mathématique
MATH_LIB                := -lm


#-------------------------------------------------------------------------------
# Configuration de la mémoire externe
#-------------------------------------------------------------------------------

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS             := -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS             := -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff

# Valeur par défaut
EXTMEMOPTS              :=


#-------------------------------------------------------------------------------
# Configuration du linker
#-------------------------------------------------------------------------------

# Options du linker
#    -Wl,...   -> Indique à avr-gcc de passer le reste de la ligne de
#                 commande au linker
#    -Map      -> Création du fichier .map
#    --cref    -> Ajout des références croisées au fichier .map
LDFLAGS                 := -Wl,-Map=$(OBJDIR)/$(TARGET).map,--cref
LDFLAGS                 += $(EXTMEMOPTS)
LDFLAGS                 += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)


#-------------------------------------------------------------------------------
# Configuration avrdude
#-------------------------------------------------------------------------------

# Commande d'écriture du fichier HEX
AVRDUDE_WRITE_FLASH     := -U flash:w:$(OBJDIR)/$(TARGET).hex

# Commande d'écriture de l'eeprom
AVRDUDE_WRITE_EEPROM    := -U eeprom:w:$(TARGET).eep

# Décommenter cette ligne pour que avrdude effectue un comptage de cycle
# d'effacement
#     Cf. manuel avrdude
#AVRDUDE_ERASE_COUNTER  := -y

# Décommenter cette ligne pour qu'il n'y ai pas de vérification après
# programmation du périphérique
#AVRDUDE_NO_VERIFY      := -V

# Niveau de verbosité (pour les rapport de bug Cf. http://savannah.nongnu.org/projects/avrdude)
#AVRDUDE_VERBOSE        := -v -v

# Création des flags avrdude
AVRDUDE_FLAGS           := -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS           += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS           += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS           += $(AVRDUDE_ERASE_COUNTER)


#-------------------------------------------------------------------------------
# Configuration informations de déboggage
#-------------------------------------------------------------------------------

# Convertion ELF vers COFF pour usage en déboggage / simulation dans AVR Studio ou VMLAB
COFFCONVERT             := $(OBJCOPY) --debugging \
                           --change-section-address .data-0x800000 \
                           --change-section-address .bss-0x800000 \
                           --change-section-address .noinit-0x800000 \
                           --change-section-address .eeprom-0x810000


#-------------------------------------------------------------------------------
# Affichage de la taille des binaires
#-------------------------------------------------------------------------------

# Pour les fichiers HEX
HEXSIZE                 := $(SIZE) --target=$(FORMAT) $(OBJDIR)/$(TARGET).hex

# Pour les fichiers ELF
ELFSIZE                 := $(SIZE) --format=avr $(OBJDIR)/$(TARGET).elf



#===============================================================================
#================= Le reste du script n'est pas à modifier =====================
#===============================================================================

#-------------------------------------------------------------------------------
# Messages
#-------------------------------------------------------------------------------

MSG_BEGIN               := -------- Début --------
MSG_END                 := --------  Fin  --------
MSG_SIZE_BEFORE         := Taille avant :
MSG_SIZE_AFTER          := Taille après :
MSG_COFF                := Convertion en AVR COFF :
MSG_EXTENDED_COFF       := Convertion en AVR COFF étendu :
MSG_FLASH               := Création du fichier de chargement pour mémoire flash :
MSG_EEPROM              := Création du fichier de chargement pour mémoire EEPROM :
MSG_EXTENDED_LISTING    := Création du listing étendu :
MSG_SYMBOL_TABLE        := Création de la table des symboles :
MSG_LINKING             := Liaison :
MSG_COMPILING           := Compilation :
MSG_ASSEMBLING          := Assemblage :
MSG_CLEANING            := Nétoyage du projet :


#-------------------------------------------------------------------------------
# Fichiers résultats
#-------------------------------------------------------------------------------

# Définition des fichiers .o
OBJ                     := $(addprefix $(OBJDIR)/,$(SRC:.c=.o)) $(addprefix $(OBJDIR)/,$(ASRC:.S=.o))

# Définition de tous les listings des fichiers
LST                     := $(addprefix $(OBJDIR)/,$(SRC:.c=.lst)) $(addprefix $(OBJDIR)/,$(ASRC:.S=.lst))


#-------------------------------------------------------------------------------
# Assemblage des lignes de commandes
#-------------------------------------------------------------------------------

# Combinaison de toutes les options de compilation
ALL_CFLAGS              := -mmcu=$(MCU) -I. $(CFLAGS)

# Combinaison de toutes les options de l'assembleur
ALL_ASFLAGS             := -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


#-------------------------------------------------------------------------------
# Génération des fichiers de dépendances
#-------------------------------------------------------------------------------

# Emplacement des fichiers de dépendances
DEPSDIR                 := $(OBJDIR)/.dep

# Liste des fichiers de dépendances à construire
DEPS                    := $(SRC:%.c=$(DEPSDIR)/%.d)

# Inclusion des fichiers de dépendances (ce qui provoque également leur création)
# lors de la première passe du GNU make
# Inutile si la cible est clean ou help
# TODO ifneq (,$(findstring clean,$(MAKEFLAGS)))
# TODO ifneq (,$(findstring help,$(MAKEFLAGS)))
     -include $(DEPS)
# TODO endif
# TODO endif

# Règle de création d'un fichier de dépendance
.DELETE_ON_ERROR:
$(DEPSDIR)/%.d: %.c | $(DEPSDIR)
	$(CC) $(ALL_ASFLAGS) -M -MT $(patsubst %.c,$(OBJDIR)/%.o,$<) -MF $@ $<


#-------------------------------------------------------------------------------
# Cibles de génération
#-------------------------------------------------------------------------------

# Cibles PHONY
.PHONY : all build program debug coff extcoff clean help \
         gccversion sizebefore sizeafter \
         elf hex eep lss sym gdb-config

# Cible par défaut
all: gccversion \
     sizebefore \
     clean \
     build \
     program \
     sizeafter

# Cible de construction des fichiers binaires
build: hex eep lss sym

# Cible de programmation du microcontrolleur
program: hex eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

# TODO : Ne plus utiliser Windows
debug: gdb-config elf
ifeq ($(DEBUG_BACKEND), avarice)
	@echo Starting AVaRICE - Press enter when "waiting to connect" message displays.
	@$(WINSHELL) /c start avarice --jtag $(JTAG_DEV) --erase --program --file \
	                      $(OBJDIR)/$(TARGET).elf $(DEBUG_HOST):$(DEBUG_PORT)
	@$(WINSHELL) /c pause
else
	@$(WINSHELL) /c start simulavr --gdbserver --device $(MCU) --clock-freq \
	                      $(DEBUG_MFREQ) --port $(DEBUG_PORT)
endif
    @$(WINSHELL) /c start avr-$(DEBUG_UI) --command=$(GDBINIT_FILE)

# Cible de convertion d'un ELF en COFF
coff: elf
	@echo
	@echo $(MSG_COFF) $(OBJDIR)/$(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(OBJDIR)/$(TARGET).cof

# Cible de convertion d'un ELF en COFF étendu
extcoff: elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(OBJDIR)/$(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(OBJDIR)/$(TARGET).cof

# Compilation des fichiers en assembleur depuis les sources C
$(OBJDIR)/%.s : %.c | $(OBJDIR)
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Compilation d'un fichier de rapport de compilation (pour les rapports de bug)
$(OBJDIR)/%.i : %.c | $(OBJDIR)
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@

# Cible pour netoyer tous les fichiers produits
clean:
	@echo
	@echo $(MSG_CLEANING)
	if test -d "$(OBJDIR)/.dep"; then $(REMOVE) "$(OBJDIR)/.dep"/*; fi
	if test -d "$(OBJDIR)/.dep"; then $(REMOVEDIR) "$(OBJDIR)/.dep"; fi
	if test -d "$(OBJDIR)"; then $(REMOVE) "$(OBJDIR)"/*; fi
	if test -d "$(OBJDIR)"; then $(REMOVEDIR) "$(OBJDIR)"; fi
	@echo


#-------------------------------------------------------------------------------
# Règles de génération des sous-cibles
#-------------------------------------------------------------------------------

# Règle pour les fichiers binaires et listings
elf: $(OBJDIR)/$(TARGET).elf
hex: $(OBJDIR)/$(TARGET).hex
eep: $(OBJDIR)/$(TARGET).eep
lss: $(OBJDIR)/$(TARGET).lss
sym: $(OBJDIR)/$(TARGET).sym

# Affichage de la version du compilateur
gccversion:
	@$(CC) --version

# Création des fichiers finaux HEX depuis le format ELF
$(OBJDIR)/%.hex: $(OBJDIR)/%.elf | $(OBJDIR)
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

# Création des fichiers finaux HEX depuis le format ELF
$(OBJDIR)/%.eep: $(OBJDIR)/%.elf | $(OBJDIR)
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags .eeprom=alloc,load \
            --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Création du listing étendu depuis le format ELF
$(OBJDIR)/%.lss: $(OBJDIR)/%.elf | $(OBJDIR)
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Création de la table des symboles depuis le format ELF
$(OBJDIR)/%.sym: $(OBJDIR)/%.elf | $(OBJDIR)
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Création du fichhier ELF à partir des fichiers O (liaison des fichiers .o)
.SECONDARY : $(OBJDIR)/$(TARGET).elf
.PRECIOUS : $(OBJ)
$(OBJDIR)/%.elf: $(OBJ) | $(OBJDIR)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# Compilation des fichiers sources C
$(OBJDIR)/%.o : %.c | $(OBJDIR)
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) "$(abspath $<)" -o $@

# Création des fichiers .o à partir des fichiers assembleur
$(OBJDIR)/%.o : %.S | $(OBJDIR)
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Affichage de la taille des binaires avant nouvelle compilation
sizebefore: hex
	@if test -f $(OBJDIR)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
        2>/dev/null; echo; fi
	@if test -f $(OBJDIR)/$(TARGET).hex; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); \
        2>/dev/null; echo; fi

# Affichage de la taille des binaires après nouvelle compilation
sizeafter: hex
	@if test -f $(OBJDIR)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
         2>/dev/null; echo; fi
	@if test -f $(OBJDIR)/$(TARGET).hex; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); \
         2>/dev/null; echo; fi

# Génération du fichier avr-gdb d'initialisation définissant :
#     L'envoi d'un signal de reset
#     Le chargement du fichier cible
#     La connexion à la cible
#     Et positionnement d'un point d'arrêt sur la fonction main()
gdb-config:
	@$(REMOVE) $(GDBINIT_FILE)
	@echo define reset >> $(GDBINIT_FILE)
	@echo SIGNAL SIGHUP >> $(GDBINIT_FILE)
	@echo end >> $(GDBINIT_FILE)
	@echo file $(OBJDIR)/$(TARGET).elf >> $(GDBINIT_FILE)
	@echo target remote $(DEBUG_HOST):$(DEBUG_PORT)  >> $(GDBINIT_FILE)
ifeq ($(DEBUG_BACKEND),simulavr)
    @echo load  >> $(GDBINIT_FILE)
endif
	@echo break main >> $(GDBINIT_FILE)


#-------------------------------------------------------------------------------
# Règles des sous-répertoires
#-------------------------------------------------------------------------------

# Règle de construction du répertoire des binaires
$(OBJDIR):
	@mkdir -p $@

# Règle de création du répertoires des dépendances
$(DEPSDIR):
	@mkdir -p $(DEPSDIR)


#-------------------------------------------------------------------------------
# Aide
#-------------------------------------------------------------------------------

help:
	@echo
	@echo "Usage :"
	@echo "   make all        => Compile le projet et programme le microcontrolleur"
	@echo
	@echo "   make build      => Compile le projet"
	@echo
	@echo "   make program    => Télécharge le fichier HEX dans le microcontrolleur en utilisant avrdude."
	@echo "                      (Merci de mettre à jour la configuration avrdude de ce fichier Makefile !)"
	@echo
	@echo "   make debug      => Démarre soit simulavr ou avarice comme spécifié pour le débuggage"
	@echo "                      (Avec avr-gdb ou avr-insight comme déboggeur)"
	@echo
	@echo "   make coff       => Converti le fichier ELF en fichier COFF pour AVR"
	@echo
	@echo "   make extcoff    => Converti le fichier ELF en fichier COFF étendu pour AVR"
	@echo
	@echo "   make filename.s => Comiple fichier 'filename.c' en code assembleur uniquement"
	@echo
	@echo "   make filename.i => Créé un fichier source précompilé utilisable pour les rapports de bug du porjet GCC"
	@echo
	@echo "   make clean      => Supprime tous les fichiers et répertoires de compilation"
	@echo
	@echo "Pour recompiler tout le projet, utiliser 'make clean' puis 'make all'"
	@echo

# EOF
