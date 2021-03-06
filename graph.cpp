/**************************************************/
/* Author: Lukas Lazarek                          */
/* Copyright (C) 2016 Lukas Lazarek               */
/* Please see LICENSE.txt for full license info   */
/**************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include "asciigraph.h"

#define DEBUG if(debug)

void fileGraph(const std::string &path, const bool debug);
void streamGraph(std::istream &in, const bool debug);

int main(int argc, char *argv[]){
  bool debug = false;

  for(int i = 1; i < argc; ++i){
    if(argv[i][0] == '-'){
      switch(argv[i][1]){
      case 'h':
	std::cout << "asciigraph is a utility to produce simple graphs"
	  " of arbitrary data in ascii. The format for running asciigraph"
	  " is as follows:\n\n"
	  "\tasciigraph [-d] <-h | -s | -f </absolute/path/to/file> >\n\n"
	  "The meaning of the switches are...\n\n"
	  "-d\tEnable debug output logging to stderr."
	  " *NOTE* This will break graphs unless stderr is redirected"
	  " elsewhere from the asciigraph's output.\n"
	  "-h\tDisplay this help message.\n"
	  "-s\tPull graph data directly from stdin.\n"
	  "-f\tPull graph data from the specified file.\n\n"
	  "Please read the readme for more information.\n\n" << std::endl;
	break;
	
      case 's':
	DEBUG std::cerr << "Pulling data from stdin..." << std::endl;
	try{
	  streamGraph(std::cin, debug);
	}catch(const invalid_data &e){
	  std::cout << "The data provided is invalid, with error \""
		    << e.what() << "\". Please read the readme for data"
	    " format requirements. Exiting..." << std::endl;
	}
	break;

      case 'f':
	DEBUG std::cerr << "Pulling data from file..." << std::endl;
	if(argc > i + 1){
	  try{
	    fileGraph(argv[i + 1], debug);
	  }catch (const file_not_found &e){
	    std::cout << "Unable to open file, with error \"" << e.what()
		      << "\". Please check the given path, that the file"
	      " exists, and its permissions. Exiting..."
		      << std::endl;
	  }
	}
	else{
	  std::cout << "No file name supplied. Exiting..."
		    << std::endl;
	  return 1;
	}
	break;

      case 'd':
	debug = true;
	DEBUG std::cerr << "DEBUG turned on" << std::endl;
	break;

      default:
	std::cout << "Invalid option supplied. For help, try \"-h\". Exiting..."
		  << std::endl;
	return 1;
      }// end switch
    }// end if
  }// end for
}

/* fileGraph():
   Graphs the data stored in the file path specified. 
   
   @params
   const std::string &path     The path of the file containing data to graph
   const bool debug            Print debug info?

   @return
   void

   @throws
   file_not_found              File unable to be opened
*/
void fileGraph(const std::string &path, const bool debug){
  std::ifstream file(path);
  if(file.is_open()){
    try{
      streamGraph(file, debug);
    }catch(const invalid_data &e){
      std::cout << "The data provided is invalid, with error \""
		<< e.what() << "\". Please read the readme"
	" for data format requirements. Exiting..." << std::endl;
    }
    file.close();
  }
  else{
    throw file_not_found("File not found");
  }
}

