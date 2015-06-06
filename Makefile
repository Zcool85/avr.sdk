# Répertoires
DOCDIR            := doc

# outils
RM                := rm
DOXYGEN           := doxygen

all: doc

.PHONY: doc
doc:
	-$(RM) -Rf $(DOCDIR)
	-$(DOXYGEN) doxygen.config

