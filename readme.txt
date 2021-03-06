			  ___________________

			   ASCIIGRAPH README

				 lukas
			  ___________________


Table of Contents
_________________

1 Summary
2 Options
3 Data format
..... 3.0.1 Basic
..... 3.0.2 Scatter
..... 3.0.3 Options
..... 3.0.4 Bar
..... 3.0.5 A note on spacing
4 Rounding
5 Author
6 Coming features
..... 6.0.1 TODO x-axis scale specification
..... 6.0.2 TODO Allow negative x-points in scatter mode





1 Summary
=========

  asciigraph is a utility to produce simple graphs of arbitrary data in
  ascii. The format for running asciigraph is as follows:

  ,----
  | tasciigraph [-d] <-h | -s | -f </absolute/path/to/file> >
  `----

  The meaning of the switches are...

  - d Enable debug output logging to stderr. *NOTE* This will break
    graphs unless stderr is redirected elsewhere from the asciigraph's
    output.
  - h Display a help message.
  - s Pull graph data directly from stdin.
  - f Pull graph data from the specified file.


2 Options
=========

  Please see [Data format] for a description of how to set these
  options.

   Option             Default value  Meaning                                                                                                                     
  ---------------------------------------------------------------------------------------------------------------------------------------------------------------
   xmin               inferred       The left-hand limit/boundary of the graph                                                                                   
   xmax               inferred       The right-hand limit/boundary of the graph                                                                                  
   xstep              none           Not yet used                                                                                                                
   ymin               inferred       The lower limit/boundary of the graph                                                                                       
   ymax               inferred       The upper limit/boundary of the graph                                                                                       
   ystep              1              The resolution of the graph                                                                                                 
   hmax               none           The maximum height (in lines) of the graph; all graphs will be guaranteed to be less than or equal to this size             
                                     ^ Warning: this option does not guarantee a standard size. Graph size is guaranteed to be: hmax/2 < size <= hmax            
                                     ^ Also note that if ystep and hmax are both set, ystep will be overridden if the chosen ystep will make the graph too large 
                                     but ystep is guaranteed to be at least as large as its set value                                                            
   X_LABEL_DENSITY    5              The interval at which the x-axis is labelled.                                                                               
                                     ^ Warning: small values may cause formatting to become messed up (unless using WIDTH_PAD) - see A note on spacing           
   GUIDELINE_DENSITY  10             The interval at which guidelines are displayed                                                                              
   GUIDELINE_CHAR     ^              The char used to display guidelines                                                                                         
   POINT_CHAR         @              The char used to display points                                                                                             
   Y_AXIS_CHAR        {pipe}         The char used to display the y-axis                                                                                         
   X_AXIS_CHAR        -              The char used to display the x-axis                                                                                         
   Y_AXIS_LABEL       y-axis         The y-axis label; Note that the label does not need quotes.                                                                 
   X_AXIS_LABEL       x-axis         The x-axis label; Note that the label does not need quotes.                                                                 
   bar                false          Interpret data as a bar graph                                                                                               
   BAR_ZERO_POINT     false          Print a point on the x-axis for zero-value data points? (see bar graph example below)                                       
   WIDTH_PAD          1              The number of spaces between columns of the graph - see A note on spacing                                                   


3 Data format
=============

  asciigraph can handle data provided in one of three formats. The
  default format is a simple data plot, in either basic or scatter
  formats. The third format is a bar graph. It follows these formats
  strictly, ceasing to read data upon either end of file (EOF) or a
  blank line. Comments may be placed anywhere in the data by starting a
  line with ';': any lines beginning with ';' will be ignored.


3.0.1 Basic
-----------

  This format produces a basic graph of a given set of numbers. The
  input format is a list of single numbers, one per line. These numbers
  are treated as y-values and are plotted against their position in the
  list. Consider the following example:

  ,----
  | data
  | ====
  | ; Example data of basic graphing mode
  | 0
  | 1
  | 2
  | 4
  | 7
  | 11
  | 16
  | 22
  | 29
  | 
  | graph
  | =====
  | y-axis
  |       29 |^         ^     @
  |       28 |
  |       27 |
  |       26 |
  |       25 |
  |       24 |
  |       23 |
  |       22 |              @
  |       21 |
  |       20 |
  |       19 |^         ^
  |       18 |
  |       17 |
  |       16 |            @
  |       15 |
  |       14 |
  |       13 |
  |       12 |
  |       11 |          @
  |       10 |
  |        9 |^         ^
  |        8 |
  |        7 |        @
  |        6 |
  |        5 |
  |        4 |      @
  |        3 |
  |        2 |    @
  |        1 |  @
  |        0 |@ - - - - - - - -
  |           ------------------
  |           0         5
  |           x-axis
  `----


