# AppleSAWS
##Apple Software Analysis WorkShop

This is a toolkit for exploring DOS 3.3 disk contents. I started this because I had become interested in a game that I used to play a long time ago and I wanted to dig deeper into the internals of how it worked. Since it was a nice combination of a series of Applesoft and Binary files, I decided that a tool to help explore those types of things would be handy. Of course, I've ended up spending far more time writing the tool than I have actually exploring the game, but nonetheless, it's been a fun ongoing project.

This code is being developed using Qt5, with development work being done on Linux, but it should compile and run without problems on Windows and Mac.

Among other things, the app includes:
* Code that reads and parses DOS-format disk images at the disk-structure level. (It's just read-only for now, and ProDOS is on the horizon)
* An Applesoft parser/retokenizer/reformatter which can show code in its normal form or reindented on multiple lines with syntax highlighting, and can optionally show integers as hex values inline. The retokenizer creates new tokens for strings, variable names, numeric values, DATA payloads, REM remarks, and other things that are nice to have -- and sets the stage to create an analyzer for deeper processing of the code later on.
* A binary file processor with a flow-tracing disassembler, which can show hex dumps and disassembly of code and data blocks (it also temporarily has some common labels for well-known addresses hard-coded into the display, though dynamic labeling of symbols is in the works.) It can also display the relocation table for type-R relocatable files.  It also displays a linear graph of jumps, subroutines, and branches!
* A Text file viewer.
* A graphics viewer which can show hires screen dumps as monochrome, with NTSC artifacts, or a hybrid mode where each pixel's natural color is shown in-place (with no white color blending.)
* An HRCG character set viewer and explorer
* Exporting files and graphics to the local filesystem (as either .txt or .png files, as appropriate)
* A disk explorer which allows viewing the disk at the sector level, exploring directory and VTOC structure, and more!
* (Though not useful to most) A maze viewer for the game which I was originally exploring (Datamost's *The Missing Ring*, by Terry Romine, 1982)
* Probably some other stuff I can't think of at the moment.

There is also a Hex/Decimal converter utility and some in-app documentation for HRCG control character codes. Mainly stuff that I found useful at the time I was writing it.

In the pipeline are some more useful features (as I get a chance):
* Syntax Highlighting **(Currently in progress!)**
* More low-level disk exploration tools (sector-level stuff, comparing DOS versions between disks, etc.)
* More analysis of Applesoft programs (Tracing flow control; determining unaccessable lines of code; finding addresses of PEEKS, POKES, CALLS, USRs, and &'s; determining "hidden" areas of memory stuffed between lines or after the end of the program itself)
* Processing and analysis of Integer Basic files
* Much more robust disassembly tools:
  * Handling differences between 6502/65C02/65816 ops, as well as "undocumented" ops on the original 6502
  * Automated detection of Code Segments vs Data Segments **(Currently in progress!)**
  * Allowing user-defined labels for addresses **(Currently in progress!)**
* Shape table processing and viewing
* ProDOS support
* Modification/Writing disk images
* Lots of optimization and refactoring -- **(Ongoing)**
* Tools for processing DHGR graphics
* Introduction of project management, allowing annotation and documention of interconnectivity between files and disk image contents, as well as preferences of groups of default viewers to open for a given image or set of images **(In progress!)**
* Plenty of other stuff!

Anyway, if you take a look at it and think it's interesting, I'd love to hear some feedback.

![Screenshot](/image/AppleSAWS_screenshot.png)
    
