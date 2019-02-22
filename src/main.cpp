// Copyright (C) 2017 by SIB Swiss Institute of Bioinformatics, Julien Dorier and Dimos Goundaroulis.
// 
// This file is part of project Knoto-ID.
// 
// Knoto-ID is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// Knoto-ID is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Knoto-ID.  If not, see <http://www.gnu.org/licenses/>.

#include "main.h"

using namespace std;

string input_filename="stdin";
string output_filename_diagram="";
string output_filename_jones="stdout";
string projectionlist_filename="";
double dx,dy,dz,weight;//projection axis, weight
bool flag_dont_use_fixed_projection=true;
bool flag_debug=false;
string debug_filename="debug";
bool flag_cyclic=false;
bool flag_3d_reduction=true;
bool flag_simplify_diagram=true;
bool flag_planar=false; //diagram is on the plane (not the sphere) 
long max_nb_random_moves_III=100000;
long max_nb_unsuccessfull_random_moves_III=2000;//max number of move without improvement.
int nb_helper_beads_per_segment=0;// for hoomd debug output, add nb_helper_beads_per_segment per segments 
unsigned long seed=time(NULL);
string jones_method="recursive";
unsigned long nb_projections=0;
string input_format="xyz"; //"xyz", "pd" or "gauss"
string output_diagram_format="pd"; //"pd" or "gauss" 
string closure_method="open";
string names_db_filename="";
time_t timeout=0;

