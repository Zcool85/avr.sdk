# Répertoires
DOCSDIR           := docs
HTMLDIR           := html

# outils
RM                := rm
DOXYGEN           := doxygen

all: doc

.PHONY: doc
doc:
	-$(DOXYGEN) docs/doxygen.config
