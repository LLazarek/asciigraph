#ifndef ASCIIGRAPH_H
#define ASCIIGRAPH_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include "asciigraph_except.h"


// Change these to adjust how the graph appears by default...
#define X_AXIS_CHAR_DEFAULT       '-'
#define Y_AXIS_CHAR_DEFAULT       '|'
#define GUIDELINE_CHAR_DEFAULT    '^'
#define POINT_CHAR_DEFAULT        '@'
#define X_LABEL_DENSITY_DEFAULT   5
#define GUIDELINE_DENSITY_DEFAULT 10
#define X_AXIS_LABEL_DEFAULT      "x-axis"
#define Y_AXIS_LABEL_DEFAULT      "y-axis"
#define WIDTH_PAD_DEFAULT         1
#define BAR_ZERO_POINT_DEFAULT    false


/* Class asciigraph:
   A tool to graph arbitrary data in plain text.
 */
class asciigraph {
public:
  /* asciigraph::Constructor:
     Creates an asciigraph object with the given points, x/y constrains,
     and steps.
     The points should be given as pairs (x, y) where x is the independent
     variable and y is the depended variable.
     * Note that xstep currently does nothing.

     == A note on steps ==
     The step parameters dictate the resolution of the graph in the specified
     dimension. For example, a graph with y values ranging from 0 to 4
     with the default ystep of 1 will have a y-axis displayed as follows:

     4   |
     3   |
     2   |
     1   |
     0   |

     Each displayed row contains the data for a single y-value. In contrast,
     the same graph with a ystep of 2 will be displayed as follows:

     4   |
     2   |
     0   |

     Each row therefore contains the data for two y-values. While precision
     is therefore lost in the process, this is often desirable when dealing
     with large, sparse data points so as to keep the graph managably sized
     and reasonable to read.
n    =====================

     @throws
     std::logic_error                         Given limits invalid

     @params
     std::vector<std::pair<int, int>> Fx      The points to be graphed
     int _xmin                                The lower bound of the x-axis
     int _xmax                                The upper bound of the x-axis
     int _xstep                               The step of the x-axis
                                              * This does nothing
     int _ymin                                The lower bound of the y-axis
     int _ymax                                The upper bound of the y-axis
     int _ystep                               The step of the y-axis
     bool _debug             = false          Log debug info to stderr?
     char _X_AXIS_CHAR       = ..._DEFAULT    Char to use for drawing x-axis
     char _Y_AXIS_CHAR       = ..._DEFAULT    Char to use for drawing y-axis
     char _GUIDELINE_CHAR    = ..._DEFAULT    Char to use for drawing guidelines
     char _POINT_CHAR        = ..._DEFAULT    Char to use for drawing points
     int  _X_LABEL_DENSITY   = ..._DEFAULT    Interval at which to label x-axis
     int  _GUIDELINE_DENSITY = ..._DEFAULT    Interval to print guidelines
     int  _WIDTH_PAD         = ..._DEFAULT    Number of spaces between columns
     bool _BAR_ZERO_POINT    = ..._DEFAULT    Bar graphs: print points on zero?
  */
  asciigraph(const std::vector<std::pair<int, int>> Fx,
	     const int _xmin, const int _xmax, const int _xstep,
	     const int _ymin, const int _ymax, const int _ystep,
	     const bool _debug = false,
	     const char _X_AXIS_CHAR         = X_AXIS_CHAR_DEFAULT,
	     const char _Y_AXIS_CHAR         = Y_AXIS_CHAR_DEFAULT,
	     const char _GUIDELINE_CHAR      = GUIDELINE_CHAR_DEFAULT,
	     const char _POINT_CHAR          = POINT_CHAR_DEFAULT,
	     const int  _X_LABEL_DENSITY     = X_LABEL_DENSITY_DEFAULT,
	     const int _GUIDELINE_DENSITY    = GUIDELINE_DENSITY_DEFAULT,
	     const std::string _X_AXIS_LABEL = X_AXIS_LABEL_DEFAULT,
	     const std::string _Y_AXIS_LABEL = Y_AXIS_LABEL_DEFAULT,
	     const int _WIDTH_PAD            = WIDTH_PAD_DEFAULT,
	     const bool _BAR_ZERO_POINT      = BAR_ZERO_POINT_DEFAULT);

  
  /* addPoint():
     Adds the given point to the list of points to be graphed.

     @params
     std::pair<int, int> p

     @return
     bool 
  */
  bool addPoint(std::pair<int, int> p){
    try{
      points.push_back(std::pair<int, int>(p.second, p.first));
      return true;
    }catch(std::bad_alloc e){
      return false;
    }
  }

  
  /* operator():
     Graphs the data stored in this asciigraph object to the given
     output stream.

     == Notes about rounding ==
     Due to the discrete & finite nature of an ascii image, points
     must fall clearly into a single row and column on the graph.
     Values falling between two rows/columns cannot be represented.
     The immediate consequence of this is that When ystep is defined
     to be greater than 1, it becomes necessary to round y-values
     to the nearest multiple of ystep so that they will fit into a single
     row on the graph. This is done in two ways:
     * Points' y-values will be rounded to the nearest multiple of ystep.
       This is done by standard rounding convention (1/2+ => 1)
       e.g. if ystep = 10, the following y values would be rounded as shown:
            0-4 ==> 0   |   5-9 == 10
     * Limits which are not multiples of ystep will be rounded to a multiple
       of ystep so as to expand the region of graphing. Thus:
       - lower limits are always rounded down
       - upper limits are always rounded up
     ==========================

     @throws
     void
     
     @params
     std::ostream &out         The stream to which to print the graoh

     @return
     void 
  */
  void operator()(std::ostream &out, const bool bar_graph = false);
  
private:
  /* asciigraph::prepare_data():
     Prepares asciigraph data for graphing by performing any necessary
     rounding, transposing the data into (y, x) format, and finally ordering
     the data into descending order.
  */
  std::vector<std::pair<int, int>>
    prepare_data(const std::vector< std::pair<int, int> > &pts,
		 int *_y, int *_ymin_rnd);
  void label_x_axis(std::ostream &out);

  
  int ymin, ymax, ystep, xmin, xmax, xstep;
  bool debug;
  //                   ( y , x )
  std::vector<std::pair<int, int>> points;
  char X_AXIS_CHAR, Y_AXIS_CHAR, GUIDELINE_CHAR, POINT_CHAR;
  int X_LABEL_DENSITY, GUIDELINE_DENSITY;
  std::string X_AXIS_LABEL, Y_AXIS_LABEL;
  int WIDTH_PAD;
  bool BAR_ZERO_POINT;
};

