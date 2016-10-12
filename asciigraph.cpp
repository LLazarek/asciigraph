/**************************************************/
/* Author: Lukas Lazarek                          */
/* Copyright (C) 2016 Lukas Lazarek               */
/* Please see LICENSE.txt for full license info   */
/**************************************************/

#include "asciigraph.h"
#include <cstdlib>
#include <iostream>

#define DEBUG if(debug)

asciigraph::asciigraph(const std::vector<std::pair<int, int>> Fx,
		       const int _xmin, const int _xmax, const int _xstep,
		       const int _ymin, const int _ymax, const int _ystep,
		       const bool _debug,               // = false
		       const char _X_AXIS_CHAR,         // = ..._DEFAULT
		       const char _Y_AXIS_CHAR,         // = ..._DEFAULT
		       const char _GUIDELINE_CHAR,      // = ..._DEFAULT
		       const char _POINT_CHAR,          // = ..._DEFAULT
		       const int  _X_LABEL_DENSITY,     // = ..._DEFAULT
		       const int  _GUIDELINE_DENSITY,   // = ..._DEFAULT
		       const std::string _X_AXIS_LABEL, // = ..._DEFAULT
		       const std::string _Y_AXIS_LABEL, // = ..._DEFAULT
		       const int _WIDTH_PAD,            // = ..._DEFAULT
		       const bool _BAR_ZERO_POINT       // = ..._DEFAULT
		       )
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
    Y_AXIS_LABEL      (_Y_AXIS_LABEL),
    WIDTH_PAD         (_WIDTH_PAD),
    BAR_ZERO_POINT    (_BAR_ZERO_POINT){
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


void asciigraph::operator()(std::ostream &out,
			    const bool bar_graph /* = false */){
  int y, ymin_rnd;
  auto graphpoints = prepare_data(points, &y, &ymin_rnd);

  int marked_last_row = 0;

  /*******************************/
  /***** Set up bar tracking *****/
  /*******************************/
  /* print_bar contains values indicating if a bar should be printed
     for each x-value at the current y-value. It is updated with the handling
     of each y value.
     0 = don't print a bar for this x-value on this y-value's row
     1 = print a bar for this x-value on this row above the x-axis
     2 = print a bar for this x-value on this row below the x-axis

     Since graphs are printed by starting at ymax and working down to ymin
     this system works well for positive y-values. print_bar begins with
     all 0s and when a point is printed, its corresponding print_bar
     value is set to 1: then every pass/row after that will print a bar
     underneath that point.
     For negative values, however, it must work in the opposite manner.
     The default must be to print the bar, until the point is reached, at which
     point the bar must stop.
  */
  std::vector< int > print_bar(xmax + 1, 0); // fill print_bar with 0s
  // Initialize any negative values print_bar values
  /* Iterate backwards (since graphpoints in descending order) to set all
     negative points in print_bar */
  for (auto it2 = graphpoints.rbegin();
       it2 != graphpoints.rend()   &&   it2 -> first < 0;
       ++it2){
    print_bar[it2 -> second] = 2;
  }

  /*********************************/
  /***** Prepare to draw graph *****/
  /*********************************/
  // Print y-axis label
  out << Y_AXIS_LABEL << std::endl;
  
  std::vector<std::pair<int, int>> used; // Keep track of points plotted

  auto it = graphpoints.begin();
  // Deal with any points above ymax (only occurs if user sets ymax)
  while(it -> first > y){ // For all points above graph
    if(bar_graph){
      // Set print_bar for this x value
      print_bar[it -> second] = (y > 0  ?  1 : 0);
    }
    // else: just skip the point
    it++;
  }

  /**********************/
  /***** Draw graph *****/
  /**********************/
  
  /* Plot points */
  // For each y-value from ymax_rnd to ymin_rnd
  // so long as there are points left to plot
  for(; y >= ymin_rnd && it != graphpoints.end(); y -= ystep){

    /* y-axis labelling */
    DEBUG std::cerr << "labelling ";
    // Label padding
    if(y >= 0)
      for(int i = ((y == 0) ? 1 : y)  ; i < 10000000  ; i *= 10) out << " ";
    else
      for(int i = -y; i < 1000000; i *= 10) out << " ";
    out << y << " "; // y-axis label
    out << Y_AXIS_CHAR; // Y-axis line

    DEBUG std::cerr << "y = " << y << std::endl;
    /* Done labelling */
    
    /* Plot points for this y value / row */
    int xpos = xmin;
    for(; it -> first == y && it != graphpoints.end(); ++it){
      DEBUG std::cerr << "Handling pt: (" << it -> first << ", "
		      << it -> second << ")\n";
      // Print filler
      while(xpos < it -> second){
	if(y == 0){
	  // if at y = 0, draw x-axis
	  out << X_AXIS_CHAR << make_str(" ", WIDTH_PAD);
	}
	else{
	  if(bar_graph  &&  print_bar[xpos] != 0){ // this xpos has bar ON
	    if( (y >= 0 && print_bar[xpos] == 1) ||
	        (y <  0 && print_bar[xpos] == 2)   ){
	      out << POINT_CHAR << make_str(" ", WIDTH_PAD); // print bar
	    }
	    else out << make_str(" ", 1 + WIDTH_PAD);
	  }
	  else if(xpos%X_LABEL_DENSITY == 0 && !marked_last_row){
	    out << GUIDELINE_CHAR << make_str(" ", WIDTH_PAD); // x guideline
	  }
	  else out << make_str(" ", 1 + WIDTH_PAD);
	}
	++xpos;
      }// end while

      // Don't plot same point twice...
      if(std::find(used.begin(), used.end(), *it) == used.end()){
	DEBUG std::cerr << "Printing point: (" << y << ", "
			<< xpos << ")\n";
	
	if(!BAR_ZERO_POINT  &&  (bar_graph && y == 0)){
	  // don't print point on axis for bar graphs
	  out << X_AXIS_CHAR << make_str(" ", WIDTH_PAD);
	}
	else{
	  out << POINT_CHAR << make_str(" ", WIDTH_PAD); // print point
	}
	if(bar_graph){
	  print_bar[xpos] = (y >= 0  ?  1 : 0); // set bar for this xpos
	}
	++xpos;
	used.push_back(*it);
      }
    }// end for
    DEBUG std::cerr << "finished line " << y << std::endl;

    /* Fill remainder of row */
    for(; xpos <= xmax; ++xpos){
      if(y == 0){
	// if at y = 0, draw x-axis
	out << X_AXIS_CHAR << make_str(" ", WIDTH_PAD);
      }
      else{
	if(bar_graph  &&  print_bar[xpos] != 0){ // if this xpos has bar ON
	  if( (y >= 0 && print_bar[xpos] == 1) ||
	      (y <  0 && print_bar[xpos] == 2)   ){
	    out << POINT_CHAR << make_str(" ", WIDTH_PAD); // bar
	  }
	  else out << make_str(" ", 1 + WIDTH_PAD);
	}
	else if(xpos%X_LABEL_DENSITY == 0 && !marked_last_row){
	  out << GUIDELINE_CHAR << make_str(" ", WIDTH_PAD); // x guideline
	}
	else out << make_str(" ", 1 + WIDTH_PAD);
      }// end else
    }// end for
    marked_last_row = (marked_last_row + 1)%GUIDELINE_DENSITY;
    out << std::endl;
  }// end for
  /* Done plotting points */

  
  /* Fill out any remaining rows of graph */
  for(; y >= ymin_rnd; y -= ystep){
    /* y-axis labelling */
    DEBUG std::cerr << "labelling ";
    // Label padding
    if(y >= 0){
      for(int i = ((y == 0) ? 1 : y)  ; i < 10000000  ; i *= 10){
	out << " ";
      }
    }
    else{
      for(int i = -y; i < 1000000; i *= 10){
	out << " ";
      }
    }
    out << y << " "; // y-axis label
    out << Y_AXIS_CHAR; // Y-axis line

    DEBUG std::cerr << "y = " << y << std::endl;
    /* Done labelling */

    
    for(int xpos = xmin; xpos <= xmax; ++xpos){
      if(y == 0){
	out << X_AXIS_CHAR << make_str(" ", WIDTH_PAD);
      }
      else if(bar_graph  &&  print_bar[xpos] != 0){ // if this xpos has bar ON
	if( (y >= 0 && print_bar[xpos] == 1) ||
	    (y <  0 && print_bar[xpos] == 2)   ){
	  out << POINT_CHAR << make_str(" ", WIDTH_PAD); // bar
	}
	else out << make_str(" ", 1 + WIDTH_PAD);
      }
      else if(xpos%X_LABEL_DENSITY == 0 && !marked_last_row){
	out << GUIDELINE_CHAR << make_str(" ", WIDTH_PAD); // x guideline
      }
      else out << make_str(" ", 1 + WIDTH_PAD);
    }
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
    out << make_str("-", 1 + WIDTH_PAD);
  }
  // Print labels
  out << "\n          ";
  for(int x = xmin; x <= xmax; x += X_LABEL_DENSITY){
    if(0 <= x && x <= 9){ // 1 char label
      out << x;
      for(int i = 1; i < X_LABEL_DENSITY*2; ++i){
	out << " ";
      }
      out << make_str(" ", WIDTH_PAD - 1);
    }
    else if((10 <= x && x <= 99) || (-9 <= x && x <= -1)){ // 2 char
      out << x;
      for(int i = 2; i < X_LABEL_DENSITY*2; ++i){
	out << " ";
      }
      out << make_str(" ", WIDTH_PAD - 1);
    }
    else if((100 <= x && x <= 999) || (-99 <= x && x <= -10)){ // 3 char
      out << x;
      for(int i = 3; i < X_LABEL_DENSITY*2; ++i){
	out << " ";
      }
      out << make_str(" ", WIDTH_PAD - 1);
    }
    else if((1000 <= x && x <= 9999) || (-999 <= x && x <= -100)){ // 4 char
      out << x;
      for(int i = 4; i < X_LABEL_DENSITY*2; ++i){
	out << " ";
      }
      out << make_str(" ", WIDTH_PAD - 1);
    }
  }
  out << std::endl << "          " << X_AXIS_LABEL;
}

// Creates a string composed to n*str
std::string make_str(std::string str, const int n,
		     std::string separator /* = "" */){
  if (n < 1){
    return "";
  }
  std::string res = "";
  for(int i = 1; i < n; ++i){
    res += str + separator;
  }
  return res + str;
}