/* streamGraph():
   Graphs data obtained from the given istream.
   
   @params
   std::istream &in       The stream from which to read data to graph
   const bool debug       Print debug info?

   @return
   void

   @throws
   invalid_data           Data invalid format or invalid limits
*/
void streamGraph(std::istream &in, const bool debug){
  int xmin  = -1, xmax  = -1;
  int ymin  = 0,  ymax  = 0;
  int xstep = 1,  ystep = 1;
  int hmax  = 0;
  bool xmin_set = false,  xmax_set  = false,
       ymin_set = false,  ymax_set  = false,
       hmax_set = false,  bar_graph = false,
              BAR_ZERO_POINT    = BAR_ZERO_POINT_DEFAULT;
  char        X_AXIS_CHAR       = X_AXIS_CHAR_DEFAULT,
              Y_AXIS_CHAR       = Y_AXIS_CHAR_DEFAULT,
              GUIDELINE_CHAR    = GUIDELINE_CHAR_DEFAULT,
              POINT_CHAR        = POINT_CHAR_DEFAULT;
  int         X_LABEL_DENSITY   = X_LABEL_DENSITY_DEFAULT,
              GUIDELINE_DENSITY = GUIDELINE_DENSITY_DEFAULT,
              WIDTH_PAD         = WIDTH_PAD_DEFAULT;
  std::string X_AXIS_LABEL      = X_AXIS_LABEL_DEFAULT,
              Y_AXIS_LABEL      = Y_AXIS_LABEL_DEFAULT;



  std::vector<std::pair<int, int>> pts;
  
  std::string line;
  bool file_continues = getline(in, line);

  /* Handle graph options if any */
  try{
    while(line != ""      &&
	  file_continues  &&
	  (line.c_str()[0] == '#' || line.c_str()[0] == ';')){
      if(line.c_str()[0] == ';'){
	DEBUG std::cerr << "skipping comment..." << std::endl;
      }
      else if(line.compare(1, 5, "ystep") == 0){
	ystep = std::stoi(line.substr(7));
	if(ystep <= 0) ystep = 1;
	DEBUG std::cerr << "Set ystep to " << ystep << std::endl;
      }
      else if(line.compare(1, 4, "ymin") == 0){
	ymin = std::stoi(line.substr(6));
	ymin_set = true;
	DEBUG std::cerr << "Set ymin to " << ymin << std::endl;
      }
      else if(line.compare(1, 4, "ymax") == 0){
	ymax = std::stoi(line.substr(6));
	ymax_set = true;
	DEBUG std::cerr << "Set ymax to " << ymax << std::endl;
      }
      else if(line.compare(1, 4, "xmin") == 0){
	xmin = std::stoi(line.substr(6));
	xmin_set = true;
	DEBUG std::cerr << "Set xmin to " << xmin << std::endl;
      }
      else if(line.compare(1, 4, "xmax") == 0){
	xmax = std::stoi(line.substr(6));
	xmax_set = true;
	DEBUG std::cerr << "Set xmax to " << xmax << std::endl;
      }
      else if(line.compare(1, 4, "hmax") == 0){
	hmax = std::stoi(line.substr(6));
	hmax_set = true;
	DEBUG std::cerr << "Set hmax to " << hmax << std::endl;
      }
      else if(line.compare(1, 11, "X_AXIS_CHAR") == 0){
	X_AXIS_CHAR = line.substr(13, 1).c_str()[0];
	DEBUG std::cerr << "Set X_AXIS_CHAR to " << X_AXIS_CHAR << std::endl;
      }
      else if(line.compare(1, 11, "Y_AXIS_CHAR") == 0){
	Y_AXIS_CHAR = line.substr(13, 1).c_str()[0];
	DEBUG std::cerr << "Set Y_AXIS_CHAR to " << Y_AXIS_CHAR << std::endl;
      }
      else if(line.compare(1, 14, "GUIDELINE_CHAR") == 0){
	GUIDELINE_CHAR = line.substr(16, 1).c_str()[0];
	DEBUG std::cerr << "Set GUIDELINE_CHAR to " << GUIDELINE_CHAR
			<< std::endl;
      }
      else if(line.compare(1, 10, "POINT_CHAR") == 0){
	POINT_CHAR = line.substr(12, 1).c_str()[0];
	DEBUG std::cerr << "Set POINT_CHAR to " << POINT_CHAR << std::endl;
      }
      else if(line.compare(1, 15, "X_LABEL_DENSITY") == 0){
	X_LABEL_DENSITY = std::stoi(line.substr(17));
	DEBUG std::cerr << "Set X_LABEL_DENSITY to " << X_LABEL_DENSITY
			<< std::endl;
      }
      else if(line.compare(1, 17, "GUIDELINE_DENSITY") == 0){
	GUIDELINE_DENSITY = std::stoi(line.substr(19));
	DEBUG std::cerr << "Set GUIDELINE_DENSITY to " << GUIDELINE_DENSITY
			<< std::endl;
      }
      else if(line.compare(1, 12, "X_AXIS_LABEL") == 0){
	X_AXIS_LABEL = line.substr(14);
	DEBUG std::cerr << "Set X_AXIS_LABEL to " << X_AXIS_LABEL
			<< std::endl;
      }
      else if(line.compare(1, 12, "Y_AXIS_LABEL") == 0){
	Y_AXIS_LABEL = line.substr(14);
	DEBUG std::cerr << "Set Y_AXIS_LABEL to " << Y_AXIS_LABEL
			<< std::endl;
      }
      else if(line.compare(1, 3, "bar") == 0){
	bar_graph = true;
	DEBUG std::cerr << "Switching to bar graph mode."
			<< std::endl;
      }
      else if(line.compare(1, 14, "BAR_ZERO_POINT") == 0){
	BAR_ZERO_POINT = true;
	DEBUG std::cerr << "Set BAR_ZERO_POINT to " << BAR_ZERO_POINT
			<< std::endl;
      }
      else if(line.compare(1, 9, "WIDTH_PAD") == 0){
	WIDTH_PAD = std::stoi(line.substr(11));
	DEBUG std::cerr << "Set WIDTH_PAD to " << WIDTH_PAD
			<< std::endl;
      }
      else{
	DEBUG std::cerr << "Skipping invalid option: \"" << line
			<< "\"" << std::endl;
      }	
      getline(in, line);
    }
  }catch(const std::invalid_argument &e){
    throw invalid_data("invalid option settings");
  }catch(const std::out_of_range &e){
    throw invalid_data("invalid option settings");
  }
  
  if(line == "" || !file_continues) return;

  std::size_t pos = line.find(",");
  
  // Check graph type
  if(!bar_graph){
    /************************/
    /** Standard data plot **/
    /************************/
    
    // Which kind? Basic (y) or scatter (x, y)?
    if(pos != std::string::npos){
      /*******************/
      /** scatter input **/
      /*******************/

      DEBUG std::cerr << "parsing data as scatter input" << std::endl;
      
      // Interpret "val1, val2" as point: (x, y)
      for(int i = 0; line != "" && file_continues;
	  ++i, file_continues = getline(in, line)){
	// Check if comment
	if (line.c_str()[0] == ';'){
	  DEBUG std::cerr << "skipping comment..." << std::endl;
	  continue;
	}
	// Parse line
	pos = line.find(",");
	DEBUG std::cerr << "parsing line {" << line << "}" << std::endl;
	int x, y;
	try{
	  x = std::stoi(line.substr(0, pos));
	  y = std::stoi(line.substr(pos + 1));
	}catch(const std::invalid_argument &e){
	  throw invalid_data("invalid format");
	}
	DEBUG std::cerr << "into x=" << x << "\ty=" << y << std::endl;
      
	if(i == 0){
	  if(!xmin_set) xmin = x;
	  if(!xmax_set) xmax = x;
	  if(!ymin_set) ymin = y;
	  if(!ymax_set) ymax = y;
	}
	if(!xmin_set && x < xmin) xmin = x;
	if(!xmax_set && x > xmax) xmax = x;
	if(!ymin_set && y < ymin) ymin = y;
	if(!ymax_set && y > ymax) ymax = y;
	pts.push_back(std::pair<int, int>(x, y));
	DEBUG std::cerr << "getting next line..." << std::endl;
      }

      // Ensure graph height <= hmax
      if(hmax_set){
	int minstep_fit = (ymax - ymin)/hmax + 1;
	if(ystep < minstep_fit){
	  DEBUG std::cerr << "Adjusting ystep to " << minstep_fit
			  << " in order to satisfy hmax" << std::endl;
	  ystep = minstep_fit;
	}
      }
    
      std::cout << "\n\n";

      try{
	asciigraph ag(pts, xmin, xmax, xstep, ymin, ymax, ystep,
		      debug,
		      X_AXIS_CHAR, Y_AXIS_CHAR, GUIDELINE_CHAR, POINT_CHAR,
		      X_LABEL_DENSITY, GUIDELINE_DENSITY, X_AXIS_LABEL,
		      Y_AXIS_LABEL, WIDTH_PAD);
	ag(std::cout);
      }catch(const std::logic_error &e){
	throw invalid_data("invalid limit values");
      }
    }// end if(pos != std::string::npos)
    else{
      /*****************/
      /** basic input **/
      /*****************/

      DEBUG std::cerr << "parsing data as basic input" << std::endl;

      // Interpret "val1" as value to be graphed against integer counter from 0
      int i = 0;
      for(; line != "" && file_continues;
	  ++i, file_continues = getline(in, line)){
	// Check if comment
	if (line.c_str()[0] == ';'){
	  DEBUG std::cerr << "skipping comment..." << std::endl;
	  continue;
	}
	// Parse line
	int y;
	try{
	  y = std::stoi(line);
	}catch(const std::invalid_argument &e){
	  throw invalid_data("invalid format");
	}
      
	if(i == 0){
	  if(!ymin_set) ymin = y;
	  if(!ymax_set) ymax = y;
	}
	if(!ymin_set && y < ymin) ymin = y;
	if(!ymax_set && y > ymax) ymax = y;
	DEBUG std::cerr << "parsing line {" << line << "}" << " into ("
			<< i << ", " << y << ")" << std::endl;
	pts.push_back(std::pair<int, int>(i, y));
      }
      DEBUG std::cerr << "min: " << ymin << ", max: " << ymax << std::endl;

      // Ensure graph height <= hmax
      if(hmax_set){
	int minstep_fit = (ymax - ymin)/hmax + 1;
	if(ystep < minstep_fit){
	  DEBUG std::cerr << "Adjusting ystep to " << minstep_fit
			  << " in order to satisfy hmax" << std::endl;
	  ystep = minstep_fit;
	}
      }
    
      std::cout << "\n\n";

      try{
	if(!xmin_set) xmin = 0;
	if(!xmax_set) xmax = i - 1;
	asciigraph ag(pts, xmin, xmax, xstep, ymin, ymax, ystep,
		      debug,
		      X_AXIS_CHAR, Y_AXIS_CHAR, GUIDELINE_CHAR, POINT_CHAR,
		      X_LABEL_DENSITY, GUIDELINE_DENSITY, X_AXIS_LABEL,
		      Y_AXIS_LABEL, WIDTH_PAD);
	ag(std::cout);
      }catch(const std::logic_error &e){
	throw invalid_data("invalid limit values");
      }
    }
  }// end if(!bar_graph)
  else{
    /***************/
    /** bar graph **/
    /***************/

    DEBUG std::cerr << "parsing data as bar graph" << std::endl;

    X_AXIS_LABEL += "\n\n== LEGEND ==";

    // lines in format "val, label"
    int i = 0;
    for(; line != "" && file_continues;
	++i, file_continues = getline(in, line)){
      // Check if comment
      if (line.c_str()[0] == ';'){
	DEBUG std::cerr << "skipping comment..." << std::endl;
	continue;
      }
      // Parse line
      pos = line.find(",");
      DEBUG std::cerr << "parsing line {" << line << "}" << std::endl;
      int y;
      std::string label;
      try{
	y = std::stoi(line.substr(0, pos));
	label = line.substr(pos + 1);
      }catch(const std::invalid_argument &e){
	throw invalid_data("invalid format");
      }
      DEBUG std::cerr << "into [" << label << ": " << y << "]" << std::endl;
      
      if(i == 0){
	if(!ymin_set) ymin = y;
	if(!ymax_set) ymax = y;
      }
      if(!ymin_set && y < ymin) ymin = y;
      if(!ymax_set && y > ymax) ymax = y;
      pts.push_back(std::pair<int, int>(i, y));
      X_AXIS_LABEL += "\n" + std::to_string(i) + " =" + label;
      DEBUG std::cerr << "getting next line..." << std::endl;
    }

    // Ensure graph height <= hmax
    if(hmax_set){
      int minstep_fit = (ymax - ymin)/hmax + 1;
      if(ystep < minstep_fit){
	DEBUG std::cerr << "Adjusting ystep to " << minstep_fit
			<< " in order to satisfy hmax" << std::endl;
	ystep = minstep_fit;
      }
    }

    std::cout << "\n\n";

    try{
      // Set bar graph defaults (if not explicitly user-set)
      if(X_LABEL_DENSITY == X_LABEL_DENSITY_DEFAULT){
	X_LABEL_DENSITY = 1;
      }
      if(!ymin_set && ymin > 0){
	ymin = 0;
      }
      if(!xmin_set) xmin = 0;
      if(!xmax_set) xmax = i - 1;
	
      asciigraph ag(pts, xmin, xmax, xstep, ymin, ymax, ystep,
		    debug,
		    X_AXIS_CHAR, Y_AXIS_CHAR, GUIDELINE_CHAR, POINT_CHAR,
		    X_LABEL_DENSITY, GUIDELINE_DENSITY, X_AXIS_LABEL,
		    Y_AXIS_LABEL, WIDTH_PAD, BAR_ZERO_POINT);
      ag(std::cout, true);
    }catch(const std::logic_error &e){
      throw invalid_data("invalid limit values");
    }
  }
}