void display_usage(char **argv,bool flag_help_all=false){
  cerr<<endl;
  cerr<<"Usage: "<<argv[0]<<" [OPTIONS] FILENAME"<<endl;
  cerr<<endl;
  cerr<<"  Load a piecewise linear curve from file FILENAME. By default,"<<endl;
  cerr<<"  the curve is open (knotoid), but it can be closed (knot) by specifying"<<endl;
  cerr<<"  a closure method with option --closure-method. The input curve is then"<<endl;
  cerr<<"  simplified using a 3D triangle elimination method."<<endl;
  cerr<<endl;
  cerr<<"  Evaluate the knot(oid) diagram obtained by projecting the curve along"<<endl;
  cerr<<"  a randomly chosen projection direction (with uniform distribution on"<<endl;
  cerr<<"  the surface of the sphere) or along the direction specified with"<<endl;
  cerr<<"  option --projection. Alternatively, if --input-format=pd or gauss, load"<<endl;
  cerr<<"  directly the PD code or extended Gauss code for a knot(oid) diagram from file."<<endl;
  cerr<<endl;
  cerr<<"  Simplify the knot(oid) diagram with Reidemeister moves."<<endl;
  cerr<<endl;
  cerr<<"  Evaluate the polynomial invariant for the knot(oid) diagram on the"<<endl;
  cerr<<"  surface of a sphere (by default) or on a plane (with option --planar)."<<endl;
  cerr<<endl;
  cerr<<"  Output the polynomial invariant, and optionally the knot(oid)"<<endl;
  cerr<<"  diagram (with option --output-diagram). If options"<<endl;
  cerr<<"  --nb-projections or --projections-list are used, the above procedure"<<endl;
  cerr<<"  is repeated for all projections, and the output is a distribution of"<<endl;
  cerr<<"  polynomials."<<endl;
  cerr<<endl;
  cerr<<"  Notes:"<<endl;
  cerr<<"    - To read from standard input, use FILENAME=\"stdin\" or \"-\"."<<endl;
  cerr<<"    - Polynomial invariants:"<<endl;
  cerr<<"       - when --closure-method=open but without --planar:"<<endl;
  cerr<<"         Jones polynomial for knotoids (on the sphere)."<<endl;
  cerr<<"       - when --closure-method=open and using --planar:"<<endl;
  cerr<<"         Turaev loop bracket for knotoids (on the plane)."<<endl;
  cerr<<"       - when --closure-method=direct or rays:"<<endl;
  cerr<<"         Classical Jones polynomial for knots."<<endl;
  cerr<<"    - By default, the polynomial invariant is the Jones polynomial"<<endl;
  cerr<<"      for knotoids (open curve, knotoid diagram on a sphere)."<<endl;
  cerr<<"    - See the user manual for more information."<<endl;
  cerr<<endl;
  cerr<<"  Input file format (xyz):"<<endl;
  cerr<<"  - Space or tab separated value format with three columns corresponding"<<endl;
  cerr<<"    to x, y and z coordinates."<<endl;
  cerr<<"  - Each row corresponds to a point of the input curve."<<endl;
  cerr<<"  - Consecutive rows correspond to consecutive points on the curve."<<endl;
  cerr<<"  - All points should be distinct. In particular the first"<<endl;
  cerr<<"    and last points should not be equal. To define a closed"<<endl;
  cerr<<"    curve, use argument --closure-method."<<endl;
  cerr<<endl;
  cerr<<"OPTIONS:"<<endl;
  cerr<<"  -h, --help"<<endl;  
  cerr<<"           print usage."<<endl;
  cerr<<endl;
  cerr<<"      --help-debug"<<endl;  
  cerr<<"           print usage, with debugging options."<<endl;
  cerr<<endl;
  cerr<<"  -V, --version"<<endl;  
  cerr<<"           print version."<<endl;
  cerr<<endl;
  cerr<<"  -s, --seed=S"<<endl;  
  cerr<<"           initialize the random number generator with seed S."<<endl;
  cerr<<"           If not specified, the seed is taken from the current time."<<endl;
  cerr<<endl;
  cerr<<"  -F, --input-format=F"<<endl;  
  cerr<<"           input format (default "<<input_format<<")."<<endl;
  cerr<<"           Possible values for F:"<<endl;
  cerr<<"            F=xyz   piecewise linear curve in xyz file format"<<endl;
  cerr<<"            F=pd    PD code for knot(oid) diagram in KnotTheory format"<<endl;
  cerr<<"                    (http://katlas.org/wiki/Planar_Diagrams)."<<endl;
  cerr<<"                    With input format \"pd\", options --projection,"<<endl;
  cerr<<"                    --nb-projections, --projections-list and"<<endl;
  cerr<<"                    --closure-method are ignored."<<endl;
  cerr<<"                    The input file can contain multiple PD codes."<<endl;
  cerr<<"            F=gauss extended Gauss code for knot(oid) diagram."<<endl;
  cerr<<"                    If --closure-method=open the diagram is open (knotoid)"<<endl;
  cerr<<"                    If --closure-method=direct or rays the diagram is closed (knot)"<<endl;
  cerr<<"                    With input format \"gauss\", options --projection,"<<endl;
  cerr<<"                    --nb-projections and --projections-list are ignored."<<endl;
  cerr<<"                    The input file can contain multiple Gauss codes (one per line)."<<endl;
  cerr<<endl;
  cerr<<"  -o, --output=FILENAME"<<endl;  
  cerr<<"           output the polynomial or distribution of polynomials"<<endl;
  cerr<<"           to FILENAME (default "<<output_filename_jones<<")."<<endl;
  cerr<<"           To write to standard output, use FILENAME=stdout or FILENAME=-"<<endl;
  cerr<<endl;
  cerr<<"      --output-diagram=FILENAME"<<endl;  
  cerr<<"           output the knot(oid) diagram or list of diagrams to file "<<endl;
  cerr<<"           using format specified with --output-diagram-format."<<endl;
  cerr<<"           To write to standard output, use FILENAME=stdout or FILENAME=-"<<endl;
  cerr<<endl;
  cerr<<"      --output-diagram-format=F"<<endl;  
  cerr<<"           output format for knot(oid) diagrams (default "<<output_diagram_format<<")."<<endl;
  cerr<<"           Possible values for F:"<<endl;
  cerr<<"            F=pd    PD code for knot(oid) diagram in KnotTheory format"<<endl;
  cerr<<"                    (http://katlas.org/wiki/Planar_Diagrams)."<<endl;
  cerr<<"            F=gauss extended Gauss code for knot(oid) diagram."<<endl;
  cerr<<endl;
  cerr<<"  -m, --closure-method=M"<<endl;  
  cerr<<"           specify how to close the input curve (default "<<closure_method<<")."<<endl;
  cerr<<"           Possible values for M:"<<endl;
  cerr<<"            M=open   =>  the curve is open (knotoid)."<<endl;
  cerr<<"            M=direct =>  connect last and first point by a straight line."<<endl;
  cerr<<"            M=rays   =>  close by extending two parallel rays along the projection"<<endl;
  cerr<<"                         direction, each originating from one of the endpoints"<<endl;
  cerr<<"                         of the curve, and connecting them  outside of the sphere"<<endl;
  cerr<<"                         enclosing the curve."<<endl;
  cerr<<endl;
  cerr<<"  -p, --planar"<<endl;
  cerr<<"           evaluate the polynomial invariant for the knot(oid) diagram on"<<endl;
  cerr<<"           a plane. If not given, evaluate the polynomial invariant for"<<endl;
  cerr<<"           the knot(oid) diagram on the surface of a sphere."<<endl;
  cerr<<"           This option is only relevant for open curves (--closure-method=open)."<<endl;
  cerr<<endl;
  cerr<<"      --nb-moves-III=N"<<endl;
  cerr<<"           max number of iterations for simplification with random Reidemeister"<<endl;
  cerr<<"           moves III (default="<<max_nb_random_moves_III<<")."<<endl;
  cerr<<endl;
  cerr<<"      --nb-unsuccessfull-moves-III=N"<<endl;
  cerr<<"           stop simplification with random Reidemeister if the diagram has not"<<endl;
  cerr<<"           been simplified during the last N iterations (default="<<max_nb_unsuccessfull_random_moves_III<<")."<<endl;
  cerr<<endl;
  cerr<<"      --projection=X,Y,Z"<<endl;
  cerr<<"           project the curve along projection direction (X,Y,Z)."<<endl;
  cerr<<"           If not specified, use randomly chosen projection direction"<<endl;
  cerr<<"           (with uniform distribution on the surface of the sphere)."<<endl;
  cerr<<endl;
  cerr<<"  -N, --nb-projections=N"<<endl;
  cerr<<"           choose N random projection directions (with uniform distribution on"<<endl;
  cerr<<"           the surface of the sphere) and evaluate the corresponding polynomial"<<endl;
  cerr<<"           invariant for each projection."<<endl;
  cerr<<"           Notes:"<<endl;
  cerr<<"            - Instead of a unique polynomial, the output specified with --output"<<endl;
  cerr<<"              will contain the distribution of polynomials."<<endl;
  cerr<<"            - The output specified with --output-diagram will contain"<<endl;
  cerr<<"              a list of projection axis with corresponding knot(oid)"<<endl;
  cerr<<"              diagrams (with format specified with --output-diagram-format) and"<<endl;
  cerr<<"              polynomials, in tab separated format."<<endl;
  cerr<<"            - For cyclic curves with closure method \"direct\" the polynomial"<<endl;
  cerr<<"              invariant (classical Jones polynomial) does not depend on the"<<endl;
  cerr<<"              projection. To avoid useless computations, N will be set to 1."<<endl;
  cerr<<"            - Option --projection will be ignored."<<endl;
  cerr<<endl;
  cerr<<"      --projections-list=FILENAME"<<endl;
  cerr<<"           load list of projection directions from file FILENAME"<<endl;
  cerr<<"           and evaluate the corresponding polynomial invariant for each projection."<<endl;
  cerr<<"           File FILENAME should have the following format:"<<endl;
  cerr<<"            - space or tab separated with three or four columns."<<endl;
  cerr<<"            - columns 1,2,3: x,y,z coordinates for the projection direction."<<endl;
  cerr<<"            - column 4: weight (optional)."<<endl;
  cerr<<"            - Each row corresponds to one projection direction."<<endl;  
  cerr<<"           Notes:"<<endl;
  cerr<<"            - Instead of a unique polynomial, the output specified with --output"<<endl;
  cerr<<"              will contain the distribution of polynomials."<<endl;
  cerr<<"            - The output specified with --output-diagram will contain"<<endl;
  cerr<<"              a list of projection axis with corresponding knot(oid)"<<endl;
  cerr<<"              diagrams (with format specified with --output-diagram-format) and"<<endl;
  cerr<<"              polynomials, in tab separated format."<<endl;
  cerr<<"            - For cyclic curves with closure method \"direct\" the polynomial"<<endl;
  cerr<<"              invariant (classical Jones polynomial) does not depend on the"<<endl;
  cerr<<"              projection. To avoid useless computations, N will be set to 1."<<endl;
  cerr<<"            - Options --projection and --nb-projections will be ignored."<<endl;
  cerr<<endl;
  cerr<<"  -n, --names-db=FILENAME"<<endl;
  cerr<<"           load a list of polynomials with corresponding knot(oid) name from"<<endl;
  cerr<<"           file FILENAME. File FILENAME should have the following format."<<endl;
  cerr<<"            - tab separated with two columns."<<endl;
  cerr<<"            - column 1: knot/knotoid name."<<endl;
  cerr<<"            - column 2: polynomial."<<endl;
  cerr<<endl;
  cerr<<"      --timeout=T"<<endl;
  cerr<<"           Abort evaluation of polynomial invariant after T seconds (integer number)."<<endl;
  cerr<<"           Note: if the evaluation of the polynomial invariant is aborted, the"<<endl;
  cerr<<"           polynomial is replaced by TIMEOUT."<<endl;
  cerr<<endl;
  
  cerr<<endl;
  if(flag_help_all)
    {
      cerr<<"DEBUGGING OPTIONS:"<<endl;
      cerr<<"      --no-3D-reduction"<<endl;
      cerr<<"           do not simplify 3D input curve."<<endl;
      cerr<<endl;
      cerr<<"      --no-diagram-simplification"<<endl;
      cerr<<"           do not simplify the knot(oid) diagram."<<endl;
      cerr<<endl;
      cerr<<"      --polynomial-method=M"<<endl;
      cerr<<"           use method M to evaluate polynomial (default "<<jones_method<<")"<<endl;
      cerr<<"           Possible values are:"<<endl;
      cerr<<"               M=simple   (i.e. brute force)"<<endl;
      cerr<<"               M=recursive"<<endl;
      cerr<<"               M=recursive-crossing-order"<<endl;
      cerr<<"               M=recursive-arc-order"<<endl;
      cerr<<"               M=recursive-region-order"<<endl;
      cerr<<endl;
      cerr<<"  -d, --debug"<<endl;
      cerr<<"           save intermediate files (debug_*) and increase verbosity."<<endl;
      cerr<<endl;
      cerr<<"      --nb-helper-beads=N"<<endl;
      cerr<<"           add N helper beads for each segment in hoomd output files debug* (only used with --debug)."<<endl;
    }    
  cerr<<""<<endl;

}

