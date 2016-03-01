# Contains generic utility classes for cags scripts.

import os

class Util:
	
	@staticmethod
	def getFileList(path):
		print "  checking %s" % path
		files = []
		dirs = []
		for f in os.listdir(path + "/"):
			if f == ".svn" or f == "." or f == "..":
				continue
			name = path + "/" + f
			try:
				os.listdir(name)
				dirs.append(name)
			except:
				files.append(name)
		if len(files) > 0:
			return files
		list = []
		for dir in dirs:
			list.extend(Util.getFileList(dir))
		return list
		
