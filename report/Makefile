TARGET=report

PDF=$(TARGET).pdf
ASPELL=aspell
LATEX=pdflatex --interaction=nonstopmode
BIBTEX=bibtex
VIEWER=evince

.PHONY: all
.PRECIOUS: %.aux %.bbl
all: $(PDF)

.PHONY: showpdf
showpdf: all
	$(VIEWER) $(PDF) &

.PHONY: spellcheck
spellcheck: $(TARGET).tex
	$(ASPELL) -t -c $<

%.pdf : %.tex %.bbl
	while( $(LATEX) $<; grep -q "Rerun to get cross" $(basename $<).log); do true; done
			
%.bbl : %.aux
	-$(BIBTEX) $(basename $<)

%.aux : %.tex
	$(LATEX) $<

.PHONY: clean
clean:
	rm -rf $(addprefix $(TARGET).,pdf aux log bbl blg dvi brf out) *~
	
clean-%:
	rm -rf $(addprefix $(*).,pdf aux log bbl blg dvi brf out) *~
