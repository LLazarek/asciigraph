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
  int minx = -1, maxx = -1;
  int miny = 0, maxy = 0;
  int xstep = 1, ystep = 1;
  bool minx_set = false, maxx_set = false,
       miny_set = false, maxy_set = false;
  char X_AXIS_CHAR       = X_AXIS_CHAR_DEFAULT,
       Y_AXIS_CHAR       = Y_AXIS_CHAR_DEFAULT,
       GUIDELINE_CHAR    = GUIDELINE_CHAR_DEFAULT,
       POINT_CHAR        = POINT_CHAR_DEFAULT;
  int  X_LABEL_DENSITY   = X_LABEL_DENSITY_DEFAULT,
       GUIDELINE_DENSITY = GUIDELINE_DENSITY_DEFAULT;



  std::vector<std::pair<int, int>> pts;
  
  std::string line;
  bool file_continues = getline(in, line);

  /* Handle graph options if any */
  try{
    while(line != "" && file_continues && line.c_str()[0] == '#'){
      if(line.compare(1, 5, "ystep") == 0){
	ystep = std::stoi(line.substr(7));
	if(ystep <= 0) ystep = 1;
	DEBUG std::cerr << "Set ystep to " << ystep << std::endl;
      }
      else if(line.compare(1, 4, "miny") == 0){
	miny = std::stoi(line.substr(6));
	miny_set = true;
      }
      else if(line.compare(1, 4, "maxy") == 0){
	maxy = std::stoi(line.substr(6));
	maxy_set = true;
      }
      else if(line.compare(1, 4, "minx") == 0){
	minx = std::stoi(line.substr(6));
	minx_set = true;
      }
      else if(line.compare(1, 4, "maxx") == 0){
	maxx = std::stoi(line.substr(6));
	maxx_set = true;
      }
      else if(line.compare(1, 11, "X_AXIS_CHAR") == 0){
	X_AXIS_CHAR = line.substr(13, 1).c_str()[0];
      }
      else if(line.compare(1, 11, "Y_AXIS_CHAR") == 0){
	Y_AXIS_CHAR = line.substr(13, 1).c_str()[0];
      }
      else if(line.compare(1, 14, "GUIDELINE_CHAR") == 0){
	GUIDELINE_CHAR = line.substr(16, 1).c_str()[0];
      }
      else if(line.compare(1, 10, "POINT_CHAR") == 0){
	POINT_CHAR = line.substr(12, 1).c_str()[0];
      }
      else if(line.compare(1, 15, "X_LABEL_DENSITY") == 0){
	X_LABEL_DENSITY = std::stoi(line.substr(17));
      }
      else if(line.compare(1, 17, "GUIDELINE_DENSITY") == 0){
	GUIDELINE_DENSITY = std::stoi(line.substr(19));
      }
      getline(in, line);
    }
  }catch(const std::invalid_argument &e){
    throw invalid_data("invalid option settings");
  }catch(const std::out_of_range &e){
    throw invalid_data("invalid option settings");
  }
  
  if(line == "" || !file_continues) return;
  
  // Determine type of data being input: (y) or (x, y)
  std::size_t pos = line.find(",");
  
  if(pos != std::string::npos){  // (x, y) input
    // Interpret "val1, val2" as point: (x, y)
    for(int i = 0; line != "" && file_continues;
	++i, file_continues = getline(in, line)){
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
	if(!minx_set) minx = x;
	if(!maxx_set) maxx = x;
	if(!miny_set) miny = y;
	if(!maxy_set) maxy = y;
      }
      if(!minx_set && x < minx) minx = x;
      if(!maxx_set && x > maxx) maxx = x;
      if(!miny_set && y < miny) miny = y;
      if(!maxy_set && y > maxy) maxy = y;
      pts.push_back(std::pair<int, int>(x, y));
      DEBUG std::cerr << "getting next line..." << std::endl;
    }
    std::cout << "\n\n";

    try{
      asciigraph ag(pts, minx, maxx, xstep, miny, maxy, ystep,
		    debug,
		    X_AXIS_CHAR, Y_AXIS_CHAR, GUIDELINE_CHAR, POINT_CHAR,
		    X_LABEL_DENSITY, GUIDELINE_DENSITY);
      ag(std::cout);
    }catch(const std::logic_error &e){
      throw invalid_data("invalid limit values");
    }
  }
  else{ // (y)
    // Interpret "val1" as value to be graphed against integer counter from 0
    int i = 0;
    for(; line != "" && file_continues;
	++i, file_continues = getline(in, line)){
      // Parse line
      int y;
      try{
	y = std::stoi(line);
      }catch(const std::invalid_argument &e){
	throw invalid_data("invalid format");
      }
      
      if(i == 0){
	if(!miny_set) miny = y;
	if(!maxy_set) maxy = y;
      }
      if(!miny_set && y < miny) miny = y;
      if(!maxy_set && y > maxy) maxy = y;
      DEBUG std::cerr << "parsing line {" << line << "}" << " into ("
		      << i << ", " << y << ")" << std::endl;
      pts.push_back(std::pair<int, int>(i, y));
    }
    DEBUG std::cerr << "min: " << miny << ", max: " << maxy << std::endl;
    std::cout << "\n\n";

    try{
      if(!minx_set) minx = 0;
      if(!maxx_set) maxx = i;
      asciigraph ag(pts, minx, maxx, xstep, miny, maxy, ystep,
		    debug,
		    X_AXIS_CHAR, Y_AXIS_CHAR, GUIDELINE_CHAR, POINT_CHAR,
		    X_LABEL_DENSITY, GUIDELINE_DENSITY);
      ag(std::cout);
    }catch(const std::logic_error &e){
      throw invalid_data("invalid limit values");
    }
  }
}
