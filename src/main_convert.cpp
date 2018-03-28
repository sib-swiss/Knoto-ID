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
string output_filename="stdout";
double dx,dy,dz;
bool flag_dont_use_fixed_projection=true;
bool flag_debug=false;
bool flag_cyclic=false;
bool flag_3d_reduction=false;
bool flag_simplify_diagram=false;
bool flag_planar=false; //diagram is on the plane (not the sphere) 
long max_nb_random_moves_III=100000;
unsigned long seed=time(NULL);
string input_format="xyz"; //"xyz", "pd" or "gauss"
string output_format="pd"; //"pd" or "gauss" or "xyz"
string closure_method="open";
long nb_iteration_relaxation=0;
int nb_additional_nodes_per_arc_noloop=0;
int nb_additional_nodes_per_arc_empty_loop=1;
int nb_additional_nodes_per_arc_enclosing_loop=3;
bool flag_force_xyzoutput=false;
double temperature_start=0.1;
double temperature_end=1e-4;
double distance_ratio_threshold=0.3;
double output_xyz_nb_points_per_spline=15;
bool flag_close_diagram=false;

void display_usage(char **argv,bool flag_help_all=false){
  cerr<<endl;
  cerr<<"Usage: "<<argv[0]<<" [OPTIONS] FILENAME"<<endl;
  cerr<<endl;
  cerr<<"  Load a knot(oid) diagram file FILENAME. To read from standard input,"<<endl;
  cerr<<"  use FILENAME=\"stdin\" or \"-\"."<<endl;
  cerr<<endl;
  cerr<<"  Depending on --input-format, different types of files can be opened:"<<endl;
  cerr<<"   --input-format=pd     load the diagram in PD code format."<<endl;
  cerr<<"   --input-format=gauss  load the diagram in extended Gauss code format."<<endl;
  cerr<<"                         Option --closure-method is used to specify whether"<<endl;
  cerr<<"                         the diagram is open or closed."<<endl;
  cerr<<"   --input-format=xyz    load a a piecewise linear curve."<<endl;
  cerr<<"                         By default, the curve is open (knotoid), but it can"<<endl;
  cerr<<"                         be closed (knot) by specifying a closure method "<<endl;
  cerr<<"                         with option --closure-method. If option --3D-reduction "<<endl;
  cerr<<"                         is given, he input curve is simplified using a"<<endl;
  cerr<<"                         3D triangle elimination method."<<endl;
  cerr<<"                         A knot(oid) diagram is then obtained by projecting the"<<endl;
  cerr<<"                         curve along a randomly chosen projection direction (with"<<endl;
  cerr<<"                         uniform distribution on the surface of the sphere) or along"<<endl;
  cerr<<"                         the direction specified with option --projection."<<endl;
  cerr<<endl;
  cerr<<"  If option --simplify-diagram is given, the knot(oid) diagram is simplified using"<<endl;
  cerr<<"  Reidemeister moves."<<endl;
  cerr<<endl;
  cerr<<"  Finally, the resulting diagram is saved to file specified by option --output with"<<endl;
  cerr<<"  format specified by --output-format."<<endl;
  cerr<<endl;
  cerr<<"  Notes:"<<endl;
  cerr<<"    - If the input is in xyz format or contains information on which arcs"<<endl;
  cerr<<"      touch the outside region, option --planar can be used to keep this information "<<endl;;
  cerr<<"      in the output."<<endl;;
  cerr<<"    - See the user manual for more information."<<endl;
  cerr<<endl;
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
  cerr<<"  -p, --planar"<<endl;
  cerr<<"           use information on what arcs are touching the outside region."<<endl;
  cerr<<"           WARNING: exit with error if the input file does not contain"<<endl;
  cerr<<"           this information."<<endl;
  cerr<<endl;
  cerr<<"  -F, --input-format=F"<<endl;  
  cerr<<"           input format (default "<<input_format<<")."<<endl;
  cerr<<"           Possible values for F:"<<endl;
  cerr<<"            F=xyz   piecewise linear curve in xyz file format"<<endl;
  cerr<<"            F=pd    PD code for knot(oid) diagram in KnotTheory format"<<endl;
  cerr<<"                    (http://katlas.org/wiki/Planar_Diagrams)."<<endl;
  cerr<<"                    With input format \"pd\", options --projection,"<<endl;
  cerr<<"                    and --closure-method are ignored."<<endl;
  cerr<<"                    If the input file contains multiple PD codes,"<<endl;
  cerr<<"                    only the first one will be used."<<endl;
  cerr<<"            F=gauss extended Gauss code for knot(oid) diagram."<<endl;
  cerr<<"                    If --closure-method=open the diagram is open (knotoid)"<<endl;
  cerr<<"                    If --closure-method=direct or rays the diagram is closed (knot)"<<endl;
  cerr<<"                    With input format \"gauss\", option --projection"<<endl;
  cerr<<"                    is ignored. If the input file contains multiple Gauss"<<endl;
  cerr<<"                    codes, only the first one will be used."<<endl;
  cerr<<endl;
  cerr<<"      --output-format=F"<<endl;  
  cerr<<"           output format for the knot(oid) diagrams (default "<<output_format<<")."<<endl;
  cerr<<"           Possible values for F:"<<endl;
  cerr<<"            F=pd     PD code for knot(oid) diagram in KnotTheory format"<<endl;
  cerr<<"            F=gauss  extended Gauss code for knot(oid) diagram."<<endl;
  cerr<<"            F=xyz    xyz format for knot(oid) diagrams (columns x, y, z and label)"<<endl;
  cerr<<"                     with spline interpolation."<<endl;
  if(flag_help_all)
    {
      cerr<<"            F=xyz+   xyz format for knot(oid) diagrams with additional information"<<endl;
      cerr<<"                     on the underlying circle packing."<<endl;
    }
  cerr<<endl;
  cerr<<"  -o, --output=FILENAME"<<endl;  
  cerr<<"           output the knot(oid) diagram to file FILENAME (default "<<output_filename<<")"<<endl;
  cerr<<"           using format specified with --output-format."<<endl;
  cerr<<"           To write to standard output, use FILENAME=stdout or FILENAME=-"<<endl;
  cerr<<endl;
  cerr<<"OPTIONS for diagram simplification:"<<endl;
  cerr<<endl;
  cerr<<"      --simplify-diagram"<<endl;
  cerr<<"           simplify the knot(oid) diagram with Reidemeister moves."<<endl;
  cerr<<endl;
  cerr<<"      --nb-moves-III=N"<<endl;
  cerr<<"           max number of iterations for simplification with random Reidemeister"<<endl;
  cerr<<"           moves III (default="<<max_nb_random_moves_III<<")."<<endl;
  cerr<<endl;
  cerr<<"OPTIONS for xyz input format:"<<endl;
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
  cerr<<"      --projection=X,Y,Z"<<endl;
  cerr<<"           project the curve along projection direction (X,Y,Z)."<<endl;
  cerr<<"           If not specified, use randomly chosen projection direction"<<endl;
  cerr<<"           (with uniform distribution on the surface of the sphere)."<<endl;
  cerr<<endl;
  cerr<<"      --3D-reduction"<<endl;
  cerr<<"           simplify 3D input curve."<<endl;
  cerr<<endl;
  cerr<<endl;
  cerr<<"OPTIONS for xyz output format for knot(oid) diagrams:"<<endl;
  cerr<<endl;
  cerr<<"      --force"<<endl;
  cerr<<"           output knot(oid) diagram without checking if it is balanced (i.e. same order of magnitude for all arc lengths)."<<endl;
  cerr<<"           Without --force, convert_diagram will quit without saving the diagram if it is not balanced."<<endl;
  cerr<<endl;
  cerr<<"      --nb-iterations-relaxation=N"<<endl;
  cerr<<"           after circle packing, relax knot(oid) diagram with N iterations"<<endl;
  cerr<<"           of simulated annealing (default="<<nb_iteration_relaxation<<")."<<endl;
  cerr<<endl;
  if(flag_help_all)
    {
      cerr<<"      --temperature-start=T"<<endl;
      cerr<<"           initial temperature for simulated annealing (default="<<temperature_start<<")."<<endl;
      cerr<<endl;
      cerr<<"      --temperature-end=T"<<endl;
      cerr<<"           final temperature for simulated annealing (default="<<temperature_end<<")."<<endl;
      cerr<<endl;
      cerr<<"      --distance-ratio-threshold=R"<<endl;
      cerr<<"           stops simulated annealing if the ratio of largest to shortest edge length"<<endl;
      cerr<<"           is above R (default="<<distance_ratio_threshold<<").  "<<endl;
      cerr<<endl;
      cerr<<"      --nodes-per-arc=N"<<endl;  
      cerr<<"           add min(1,2*N) nodes per (open) arc (default "<<nb_additional_nodes_per_arc_noloop<<"):"<<endl;
      cerr<<endl;
      cerr<<"      --nodes-per-empty-loop=N"<<endl;  
      cerr<<"           add min(2,2*N) nodes per arc forming a loop inside the diagram (default "<<nb_additional_nodes_per_arc_empty_loop<<"):"<<endl;
      cerr<<endl;
      cerr<<"      --nodes-per-enclosing-loop=N"<<endl;  
      cerr<<"           add min(2,2*N) nodes per arc forming a loop enclosing the diagram (default "<<nb_additional_nodes_per_arc_enclosing_loop<<"):"<<endl;
      cerr<<endl;
      cerr<<"      --nb-points-spline=N"<<endl;
      cerr<<"           use N points per spline. If N<0: output only control points (default="<<output_xyz_nb_points_per_spline<<")."<<endl;
      cerr<<endl;
      cerr<<"DEBUGGING OPTIONS:"<<endl;
      cerr<<endl;
      cerr<<"  -d, --debug"<<endl;
      cerr<<"           save intermediate files (debug*) and increase verbosity."<<endl;
      cerr<<""<<endl;
      cerr<<"      --close-diagram"<<endl;
      cerr<<"           close the diagram (before simplification with Reidemeister moves) by adding crossings if necessary."<<endl;
      cerr<<""<<endl;
  }    

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
    { "projection", required_argument, NULL, 1 },
    { "3D-reduction", no_argument, NULL, 2 },
    { "nb-moves-III", required_argument, NULL, 3 },
    { "simplify-diagram", no_argument, NULL, 4 },
    { "planar", no_argument, NULL, 'p' },
    { "help-debug", no_argument, NULL, 9 },
    { "closure-method", required_argument, NULL, 'm' },
    { "output-format", required_argument, NULL, 10 },
    { "nb-iterations-relaxation", required_argument, NULL, 5 },
    { "nodes-per-arc", required_argument, NULL, 7 },
    { "nodes-per-empty-loop", required_argument, NULL, 8 },
    { "nodes-per-enclosing-loop", required_argument, NULL, 11 },
    { "force", no_argument, NULL, 12 },
    { "temperature-start", required_argument, NULL, 13 },
    { "temperature-end", required_argument, NULL, 14 },
    { "distance-ratio-threshold", required_argument, NULL, 15 },
    { "nb-points-spline", required_argument, NULL, 16 },
    { "close-diagram", no_argument, NULL, 17 },
    { NULL, no_argument, NULL, 0 }
  };
  opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  while( opt != -1 ) {
    switch( opt ) {
    case 'o':
      output_filename = optarg;
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
    case 'p':
      flag_planar=true;
      break;
    case 'm':
      closure_method = optarg;
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
      cout<<"J. Dorier, D. Goundaroulis, F. Benedetti and A. Stasiak, \"Knoto-ID: a tool to study the entanglement of open protein chains using the concept of knotoids\", submitted (2018)."<<endl;
      cout<<endl;
      exit(0);
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
      flag_3d_reduction = true;
      break;
    case 3:
      max_nb_random_moves_III=atol(optarg);
      break;
    case 4:
      flag_simplify_diagram=true;
      break;
    case 5:
      nb_iteration_relaxation=atol(optarg);
      break;      
    case 7:
      nb_additional_nodes_per_arc_noloop=atoi(optarg);
      break;
    case 8:
      nb_additional_nodes_per_arc_empty_loop=atoi(optarg);
      break;
    case 9:
      display_usage(argv,true);
      exit(0);
    case 10:
      output_format = optarg;
      break;
    case 11:
      nb_additional_nodes_per_arc_enclosing_loop=atoi(optarg);
      break;
    case 12:
      flag_force_xyzoutput=true;
      break;
    case 13:
      temperature_start=atof(optarg);
      break;
    case 14:
      temperature_end=atof(optarg);
      break;
    case 15:
      distance_ratio_threshold=atof(optarg);
      break;
    case 16:
      output_xyz_nb_points_per_spline=atoi(optarg);
      break;      
    case 17:
      flag_close_diagram=true;
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
  //loading
  ///////////////////////////
  PlanarDiagram diagram(flag_planar,flag_debug);

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

      if(input_diagrams.size()>1)
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"WANRING: input contains multiple diagrams."<<endl;
	  cerr<<"Using only the first diagram."<<endl;
	  cerr<<"*********************************************************"<<endl;
	}

      if(input_filename!="stdin"&&input_filename!="-")
	filein_tmp.close();

      
      cerr<<"Loading input diagram"<<endl;
      try
	{
	  if(input_format=="pd")
	    diagram.load_from_string_KnotTheory(input_diagrams[0]);
	  else if(input_format=="gauss")
	    diagram.load_from_string_extended_gauss_code(input_diagrams[0],flag_cyclic);
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
    }
  else if(input_format=="xyz")
    {
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
            
      if(flag_3d_reduction)
	{
	  cerr<<"Simplifying 3D curve"<<endl;
	  polygon.simplify_polygon(dx,dy,dz);
	  cerr<<"3D curve has "<<polygon.get_nb_points()<<" vertices"<<endl;
	}
      
      cerr<<"Evaluating diagram"<<endl;
      diagram.set_debug(false);
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
      
      
    }
  else
    {
      cerr<<"********************************************************"<<endl;
      cerr<<"ERROR: --input-format="<<input_format<<" not implemented."<<endl;
      cerr<<"Aborting"<<endl;
      cerr<<"********************************************************"<<endl;
      exit(1);      
    }

  if(flag_close_diagram)
    {
      cerr<<"Closing diagram"<<endl;
      diagram.close();
      flag_cyclic=true;
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
	  diagram.simplify_with_random_reidemeister_moves_III(max_nb_random_moves_III);
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

  //open output file
  ostream file(cout.rdbuf());
  ofstream file_tmp;
  if(output_filename!="stdout"&&output_filename!="-"&&output_filename!="")
    {
      cerr<<"saving diagram to file "<<output_filename<<endl;
      file_tmp.open(output_filename.c_str());
      file.rdbuf(file_tmp.rdbuf());
      if(!file_tmp.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR problem with file "<<output_filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
    }

  if(output_format=="pd")
    diagram.save_to_file_KnotTheory(file);
  else if(output_format=="gauss")
    diagram.save_to_file_extended_gauss_code(file);
  else if(output_format=="xyz")
    diagram.save_to_file_xyz(file,output_xyz_nb_points_per_spline,nb_iteration_relaxation,distance_ratio_threshold,temperature_start,temperature_end,!flag_force_xyzoutput,false,nb_additional_nodes_per_arc_noloop,nb_additional_nodes_per_arc_empty_loop,nb_additional_nodes_per_arc_enclosing_loop);
  else if(output_format=="xyz+")
    diagram.save_to_file_xyz(file,output_xyz_nb_points_per_spline,nb_iteration_relaxation,distance_ratio_threshold,temperature_start,temperature_end,!flag_force_xyzoutput,true,nb_additional_nodes_per_arc_noloop,nb_additional_nodes_per_arc_empty_loop,nb_additional_nodes_per_arc_enclosing_loop);
  else
    {
      cerr<<"********************************************************"<<endl;
      cerr<<"ERROR: --output-format="<<output_format<<" not implemented."<<endl;
      cerr<<"Aborting"<<endl;
      cerr<<"********************************************************"<<endl;
      exit(1);
    }
  if(output_filename!="stdout"&&output_filename!="-"&&output_filename!="")	
    file_tmp.close();

	
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
