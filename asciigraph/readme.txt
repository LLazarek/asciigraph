                              asciigraph
                              ==========

Author: Lukas Lazarek
Date: 2016-02-14 18:14:19 EST


Table of Contents
=================
1 Summary/help
2 Options
3 Data format
    3.1 Basic
    3.2 Scatter
    3.3 Options
4 Rounding
5 Author
6 Coming features
    6.1 Make exception throwing/handling better
    6.2 Make other settings changeable in data file
    6.3 Move code to git, update ~/batlog.sh


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
 -------------------+---------------+-------------------------------------------------------------------
  minx                inferred        The left-hand limit/boundary of the graph                          
  maxx                inferred        The right-hand limit/boundary of the graph                         
  xstep               none            Not yet used                                                       
  miny                inferred        The lower limit/boundary of the graph                              
  maxy                inferred        The upper limit/boundary of the graph                              
  ystep               1               The resolution of the graph                                        
  X_LABEL_DENSITY     5               The interval at which the x-axis is labelled.                      
                                      ^ Warning: small values will cause formatting to become messed up  
  GUIDELINE_DENSITY   10              The interval at which guidelines are displayed                     
  GUIDELINE_CHAR      ^               The char used to display guidelines                                
  POINT_CHAR          @               The char used to display points                                    
  Y_AXIS_CHAR         {pipe}          The char used to display the y-axis                                
  X_AXIS_CHAR         -               The char used to display the x-axis                                

3 Data format 
==============
asciigraph can handle data provided in one of two formats, basic and scatter.
It follows these formats strictly, ceasing to read data upon either end of file (EOF) or a blank line.

3.1 Basic 
----------
This format produces a basic graph of a given set of numbers. The input format is a list of single numbers, one per line. These numbers are treated as y-values and are plotted against their position in the list. Consider the following example:

data
====
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
29   |^         ^     @
28   |
27   |
26   |
25   |
24   |
23   |
22   |              @
21   |
20   |
19   |^         ^
18   |
17   |
16   |            @
15   |
14   |
13   |
12   |
11   |          @
10   |
9    |^         ^
8    |
7    |        @
6    |
5    |
4    |      @
3    |
2    |    @
1    |  @
0    |@ - - - - - - - - -
      0         5

3.2 Scatter 
------------
This format produces a scatter plot of the given list of points in the format "x, y". Consider the following example:

data
====
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
9    |^         ^       @ ^         ^
8    |
7    |              @
6    |
5    |      @ @
4    |                              @
3    |
2    |    @
1    |
0    |@ - - - - - - - - - - - - @ - -
      0         5         10        15

3.3 Options 
------------
[Options] can be set at the start of data entry. The format for setting an option is as follows:

#option value

You can set as many options as you like, but all option settings must appear at the start of the file, before any data.
The data should follow immediately after the option settings. For example, consider the following example where the same data as was used in the [Basic] example is graphed but the ystep option is set to 2:

data
====
#ystep 2
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

6.1 TODO Make exception throwing/handling better 
-------------------------------------------------

6.2 TODO Make other settings changeable in data file 
-----------------------------------------------------

6.3 TODO Move code to git, update ~/batlog.sh 
----------------------------------------------
