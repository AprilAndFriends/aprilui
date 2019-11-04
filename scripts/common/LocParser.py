# Contains generic utility classes for working with localizations files.

import os
import os.path
import re

class LocFile:

	def __init__(self, filename, language, entries):
		self.filename = filename
		self.language = language
		self.entries = entries

	def getReferenceFilename(self):
		filename = self.filename
		if self.language != "":
			filename = filename.replace("/" + self.language + "/", "/")
		return filename

	def getShortFilename(self):
		return os.path.basename(self.filename)

	def findEntry(self, key):
		for entry in self.entries:
			if entry.key == key:
				return entry
		return None

	def __repr__(self):
		return "%s<%s, %d entries>" % (self.__class__.__name__, self.filename.__repr__(), len(self.entries))

class LocEntry:

	def __init__(self, key, value, original, comment):
		self.key = key
		self.value = value.replace("­", "-").replace("–", "-").replace("´", "'").replace("’", "'").replace("`", "'").replace("‘", "'").replace("''", '"').replace("“", '"').replace("”", '"')
		self.original = original.replace("­", "-").replace("–", "-").replace("´", "'").replace("’", "'").replace("`", "'").replace("‘", "'").replace("''", '"').replace("“", '"').replace("”", '"')
		self.comment = comment

	def __repr__(self):
		result = self.key
		if self.comment != "":
			result += " (%s)" % self.comment
		result += ":%s" % self.value.__repr__()
		return "%s<%s>" % (self.__class__.__name__, result)

class LocFullFile:

	def __init__(self, path, filename, languages, entries):
		self.path = path
		self.filename = filename
		self.languages = languages
		self.entries = entries

	def getFullFilename(self):
		return (self.path + "/" + self.filename)

	def __repr__(self):
		return "%s<%s, %s, %d entries>" % (self.__class__.__name__, self.path.__repr__(), self.filename.__repr__(), len(self.entries))

class LocFullEntry:

	def __init__(self, key, values, comment):
		self.key = key
		self.values = []
		for value in values:
			self.values.append(value.replace("­", "-").replace("–", "-").replace("´", "'").replace("’", "'").replace("`", "'").replace("‘", "'").replace("''", '"').replace("“", '"').replace("”", '"'))
		self.comment = comment

	def __repr__(self):
		result = self.key
		if self.comment != "":
			result += " (%s)" % self.comment
		result += ":%d languages" % len(self.languages)
		return "%s<%s>" % (self.__class__.__name__, result)

class LocParser:

	EXTENSION = ".loc"
	BOM = b"\xEF\xBB\xBF"

	@staticmethod
	def parse(filename, language):
		entries = []
		file = open(filename, "rb")
		# the first line of a UTF8 file can be "screwed up"
		skip = 0
		line = file.readline()
		for i in line:
			if i < 128:
				break
			skip += 1
		file.seek(skip, os.SEEK_SET)
		# read data
		string = file.read().decode().replace("\r", "")
		file.close()
		# regular expressions are awesome
		regex = ""
		# at least one non-space character surrounded by space characters
		regex += " *(\S+) *"
		# non-greedy extraction of comment starting with # after the key followed by any number of spaces and then any number of characters
		regex += "(?:# *(.*))?"
		regex += "\n"
		# non-greedy extraction of value surrounded by {}, both being the only character in their lines
		regex += "\{\n((?:.|\n)*?)\n\}"
		# now using that regex to extract all localization entries
		matches = re.findall(regex, string)
		for match in matches:
			entries.append(LocEntry(match[0], match[2], match[2], match[1]))
		if language == "":
			language = os.path.basename(os.path.dirname(filename))
		return LocFile(filename, language, entries)

	@staticmethod
	def getFileList(path, language, silent = False):
		if not silent:
			print("  checking %s" % path)
		files = []
		dirs = []
		dirListing = os.listdir(path + "/")
		dirListing.sort(key = lambda filename: [int(match) if match.isdigit() else match for match in re.split('(\d+)', filename)])
		for f in dirListing:
			if f == ".svn" or f == "." or f == "..":
				continue
			name = path + "/" + f
			try:
				os.listdir(name)
				dirs.append(name)
			except:
				if name.lower().endswith(LocParser.EXTENSION.lower()) and (language == "" or path.lower().endswith("/" + language.lower())):
					files.append(name)
					if not silent:
						print("  -> %s" % LocParser._getBasename(name, path))
		if len(files) > 0:
			return files
		result = []
		for dir in dirs:
			result.extend(LocParser.getFileList(dir, language, silent))
		return result

	@staticmethod
	def joinEntries(locEntries):
		merged = []
		for locEntry in locEntries:
			if locEntry.comment != "":
				merged.append("%s # %s\n{\n%s\n}\n" % (locEntry.key, locEntry.comment, locEntry.value))
			else:
				merged.append("%s\n{\n%s\n}\n" % (locEntry.key, locEntry.value))
		result = "\n".join(merged) + "\n"
		return result

	@staticmethod
	def _getBasename(filename, path):
		return filename.replace(path + "/", "", 1)

