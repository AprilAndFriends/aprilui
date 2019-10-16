# Contains utility classes for working with merged TSV localization files.

import os.path
import re

from LocParser import *

class TsvParser:

	DELIMITER = "\t"
	DASH = "\xC2\xAD" # the "other" dash character in UTF-8

	@staticmethod
	def parse(filename):
		entries = []
		file = open(filename, "r")
		# the first line of a UTF8 file can be "screwed up"
		skip = 0
		line = file.readline()
		for i in line:
			if ord(i) < 128:
				break
			skip += 1
		file.seek(skip, os.SEEK_SET)
		# read data
		string = file.read().decode().replace("\r", "")
		file.close()
		# regular expressions are awesome
		regex = ""
		# normal entry
		normal = "(?:.*?)"
		# special entry with escaped characters and \n support
		special = "\"(?:.|\n)*?\""
		# one entry is either normal or special
		entry = "(" + normal + "|" + special + ")?"
		# the regex is 4 entries separated by delimiter characters and ending with \n
		regex += "(?:" + entry + TsvParser.DELIMITER + entry + TsvParser.DELIMITER + entry + TsvParser.DELIMITER + entry + "\n)"
		# now using that regex to extract all entries
		matches = re.findall(regex, string)
		# extracting LocFile instances
		locFiles = []
		locFile = None
		for match in matches:
			key, value, original, comment = match
			columns = [key, value, original, comment]
			for i in range(len(columns)):
				if columns[i].startswith("\"") and columns[i].endswith("\""):
					columns[i] = columns[i][1:-1].replace("\"\"", "\"")
				# this feature has been disabled, because it breaks other spreadsheet software
				# changes back the MS Excel fix for "-"
				#columns[i] = columns[i].replace(TsvParser.DASH, "-")
			if columns[0] == "###":
				if locFile != None:
					locFiles.append(locFile)
				language = columns[2]
				if language == "###":
					language = ""
				locFile = LocFile(columns[3], language, []) # cannot make [] a default argument because there's a bug in Python
			elif columns[0] != "" and locFile != None:
				locFile.entries.append(LocEntry(columns[0], columns[1], columns[2], columns[3]))
		if locFile != None:
			locFiles.append(locFile)
		return locFiles
	
	@staticmethod
	def generateFile(locFiles):
		result = ""
		result += TsvParser._makeEntry("Key", "Translation", "Original", "Comment")
		for locFile in locFiles:
			result += TsvParser._makeEntry("", "", "", "")
			language = locFile.language
			if language == "":
				language = "###"
			result += TsvParser._makeEntry("###", "###", language, locFile.filename)
			result += TsvParser._makeEntry("", "", "", "")
			for locEntry in locFile.entries:
				key, value, original, comment = locEntry.key, locEntry.value, locEntry.original, locEntry.comment
				columns = [key, value, original, comment]
				# this feature has been disabled, because it breaks other spreadsheet software
				#for i in range(len(columns)):
				#	# prevents MS Excel from seeing "-" as a formula indicator
				#	columns[i] = columns[i].replace("-", TsvParser.DASH)
				result += TsvParser._makeEntry(columns[0], columns[1], columns[2], columns[3])
		return result
		
	@staticmethod
	def _makeEntry(key, translation, original, comment):
		return "\"%s\"%s\"%s\"%s\"%s\"%s\"%s\"\n" % (key.replace("\"", "\"\""), TsvParser.DELIMITER, translation.replace("\"", "\"\""),
			TsvParser.DELIMITER, original.replace("\"", "\"\""), TsvParser.DELIMITER, comment.replace("\"", "\"\""))
	
