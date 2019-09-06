# Localization Full Creator

import os
import sys

from common.LocKit import *

VERSION = "1.0"

OUTPUT_FILENAME = "_loc_kit_output.txt"

def process():
	print "-------------------------------------------------------------------------------"
	print "| AprilUI Localization TSV Creator " + VERSION
	print "-------------------------------------------------------------------------------"
	
	if len(sys.argv) != 3:
		help()
		return
	path = sys.argv[1]
	baseLanguage = sys.argv[2]
	fullLocFiles = LocKit.readFullLocFiles(path, baseLanguage)
	LocKit.writeFullTsvFile(OUTPUT_FILENAME, fullLocFiles)
		
def help():
	print ""
	print "Exports all localization files (as defined by AprilUI) into a single TSV file."
	print "See examples/generic.loc for reference."
	print ""
	print "usage:   loc_kit_create_full.py PATH BASE_LANG"
	print ""
	print "  PATH      - Directory path to use as root recursive starting point."
	print "  BASE_LANG - Specific directory prefix for the base language files."
	print "              e.g. 'en' would make English the base language"
	print ""
	print "example: loc_kit_create_full.py ../../data en"
	print ""
	os.system("pause")

process()