/* Model asciigraph:
  |
  |
  |
  |
  |          *
  |           *
  |         *  ***
  |        *
  |       *
  |   *  *
  | *  **
  |     
  |*
  ------------------
*/

/* struct descending_y_order:
   A functor defining the sorting scheme of integer pairs (a, b):
   'a' descending as the primary sorting with 'b' ascending as the secondary.
   i.e. the following set of points
       { (1, 2), (1, 5), (1, 3), (4, 2), (4, 5), (3, 0), (4, 4) }
   would be sorted to become
       { (4, 2), (4, 4), (4, 5), (3, 0), (1, 2), (1, 3), (1, 5) }
*/
struct descending_y_order {
  bool operator()(const std::pair<int, int> p1, const std::pair<int, int> p2){
    if(p1.first > p2.first) return true;
    else{
      if(p1.first < p2.first) return false;
      else{
	return p1.second <= p2.second;
      }
    }
  }
};

/* make_str():
   Creates a string composed of n copies of str, optionally separated by
   a separator string.

   @throws
   void
   
   @params
   std::string str                   The string to be copied
   int n                             The number of copies of str
   std::string separator  = ""       A string separating each copy of str

   @return
   std::string                       The resulting string
*/
std::string make_str(std::string str, const int n,
		       std::string separator = "");

#endif