vector<string> split_string(string str,string sep_list);
//split input with multiple PDs into strings, one per PD
vector<string> split_input(string str);

int main(int argc, char **argv)
{

  
  int opt = 0;
  int longIndex = 0;
  static const char *optString = "ho:ds:VN:F:n:m:p";
  static const struct option longOpts[] = {
    { "help", no_argument, NULL, 'h' },
    { "output", required_argument, NULL, 'o' },
    { "debug", no_argument, NULL, 'd' },
    { "seed", required_argument, NULL, 's' },
    { "input-format", required_argument, NULL, 'F' },
    { "version", no_argument, NULL, 'V' },
    { "names-db", required_argument, NULL, 'n' },
    { "nb-projections", required_argument, NULL, 'N' },
    { "projection", required_argument, NULL, 1 },
    { "no-3D-reduction", no_argument, NULL, 2 },
    { "nb-moves-III", required_argument, NULL, 3 },
    { "nb-unsuccessfull-moves-III", required_argument, NULL, 12 },
    { "no-diagram-simplification", no_argument, NULL, 4 },
    { "planar", no_argument, NULL, 'p' },
    { "nb-helper-beads", required_argument, NULL, 6 },
    { "output-diagram", required_argument, NULL, 7 },
    { "polynomial-method", required_argument, NULL, 8 },
    { "help-debug", no_argument, NULL, 9 },
    { "closure-method", required_argument, NULL, 'm' },
    { "timeout", required_argument, NULL, 11 },
    { "projections-list", required_argument, NULL, 5 },
    { "output-diagram-format", required_argument, NULL, 10 },
    { NULL, no_argument, NULL, 0 }
  };
  opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  while( opt != -1 ) {
    switch( opt ) {
    case 'o':
      output_filename_jones = optarg;
      break;
    case 'F':
      input_format = optarg;
      break;
    case 'd':
      flag_debug = true;
      break;
    case 's':
      seed =atol(optarg);
      break;
    case 'V': 
      cout<<endl;
      cout<<"Knoto-ID version: "<<KNOTOID_VERSION_MAJOR<<"."<<KNOTOID_VERSION_MINOR<<"."<<KNOTOID_VERSION_PATCH<<endl;
      cout<<endl;
      cout<<"Copyright (C) 2017 by SIB Swiss Institute of Bioinformatics, Julien Dorier and Dimos Goundaroulis."<<endl;
      cout<<endl;
      cout<<"Knoto-ID is free software: you can redistribute it and/or modify"<<endl;
      cout<<"it under the terms of the GNU General Public License as published by"<<endl;
      cout<<"the Free Software Foundation, either version 2 of the License, or"<<endl;
      cout<<"(at your option) any later version."<<endl;
      cout<<endl;
      cout<<"Knoto-ID is distributed in the hope that it will be useful,"<<endl;
      cout<<"but WITHOUT ANY WARRANTY; without even the implied warranty of"<<endl;
      cout<<"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"<<endl;
      cout<<"GNU General Public License for more details."<<endl;
      cout<<endl;
      cout<<"You should have received a copy of the GNU General Public License"<<endl;
      cout<<"along with Knoto-ID.  If not, see <http://www.gnu.org/licenses/>."<<endl;
      cout<<endl;
      cout<<"If you use this software for a publication, please cite:"<<endl;
      cout<<"J. Dorier, D. Goundaroulis, F. Benedetti and A. Stasiak, \"Knoto-ID: a tool to study the entanglement of open protein chains using the concept of knotoids\", Bioinformatics 34, 3402-3404 (2018)."<<endl;
      cout<<endl;
      cout<<"If you use the knotoid classification given in files"<<endl;
      cout<<"examples/knotoid_names_sphere.txt or examples/knotoid_names_planar.txt,"<<endl;
      cout<<"please cite:"<<endl;
      cout<<"D. Goundaroulis, J. Dorier and A. Stasiak, \"A systematic classification of knotoids on the plane and on the sphere\", arXiv:1902.07277 [math.GT]"<<endl;
      cout<<endl;
      exit(0);
    case 'N':
      nb_projections =atol(optarg);
      break;
    case 'n':
      names_db_filename = optarg;
      break;
    case 1:
      {
	flag_dont_use_fixed_projection=false;
	vector<string> tmp=split_string(optarg,",");
	if(tmp.size()!=3)
	  {
	    cerr<<"*****************************************************"<<endl;	  
	    cerr<<"ERROR: --projection="<<optarg<<" is invalid."<<endl;
	    cerr<<"*****************************************************"<<endl;	  
	    display_usage(argv);
	    exit(1);	  
	  }
	dx=atof(tmp[0].c_str());
	dy=atof(tmp[1].c_str());
	dz=atof(tmp[2].c_str());
      }
      break;
    case 2:
      flag_3d_reduction = false;
      break;
    case 3:
      max_nb_random_moves_III=atol(optarg);
      break;
    case 12:
      max_nb_unsuccessfull_random_moves_III=atol(optarg);
      break;
    case 4:
      flag_simplify_diagram=false;
      break;
    case 'p':
      flag_planar=true;
      break;
    case 6:
      nb_helper_beads_per_segment=atoi(optarg);
      break;
    case 7:
      output_filename_diagram = optarg;
      break;
    case 8:
      jones_method = optarg;
      break;
    case 9:
      display_usage(argv,true);
      exit(0);
    case 'm':
      closure_method = optarg;
      break;
    case 11:
      timeout=ceil(atof(optarg));
      break;
    case 10:
      output_diagram_format = optarg;
      break;
    case 5:
      projectionlist_filename = optarg;
      break;      
    case 'h':   /* fall-through is intentional */
      display_usage(argv);
      exit(0);
    case '?':
      display_usage(argv);
      exit(1);
    default:
      break;
    }

    opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  }

  if (optind == argc-1)
    {
      input_filename = argv[optind];
    }
  else
    {
      display_usage(argv);
      exit(1);
    }

  if(closure_method=="open")
    {
      flag_cyclic=false;
    }
  else
    {
      flag_cyclic=true;
    }

  ///////////////////////////
  //check
  ///////////////////////////
  if(flag_planar&&flag_cyclic)
    {
      cerr<<"Ignoring argument --planar: for cyclic curves, projection on a sphere and on a plane produce the same classical Jones polynomial."<<endl;
      flag_planar=false;      
    }
  if(projectionlist_filename!=""&&flag_cyclic&&(closure_method=="direct"||closure_method=="straight")) //"straight" for backward compatitibility
    {
      cerr<<"Cyclic curve: ignoring --projections-list and forcing --nb-projection=1"<<endl;
      nb_projections=1;
      projectionlist_filename="";
    }
  if(nb_projections>0&&flag_cyclic&&(closure_method=="direct"||closure_method=="straight")) //"straight" for backward compatitibility
    {
      cerr<<"Cyclic curve: forcing --nb-projection=1"<<endl;
      nb_projections=1;
    }
  if(projectionlist_filename!=""&&flag_dont_use_fixed_projection==false)
    {
      cerr<<"Conflicting parameters --projections-list and --projection. Ignoring --projection."<<endl;
      flag_dont_use_fixed_projection=true;
    }
  if(nb_projections>0&&flag_dont_use_fixed_projection==false)
    {
      cerr<<"Conflicting parameters --nb-projections and --projection. Ignoring --projection."<<endl;
      flag_dont_use_fixed_projection=true;
    }
  if(input_format!="xyz"&&input_format!="pd"&&input_format!="gauss")
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR unknown --input-format="<<input_format<<"."<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }

  random_generator.seed(static_cast<unsigned int>(seed));
  cerr<<"seed: "<<seed<<endl;
  if(flag_debug)
    {
      if(flag_cyclic)
	cerr<<"flag_cyclic=true"<<endl;
      else
	cerr<<"flag_cyclic=false"<<endl;
    }

  
  ///////////////////////////
  //Loading names data base
  ////////////////////////////
  map<string,string> map_jones_to_name;
  if(names_db_filename!="")
    {
      cerr<<"Loading names data base"<<endl;
      istream filein(cin.rdbuf());
      ifstream filein_tmp;
      map<string,string>::iterator it_map;
      if(names_db_filename!="stdin"&&names_db_filename!="-")
	{
	  filein_tmp.open(names_db_filename.c_str());
	  filein.rdbuf(filein_tmp.rdbuf());
	  if(!filein_tmp.good())
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR problem with file "<<names_db_filename <<"."<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	}
      string line;
      long line_no=0;
      vector<string> name_jones_tmp;
      while (filein.good())
	{
	  line_no++;
	  getline(filein, line);
	  name_jones_tmp=split_string(line,"\t");
	  if(name_jones_tmp.size()==2)
	    {
	      Polynomial p;
	      if(!p.load_from_string(name_jones_tmp[1],"A","v"))
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR --names-db="<<names_db_filename<<endl;
		  cerr<<"Invalid polynomial (line "<<line_no<<"):"<<endl;
		  cerr<<name_jones_tmp[1]<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);
		}
	      if((p.get_variable1_name()!="A"&&p.get_variable1_name()!="v"&&p.get_variable1_name()!="")
		 ||(p.get_variable2_name()!="A"&&p.get_variable2_name()!="v"&&p.get_variable2_name()!=""))
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR --names-db="<<names_db_filename<<" (line "<<line_no<<")"<<endl;
		  cerr<<"Invalid polynomial (line "<<line_no<<"): Variable names should be \"A\" and \"v\"."<<endl;
		  cerr<<name_jones_tmp[1]<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);		  
		}
	      it_map=map_jones_to_name.find(p.to_string());
	      if(it_map== map_jones_to_name.end())
		{
		  map_jones_to_name[p.to_string()]=name_jones_tmp[0];
		}
	      else
		{
		  it_map->second=it_map->second+"|"+name_jones_tmp[0];
		}
	    }
	  else if(name_jones_tmp.size()!=0)
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR --names-db="<<names_db_filename<<" (line "<<line_no<<"): invalid file format."<<endl;
	      cerr<<line<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	}
      
      if(names_db_filename!="stdin"&&names_db_filename!="-")
	filein_tmp.close();

      //add special cases
      map_jones_to_name["TIMEOUT"]="TIMEOUT";
      map_jones_to_name["failed_projection"]="failed_projection";

    }

  ///////////////////////////
  //Loading list of projections axis
  ////////////////////////////
  vector<vector<double> > projectionlist_projections;
  vector<double> projectionlist_weights;
  if(projectionlist_filename!="")
    {
      cerr<<"Loading projections"<<endl;
      istream filein(cin.rdbuf());
      ifstream filein_tmp;
      if(projectionlist_filename!="stdin"&&projectionlist_filename!="-")
	{
	  filein_tmp.open(projectionlist_filename.c_str());
	  filein.rdbuf(filein_tmp.rdbuf());
	  if(!filein_tmp.good())
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR problem with file "<<projectionlist_filename <<"."<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	}
      string line;
      long line_no=0;
      double totalweight=0;
      while (filein.good())
	{
	  line_no++;
	  getline(filein, line);
	  //remove comments
	  size_t pos1 = line.find('#');
	  if (pos1!=string::npos)
	    {
	      line.erase(pos1);
	    }

	  vector<string> tmpline;
	  tmpline=split_string(line," \t");
	  if(tmpline.size()>=3)
	    {
	      //remove trailing \r 
	      if(tmpline[tmpline.size()-1].size()>0&&tmpline[tmpline.size()-1][tmpline[tmpline.size()-1].size()-1]=='\r')
		tmpline[tmpline.size()-1].erase(tmpline[tmpline.size()-1].end() - 1);

	      vector<double> tmp;
	      for(int i=0;i<3;i++)
		tmp.push_back(atof(tmpline[i].c_str()));
	      projectionlist_projections.push_back(tmp);
	      if(tmpline.size()==4)
		{
		  projectionlist_weights.push_back(atof(tmpline[3].c_str()));
		  totalweight+=projectionlist_weights.back();
		}
	      else
		{
		  projectionlist_weights.push_back(1);
		  totalweight+=projectionlist_weights.back();
		}
	    }
	  if(tmpline.size()!=0&&tmpline.size()!=3&&tmpline.size()!=4)
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR --projections-list="<<projectionlist_filename<<" (line "<<line_no<<"): invalid file format."<<endl;
	      cerr<<line<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	}
      if(nb_projections>0)
	{
	  cerr<<"Conflicting parameters --nb-projections and --projections-list. Ignoring --nb-projections."<<endl;
	}
      nb_projections=projectionlist_projections.size();
      //normalize
      for(int i=0;i<projectionlist_weights.size();i++)
	projectionlist_weights[i]=projectionlist_weights[i]/totalweight;
      if(projectionlist_filename!="stdin"&&projectionlist_filename!="-")
	filein_tmp.close();
    }

  
  ///////////////////////////
  //loading
  ///////////////////////////
  if(input_format=="pd"||input_format=="gauss")
    {
      istream filein(cin.rdbuf());
      ifstream filein_tmp;
      if(input_filename!="stdin"&&input_filename!="-")
	{
	  filein_tmp.open(input_filename.c_str());
	  filein.rdbuf(filein_tmp.rdbuf());
	  if(!filein_tmp.good())
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR problem with file "<<input_filename <<"."<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	}
      stringstream filein_str;
      filein_str<<filein.rdbuf();
      vector<string> input_diagrams;
      if(input_format=="pd")
	input_diagrams=split_input(filein_str.str());
      else if(input_format=="gauss")
	input_diagrams=split_string(filein_str.str(),"\n");

      if(input_diagrams.size()==0)
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR: no diagram found in input."<<endl;
	  cerr<<"Check input format (--input-format="<<input_format<<")."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}

      if(input_filename!="stdin"&&input_filename!="-")
	filein_tmp.close();

      //open output file
      ostream file(cout.rdbuf());
      ofstream file_tmp;
      if(output_filename_jones!="stdout"&&output_filename_jones!="-")
	{
	  file_tmp.open(output_filename_jones.c_str());
	  file.rdbuf(file_tmp.rdbuf());
	  if(!file_tmp.good())
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR problem with file "<<output_filename_jones <<"."<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	}
      
      for(int input=0;input<input_diagrams.size();input++)
	{
	  cerr<<"Loading input diagram "<<endl;
	  PlanarDiagram diagram(flag_planar,flag_debug);
	  try
	    {
	      if(input_format=="pd")
		diagram.load_from_string_KnotTheory(input_diagrams[input]);
	      else if(input_format=="gauss")
		diagram.load_from_string_extended_gauss_code(input_diagrams[input],flag_cyclic);
	    }
	  catch(range_error& e)
	    {
	      cerr<<"**************************************"<<endl;
	      cerr<<"ERROR knot(oid) diagram not valid!"<<endl;
	      cerr<<"**************************************"<<endl;
	      exit(1);
	    }
	  flag_cyclic=diagram.is_cyclic();
	  if(flag_cyclic)
	    cerr<<"diagram is cyclic"<<endl;
	  else
	    cerr<<"diagram is not cyclic"<<endl;	  
	  cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
	  if(flag_debug)
	    {
	      cerr<<"diagram"<<endl;
	      diagram.save_to_file_list(cerr);
	    }
	  /////polynomial invariants
	  if(flag_cyclic)
	    {
	      cerr<<"polynomial invariant: classical Jones polynomial."<<endl;
	    }
	  if(!flag_cyclic&&!flag_planar)
	    {
	      cerr<<"polynomial invariant: Jones polynomial for knotoids."<<endl;
	    }
	  if(!flag_cyclic&&flag_planar)
	    {
	      cerr<<"polynomial invariant: Turaev loop bracket."<<endl;
	    }
	  

	  if(flag_simplify_diagram)
	    {
	      cerr<<"Simplifying diagram"<<endl;
	      diagram.simplify();
	      cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
	      if(flag_debug)
		{
		  cerr<<"diagram:"<<endl;
		  diagram.save_to_file_list(cerr);
		}
	      if(max_nb_random_moves_III>0)
		{
		  cerr<<"Simplifying diagram with random Reidemeister moves III (max "<<max_nb_random_moves_III<<" moves)"<<endl;
		  diagram.simplify_with_random_reidemeister_moves_III(max_nb_random_moves_III,max_nb_unsuccessfull_random_moves_III);
		  cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
		  if(flag_debug)
		    {
		      cerr<<"diagram:"<<endl;
		      diagram.save_to_file_list(cerr);
		    }
		  cerr<<"Final simplifying diagram"<<endl;
		  diagram.simplify();
		  cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
		  if(flag_debug)
		    {
		      cerr<<"diagram:"<<endl;
		      diagram.save_to_file_list(cerr);
		    }
		}
	    }
		            
	  //output diagram
	  if(output_filename_diagram!="")
	    {
	      if(output_filename_diagram!="stdout"&&output_filename_diagram!="-")
		cerr<<"saving diagram to file "<<output_filename_diagram<<endl;
	      if(output_diagram_format=="pd")
		diagram.save_to_file_KnotTheory(output_filename_diagram);
	      else if(output_diagram_format=="gauss")
		diagram.save_to_file_extended_gauss_code(output_filename_diagram);
	      else
		{
		  cerr<<"********************************************************"<<endl;
		  cerr<<"ERROR: --output-diagram-format="<<output_diagram_format<<" not implemented."<<endl;
		  cerr<<"Aborting"<<endl;
		  cerr<<"********************************************************"<<endl;
		  exit(1);
		}
	    }
      
	  //////////jones//////////////
	  if(output_filename_jones!="")
	    {      
	      PolynomialInvariant jones(diagram,flag_planar,flag_debug);
	      jones.set_timeout(timeout);
	      Polynomial jones_polynomial;      
	      time_t t0=time(NULL);
	      clock_t clock_t0 = clock();      
	      /////////////jone bgl
	      try
		{
		  if(jones_method=="simple")
		    jones_polynomial=jones.get_polynomial_simple();
		  else if(jones_method=="recursive")
		    jones_polynomial=jones.get_polynomial_recursive();
		  else if(jones_method=="recursive-crossing-order")
		    jones_polynomial=jones.get_polynomial_recursive("crossing_order");
		  else if(jones_method=="recursive-arc-order")
		    jones_polynomial=jones.get_polynomial_recursive("arc_order");
		  else if(jones_method=="recursive-region-order")
		    jones_polynomial=jones.get_polynomial_recursive("region_order");
		  else
		    {
		      cerr<<"********************************************************"<<endl;	  
		      cerr<<"ERROR: --polynomial-method="<<jones_method<<" not implemented"<<endl;
		      cerr<<"********************************************************"<<endl;	  
		      exit(1);
		    }
      

		  if(flag_debug)cerr<<"polynomial: time "<<time(NULL)-t0<<" s"<<endl;
		  if(flag_debug)cerr<<"polynomial: time_clock "<<((float)(clock() - clock_t0))/CLOCKS_PER_SEC<<" s "<<endl;

		  string name="UNKNOWN";	      
		  map<string,string>::iterator it_name=map_jones_to_name.find(jones_polynomial.to_string());
		  if (it_name != map_jones_to_name.end())
		    name=it_name->second;
		  if(output_filename_jones=="stdout"||output_filename_jones=="-")
		    {
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    cout<<"Knot type: "<<name<<"\t";
			  else
			    cout<<"Knotoid type: "<<name<<"\t";
			}
		      cout<<"Polynomial: "<<jones_polynomial<<endl;
		    }
		  else
		    {
		      cerr<<"saving polynomial to file "<<output_filename_jones<<endl;
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    file<<"Knot type: "<<name<<"\t";
			  else
			    file<<"Knotoid type: "<<name<<"\t";
			}
		      file<<"Polynomial: "<<jones_polynomial<<endl;	
		    }
		}
	      catch (exception& e)//timeout  
		{
		  if(flag_debug)cerr<<"polynomial: time "<<time(NULL)-t0<<" s"<<endl;
		  if(flag_debug)cerr<<"polynomial: time_clock "<<((float)(clock() - clock_t0))/CLOCKS_PER_SEC<<" s "<<endl;
		  cerr<< e.what()<<endl;

		  string name="TIMEOUT";	      
		  if(output_filename_jones=="stdout"||output_filename_jones=="-")
		    {
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    cout<<"Knot type: "<<name<<"\t";
			  else
			    cout<<"Knotoid type: "<<name<<"\t";
			}
		      cout<<"Polynomial: "<<"TIMEOUT"<<endl;
		    }
		  else
		    {
		      cerr<<"saving polynomial to file "<<output_filename_jones<<endl;
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    file<<"Knot type: "<<name<<"\t";
			  else
			    file<<"Knotoid type: "<<name<<"\t";
			}
		      file<<"Polynomial: "<<"TIMEOUT"<<endl;	
		    }
		}

	    }
	}//loop on input diagrams
      if(output_filename_jones!="stdout"&&output_filename_jones!="-")	
	file_tmp.close();
    }
  else//xyz format
    {
      /////polynomial invariants
      if((closure_method=="direct"||closure_method=="straight"))
	{
	  cerr<<"polynomial invariant: classical Jones polynomial."<<endl;
	}
      if((closure_method=="open"&&!flag_planar))
	{
	  cerr<<"polynomial invariant: Jones polynomial for knotoids."<<endl;
	}
      if((closure_method=="open"&&flag_planar))
	{
	  cerr<<"polynomial invariant: Turaev loop bracket."<<endl;
	}

      cerr<<"Loading input curve"<<endl;
      Polygon polygon(flag_debug);
      polygon.load_from_file_xyz(input_filename,flag_cyclic);
      cerr<<"3D curve has "<<polygon.get_nb_points()<<" vertices"<<endl;  
      if(polygon.get_nb_points()<2)
	{
	  cerr<<"********************************************************"<<endl;	  
	  cerr<<"ERROR: input file "<<input_filename<<" is empty. Aborting..."<<endl;
	  cerr<<"********************************************************"<<endl;	  
	  exit(1);
	}
      if(nb_projections>0)
	{
	  ////////////////////////////////////////////
	  //Multiple projections
	  ////////////////////////////////////////////
	  ostream file(cout.rdbuf());
	  ofstream file_tmp;
	  if(output_filename_diagram!="")
	    {
	      if(output_filename_diagram!="stdout"&&output_filename_diagram!="-")
		{
		  file_tmp.open(output_filename_diagram.c_str());
		  file.rdbuf(file_tmp.rdbuf());
		}
	      file<<"#projection.x\tprojection.y\tprojection.z";
	      if(names_db_filename!="")
		{
		  if(flag_cyclic)
		    file<<"\tknot_type";
		  else
		    file<<"\tknotoid_type";
		}
	      file<<"\tpolynomial";
	      if(output_diagram_format=="pd")
		file<<"\tPD_code"<<endl;
	      else if(output_diagram_format=="gauss")
		file<<"\tgauss_code"<<endl;
	      else
		{
		  cerr<<"********************************************************"<<endl;
		  cerr<<"ERROR: --output-diagram-format="<<output_diagram_format<<" not implemented"<<endl;
		  cerr<<"with multiple projections. Aborting..."<<endl;
		  cerr<<"********************************************************"<<endl;
		  exit(1);
		}
	      file<<setprecision(9);

	    }
      
	  map<string,double> histogram_jones;
	  time_t t0=time(NULL);
	  time_t t1=t0;
	  for(unsigned long p=0;p<nb_projections;p++)
	    {
	      Polygon polygontmp=polygon;
	      if(!flag_debug&&time(NULL)-t1>=2)
		{
		  time_t t_tot=time(NULL)-t0;
		  cerr<<"\r"<<"Projection "<<p<<"/"<<nb_projections<<" ("<<(p*100.0/nb_projections)<<" %)"<<" elapsed time: "<<t_tot<<" s, remaining time: "<<lround(t_tot*(nb_projections/(double)(p+1)-1))<<" s                 ";

		  t1=time(NULL);
		}
	  
	      if(flag_dont_use_fixed_projection)
		{
		  if(projectionlist_projections.size()>0)
		    {
		      dx=projectionlist_projections[p][0];
		      dy=projectionlist_projections[p][1];
		      dz=projectionlist_projections[p][2];
		      weight=projectionlist_weights[p];
		    }
		  else
		    {
		      double u=random01();
		      double v=random01();
		      double theta=2*3.14159265358979*u;
		      double phi=acos(2*v-1);
		      dx=cos(theta)*sin(phi);
		      dy=sin(theta)*sin(phi);
		      dz=cos(phi);
		      weight=1/(double)nb_projections;
		    }
		}
	      //close polygon
	      polygontmp.set_closure(dx,dy,dz,closure_method);
	      if(flag_3d_reduction)
		{
		  if(flag_debug)cerr<<"Simplifying 3D curve"<<endl;
		  polygontmp.simplify_polygon(dx,dy,dz);
		  if(flag_debug)cerr<<"3D curve has "<<polygontmp.get_nb_points()<<" vertices"<<endl;
		}

	      PlanarDiagram diagram(flag_planar);
	      bool flag_valid_projection=true;
	      try
		{
		  diagram=polygontmp.get_planar_diagram(dx,dy,dz,flag_planar);
		}
	      catch (exception& e)//projection failed  
		{
		  flag_valid_projection=false;
		  cerr<< e.what()<<endl;
		}

	      if(flag_valid_projection)
		{
		  diagram.set_debug(flag_debug);
		  if(flag_simplify_diagram)
		    {
		      diagram.simplify();
		      if(max_nb_random_moves_III>0)
			{
			  diagram.simplify_with_random_reidemeister_moves_III(max_nb_random_moves_III,max_nb_unsuccessfull_random_moves_III);
			  diagram.simplify();
			}
		    }
      
      

		  //////////evaluate jones//////////////
		  if(output_filename_jones!=""||output_filename_diagram!="")
		    {      
		      PolynomialInvariant jones(diagram,flag_planar,flag_debug);
		      jones.set_timeout(timeout);
		      Polynomial jones_polynomial;      
		      /////////////jone bgl
		      try
			{
			  if(jones_method=="simple")
			    jones_polynomial=jones.get_polynomial_simple();
			  else if(jones_method=="recursive")
			    jones_polynomial=jones.get_polynomial_recursive("default",true);
			  else if(jones_method=="recursive-crossing-order")
			    jones_polynomial=jones.get_polynomial_recursive("crossing_order",true);
			  else if(jones_method=="recursive-arc-order")
			    jones_polynomial=jones.get_polynomial_recursive("arc_order",true);
			  else if(jones_method=="recursive-region-order")
			    jones_polynomial=jones.get_polynomial_recursive("region_order",true);
			  else
			    {
			      cerr<<"********************************************************"<<endl;	  
			      cerr<<"ERROR: --polynomial-method="<<jones_method<<" not implemented"<<endl;
			      cerr<<"********************************************************"<<endl;	  
			      exit(1);
			    }
      
			  
			  histogram_jones[jones_polynomial.to_string()]+=weight;
			}
		      catch (exception& e)//timeout  
			{
			  histogram_jones["TIMEOUT"]+=weight;
			}
			  
		      //output list of planar diagrams
		      if(output_filename_diagram!="")
			{
			  string name="UNKNOWN";
			  if(names_db_filename!="")
			    {
			      map<string,string>::iterator it_name=map_jones_to_name.find(jones_polynomial.to_string());
			      if (it_name != map_jones_to_name.end())
				name=it_name->second;
			    }
			  stringstream ss;
			  if(output_diagram_format=="pd")
			    diagram.save_to_file_KnotTheory(ss,true);		  
			  else
			    diagram.save_to_file_extended_gauss_code(ss,true);
			  file<<dx<<"\t"<<dy<<"\t"<<dz;
			  if(names_db_filename!="")file<<"\t"<<name;
			  file<<"\t"<<jones_polynomial<<"\t"<<ss.str()<<endl;
			}
		    }
		}
	      else//flag_valid_projection==false
		{
		  histogram_jones["failed_projection"]+=weight;
		  //output list of planar diagrams
		  if(output_filename_diagram!="")
		    {
		      stringstream ss;
		      if(output_diagram_format=="pd")
			diagram.save_to_file_KnotTheory(ss,true);		  
		      else
			diagram.save_to_file_extended_gauss_code(ss,true);
		      file<<dx<<"\t"<<dy<<"\t"<<dz;
		      if(names_db_filename!="")file<<"\t"<<"failed_projection";
		      file<<"\t"<<"failed_projection"<<"\t"<<"failed_projection"<<endl;
		    }	      
		}

	    }
	  if(t1!=t0)
	    cerr<<endl;

	  if(output_filename_diagram!="stdout"&&output_filename_diagram!="-")
	    file_tmp.close();

	  //reorder histogram
	  vector<pair<double, string> > histogram_jones_ordered;
	  for(map<string,double>::iterator it=histogram_jones.begin();it!=histogram_jones.end();it++)
	    histogram_jones_ordered.push_back(make_pair(it->second,it->first));
	  //copy(histogram_jones.begin(), histogram_jones.end(), back_inserter(histogram_jones_ordered));
	  sort(histogram_jones_ordered.begin(), histogram_jones_ordered.end());

	  //output
	  file.rdbuf(cout.rdbuf());
	  if(output_filename_jones!="stdout"&&output_filename_jones!="-")
	    {
	      file_tmp.open(output_filename_jones.c_str());
	      file.rdbuf(file_tmp.rdbuf());
	    }
	  file<<"#frequency";
	  if(names_db_filename!="")
	    {
	      if(flag_cyclic)
		file<<"\tknot_type";
	      else
		file<<"\tknotoid_type";
	    }
	  file<<"\tpolynomial";
	  file<<endl;
	  for(int i=histogram_jones_ordered.size()-1;i>=0;i--)
	    {
	      file<<histogram_jones_ordered[i].first;
	      if(names_db_filename!="")
		{
		  string name="UNKNOWN";		  
		  map<string,string>::iterator it_name=map_jones_to_name.find(histogram_jones_ordered[i].second);
		  if (it_name != map_jones_to_name.end())
		    name=it_name->second;
		  file<<"\t"<<name;
		}
	      file<<"\t"<<histogram_jones_ordered[i].second<<endl;
	    }
	  if(output_filename_jones!="stdout"&&output_filename_jones!="-")
	    file_tmp.close();
	}
      else//nb_projections==0
	{      
	  ////////////////////////////////////////////
	  //Only 1 projection
	  ////////////////////////////////////////////
	  if(flag_dont_use_fixed_projection)
	    {
	      double u=random01();
	      double v=random01();
	      double theta=2*3.14159265358979*u;
	      double phi=acos(2*v-1);
	      dx=cos(theta)*sin(phi);
	      dy=sin(theta)*sin(phi);
	      dz=cos(phi);
	    }
	  cerr<<"projection: "<<dx<<","<<dy<<","<<dz<<endl;
	  //close polygon
	  polygon.set_closure(dx,dy,dz,closure_method);

	  if(flag_debug)
	    {
	      cerr<<"Creating "<< debug_filename+"_original.xml (hoomd format)"<<endl;
	      polygon.save_to_file_hoomd(debug_filename+"_original.xml",nb_helper_beads_per_segment,dx,dy,dz);
	    }
	  if(flag_debug)
	    {
	      cerr<<"Creating "<< debug_filename+"_original_projected.xml (hoomd format)"<<endl;
	      Polygon polygon_projected=polygon;
	      polygon_projected.project(dx,dy,dz);
	      polygon_projected.save_to_file_hoomd(debug_filename+"_original_projected.xml",nb_helper_beads_per_segment,dx,dy,dz);
	    }

	  if(flag_3d_reduction)
	    {
	      cerr<<"Simplifying 3D curve"<<endl;
	      polygon.simplify_polygon(dx,dy,dz);
	      cerr<<"3D curve has "<<polygon.get_nb_points()<<" vertices"<<endl;
	    }
	  if(flag_debug)
	    {
	      cerr<<"Creating "<< debug_filename+"_simplified.xml (hoomd format)"<<endl;
	      polygon.save_to_file_hoomd(debug_filename+"_simplified.xml",nb_helper_beads_per_segment,dx,dy,dz);
	    }

	  cerr<<"Evaluating diagram"<<endl;
	  if(flag_debug)
	    {
	      cerr<<"Creating "<< debug_filename+"_simplified_projected.xml (hoomd format)"<<endl;
	      Polygon polygon_projected=polygon;
	      polygon_projected.project(dx,dy,dz);
	      polygon_projected.save_to_file_hoomd(debug_filename+"_simplified_projected.xml",nb_helper_beads_per_segment,dx,dy,dz);
	    }
	  PlanarDiagram diagram(flag_planar);
	  try
	    {
	      diagram=polygon.get_planar_diagram(dx,dy,dz,flag_planar);
	    }
	  catch (exception& e)//projection failed  
	    {
	      cerr<<"**********************************************************************************"<<endl;	  
	      cerr<<"ERROR: "<< e.what()<<endl;
	      cerr<<"**********************************************************************************"<<endl;	  
	      exit(1);
	    }
	  cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
	  diagram.set_debug(flag_debug);
	  if(flag_debug)
	    {
	      cerr<<"diagram"<<endl;
	      diagram.save_to_file_list(cerr);
	    }

	  if(flag_simplify_diagram)
	    {
	      cerr<<"Simplifying diagram"<<endl;
	      diagram.simplify();
	      cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
	      if(flag_debug)
		{
		  cerr<<"diagram:"<<endl;
		  diagram.save_to_file_list(cerr);
		}
	      if(max_nb_random_moves_III>0)
		{
		  cerr<<"Simplifying diagram with random Reidemeister moves III (max "<<max_nb_random_moves_III<<" moves)"<<endl;
		  diagram.simplify_with_random_reidemeister_moves_III(max_nb_random_moves_III,max_nb_unsuccessfull_random_moves_III);
		  cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
		  if(flag_debug)
		    {
		      cerr<<"diagram:"<<endl;
		      diagram.save_to_file_list(cerr);
		    }
		  cerr<<"Final simplifying diagram"<<endl;
		  diagram.simplify();
		  cerr<<"diagram has "<<diagram.get_nb_crossings(true)<<" crossings"<<endl;
		  if(flag_debug)
		    {
		      cerr<<"diagram:"<<endl;
		      diagram.save_to_file_list(cerr);
		    }
		}
	    }
		            
	  //output diagram
	  if(output_filename_diagram!="")
	    {
	      if(output_filename_diagram!="stdout"&&output_filename_diagram!="-")
		cerr<<"saving diagram to file "<<output_filename_diagram<<endl; 
	      if(output_diagram_format=="pd")
		diagram.save_to_file_KnotTheory(output_filename_diagram);		  
	      else if(output_diagram_format=="gauss")
		diagram.save_to_file_extended_gauss_code(output_filename_diagram);
	      else
		{
		  cerr<<"********************************************************"<<endl;
		  cerr<<"ERROR: --output-diagram-format="<<output_diagram_format<<" not implemented."<<endl;
		  cerr<<"Aborting"<<endl;
		  cerr<<"********************************************************"<<endl;
		  exit(1);
		}
	    }

	  //////////jones//////////////
	  if(output_filename_jones!="")
	    {      
	      PolynomialInvariant jones(diagram,flag_planar,flag_debug);
	      jones.set_timeout(timeout);
	      Polynomial jones_polynomial;      
	      time_t t0=time(NULL);
	      clock_t clock_t0 = clock();      
	      /////////////jone bgl
	      try
		{
		  if(jones_method=="simple")
		    jones_polynomial=jones.get_polynomial_simple();
		  else if(jones_method=="recursive")
		    jones_polynomial=jones.get_polynomial_recursive();
		  else if(jones_method=="recursive-crossing-order")
		    jones_polynomial=jones.get_polynomial_recursive("crossing_order");
		  else if(jones_method=="recursive-arc-order")
		    jones_polynomial=jones.get_polynomial_recursive("arc_order");
		  else if(jones_method=="recursive-region-order")
		    jones_polynomial=jones.get_polynomial_recursive("region_order");
		  else
		    {
		      cerr<<"********************************************************"<<endl;	  
		      cerr<<"ERROR: --polynomial-method="<<jones_method<<" not implemented"<<endl;
		      cerr<<"********************************************************"<<endl;	  
		      exit(1);
		    }
		  
		  
		  if(flag_debug)cerr<<"polynomial: time "<<time(NULL)-t0<<" s"<<endl;
		  if(flag_debug)cerr<<"polynomial: time_clock "<<((float)(clock() - clock_t0))/CLOCKS_PER_SEC<<" s "<<endl;
		  
		  string name="UNKNOWN";	      
		  map<string,string>::iterator it_name=map_jones_to_name.find(jones_polynomial.to_string());
		  if (it_name != map_jones_to_name.end())
		    name=it_name->second;
		  if(output_filename_jones=="stdout"||output_filename_jones=="-")
		    {
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    cout<<"Knot type: "<<name<<"\t";
			  else
			    cout<<"Knotoid type: "<<name<<"\t";
			}
		      cout<<"Polynomial: "<<jones_polynomial<<endl;
		    }
		  else
		    {
		      cerr<<"saving polynomial to file "<<output_filename_jones<<endl;
		      ofstream file(output_filename_jones.c_str());
		      if(!file.good())
			{
			  cerr<<"*********************************************************"<<endl;
			  cerr<<"ERROR problem with file "<<output_filename_jones <<"."<<endl;
			  cerr<<"*********************************************************"<<endl;
			  exit(1);
			}
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    file<<"Knot type: "<<name<<"\t";
			  else
			    file<<"Knotoid type: "<<name<<"\t";
			}
		      file<<"Polynomial: "<<jones_polynomial<<endl;	
		      file.close();
		      
		    }
		}
	      catch (exception& e)//timeout  
		{
		  if(flag_debug)cerr<<"polynomial: time "<<time(NULL)-t0<<" s"<<endl;
		  if(flag_debug)cerr<<"polynomial: time_clock "<<((float)(clock() - clock_t0))/CLOCKS_PER_SEC<<" s "<<endl;
		  cerr<< e.what()<<endl;
		  
		  string name="TIMEOUT";	      
		  if(output_filename_jones=="stdout"||output_filename_jones=="-")
		    {
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    cout<<"Knot type: "<<name<<"\t";
			  else
			    cout<<"Knotoid type: "<<name<<"\t";
			}
		      cout<<"Polynomial: "<<"TIMEOUT"<<endl;
		    }
		  else
		    {
		      cerr<<"saving polynomial to file "<<output_filename_jones<<endl;
		      ofstream file(output_filename_jones.c_str());
		      if(!file.good())
			{
			  cerr<<"*********************************************************"<<endl;
			  cerr<<"ERROR problem with file "<<output_filename_jones <<"."<<endl;
			  cerr<<"*********************************************************"<<endl;
			  exit(1);
			}
		      if(names_db_filename!="")
			{
			  if(flag_cyclic)
			    file<<"Knot type: "<<name<<"\t";
			  else
			    file<<"Knotoid type: "<<name<<"\t";
			}
		      file<<"Polynomial: "<<"TIMEOUT"<<endl;	
		      file.close();
		      
		    }
		}

	    }
	}
    }//xyz format
};


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

