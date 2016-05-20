#include "asciigraph.h"
#include <cstdlib>
#include <iostream>

#define DEBUG if(debug)

asciigraph::asciigraph(const std::vector<std::pair<int, int>> Fx,
		       const int _xmin, const int _xmax, const int _xstep,
		       const int _ymin, const int _ymax, const int _ystep,
		       const bool _debug,
		       const char _X_AXIS_CHAR,
		       const char _Y_AXIS_CHAR,
		       const char _GUIDELINE_CHAR,
		       const char _POINT_CHAR,
		       const int  _X_LABEL_DENSITY,
		       const int  _GUIDELINE_DENSITY,
		       const std::string _X_AXIS_LABEL,
		       const std::string _Y_AXIS_LABEL)
  : ymin(_ymin), ymax(_ymax), ystep(_ystep),
    xmin(_xmin), xmax(_xmax), xstep(_xstep),
    debug(_debug),
    X_AXIS_CHAR       (_X_AXIS_CHAR),
    Y_AXIS_CHAR       (_Y_AXIS_CHAR),
    GUIDELINE_CHAR    (_GUIDELINE_CHAR),
    POINT_CHAR        (_POINT_CHAR), 
    X_LABEL_DENSITY   (_X_LABEL_DENSITY),
    GUIDELINE_DENSITY (_GUIDELINE_DENSITY),
    X_AXIS_LABEL      (_X_AXIS_LABEL),
    Y_AXIS_LABEL      (_Y_AXIS_LABEL){
  /* Error checking */
  if(ymin >= ymax || ystep < 1 ||
     xmin >= xmax || xstep < 1){
    throw std::logic_error("Limits or steps illogical");
  }
    
  // Points given as (x, y) - transpose into (y , x)
  for(auto it = Fx.begin(); it != Fx.end(); ++it){
    points.push_back(std::pair<int, int>(it -> second, it -> first));
  }
}


void asciigraph::operator()(std::ostream &out){
  int y, ymin_rnd;
  auto graphpoints = prepare_data(points, &y, &ymin_rnd);

  //  bool marked_last_row = false;
  int marked_last_row = 0;

  /* Draw graph */
  // Print y-axis label
  out << Y_AXIS_LABEL << std::endl;
  
  std::vector<std::pair<int, int>> used; // Keep track of points plotted
  auto it = graphpoints.begin();
  // For each y-value in range...
  for(; y >= ymin_rnd && it != graphpoints.end(); y -= ystep){
    /* y-axis labelling */
    DEBUG std::cerr << "labelling ";
    // Label padding
    if(y >= 0)
      for(int i =  (y == 0) ? 1 : y  ; i < 10000000  ; i *= 10) out << " ";
    else
      for(int i = -y; i < 1000000; i *= 10) out << " ";
    out << y << " "; // y-axis label
    out << Y_AXIS_CHAR; // Y-axis line

    DEBUG std::cerr << "y = " << y << std::endl;
    /* Done labelling */


    /* Plot points */
    int xpos = xmin;
    while(it -> first > y) it++; // Skip points out of graph range
    // Plot points for this y value
    for(; it -> first == y && it != graphpoints.end(); ++it){
      DEBUG std::cerr << "Handling pt: (" << it -> first << ", "
		      << it -> second << ")\n";
      // Print filler
      while(xpos < it -> second){
	if(y == 0) out << X_AXIS_CHAR << " "; // if at y = 0, draw x-axis
	else{
	  if(xpos%X_LABEL_DENSITY == 0 && !marked_last_row){
	    out<< GUIDELINE_CHAR << " "; // print x guideline
	  }
	  else out << "  ";
	}
	++xpos;
      }// end while

      // Don't plot same point twice...
      if(std::find(used.begin(), used.end(), *it) == used.end()){
	DEBUG std::cerr << "Printing point: (" << y << ", "
			<< xpos << ")\n";
	out << POINT_CHAR << " ";// Haven't already printed this point
	++xpos;
	used.push_back(*it);
      }
    }// end for
    DEBUG std::cerr << "finished line " << y << std::endl;

    
    // Fill in rest of line for this y-value
    for(; xpos <= xmax; ++xpos){
      if(y == 0) out << X_AXIS_CHAR << " ";
      else{
	if(xpos%X_LABEL_DENSITY == 0 && !marked_last_row){
	  out << GUIDELINE_CHAR << " "; // print x guideline
	}
	else out << "  ";
      }
    }
    marked_last_row = (marked_last_row + 1)%GUIDELINE_DENSITY;
    out << std::endl;
  }// end for
  /* Done plotting points */

  
  /* Fill out any remaining rows of graph, even though no points fall on them */
  for(; y >= ymin_rnd; y -= ystep){
    out << y; // y-axis label
    // Label padding
    DEBUG std::cerr << "labelling... ";
    if(y >= 0)
      for(int i =  (y == 0) ? 1 : y; i < 10000; i *= 10) out << " ";
    else
      for(int i = -y; i < 1000; i *= 10) out << " ";
    
    out << Y_AXIS_CHAR; // Y-axis line
    for(int xpos = xmin; xpos <= xmax; ++xpos){
      if(xpos%X_LABEL_DENSITY == 0 && !marked_last_row){
	out << GUIDELINE_CHAR << " "; // print x guideline
      }
      else if(y == 0) out << X_AXIS_CHAR << " ";
      else out << "  ";
    }
    //    marked_last_row = !marked_last_row; // print x-markers every other row
    marked_last_row = (marked_last_row + 1)%GUIDELINE_DENSITY;
    out << std::endl;
    DEBUG std::cerr << "y = " << y << std::endl;
  }
  /* Done filling in graph */
  
  label_x_axis(out);
  out << "\n" << std::endl;
}


