# DuploQ - GUI for finding duplicates in source files

[![Download DuploQ](https://img.shields.io/sourceforge/dt/duploq.svg)](https://sourceforge.net/projects/duploq/files/latest/download)

[![Download DuploQ](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/duploq/files/latest/download)

DuploQ is a GUI frontend for [Duplo](https://github.com/dlidstrom/Duplo) duplicate finder console tool.
Its goal is to find duplicates (i.e. copy-pasted parts of code) across several source files.

Locating and eliminating source code duplicates is an important part of the overall refactoring process which leads to decreasing
a project's technical dept and increasing stability of the software.

![duploq-0 1-linux-1](https://user-images.githubusercontent.com/70297699/91775216-d4020c00-ebea-11ea-9b18-ceeb7e05cc2b.PNG)

## Supported languages
Currently DuploQ directly supports C, C++, C#, Java and JavaScript languages, but it can be used for other languages as well.

## Supported platforms
Currently, Microsoft Windows and Linux platforms are supported.

Since DuploQ is based on Qt C++ framework, it can easily be ported across several platforms.

## Installation
DuploQ could be either compiled & installed from sources or by installer program (Windows) or package manager (Linux).

### Binaries
The latest binaries can be downloaded from [releases page](https://github.com/duploq/duploq/releases).

### Sources
To install DuploQ from sources:
1. Make sure that you have _Qt framework_ and _C++ compiler_ installed on your system.
2. Download and extract source code package in a directory where it has to be built (let's name it SOURCE_DIR).
3. Go into SOURCE_DIR/src directory and run _qmake -r_.
4. Run _make_ (if under Linux) or _nmake_ (if using MSVC under Windows) or _mingw32-make_ (if using MinGW under Windows) etc.

After the DuploQ has been built, you can install it running _sudo make install_ (under Linux).

## Workflow
DuploQ's approach is a pretty straighforward.

First, DuploQ allows you to choose where to look for the duplicates (files or folders).

Then it builds list of input files and passes it to the Duplo engine together with necessary parameters.

After the files have been processed, DuploQ parses Duplo's output and visualises the results in easy and intuitive way.
Also it provides additional statistics information which is not a part of Duplo output.

DuploQ shares the results across several views: file-based statistics, side-by-side view, block-based view, raw engine output.

**File-based view** displays a list of files which contain duplicated blocks, their amount and size (duplicated lines of code).
The results are sorted to let you immediately see which files include the longest duplicated blocks.
For every file, internal (i.e. inside the same file) and external duplicates (i.e. across more files) are represented.

**Side-by-side view** allows to navigate through the files containing duplicates to see them more in detail. 
The duplicated blocks are visually highlighted so you can easily see what exactly has been copy-pasted.

**Block-based output** shows you all the found duplicated blocks in a hierarchical structure.
It allows you to see which parts of code are repeated at most.

## Links

DuploQ page on [Softpedia](https://www.softpedia.com/get/Programming/Other-Programming-Files/DuploQ.shtml)

DuploQ mirror on [![Download DuploQ](https://sourceforge.net/sflogo.php?type=14&group_id=3265441)](https://sourceforge.net/p/duploq/)