vector<string> split_string(string str,string sep_list)
{
  vector<string> result;
  string strtmp;
  size_t pos1=0,pos2,postmp;
  pos2=string::npos;
  for(int i=0;i<sep_list.length();i++){
    postmp=str.find(sep_list[i]);
    if(postmp<pos2)
      pos2=postmp;
  }
   
  while( pos2!=str.npos){
    strtmp=str.substr (pos1,pos2-pos1);
    if(strtmp.size()>0)
      result.push_back(strtmp);
    pos1=pos2+1;
     
    pos2=string::npos;
    for(int i=0;i<sep_list.length();i++){
      postmp=str.find(sep_list[i],pos1);
      if(postmp<pos2)
 	pos2=postmp;
    }
  }
  strtmp=str.substr (pos1,pos2-pos1);
  if(strtmp.size()>0)
    result.push_back(strtmp);
  return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> split_input(string str)
{
  vector<string> result;
  string strtmp;
  string sep="PD[";
  int pos1=0,pos2=str.find(sep);
  
  while( pos2!=str.npos){
    if(pos1>0)//first substring is ignored
      {
	strtmp=str.substr (pos1-1,pos2-pos1+1);
	if(strtmp.size()>0)
	  result.push_back(strtmp);
      }
    pos1=pos2+1;
    pos2=str.find(sep,pos1);    
  }
  if(pos1>0)
    {
      strtmp=str.substr (pos1-1,pos2-pos1+1);
      if(strtmp.size()>0)
	result.push_back(strtmp);
    }
  return result;
}
