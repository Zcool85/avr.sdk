# Répertoires
DOCSDIR           := docs
HTMLDIR           := html

# outils
RM                := rm
DOXYGEN           := doxygen
GIT               := git

all: doc

.PHONY: doc
doc:
	@$(DOXYGEN) $(DOCSDIR)/doxygen.config

.PHONY: clean
clean:
	rm -Rf $(HTMLDIR)

.PHONY: push
push:
	@cd $(HTMLDIR) && $(GIT) add .
	@cd $(HTMLDIR) && $(GIT) commit -m "Update doc"
	@cd $(HTMLDIR) && $(GIT) push origin gh-pages
