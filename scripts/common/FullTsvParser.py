# Contains utility classes for working with merged Full-TSV localization files.

import os.path
import re

from LocParser import *

class FullTsvParser:

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
		string = file.read().replace("\r", "")
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
		regex += "(?:" + entry + FullTsvParser.DELIMITER + entry + FullTsvParser.DELIMITER + entry + FullTsvParser.DELIMITER + entry + "\n)"
		# now using that regex to extract all entries
		matches = re.findall(regex, string)
		# extracting LocFile instances
		locFiles = []
		locFile = None
		for match in matches:
			key, value, original, comment = match
			columns = [key, value, original, comment]
			for i in xrange(len(columns)):
				if columns[i].startswith("\"") and columns[i].endswith("\""):
					columns[i] = columns[i][1:-1].replace("\"\"", "\"")
				# this feature has been disabled, because it breaks other spreadsheet software
				# changes back the MS Excel fix for "-"
				#columns[i] = columns[i].replace(FullTsvParser.DASH, "-")
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
		languages = []
		if len(locFiles) > 0:
			languages = locFiles[0].languages
		result = ""
		result += FullTsvParser._makeEntry("Key", "Comment", languages)
		for locFile in locFiles:
			result += "\n"
			result += FullTsvParser._makeEntry("###", locFile.getFullFilename(), languages)
			result += "\n"
			for locEntry in locFile.entries:
				result += FullTsvParser._makeEntry(locEntry.key, locEntry.comment, locEntry.values)
		return result
		
	@staticmethod
	def _makeEntry(key, comment, values):
		finalValues = []
		for value in values:
			finalValues.append(value.replace("\"", "\"\""))
		#'"' + ('"' + FullTsvParser.DELIMITER + '"').join(finalValues) + '"'
		return '"%s"%s"%s"%s"%s"\n' % (key.replace("\"", "\"\""), FullTsvParser.DELIMITER, comment.replace("\"", "\"\""), FullTsvParser.DELIMITER,
			('"' + FullTsvParser.DELIMITER + '"').join(finalValues))
	