// rounds and sorts data for graphing
std::vector< std::pair<int, int> >
asciigraph::prepare_data(const std::vector< std::pair<int, int> > &pts,
			 int *_y, int *_ymin_rnd){
  /* If ystep > 1, round points to fit into multiple of ystep */
  auto graphpoints = points; // Make copy to be manipulated for rounding
  if(ystep > 1){
    DEBUG std::cerr << "ystep > 1 - performing rounding...\n";
    for(auto it = graphpoints.begin(); it != graphpoints.end(); ++it){
      int pt_off_by = (it -> first)%ystep;
      if(pt_off_by != 0){
	DEBUG std::cerr << "Rounded (" << it -> first << ", "
			 << it -> second << ") to ";
	// Round the y-value to nearest multiple of ystep
	if(it -> first > 0){
	  it -> first +=
	    (pt_off_by < ystep/2)  ?  -pt_off_by  :  ystep - pt_off_by;
	}
	else{
	  DEBUG std::cerr << pt_off_by << " >= " << -ystep/2 << std::endl;
	  it -> first +=
	    (pt_off_by >= -ystep/2)  ?  -pt_off_by  :  -(ystep + pt_off_by);
	}
	DEBUG std::cerr << "(" << it -> first << ", " << it -> second << ")\n";
      }
    }
  }



  /* Before graphing, sort the points into (1) y-descending, (2) x-ascending */
  std::sort(graphpoints.begin(), graphpoints.end(), descending_y_order());


  /* Round graph limits */
  int &y = *_y;
  int &ymin_rnd = *_ymin_rnd;
  // Round ymax up to a multiple of ystep
  y = ymax;
  if(y%ystep != 0) y += ystep - y%ystep;
  // Round ymin down to a multiple of ystep
  ymin_rnd = ymin;
  if(ymin%ystep != 0){
    if(ymin < 0) ymin_rnd -= ystep + ymin%ystep;
    else ymin_rnd -= ymin%ystep;
  }
  DEBUG std::cerr << "ylimits: " << ymin_rnd << ", " << y << std::endl;

  return graphpoints;
}

// Prints x-axis labels
void asciigraph::label_x_axis(std::ostream &out){
  // Print bottom border
  out << "          ";
  for (int x = xmin; x <= xmax; ++x){
    out << "- ";
  }
  // Print labels
  out << "\n          ";
  for(int x = xmin; x <= xmax; x += X_LABEL_DENSITY){
    if(0 <= x && x <= 9){ // 1 char label
      out << x;
      for(int i = 1; i < X_LABEL_DENSITY*2; ++i) out << ' ';
    }
    else if((10 <= x && x <= 99) || (-9 <= x && x <= -1)){ // 2 char
      out << x;
      for(int i = 2; i < X_LABEL_DENSITY*2; ++i) out << ' ';
    }
    else if((100 <= x && x <= 999) || (-99 <= x && x <= -10)){ // 3 char
      out << x;
      for(int i = 3; i < X_LABEL_DENSITY*2; ++i) out << ' ';
    }
    else if((1000 <= x && x <= 9999) || (-999 <= x && x <= -100)){ // 4 char
      out << x;
      for(int i = 4; i < X_LABEL_DENSITY*2; ++i) out << ' ';
    }
  }
  out << std::endl << "          " << X_AXIS_LABEL;
}
