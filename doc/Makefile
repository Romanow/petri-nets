# Tools
PDFLATEX=pdflatex -interaction=nonstopmode

# Output file
PDF=rpz.pdf

# Input paths
TEX=tex
INCLUDE=$(TEX)/inc

# Input files
MAINTEX=rpz
BIBFILE=$(TEX)/rpz.bib
PREAMBLE=preamble-std.tex
STYLES=$(TEX)/GostBase.clo $(TEX)/G7-32.sty $(TEX)/G7-32.cls $(TEX)/G2-105.sty
PARTS_TEX = $(wildcard $(TEX)/[0-9][0-9]-*.tex)

all: $(PDF)

.PHONY: all clean

$(PDF): $(TEX)/$(MAINTEX).tex $(STYLES) $(BIBFILE)
	cd tex && $(PDFLATEX) $(MAINTEX) && bibtex $(MAINTEX) && $(PDFLATEX) $(MAINTEX) && $(PDFLATEX) $(MAINTEX) && cp $(PDF) ..

clean:
	find $(TEX)/ -regextype posix-egrep -type f ! -regex ".*\.(sty|tex|clo|cls|bib|bst|gitignore)" -exec $(RM) {} \; ;
