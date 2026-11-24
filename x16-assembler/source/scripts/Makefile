# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	Makefile for scripts
#		Date :		11th March 2025
#		Author : 	Paul Robson (paul@robsons.org.uk)
#       Modified by Pete Wyspianski (AKA Gollan petegollan@outlook.com)
#       November 2026 for the X16-Assembler in ROM project
#
# *******************************************************************************************
# *******************************************************************************************

PYTHON = python3
BUILDDIR = ../../build/

.any:

#
# These output files to source/assembler/generated
#
build:	.any
	$(PYTHON) exprtable.py
	$(PYTHON) makeasmtable816.py
	$(PYTHON) makelib.py > $(BUILDDIR)libassembler.asm
	@echo "Scripts run successfully."