                              asciigraph
                              ==========

Author: Lukas Lazarek
Date: 2016-06-04 12:13:48 AWST


Table of Contents
=================
1 Summary/help
2 Options
3 Data format
    3.1 Basic
    3.2 Scatter
    3.3 Options
    3.4 Bar
4 Rounding
5 Author
6 Coming features
    6.1 Make exception throwing/handling better
    6.2 Make other settings changeable in data file
    6.3 Move code to git, update ~/batlog.sh
    6.4 hmax option
    6.5 x-axis scale specification
    6.6 x-axis labelling of specific points and whole axis
    6.7 y-axis labelling
    6.8 Allow comments in data: lines starting with ";"
    6.9 bar graphs
    6.10 Stop printing extra point at end


1 Summary/help 
===============
asciigraph is a utility to
asciigraph is a utility to produce simple graphs of arbitrary data in ascii. The format for running asciigraph is as follows:

                    tasciigraph [-d] <-h | -s | -f </absolute/path/to/file> >

The meaning of the switches are...

-d          Enable debug output logging to stderr. *NOTE* This will break graphs unless stderr is redirected elsewhere from the asciigraph's output.
-h          Display a help message.
-s          Pull graph data directly from stdin.
-f          Pull graph data from the specified file.


2 Options 
==========
Please see [Data format] for a description of how to set these options.

  Option              Default value   Meaning                                                                                                                      
 -------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------
  xmin                inferred        The left-hand limit/boundary of the graph                                                                                    
  xmax                inferred        The right-hand limit/boundary of the graph                                                                                   
  xstep               none            Not yet used                                                                                                                 
  ymin                inferred        The lower limit/boundary of the graph                                                                                        
  ymax                inferred        The upper limit/boundary of the graph                                                                                        
  ystep               1               The resolution of the graph                                                                                                  
  hmax                none            The maximum height (in lines) of the graph; all graphs will be guaranteed to be less than or equal to this size              
                                      ^ Warning: this option does not guarantee a standard size. Graph size is guaranteed to be: hmax/2 < size <= hmax             
                                      ^ Also note that if ystep and hmax are both set, ystep will be overridden if the chosen ystep will make the graph too large  
                                      but ystep is guaranteed to be at least as large as its set value                                                             
  X_LABEL_DENSITY     5               The interval at which the x-axis is labelled.                                                                                
                                      ^ Warning: small values will cause formatting to become messed up                                                            
  GUIDELINE_DENSITY   10              The interval at which guidelines are displayed                                                                               
  GUIDELINE_CHAR      ^               The char used to display guidelines                                                                                          
  POINT_CHAR          @               The char used to display points                                                                                              
  Y_AXIS_CHAR         {pipe}          The char used to display the y-axis                                                                                          
  X_AXIS_CHAR         -               The char used to display the x-axis                                                                                          
  Y_AXIS_LABEL        y-axis          The y-axis label; Note that the label does not need quotes.                                                                  
  X_AXIS_LABEL        x-axis          The x-axis label; Note that the label does not need quotes.                                                                  
  bar                 false           Interpret data as a bar graph                                                                                                
  BAR_ZERO_POINT      false           Print a point on the x-axis for zero-value data points? (see bar graph example below)                                        

3 Data format 
==============
asciigraph can handle data provided in one of three formats. The default format is a simple data plot, in either basic or scatter formats. The third format is a bar graph.
It follows these formats strictly, ceasing to read data upon either end of file (EOF) or a blank line.
Comments may be placed anywhere in the data by starting a line with ';': any lines beginning with ';' will be ignored.

3.1 Basic 
----------
This format produces a basic graph of a given set of numbers. The input format is a list of single numbers, one per line. These numbers are treated as y-values and are plotted against their position in the list. Consider the following example:

data
====
; Example data of basic graphing mode
0
1
2
4
7
11
16
22
29

graph
=====
y-axis
      29 |^         ^     @
      28 |
      27 |
      26 |
      25 |
      24 |
      23 |
      22 |              @
      21 |
      20 |
      19 |^         ^
      18 |
      17 |
      16 |            @
      15 |
      14 |
      13 |
      12 |
      11 |          @
      10 |
       9 |^         ^
       8 |
       7 |        @
       6 |
       5 |
       4 |      @
       3 |
       2 |    @
       1 |  @
       0 |@ - - - - - - - -
          ------------------
          0         5
          x-axis

3.2 Scatter 
------------
This format produces a scatter plot of the given list of points in the format "x, y". Consider the following example:

data
====
; Example data of scatter graphing mode
; x, y
0, 0
4, 5
2, 2
7, 7
3, 5
9, 9
15, 4
13, 0

graph
=====
y-axis
       9 |^         ^       @ ^         ^
       8 |
       7 |              @
       6 |
       5 |      @ @
       4 |                              @
       3 |
       2 |    @
       1 |
       0 |@ - - - - - - - - - - - - @ - -
          --------------------------------
          0         5         10        15
          x-axis

3.3 Options 
------------
Options can be set at the start of data entry. The format for setting most options is as follows:

#option value

The only exception is boolean options (true/false) which are set to true as follows:

#option

You can set as many options as you like, but all option settings must appear at the start of the file, before any data. Any invalid, mispelled, or otherwise unrecognized options will be ignored, so take care that they are correctly written. If you set an option and it does not seem to affect the graph, check the syntax first. If it still seems correct, run in debug mode to see if any options are marked as invalid.
The data should follow immediately after the option settings. For example, consider the following example where the same data as was used in the [Basic] example is graphed but the ystep option is set to 2:

