# Localization TSV Creator

import os
import sys

from common.LocKit import *

VERSION = "2.2"

OUTPUT_FILENAME = "_loc_kit_output.txt"

def process():
	print "-------------------------------------------------------------------------------"
	print "| AprilUI Localization TSV Creator " + VERSION
	print "-------------------------------------------------------------------------------"
	
	if len(sys.argv) != 2 and len(sys.argv) != 4:
		help()
		return
	path = sys.argv[1]
	language = ""
	originalLanguage = ""
	if len(sys.argv) == 4:
		language = sys.argv[2]
		originalLanguage = sys.argv[3]
		
	locFiles = LocKit.readLocFiles(path, language)
	originalLocFiles = None
	if originalLanguage != language:
		originalLocFiles = LocKit.readLocFiles(path, originalLanguage)
		locFiles = LocKit.insertOriginalLocFiles(locFiles, originalLocFiles)
	LocKit.writeTsvFile(OUTPUT_FILENAME, locFiles)
		
def help():
	print ""
	print "Exports localization files (as defined by AprilUI) into a single TSV file."
	print "See examples/generic.loc for reference."
	print ""
	print "usage:   loc_kit_create_tsv.py PATH [LANG ORG_LANG]"
	print ""
	print "  PATH      - Directory path to use as root recursive starting point."
	print "  LANG      - Specific directory prefix for the language files."
	print "              e.g. 'de' would cause only files that look like"
	print "              '*de/FILENAME.EXTENSION' to be read."
	print "  ORG_LANG  - Specific directory prefix for the original language files."
	print "              e.g. 'en' would cause only files that look like"
	print "              '*en/FILENAME.EXTENSION' to be read."
	print ""
	print "example: loc_kit_create_tsv.py ../../data/localization de en"
	print ""
	os.system("pause")

process()