3.0.2 Scatter
-------------

  This format produces a scatter plot of the given list of points in the
  format "x, y". Consider the following example:

  ,----
  | data
  | ====
  | ; Example data of scatter graphing mode
  | ; x, y
  | 0, 0
  | 4, 5
  | 2, 2
  | 7, 7
  | 3, 5
  | 9, 9
  | 15, 4
  | 13, 0
  | 
  | graph
  | =====
  | y-axis
  |        9 |^         ^       @ ^         ^
  |        8 |
  |        7 |              @
  |        6 |
  |        5 |      @ @
  |        4 |                              @
  |        3 |
  |        2 |    @
  |        1 |
  |        0 |@ - - - - - - - - - - - - @ - -
  |           --------------------------------
  |           0         5         10        15
  |           x-axis
  `----


3.0.3 Options
-------------

  Options can be set at the start of data entry. The format for setting
  most options is as follows:

  ,----
  | #option value
  `----

  The only exception is boolean options (true/false) which are set to
  true as follows:

  ,----
  | #option
  `----

  You can set as many options as you like, but all option settings must
  appear at the start of the file, before any data. Any invalid,
  mispelled, or otherwise unrecognized options will be ignored, so take
  care that they are correctly written. If you set an option and it does
  not seem to affect the graph, check the syntax first. If it still
  seems correct, run in debug mode to see if any options are marked as
  invalid. The data should follow immediately after the option settings.
  For example, consider the following example where the same data as was
  used in the [Basic] example is graphed but the ystep option is set to
  2:

  ,----
  | data
  | ====
  | ; Example data setting some options
  | ; Set the y axis label to "foo bar"
  | #Y_AXIS_LABEL foo bar
  | ; Set the granularity of the y-axis to 2
  | #ystep 2
  | ; Begin the data: note no space separating options and data
  | ; also note that no options may be set after the data has started
  | 0
  | 1
  | 2
  | 4
  | 7
  | 11
  | 16
  | 22
  | 29
  | 
  | graph
  | =====
  | foo bar
  | 30   |^         ^     @
  | 28   |
  | 26   |
  | 24   |
  | 22   |              @
  | 20   |
  | 18   |
  | 16   |            @
  | 14   |
  | 12   |          @
  | 10   |^         ^
  | 8    |        @
  | 6    |
  | 4    |      @
  | 2    |  @ @
  | 0    |@ - - - - - - - - -
  |       0         5
  |      x-axis
  `----


