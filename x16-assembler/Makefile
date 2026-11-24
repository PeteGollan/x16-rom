# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	The top-level make (called by the x16-rom make system)
#		Date :		11th March 2025
#		Author : 	Paul Robson (paul@robsons.org.uk)
#       Modified by Pete Wyspianski (AKA Gollan petegollan@outlook.com)
#       November 2026 for the X16-Assembler in ROM project
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#
#										Configuration
#
# *******************************************************************************************

PYTHON = python3
MAKE = make --no-print-directory 
EMULATOREXEC = x16emu

# *******************************************************************************************
#
#										Directories
#
# *******************************************************************************************
#
#  		Artefacts of builds.
#
ROOTDIR = ../

BUILDDIR = build/
#
#  Config files.
#
CONFDIR = conf/
#
#  Source related files.
#
SOURCEDIR = source/
#
# Python scripts.
#
SCRIPTDIR = $(SOURCEDIR)scripts/
#
#
#		Run the assembler
#
ASSEMBLER = cl65 --asm-define DEBUG=0 --asm-args -g -o $(BUILDDIR)asm-rom.bin -t cx16 -C $(CONFDIR)x16asm-rom.cfg --mapfile $(BUILDDIR)asm-rom.map -Ln $(BUILDDIR)asm-rom.sym -l $(BUILDDIR)asm-rom.lst $(SOURCEDIR)main.asm
DEBUG_ASSEMBLER = cl65 --asm-define DEBUG=1 --asm-args -g -o $(BUILDDIR)asm-rom.bin -t cx16 -C $(CONFDIR)x16asm-rom.cfg --mapfile $(BUILDDIR)asm-rom.map -Ln $(BUILDDIR)asm-rom.sym -l $(BUILDDIR)asm-rom.lst $(SOURCEDIR)main.asm

build: status clean prelim 
	$(ASSEMBLER)
	echo "X16-Assembler built successfully."
 
debug: status clean prelim 
	$(DEBUG_ASSEMBLER)
	echo "X16-Assembler DEBUG built successfully."

# We should probably clean up some things here:
clean:
	rm -f $(BUILDDIR)asm.bin
	rm -f $(BUILDDIR)libassembler.asm
	rm -f $(BUILDDIR)*.lst
	rm -f $(BUILDDIR)*.map
	rm -f $(BUILDDIR)*.sym

prelim:
	rm -f dump*.bin
	$(MAKE) -C $(SCRIPTDIR) build

status:
	@echo "building X16-Assembler..."

# *******************************************************************************************
#
#				    Uncommenting .SILENT will shut the whole build up.
#
# *******************************************************************************************

ifndef VERBOSE
.SILENT:
endif

default: build 

