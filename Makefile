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

.PHONY: push
push:
	-cd html && git add .
	-cd html && git commit -m "Update doc"
	-cd html && git push origin gh-pages
