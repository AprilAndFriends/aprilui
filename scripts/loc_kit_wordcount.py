# Localization Word Counter

import os
import re
import sys

from common.LocKit import *

VERSION = "2.3"

EXTENSION = ".loc"
DEFAULT_PATH = "."
DEFAULT_LANGUAGE = ""

def process():
	print "-------------------------------------------------------------------------------"
	print "| AprilUI Localization Word Counter " + VERSION
	print "-------------------------------------------------------------------------------"
	
	if len(sys.argv) != 2 and len(sys.argv) != 3:
		help()
		return
	path = sys.argv[1]
	language = ""
	if len(sys.argv) == 3:
		language = sys.argv[2]
	
	print ""
	print "Searching with parameters..."
	print "- path: %s" % path
	print "- language: %s" % language
	locFiles = LocKit.readLocFiles(path, language)
	
	print ""
	print "Analyzing of %d file(s)..." % len(locFiles)
	count = 0
	words = 0
	longest = 0
	for locFile in locFiles:
		print "  - %s  (%d entries)" % (locFile.filename, len(locFile.entries))
		for locEntry in locFile.entries:
			length = len(re.findall(r"\w+", locEntry.value))
			words += length
			longest = max(longest, length)
		count += len(locFile.entries)
	
	print ""
	print "Statistics:"
	print "  - entries: %d" % count
	print "  - words: %d" % words
	print "  - longest entry: %d" % longest

def help():
	print ""
	print "Counts the entries and words in localization files as defined by AprilUI."
	print "See examples/generic.loc for reference."
	print ""
	print "usage:   loc_kit_wordcount.py PATH [LANGUAGE]"
	print ""
	print "  PATH     - Directory path to use as root recursive starting point."
	print "  LANGUAGE - Specific directory prefix for the original language files."
	print "             e.g. 'en' would cause only files that look like"
	print "             '*en/FILENAME.EXTENSION' to be read."
	print ""
	print "example: loc_kit_wordcount.py ../../data/localization en"
	print ""
	os.system("pause")
	
process()
