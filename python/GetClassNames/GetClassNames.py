# Copyright (c) 2011 Florian Muecke. All rights reserved.
#

import codecs
#import getopt
#import math    # for log
import os
#import re
#import sre_compile
#import string
import sys
import unicodedata
import pprint

_USAGE = """
Syntax: GetClassNames.py <full path to project base directory>

        Default setting: <PROJECT>_<PATH>_<FILE>_H_
        
        Examples: c:\\dev\\cpp\\MyProject

"""

_EXCLUDES={".svn"}
_FORMAT="<PROJECT>__<PATH>__<FILE>_H_"

_BASEPATH=""

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

    if len(args) < 1:
        PrintUsage('Invalid arguments.')
    
    global _BASEPATH;
    _BASEPATH = os.path.abspath(args[0])

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

def GetClassName(filename):
    """Retrieves all class definitions from that file.

    Args:
        filename: The name of the C++ header file.
    """

    classList = []
    relevantName = filename[len(_BASEPATH)+1:]
    suggestedGuard = "__".join(relevantName.split("\\",1)).upper().replace('-', '_').replace('.', '_') + '_'

    file = open(filename, 'r') # reading utf-8 kills line endings!
    lines = file.readlines()

    for linenum, line in enumerate(lines):
        linesplit = line.split()
        if len(linesplit) >= 2:
            # find the first occurrence of class
            for i in range(1, len(linesplit)):
            #for token in linesplit:
                if (linesplit[i-1] == 'class' or linesplit[i-1] == 'struct'):
                    classList.append(linesplit[i])
    file.close()
    return classList

def ProcessFile(file):
    return GetClassName(file)


def main():
    headerFiles = ParseArgs(sys.argv[1:])

    data = {'':[]}
    for filename in headerFiles:
        classList = ProcessFile(filename)
        if not classList:
            print filename + ": no class declaration found"
        else:
            data[filename] = classList

    print len(headerFiles), "files processed"

    #print data
    pp = pprint.PrettyPrinter(indent=4)
    pp.pprint(data)

    #sys.exit(0)


main() 