data
====
; Example data setting some options
; Set the y axis label to "foo bar"
#Y_AXIS_LABEL foo bar
; Set the granularity of the y-axis to 2
#ystep 2
; Begin the data: note no space separating options and data
; also note that no options may be set after the data has started
0
1
2
4
7
11
16
22
29

graph
=====
foo bar
30   |^         ^     @
28   |
26   |
24   |
22   |              @
20   |
18   |
16   |            @
14   |
12   |          @
10   |^         ^
8    |        @
6    |
4    |      @
2    |  @ @
0    |@ - - - - - - - - -
      0         5
     x-axis

3.4 Bar 
--------
This format produces a bar graph of value-label pairs in the format "value, label".
Because this format has a number of special considerations and different defaults from the two standard formats, this format must be enabled by setting the bar option. The following options have different default values for bar graphs (explicitly set options will still override these defaults):

  Option            Default value                            Reason for changing default                                              
 -----------------+----------------------------------------+-------------------------------------------------------------------------
  ymin              0 (unless negative values are entered)   Bar graphs typically always display the x-axis                           
  X_LABEL_DENSITY   1                                        Each point has an individual label, so this improves clarity             
  X_AXIS_LABEL      x-axis (* see below)                     The label is used to store and print the legend (containing the labels)  
 * If this value is explicitly set, the legend will be added onto the end of whatever it is set to.

Consider the following example:

data
====
; Example data of bar graphing mode
; value, label
#bar
; #BAR_ZERO_POINT
#X_AXIS_LABEL my label
6, foo
10, bar
2, baz
0, foo, bar
7, bar baz

graph
=====
y-axis
      10 |^ @ ^ ^ ^
       9 |  @
       8 |  @
       7 |  @     @
       6 |@ @     @
       5 |@ @     @
       4 |@ @     @
       3 |@ @     @
       2 |@ @ @   @
       1 |@ @ @   @
       0 |- - - - -
          ----------
          0 1 2 3 4
          my label

== LEGEND ==
0 = foo
1 = bar
2 = baz
3 = foo, bar
4 = bar baz


 * Note that data point "foo, bar" is zero and so does not create any bar. If this seems unclear and you want a point printed to show that "foo, bar" is zero, setting the option BAR_ZERO_POINT (as commented out in the example) will cause a point to be printed on the x-axis for any zero-value data points.

4 Rounding 
===========
When setting the ystep option to values other than 1, you may notice some distortion in the graph produced. This is not a bug; it is the result of rounding. Due to the discrete & finite nature of an ascii image, points must fall clearly into a single row and column on the graph. Values falling between two rows/columns cannot be represented. The immediate consequence of this is that When ystep is defined to be greater than 1, it becomes necessary to round y-values to the nearest multiple of ystep so that they will fit into a single row on the graph. This is done in two ways:
 - Points' y-values will be rounded to the nearest multiple of ystep.
   This is done by standard rounding convention (1/2+ => 1)
   e.g. if ystep = 10, the following y values would be rounded as shown:
            0-4 ==> 0   |   5-9 == 10
 - Limits which are not multiples of ystep will be rounded to a multiple of ystep so as to expand the region of graphing. Thus:
   - lower limits are always rounded down
   - upper limits are always rounded up

5 Author 
=========
asciigraph was written by Lukas Lazarek <lukasalazarek@gmail.com>

6 Coming features 
==================

6.1 DONE Make exception throwing/handling better 
-------------------------------------------------

6.2 DONE Make other settings changeable in data file 
-----------------------------------------------------

6.3 DONE Move code to git, update ~/batlog.sh 
----------------------------------------------

6.4 DONE hmax option 
---------------------

6.5 TODO x-axis scale specification 
------------------------------------

6.6 DONE x-axis labelling of specific points and whole axis 
------------------------------------------------------------

6.7 DONE y-axis labelling 
--------------------------

6.8 DONE Allow comments in data: lines starting with ";" 
---------------------------------------------------------

6.9 DONE bar graphs 
--------------------
>>> Use a string replace function to replace occurrences of "\n" with a newline in X_AXIS_LABEL



  bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_position = str.find(from);
    if(start_position == std::string::npos)
      return false;
    str.replace(start_position, from.length(), to);
    return true;
  }
  
  std::string string("hello $name");
  replace(string, "$name", "Somename");


thus, to make a bar graph, one would just set X_LABEL_DENSITY to 1, and then use the X_AXIS_LABEL to write a legend along the lines of:
1 = expenses
2 = expenses:food
3 = expenses:disc
4 = expenses:misc
...

Alternatively, I could implement a bar graph option which takes data in the format:
"""
; [number] [description]
22 expenses:disc
15 expenses:food
5 expenses:misc
42 total expenses
"""
So it would just need to extract the number, then get the rest of the line as the label
And then does exactly what I was saying above.

> What if I keep a vector which just contains true/false for if the height of each x-point has been reached yet, and this is decided if an '@' is printed at that column for each row
ie: if we have a graph like so:
. |
. |
. |      @
. |      @
. |      @
.9|      @
.8|  @   @ @
.7|  @   @ @
.6|  @ @ @ @
.5|  @ @ @ @
.4|@ @ @ @ @
.3|@ @ @ @ @
.2|@ @ @ @ @ @
.1|@ @ @ @ @ @ @
. ---------------------------
.  1 2 3 4 5 6 7

And we're currently printing row 7, then our vector looks like ([X] = t, [ ] = f)
    [ ][x][ ][x][x][ ][ ]
vec[ 0  1  2  3  4  5  6 ]

And then we get to row 6, we get the next point and if it starts on that row, we set vec[point.second()] = true
so the vector would become
    [ ][x][x][x][x][ ][ ]
vec[ 0  1  2  3  4  5  6 ]

6.10 DONE Stop printing extra point at end 
-------------------------------------------