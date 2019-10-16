# Localization XLS Extractor

import os
import sys

from common.LocKit import *

VERSION = "2.2"

def process():
	print("-------------------------------------------------------------------------------")
	print("| AprilUI Localization XLS Extractor " + VERSION)
	print("-------------------------------------------------------------------------------")
	
	if len(sys.argv) != 3:
		help()
		return
	path = sys.argv[1]
	language = ""
	inputFilename = sys.argv[2]
	
	locFiles = LocKit.readXlsFile(inputFilename)
	LocKit.writeLocFiles(path, locFiles)
	
def help():
	print("")
	print("Splits localization XLS file into localization files (as defined by AprilUI).")
	print("See examples/merged.txt for reference.")
	print("")
	print("usage:   loc_kit_extract_tsv.py PATH TSV_FILENAME")
	print("")
	print("  PATH          - Directory path to for .loc file output.")
	print("  TSV_FILENAME  - Filename of the input XLS file.")
	print("")
	print("example: loc_kit_extract_xls.py ../data/localization locale.txt")
	print("")
	os.system("pause")

process()
