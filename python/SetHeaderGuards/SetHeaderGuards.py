# Copyright (c) 2011 Florian Muecke. All rights reserved.
#

import codecs
#import getopt
#import math	# for log
import os
#import re
#import sre_compile
#import string
import sys
import unicodedata


_USAGE = """
Syntax: SetHeaderGuards.py <mode> <full path to project base directory>

		Mode: --list or --write

		Default setting: <PROJECT>_<PATH>_<FILE>_H_
		
		Examples: --list c:\\dev\\cpp\\MyProject

"""

_EXCLUDES={".svn"}
_FORMAT="<PROJECT>__<PATH>__<FILE>_H_"

_BASEPATH=""
_MODE=""

def PrintUsage(message):
	"""Prints a brief usage string and exits, optionally with an error message.

	Args:
		message: The optional error message.
	"""
	sys.stderr.write(_USAGE)
	if message:
		sys.exit('\nFATAL ERROR: ' + message)
	else:
		sys.exit(1)


def ParseArgs(args):
	"""Parses the command line arguments.

	Args:
		args: The command line arguments:

	Returns:
		The list of filenames to handle.
	"""

	if len(args) < 2:
		PrintUsage('Invalid arguments.')
	
	global _BASEPATH;
	_BASEPATH = os.path.abspath(args[1])

	#if len(_BASEPATH) < 3:
	#		print "ERROR: dir is not absolute"
	#		PrintUsage('Invalid arguments.')

	global _MODE;
	_MODE = args[0]

	return GetFileList(_BASEPATH) 


def GetFileList(path):
	"""Updates the global list of header files.

	Args:
		path: the base directory to search for header files.
	"""
	fileList = []
	for root, subFolders, files in os.walk(path):
		for folder in subFolders:
			if folder in _EXCLUDES:
				subFolders.remove(folder)
		for file in files:
			ext = file[file.rfind('.') + 1:] 
			if ext == "h" or ext == "H":
				fileList.append(os.path.join(root,file))

	return fileList

def GetHeaderGuard(filename):
	"""Checks that the file contains a header guard.

	Args:
		filename: The name of the C++ header file.
	"""

	relevantName = filename[len(_BASEPATH)+1:]
	suggestedGuard = "__".join(relevantName.split("\\",1)).upper().replace('-', '_').replace('.', '_') + '_'

	file = open(filename, 'r') # reading utf-8 kills line endings!
	lines = file.readlines()

	guardIfndef = None
	guardIfndef_lineNum = 0
	guardDefine = None
	guardEndif = None
	guardEndif_lineNum = 0
	for linenum, line in enumerate(lines):
		linesplit = line.split()
		if len(linesplit) >= 2:
			# find the first occurrence of #ifndef and #define, save arg
			if not guardIfndef and linesplit[0] == '#ifndef':
				# set ifndef to the header guard presented on the #ifndef line.
				guardIfndef = linesplit[1]
				guardIfndef_lineNum = linenum
			if not guardDefine and linesplit[0] == '#define':
				guardDefine = linesplit[1]
		# find the last occurrence of #endif, save entire line
		if line.startswith('#endif'):
			guardEndif = line
			guardEndif_lineNum = linenum

	file.close()

	if not guardIfndef or not guardDefine or guardIfndef != guardDefine:
		print filename, ': Header guard not found! Use '+ suggestedGuard
		file.close()
		return

	if _MODE == '--write':
		# strip guard lines
		lines = lines[0:guardIfndef_lineNum] + lines[guardIfndef_lineNum+2:guardEndif_lineNum]
		# add new guard
		lines = ['#ifndef ' + suggestedGuard + '\n#define ' + suggestedGuard + '\n'] \
			+ lines \
			+ ['#endif  // ' + suggestedGuard + '\n']
		file = open(filename, 'w') 
		file.writelines(lines)
		file.close()

	if guardIfndef != suggestedGuard:
		print guardIfndef + ' --> ' + suggestedGuard
	else:
		print 'Ok: ' + guardIfndef


def ProcessFile(file):
	GetHeaderGuard(file)


def main():
	headerFiles = ParseArgs(sys.argv[1:])

	for filename in headerFiles:
		ProcessFile(filename)
	print len(headerFiles), "files processed"

	#sys.exit(0)


main() 
