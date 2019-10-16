# Localization Analyzer

import os
import sys

from common.LocKit import *

OUTPUT_FILENAME = "_loc_kit_output.txt"
OUTPUT_FILENAME_REMOVED = "_loc_kit_removed.txt"

VERSION = "2.3"

def process():
	print("-------------------------------------------------------------------------------")
	print("| AprilUI Localization TSV Differentiator " + VERSION)
	print("-------------------------------------------------------------------------------")
	
	if len(sys.argv) < 4 or len(sys.argv) > 5:
		help()
		return
	path = sys.argv[1]
	language = sys.argv[2]
	originalLanguage = sys.argv[3]
	changedKeys = []
	if len(sys.argv) == 5:
		file = open(sys.argv[4])
		changedKeys = file.read().decode().replace("\r", "").split("\n")
		file.close()
		while "" in changedKeys:
			changedKeys.remove("")
	
	locFiles = LocKit.readLocFiles(path, language)
	originalLocFiles = LocKit.readLocFiles(path, originalLanguage)
	added = LocKit.createDifferenceLocFiles(locFiles, originalLocFiles, originalLanguage, changedKeys)
	LocKit.writeTsvFile(OUTPUT_FILENAME, added)
	removed = LocKit.createDifferenceLocFiles(originalLocFiles, locFiles, language, [])
	LocKit.writeTsvFile(OUTPUT_FILENAME_REMOVED, removed)
				
def help():
	print("")
	print("Generates localization file differences (as defined by AprilUI) into a single TSV file.")
	print("See examples/generic.loc for reference.")
	print("")
	print("usage:   loc_kit_diff_tsv.py PATH LANG ORG_LANG [CHANGED_KEYS]")
	print("")
	print("  PATH         - Directory path to use as root recursive starting point.")
	print("  LANG         - Specific directory prefix for the language files.")
	print("                 e.g. 'de' would cause only files that look like")
	print("                 '*de/FILENAME.EXTENSION' to be read.")
	print("  ORG_LANG     - Specific directory prefix for the original language files.")
	print("                 e.g. 'en' would cause only files that look like")
	print("                 '*en/FILENAME.EXTENSION' to be read.")
	print("  CHANGED_KEYS - Filename that contains a list of keys that have been altered")
	print("                 in the original language.")
	print("                 See examples/changed_keys.txt for reference.")
	print("")
	print("example: loc_kit_diff_tsv.py ../../data/localization de en changed_keys.txt")
	print("")
	os.system("pause")

process()
