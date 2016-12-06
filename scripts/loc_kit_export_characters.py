# Localization UTF-8 Character Exporter

import os
import sys

from common.LocKit import *

VERSION = "1.0"
DATA_DIR_LOCATION = "../../../data"

def process():
	print "-------------------------------------------------------------------------------"
	print "| AprilUI Localization UTF-8 Character Exporter " + VERSION
	print "-------------------------------------------------------------------------------"

	if len(sys.argv) != 2:
		help()
		return
	language = sys.argv[1]
	characterString = ""

	outputFilename = "output_" + language + ".txt"

	file = open(outputFilename, "w")
	i = 0
	locFiles = LocKit.readLocFiles(DATA_DIR_LOCATION, language)
	for locFile in locFiles:
		for entry in locFile.entries:
			# print entry.value
			for character in entry.value.decode("utf-8"):
				if not character in characterString.decode("utf-8") and ord(character) >= 128:
					print character.encode("utf-8") + " " + str(ord(character))
					characterString += character.encode("utf-8")
					spaces = "    "
					if i < 10:
						spaces += " "
					file.write(str(i) + ":" + spaces + character.encode("utf-8") + "    [" + str(ord(character)) + "]\n")
					i += 1
	file.close()

def help():
	print ""
	print "Exports UTF-8 character list for a given language."
	print ""
	print "usage:   loc_kit_export_characters.py LANG"
	print ""
	print "  LANG      - Specific directory prefix for the language files."
	print "              e.g. 'de' would cause only files that look like"
	print "              '*de/FILENAME.EXTENSION' to be read."
	print ""
	print "example: loc_kit_export_characters.py pl"
	print ""
	os.system("pause")

process()