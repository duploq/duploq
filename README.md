# DuploQ
DuploQ is a GUI frontend for [Duplo](https://github.com/dlidstrom/Duplo) duplicate finder console tool.
Its goal is to find duplicates (i.e. copy-pasted parts of code) across several source files.

![duploq-0 1-linux-1](https://user-images.githubusercontent.com/70297699/91775216-d4020c00-ebea-11ea-9b18-ceeb7e05cc2b.PNG)

Currently DuploQ directly supports C, C++, C#, Java and JavaScript languages, but it can be used for other languages as well.

DuploQ's approach is a pretty straighforward.
First, DuploQ allows you to choose where to look for the duplicates (files or folders).
Then it builds list of input files and passes it to the Duplo engine together with necessary parameters.
After the files have been processed, DuploQ parses Duplo's output and visualises the results in easy and intuitive way.
Also it provides additional statistics information which is not a part of Duplo output.

DuploQ shares the results across several views: file-based statistics, side-by-side view, block-based view, raw engine output.

File-based view displays a list of files which contain duplicated blocks, their amount and size (duplicated lines of code).
The results are sorted to let you immediately see which files include the longest duplicated blocks.
For every file, internal (i.e. inside the same file) and external duplicates (i.e. across more files) are represented.

Side-by-side view allows to navigate through the files containing duplicates to see them more in detail. 
The duplicated blocks are visually highlighted so you can easily see what exactly has been copy-pasted.

Block-based output shows you all the found duplicated blocks in a hierarchical structure.
It allows you to see which parts of code are repeated at most.

DuploQ is based on Qt C++ framework so it can easily be ported across several platforms.
