# Localization Analyzer

import os
import sys

from common.LocKit import *

VERSION = "2.2"

def process():
	print "-------------------------------------------------------------------------------"
	print "| AprilUI Localization Key Renamer " + VERSION
	print "-------------------------------------------------------------------------------"
	
	if len(sys.argv) != 4:
		help()
		return
	path = sys.argv[1]
	language = sys.argv[2]
	
	file = open(sys.argv[3], "rb")
	# the first line of a UTF8 file can be "screwed up"
	skip = 0
	line = file.readline()
	for i in line:
		if ord(i) < 128:
			break
		skip += 1
	file.seek(skip, os.SEEK_SET)
	# read data
	string = file.read().replace("\r", "")
	file.close()
	renamedKeys = {}
	print "Keys to rename:"
	for line in string.split("\n"):
		pair = line.split("\t")
		while "" in pair:
			pair.remove("")
		if len(pair) >= 2:
			renamedKeys[pair[0]] = pair[1]
			print " - " + pair[0] + " -> " + pair[1]
	
	locFiles = LocKit.readLocFiles(path, language)
	locFiles = LocKit.renameKeys(locFiles, renamedKeys)
	LocKit.writeLocFiles(path, locFiles)
				
def help():
	print ""
	print "Renames localization keys (as defined by AprilUI) automated by a definition file."
	print "See examples/generic.loc for reference."
	print ""
	print "usage:   loc_kit_rename_keys.py PATH LANG ORG_LANG [RENAMED_KEYS]"
	print ""
	print "  PATH         - Directory path to use as root recursive starting point."
	print "  LANG         - Specific directory prefix for the language files."
	print "                 e.g. 'de' would cause only files that look like"
	print "                 '*de/FILENAME.EXTENSION' to be read."
	print "  RENAMED_KEYS - Filename that contains a list of keys that have been altered"
	print "                 in the original language. Each rename is in a separate line"
	print "                 and the format is 'OLD_KEY_NAME\\tNEW_KEY_NAME' where '\\t'"
	print "                 is a tab character."
	print "                 See examples/renamed_keys.txt for reference."
	print ""
	print "example: loc_kit_rename_keys.py ../../data/localization en renamed_keys.txt"
	print ""
	os.system("pause")

process()
