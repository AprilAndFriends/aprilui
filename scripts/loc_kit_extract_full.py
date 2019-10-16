# Localization Full-TSV Extractor

import os
import sys

from common.LocKit import *

VERSION = "1.0"

def process():
	print("-------------------------------------------------------------------------------")
	print("| AprilUI Localization Full-TSV Extractor " + VERSION)
	print("-------------------------------------------------------------------------------")
	
	if len(sys.argv) != 3:
		help()
		return
	path = sys.argv[1]
	language = ""
	inputFilename = sys.argv[2]
	
	locFiles = LocKit.readFullTsvFile(inputFilename)
	LocKit.writeLocFiles(path, locFiles)
	
def help():
	print("")
	print("Splits localization TSV file into localization files (as defined by AprilUI).")
	print("See examples/merged.txt for reference.")
	print("")
	print("usage:   loc_kit_extract_tsv.py PATH TSV_FILENAME")
	print("")
	print("  PATH          - Directory path to for .loc file output.")
	print("  TSV_FILENAME  - Filename of the input TSV file.")
	print("")
	print("example: loc_kit_extract_full.py ../data/localization locale.txt")
	print("")
	os.system("pause")

process()