3.0.4 Bar
---------

  This format produces a bar graph of value-label pairs in the format
  "value, label". Because this format has a number of special
  considerations and different defaults from the two standard formats,
  this format must be enabled by setting the bar option. The following
  options have different default values for bar graphs (explicitly set
  options will still override these defaults):

   Option           Default value                           Reason for changing default                                             
  ----------------------------------------------------------------------------------------------------------------------------------
   ymin             0 (unless negative values are entered)  Bar graphs typically always display the x-axis                          
   X_LABEL_DENSITY  1                                       Each point has an individual label, so this improves clarity            
   X_AXIS_LABEL     x-axis (* see below)                    The label is used to store and print the legend (containing the labels) 
  * If this value is explicitly set, the legend will be added onto the
    end of whatever it is set to.

  Consider the following example:

  ,----
  | data
  | ====
  | ; Example data of bar graphing mode
  | ; value, label
  | #bar
  | ; #BAR_ZERO_POINT
  | #X_AXIS_LABEL my label
  | 6, foo
  | 10, bar
  | 2, baz
  | 0, foo, bar
  | 7, bar baz
  | 
  | graph
  | =====
  | y-axis
  |       10 |^ @ ^ ^ ^
  |        9 |  @
  |        8 |  @
  |        7 |  @     @
  |        6 |@ @     @
  |        5 |@ @     @
  |        4 |@ @     @
  |        3 |@ @     @
  |        2 |@ @ @   @
  |        1 |@ @ @   @
  |        0 |- - - - -
  |           ----------
  |           0 1 2 3 4
  |           my label
  | 
  | == LEGEND ==
  | 0 = foo
  | 1 = bar
  | 2 = baz
  | 3 = foo, bar
  | 4 = bar baz
  `----

  * Note that data point "foo, bar" is zero and so does not create any
    bar. If this seems unclear and you want a point printed to show that
    "foo, bar" is zero, setting the option BAR_ZERO_POINT (as commented
    out in the example) will cause a point to be printed on the x-axis
    for any zero-value data points.


3.0.5 A note on spacing
-----------------------

  The options X_LABEL_DENSITY and WIDTH_PAD have particular importance
  for the readability of graphs produced by asciigraph. On one hand,
  labelling every point along the x-axis (i.e: X_LABEL_DENSITY 1) can
  improve clarity, but on the other hand it can also make things
  cluttered. This is especially true when more than ten data points are
  plotted (or, if in scatter mode, the xmax - xmin >= 10), because if
  X_LABEL_DENSITY is set to 1 in these cases the two-digit labels end up
  with no spacing between them. For example:

  ,----
  | data
  | ====
  | #X_LABEL_DENSITY 1
  | 2, 5
  | 15, 3
  | 
  | 
  | graph
  | =====
  | y-axis
  |        5 |@ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
  |        4 |
  |        3 |                          @
  |           ----------------------------
  |           2 3 4 5 6 7 8 9 101112131415
  |           x-axis
  `----

  Clearly this is undesirable. Should the labels become three or more
  digits, they would displace each other and not even line up correctly.
  There are two ways to avoid this: either increase the value of
  X_LABEL_DENSITY or of WIDTH_PAD. The effect of increasing
  X_LABEL_DENSITY should be obvious: since labels are printed less
  frequently, every label gets more "breathing room". Increasing
  WIDTH_PAD, on the other hand, solves this problem by increasing the
  horizontal spacing of the entire graph. The default value of WIDTH_PAD
  (1) means that, normally, there is one space of padding between each
  'column' in the graph; increasing WIDTH_PAD correspondingly increases
  the number of spaces between columns (and the reverse is true: setting
  it to 0 removes the padding). For example:

  ,----
  | data
  | ====
  | #X_LABEL_DENSITY 1
  | #WIDTH_PAD 2
  | 2, 5
  | 15, 3
  | 
  | graph
  | =====
  | y-axis
  |        5 |@  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^
  |        4 |
  |        3 |                                       @
  |           ------------------------------------------
  |           2  3  4  5  6  7  8  9  10 11 12 13 14 15
  |           x-axis
  `----


4 Rounding
==========

  When setting the ystep option to values other than 1, you may notice
  some distortion in the graph produced. This is not a bug; it is the
  result of rounding. Due to the discrete & finite nature of an ascii
  image, points must fall clearly into a single row and column on the
  graph. Values falling between two rows/columns cannot be represented.
  The immediate consequence of this is that When ystep is defined to be
  greater than 1, it becomes necessary to round y-values to the nearest
  multiple of ystep so that they will fit into a single row on the
  graph. This is done in two ways:
  - Points' y-values will be rounded to the nearest multiple of ystep.
    - This is done by standard rounding convention (1/2+ => 1)
    - e.g. if ystep = 10, the following y values would be rounded as
      shown:
      ,----
      | 0-4 ==> 0   |   5-9 == 10
      `----
  - Limits which are not multiples of ystep will be rounded to a
    multiple of ystep so as to expand the region of graphing. Thus:
    - lower limits are always rounded down
    - upper limits are always rounded up


5 Author
========

  asciigraph was written by Lukas Lazarek <lukasalazarek@gmail.com>


6 Coming features
=================

6.0.1 TODO x-axis scale specification
-------------------------------------


6.0.2 TODO Allow negative x-points in scatter mode
--------------------------------------------------

  >> must print a y-axis on x=0
