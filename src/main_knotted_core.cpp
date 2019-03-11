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
string output_filename_all="";
string output_filename_search="";
string output_filename="stdout";
string projectionlist_filename="";
double dx,dy,dz,weight;//projection axis
bool flag_debug=false;
string debug_filename="debug";
bool flag_cyclic_input=false;//input curve is cyclic
bool flag_cyclic=false;//jones polynomial evaluation: flag_cyclic=true: knot, flag_cyclic=false: knotoid
bool flag_3d_reduction=true;
bool flag_simplify_diagram=true;
bool flag_planar=false; //diagram is on the plane (not the sphere) 
bool flag_arrow_polynomial=false; 
long max_nb_random_moves_III=1000;
long max_nb_unsuccessfull_random_moves_III=10;//max number of move without improvement.
int nb_helper_beads_per_segment=0;// for hoomd debug output, add nb_helper_beads_per_segment per segments 
unsigned long seed=time(NULL);
string jones_method="recursive";
unsigned long nb_projections=20;
string closure_method="open";
string names_db_filename="";
time_t timeout=0;
vector<vector<double> > projectionlist_projections;
vector<double> projectionlist_weights;

void display_usage(char **argv,bool flag_help_all=false){
  cerr<<endl;
  cerr<<"Usage: "<<argv[0]<<" [OPTIONS] FILENAME"<<endl;
  cerr<<endl;
  cerr<<"  Load a piecewise linear curve from file FILENAME and"<<endl;
  cerr<<"  evaluate the knotted core. By default, the curve is considered open"<<endl;
  cerr<<"  but it can be closed with option --cyclic-input."<<endl;
  cerr<<"  Output the knotted core, and optionally all subchains tested when"<<endl;
  cerr<<"  searching the knotted core (with option --output-search) as well as"<<endl;
  cerr<<"  all possible subchains of the of the input curve (with option"<<endl;
  cerr<<"  --output-all)."<<endl;
  cerr<<endl;
  cerr<<"  Notes:"<<endl;
  cerr<<"  - The knotted core corresponds to the \"top-down\" knotted core discussed "<<endl;
  cerr<<"    by Tubiana et al. Prog. Theor. Phys. Suppl. 191, 192–204  (2011)."<<endl;
  cerr<<"  - To read from standard input, use FILENAME=\"stdin\" or \"-\"."<<endl;    
  cerr<<"  - Polynomial invariants:"<<endl;  
  cerr<<"     - with: --closure-method=direct or rays:"<<endl;  
  cerr<<"       Classical Jones polynomial for knots."<<endl;  
  cerr<<"     - with: --closure-method=open (without --planar nor --arrow-polynomial):"<<endl;  
  cerr<<"       Jones polynomial for knotoids (on the sphere)."<<endl;  
  cerr<<"     - with: --closure-method=open --planar (without --arrow-polynomial):"<<endl;  
  cerr<<"       Turaev loop bracket for knotoids (on the plane)."<<endl;  
  cerr<<"     - when --closure-method=open --arrow-polynomial (without --planar):"<<endl;  
  cerr<<"       Arrow polynomial for knotoids (on the sphere)."<<endl;  
  cerr<<"     - when --closure-method=open --arrow-polynomial --planar:"<<endl;  
  cerr<<"       Loop arrow polynomial for knotoids (on the plane)."<<endl;  
  cerr<<"  - By default, the polynomial invariant is the Jones polynomial"<<endl;
  cerr<<"    for knotoids (open curve, knotoid diagram on a sphere)."<<endl;  
  cerr<<"  - By default, the number of iterations for simplification with"<<endl;
  cerr<<"    random Reidemeister moves III is "<<max_nb_random_moves_III<<". "<<endl;
  cerr<<"    Consider increasing it with --nb-moves-III for complex knot(oid) diagrams."<<endl;

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
  cerr<<"  -o, --output=FILENAME"<<endl;  
  cerr<<"           output the knotted core to FILENAME (default "<<output_filename<<")."<<endl;
  cerr<<"           To write to standard output, use FILENAME=\"stdout\" or FILENAME=\"-\""<<endl;
  cerr<<endl;
  cerr<<"      --output-search=FILENAME"<<endl;  
  cerr<<"           output all subchains tested when searching for the knotted"<<endl;
  cerr<<"           core to file FILENAME. To write to standard output,"<<endl;
  cerr<<"           use FILENAME=\"stdout\" or FILENAME=\"-\""<<endl;
  cerr<<endl;
  cerr<<"      --output-all=FILENAME"<<endl;  
  cerr<<"           output all possible subchains of the input curve to file FILENAME."<<endl;
  cerr<<"           To write to standard output, use FILENAME=\"stdout\" or FILENAME=\"-\""<<endl;
  cerr<<"           Warning: evaluating the polynomial invariants for all subchains of the"<<endl;
  cerr<<"           input curve may be slow."<<endl;
  cerr<<endl;
  cerr<<"  -C, --cyclic-input"<<endl;  
  cerr<<"           Close the input curve by connecting its last point to its first"<<endl;
  cerr<<"           point by a straight line."<<endl;
  cerr<<endl;
  cerr<<"SUBCHAIN OPTIONS:"<<endl;
  cerr<<"  -m, --closure-method=M"<<endl;  
  cerr<<"           specify how to close each subchain (default "<<closure_method<<")."<<endl;
  cerr<<"           Possible values for M:"<<endl;
  cerr<<"            M=open   =>  the curve is open (knotoid)."<<endl;
  cerr<<"            M=direct =>  connect last and first point by a straight line."<<endl;
  cerr<<"            M=rays   =>  close by extending two parallel rays along the projection"<<endl;
  cerr<<"                         direction, each originating from one of the endpoints"<<endl;
  cerr<<"                         of the curve, and connecting them  outside of the sphere"<<endl;
  cerr<<"                         enclosing the curve."<<endl;
  cerr<<endl;
  cerr<<"  -p, --planar"<<endl;  
  cerr<<"           for each subchain, evaluate its polynomial invariant for the knot(oid) diagram on"<<endl;
  cerr<<"           a plane. If not given, evaluate the polynomial invariant for"<<endl;
  cerr<<"           the knot(oid) diagram on the surface of a sphere."<<endl;
  cerr<<"           This option is only relevant for open curves (--closure-method=open)."<<endl;
  cerr<<endl;
  cerr<<"      --arrow-polynomial"<<endl;
  cerr<<"           evaluate the arrow polynomial instead of Jones polynomial"<<endl;
  cerr<<"           for knotoids and loop arrow polynomial instead of Turaev loop bracket."<<endl;
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
  cerr<<"  -N, --nb-projections=N"<<endl;  
  cerr<<"           use N random projection directions (with uniform distribution on"<<endl;
  cerr<<"           the surface of the sphere) to evaluate the dominant polynomial"<<endl;
  cerr<<"           invariant of each subchain (default "<<nb_projections<<")"<<endl;
  cerr<<"           Notes:"<<endl;
  cerr<<"            - For cyclic curves with closure method \"direct\" the polynomial"<<endl;
  cerr<<"              invariant (classical Jones polynomial) does not depend on the"<<endl;
  cerr<<"              projection. To avoid useless computations, N will be set to 1."<<endl;
  cerr<<endl;
  cerr<<"      --projections-list=FILENAME"<<endl;  
  cerr<<"           Evaluate the dominant polynomial using the list of projection"<<endl;
  cerr<<"           directions in file FILENAME."<<endl;
  cerr<<"           File FILENAME should have the following format:"<<endl;
  cerr<<"            - space or tab separated with three or four columns."<<endl;
  cerr<<"            - columns 1,2,3: x,y,z coordinates for the projection direction."<<endl;
  cerr<<"            - column 4: weight (optional)."<<endl;
  cerr<<"            - Each row corresponds to one projection direction."<<endl;  
  cerr<<"           Notes:"<<endl;
  cerr<<"            - For cyclic curves with closure method \"direct\" the polynomial"<<endl;
  cerr<<"              invariant (classical Jones polynomial) does not depend on the"<<endl;
  cerr<<"              projection. To avoid useless computations, N will be set to 1."<<endl;
  cerr<<"            - Option --nb-projections will be ignored."<<endl;
  cerr<<endl;
  cerr<<"  -n, --names-db=FILENAME"<<endl;  
  cerr<<"           load a list of polynomials with corresponding knot(oid) name from"<<endl;
  cerr<<"           file FILENAME. File FILENAME should have the following format."<<endl;
  cerr<<"            - tab separated with two columns."<<endl;
  cerr<<"            - column 1: knot/knotoid name."<<endl;
  cerr<<"            - column 2: polynomial."<<endl;
  cerr<<"           If FILENAME=\"internal\": use internal database."<<endl;
  cerr<<"           For knots, this database is based on the Rolfsen and Hoste-Thistlethwaite Knot"<<endl;
  cerr<<"           tables, while for knotoids it is based on the classification proposed in:"<<endl;
  cout<<"           D. Goundaroulis, J. Dorier and A. Stasiak, \"A systematic classification"<<endl;
  cerr<<"           of knotoids on the plane and on the sphere\", arXiv:1902.07277 [math.GT]"<<endl;
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
      cerr<<"           do not simplify 3D subchains."<<endl;
      cerr<<endl;
      cerr<<"      --no-diagram-simplification"<<endl;  
      cerr<<"           do not simplify the knot(oid) diagram."<<endl;
      cerr<<endl;
      cerr<<"      --polynomial-method=M"<<endl;  
      cerr<<"           use method M to evaluate the polynomial (default "<<jones_method<<")"<<endl;
      cerr<<"           Possible values are:"<<endl;
      cerr<<"               M=simple   (i.e. brute force)"<<endl;
      cerr<<"               M=recursive"<<endl;
      cerr<<"               M=recursive-crossing-order"<<endl;
      cerr<<"               M=recursive-arc-order"<<endl;
      cerr<<"               M=recursive-region-order"<<endl;
      cerr<<endl;
      cerr<<"  -d, --debug"<<endl;  
      cerr<<"           save intermediate files (debug*) and increase verbosity."<<endl;
      cerr<<endl;
      cerr<<"      --nb-helper-beads=N"<<endl;  
      cerr<<"           add N helper beads for each segment in hoomd output files debug* (only used with --debug)."<<endl;
    }    
  cerr<<""<<endl;

}

//return jones_polynomial (and frequency) 
string get_jones(Polygon & polygon,double & frequency,unsigned long nb_projections,string closure_method);

int mod(int x,int N){if(x>=0)return x%N;else return (x+N*(2-x/N))%N;}

vector<string> split_string(string str,string sep_list);
//split input with multiple PDs into strings, one per PD
vector<string> split_input(string str);

int main(int argc, char **argv)
{
  int opt = 0;
  int longIndex = 0;
  static const char *optString = "ho:ds:VN:n:m:Cp";
  static const struct option longOpts[] = {
    { "help", no_argument, NULL, 'h' },
    { "output", required_argument, NULL, 'o' },
    { "debug", no_argument, NULL, 'd' },
    { "seed", required_argument, NULL, 's' },
    { "version", no_argument, NULL, 'V' },
    { "names-db", required_argument, NULL, 'n' },
    { "nb-projections", required_argument, NULL, 'N' },
    { "cyclic-input", no_argument, NULL, 'C' },
    { "no-3D-reduction", no_argument, NULL, 2 },
    { "nb-moves-III", required_argument, NULL, 3 },
    { "nb-unsuccessfull-moves-III", required_argument, NULL, 12 },
    { "no-diagram-simplification", no_argument, NULL, 4 },
    { "planar", no_argument, NULL, 'p' },
    { "nb-helper-beads", required_argument, NULL, 6 },
    { "output-all", required_argument, NULL, 7 },
    { "polynomial-method", required_argument, NULL, 8 },
    { "help-debug", no_argument, NULL, 9 },
    { "closure-method", required_argument, NULL, 'm' },
    { "output-search", required_argument, NULL, 11 },
    { "timeout", required_argument, NULL, 1 },
    { "projections-list", required_argument, NULL, 5 },
    { "arrow-polynomial", no_argument, NULL, 10 },
    { NULL, no_argument, NULL, 0 }
  };
  opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  while( opt != -1 ) {
    switch( opt ) {
    case 'o':
      output_filename = optarg;
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
      cout<<"If you use the knotoid classification from the internal database (with --names-db=internal)"<<endl;
      cout<<"or from files examples/knotoid_names_sphere.txt, examples/knotoid_names_planar.txt,"<<endl;
      cout<<"examples/knotoid_names_sphere_arrow.txt or examples/knotoid_names_planar_arrow.txt"<<endl;
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
    case 'C':
      flag_cyclic_input = true;
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
      output_filename_all = optarg;
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
    case 10:
      flag_arrow_polynomial=true;
      break;
    case 11:
      output_filename_search = optarg;
      break;
    case 1:
      timeout=ceil(atof(optarg));
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
  if(flag_planar&flag_cyclic)
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
  
  random_generator.seed(static_cast<unsigned int>(seed));
  cerr<<"seed: "<<seed<<endl;
  if(flag_debug)
    {
      if(flag_cyclic)
	cerr<<"flag_cyclic=true"<<endl;
      else
	cerr<<"flag_cyclic=false"<<endl;
    }

  /////polynomial invariants
  if(closure_method!="open")//cyclic
    {
      cerr<<"polynomial invariant: classical Jones polynomial."<<endl;
    }
  else//not cyclic
    {
      if(flag_arrow_polynomial)
	{
	  if(!flag_planar)
	    cerr<<"polynomial invariant: arrow polynomial for knotoids."<<endl;
	  else
	    cerr<<"polynomial invariant: loop arrow polynomial for planar knotoids."<<endl;
	}
      else
	{
	  if(!flag_planar)
	    cerr<<"polynomial invariant: Jones polynomial for knotoids."<<endl;
	  if(flag_planar)
	    cerr<<"polynomial invariant: Turaev loop bracket."<<endl;
	}
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
      if(names_db_filename=="internal")
	{
	  if(flag_cyclic)
	    {
	      filein.rdbuf(knot_names.rdbuf());
	    }
	  else
	    {
	      if(flag_planar==true&&flag_arrow_polynomial==true)
		{
		  filein.rdbuf(knotoid_names_planar_arrow.rdbuf());
		}
	      else if(flag_planar==true&&flag_arrow_polynomial==false)
		{
		  filein.rdbuf(knotoid_names_planar.rdbuf());
		}
	      else if(flag_planar==false&&flag_arrow_polynomial==true)
		{
		  filein.rdbuf(knotoid_names_sphere_arrow.rdbuf());
		}
	      else if(flag_planar==false&&flag_arrow_polynomial==false)
		{
		  filein.rdbuf(knotoid_names_sphere.rdbuf());
		}
	    }
	}
      else if(names_db_filename!="stdin"&&names_db_filename!="-")
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
      const boost::regex validvariable("^(A|v|[Lmwpq][0-9]+)$");
      while (filein.good())
	{
	  line_no++;
	  getline(filein, line);
	  name_jones_tmp=split_string(line,"\t");
	  if(name_jones_tmp.size()==2)
	    {
	      Polynomial p;
	      if(!p.load_from_string(name_jones_tmp[1]))
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR --names-db="<<names_db_filename<<endl;
		  cerr<<"Invalid polynomial (line "<<line_no<<"):"<<endl;
		  cerr<<name_jones_tmp[1]<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);
		}		    
	      //check variable names
	      vector<string> var_names=p.get_variable_names();
	      for(int i=0;i<var_names.size();i++)
		{
		  if(!regex_match(var_names[i],validvariable,boost::match_default))
		    {
		      cerr<<"*********************************************************"<<endl;
		      cerr<<"ERROR --names-db="<<names_db_filename<<" (line "<<line_no<<")"<<endl;
		      cerr<<"Invalid polynomial:"<<endl;
		      cerr<<name_jones_tmp[1]<<endl;
		      cerr<<"Invalid variable name \""<<var_names[i]<<"\". Variable names should be"<<endl;
		      cerr<<" \"A\",\"v\",\"L1\",\"L2\",...,\"m1\",\"m2\",...,\"w1\",\"w2\",..."<<endl;
		      cerr<<" \"p1\",\"p2\",...,\"q1\",\"q2\",..."<<endl;
		      cerr<<"*********************************************************"<<endl;
		      exit(1);		  
		    }
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
      
      if(names_db_filename!="stdin"&&names_db_filename!="-"&&names_db_filename!="internal")
	filein_tmp.close();

      //add special cases
      map_jones_to_name["TIMEOUT"]="TIMEOUT";
      map_jones_to_name["failed_projection"]="failed_projection";
    }

  ///////////////////////////
  //Loading list of projections axis
  ////////////////////////////
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
  cerr<<"Loading input curve"<<endl;
  Polygon polygon(flag_debug);
  polygon.load_from_file_xyz(input_filename,flag_cyclic_input);
  cerr<<"3D curve has "<<polygon.get_nb_points()<<" vertices"<<endl;
  if(polygon.get_nb_points()<2)
    {
      cerr<<"********************************************************"<<endl;	  
      cerr<<"ERROR: input file "<<input_filename<<" is empty. Aborting..."<<endl;
      cerr<<"********************************************************"<<endl;	  
      exit(1);
    }
  
  ///////////////////////////
  //Search knotted core
  ///////////////////////////
  cerr<<"Searching knotted core"<<endl;
  //i1=start index, l=length i2=i1+l=end index
  //moves: i1++,i1--,i2++,i2--
  // <=> i1++,i1--,l++,l--
  //first phase: decrease l until changing type
  //second phase: follow boundary
  int phase=1;
  int N=polygon.get_nb_points();
  int lmax=N-1;
  if(flag_cyclic_input)//for cyclic input curve, l>=N => full CLOSED chain. (trick to avoid being locked)
    lmax=N+1;
  int i1=0;
  int l=lmax;
  int i1_last,l_last;
  int i1_start=i1,l_start=l,direction_start,total_rotation=0;//stop when reaching again this point
  //directions in (i1,l) plane: 0-> (1,-1), 1-> (0,1), 2-> (-1,0), 3-> (0,-1)
  //direction are with respect to (i1_last,l_last)
  int direction=3;//last direction
  int direction_test=3;
  vector<pair<int,int> > directions;
  directions.push_back(make_pair(1,-1));
  directions.push_back(make_pair(0,1));
  directions.push_back(make_pair(-1,1));
  directions.push_back(make_pair(0,-1));
  string jones_initial="";
  map<pair<int,int>,pair<string,double> > pos_to_jones;//pos_to_jones[pair(i1,l)]=pair(jones,frequ);
  //knotted core
  set<pair<int,int> > knotted_cores;
  int best_l=l;
  knotted_cores.insert(make_pair(i1,l));
  int nb_steps=0;
  //output
  ostream file_search(cout.rdbuf());
  ofstream file_search_tmp;
  bool flag_timeout_during_search=false;//set to true if one dominant knot type was TIMEOUT
  if(output_filename_search!="")
    {
      if(output_filename_search!="stdout"&&output_filename_search!="-")
	{
	  file_search_tmp.open(output_filename_search.c_str());
	  file_search.rdbuf(file_search_tmp.rdbuf());
	}
      file_search<<"#index_first\tindex_last\tlength\tfrequency";
      if(names_db_filename!="")
	{
	  if(flag_cyclic)
	    file_search<<"\tknot_type";
	  else
	    file_search<<"\tknotoid_type";
	}
      file_search<<"\tpolynomial"<<endl;

    }

  while(true)
    {
      cerr<<"\r"<<" testing subchain "<<mod(i1,N)<<"-"<<mod(i1+l,N)<<"      ";
      string jones;
      double frequ;
      if((l<=0)||(l>lmax)||(flag_cyclic_input==false&&i1<0)||(flag_cyclic_input==false&&i1+l>=N))//i1>=N
	{
	  jones="OUT_OF_BOUNDS";
	  frequ=1;
	}
      else
	{
	  map<pair<int,int>,pair<string,double> >::iterator it_jones;
	  it_jones = pos_to_jones.find(make_pair(mod(i1,N),l));      
	  if (it_jones!= pos_to_jones.end())//jones already evaluated
	    {
	      jones=it_jones->second.first;
	      frequ=it_jones->second.second;
	    }
	  else//evaluate jones
	    {
	      if(flag_cyclic_input&&l>=N)
		{//evaluate jone for full polygon, cyclic
		  jones=get_jones(polygon,frequ,1,"direct");
		}
	      else
		{
		  Polygon polygontmp=polygon.get_polygon(mod(i1,N),mod(i1+l,N),flag_cyclic);
		  jones=get_jones(polygontmp,frequ,nb_projections,closure_method);
		}
	      if(jones=="TIMEOUT")
		{
		  cerr<<endl;
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"WARNING: timeout during evaluation of the polynomial."<<endl;
		  cerr<<"*********************************************************"<<endl;
		  flag_timeout_during_search=true;
		}
	      if(pos_to_jones.size()==0)
		jones_initial=jones;
	      pos_to_jones[make_pair(mod(i1,N),l)]=make_pair(jones,frequ);		  
	    }
	}
      //output
      if(output_filename_search!="")
	{
	  string knot_type="UNKNOWN";	      
	  if(names_db_filename!="")
	    {      
	      map<string,string>::iterator it=map_jones_to_name.find(jones);
	      if (it != map_jones_to_name.end())
		knot_type=it->second;
	    }
	  if(jones!="OUT_OF_BOUNDS")
	    {
	      file_search<<mod(i1,N)<<"\t"<<mod(i1+l,N)<<"\t"<<l+1<<"\t"<<frequ;
	      if(names_db_filename!="")file_search<<"\t"<<knot_type;
	      file_search<<"\t"<<jones<<endl;
	    }
	}
      //next points
      if(phase==1)
	{
	  if(jones==jones_initial)//valid jones
	    {
	      i1_last=i1;
	      l_last=l;
	      direction=direction_test;
	      direction_test=3;
	    }
	  else//end of phase 1
	    {
	      phase=2;
	      direction=0;
	      direction_test=0;//we already tested direction 3
	      nb_steps=0;
	    }
	}
      else if(phase==2)
	{
	  if(jones==jones_initial)//valid move
	    {
	      nb_steps++;
	      i1_last=i1;
	      l_last=l;
	      if(nb_steps==1)
		{
		  i1_start=i1_last;
		  l_start=l_last;
		  direction_start=direction_test;
		  total_rotation=0;
		}
	      else if(nb_steps>1)
		{
		  if(mod(direction_test-direction,4)==3)//direction_test starts at (direction+3)%4 (i.e. -1) and only increase
		    total_rotation-=1;
		  else
		    total_rotation+=mod(direction_test-direction,4);		    
		}
	      direction=direction_test;
	      direction_test=(direction+3)%4;//last direction
	    }
	  else
	    {
	      direction_test=(direction_test+1)%4;
	    }
	}

      //store best results
      if(jones==jones_initial)
	{
	  if(l<best_l)
	    {
	      best_l=l;
	      knotted_cores.clear();
	      knotted_cores.insert(make_pair(mod(i1,N),l));
	    }
	  else if(l==best_l)
	    {
	      knotted_cores.insert(make_pair(mod(i1,N),l));
	    }
	    
	}
      //move
      i1=i1_last+directions[direction_test].first;
      l=l_last+directions[direction_test].second;
      
      //check for termination
      if(flag_cyclic_input==false&&i1+l>=N)
       	{
       	  break;
      	}

      if(phase==2&&nb_steps>1&&mod(i1_last,N)==i1_start&&l_last==l_start&&direction_start==direction)
       	{
	  if(total_rotation==0)//finished
	    break;
	  else if(total_rotation==-4)//isolated loop.
	    {
	      phase=1;
	      //pick one of the knotted_cores as new starting point
	      i1_last=knotted_cores.begin()->first;
	      l_last=knotted_cores.begin()->second;
	      direction=3;
	      direction_test=3;
	      //move
	      i1=i1_last+directions[direction_test].first;
	      l=l_last+directions[direction_test].second;
	    }
      	}
       if(l>lmax)
       	{
	  cerr<<"OUPS: l>lmax"<<endl;
       	  break;
      	}
    }
  
  if(output_filename_search!="")
    if(output_filename_search!="stdout"&&output_filename_search!="-")
      file_search_tmp.close();

  cerr<<endl;
  cerr<<"done"<<endl;
  ////////////////////////////
  //output knotted core
  ///////////////////////////
  //output
  ostream file(cout.rdbuf());
  ofstream file_tmp;
  if(output_filename!="stdout"&&output_filename!="-")
    {
      file_tmp.open(output_filename.c_str());
      file.rdbuf(file_tmp.rdbuf());
    }
  file<<"#index_first\tindex_last\tlength\tfrequency";
      if(names_db_filename!="")
	{
	  if(flag_cyclic)
	    file<<"\tknot_type";
	  else
	    file<<"\tknotoid_type";
	}
  file<<"\tpolynomial"<<endl;

  for(set<pair<int,int> >::iterator it=knotted_cores.begin();it!=knotted_cores.end();it++)
    {
      int i1=it->first;
      int l=it->second;
      double frequ;
      string jones;
      map<pair<int,int>,pair<string,double> >::iterator it_jones = pos_to_jones.find(make_pair(i1,l));      
      if (it_jones!= pos_to_jones.end())//jones already evaluated
	{
	  jones=it_jones->second.first;
	  frequ=it_jones->second.second;
	}
      else
	{
	  cerr<<"ERROR: should not happen!!!  i1="<<i1<<" l="<<l<<endl;
	  exit(1);
	}
      string knot_type="UNKNOWN";	      
      if(names_db_filename!="")
	{      
	  map<string,string>::iterator it=map_jones_to_name.find(jones);
	  if (it != map_jones_to_name.end())
	    knot_type=it->second;
	}
      if(flag_timeout_during_search)
	{
	  knot_type="TIMEOUT:"+knot_type;
	  jones="TIMEOUT:"+jones;
	}
      
      file<<i1<<"\t"<<mod(i1+l,N)<<"\t"<<l+1<<"\t"<<frequ;
      if(names_db_filename!="")file<<"\t"<<knot_type;
      file<<"\t"<<jones<<endl;
    }
  
  if(output_filename!="stdout"&&output_filename!="-")
    file_tmp.close();
  
  ///////////////////////////
  //Evaluate all subchains
  ///////////////////////////
  if(output_filename_all!="")
    {
      cerr<<"evaluating all subchains"<<endl;     
      ostream file_all(cout.rdbuf());
      ofstream file_all_tmp;
      if(output_filename_all!="")
	{
	  if(output_filename_all!="stdout"&&output_filename_all!="-")
	    {
	      file_all_tmp.open(output_filename_all.c_str());
	      file_all.rdbuf(file_all_tmp.rdbuf());
	    }
	}
      file_all<<"#index_first\tindex_last\tlength\tfrequency";
      if(names_db_filename!="")
	{
	  if(flag_cyclic)
	    file_all<<"\tknot_type";
	  else
	    file_all<<"\tknotoid_type";
	}
      file_all<<"\tpolynomial"<<endl;

      int N=polygon.get_nb_points();
      for(int i1=0;i1<N;i1++)
	{
	  int lmax2;
	  if(flag_cyclic_input)
	    lmax2=N+1;
	  else
	    lmax2=N-i1;
	  for(int l=1;l<lmax2;l++)
	    {
	      cerr<<"\r"<<" subchain "<<i1<<"-"<<mod(i1+l,N)<<"      ";
	      Polygon polygontmp=polygon.get_polygon(i1,mod(i1+l,N),flag_cyclic);

	      //check if jones already evaluated
	      double frequ;
	      string jones;
	      map<pair<int,int>,pair<string,double> >::iterator it_jones;
	      it_jones = pos_to_jones.find(make_pair(i1,l));      
	      if (it_jones!= pos_to_jones.end())//jones already evaluated
		{
		  jones=it_jones->second.first;
		  frequ=it_jones->second.second;
		}
	      else//evaluate jones
		{
		  if(flag_cyclic_input&&l>=N)
		    {//evaluate jone for full polygon, cyclic
		      jones=get_jones(polygon,frequ,1,"direct");
		    }
		  else
		    {
		      jones=get_jones(polygontmp,frequ,nb_projections,closure_method);
		    }		  
		}
	      
	      //double frequ;
	      string knot_type="UNKNOWN";	      
	      if(names_db_filename!="")
		{      
		  map<string,string>::iterator it=map_jones_to_name.find(jones);
		  if (it != map_jones_to_name.end())
		    knot_type=it->second;
		}
	      file_all<<i1<<"\t"<<mod(i1+l,N)<<"\t"<<l+1<<"\t"<<frequ;
	      if(names_db_filename!="")file_all<<"\t"<<knot_type;
	      file_all<<"\t"<<jones<<endl;
	    }
	}

      if(output_filename_all!="stdout"&&output_filename_all!="-")
	file_all_tmp.close();

      cerr<<endl;
      cerr<<"done"<<endl;
    }



  
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

string get_jones(Polygon & polygon,double & frequ,unsigned long nb_projections,string closure_method)
{

  
  map<string,double> histogram_jones;
  for(unsigned long p=0;p<nb_projections;p++)
    {
      Polygon polygontmp=polygon;
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
	  PolynomialInvariant jones(diagram,flag_planar,flag_arrow_polynomial,flag_debug);
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

	}
      else//flag_valid_projection==false
	{
	  histogram_jones["failed_projection"]+=weight;
	}
      
    }

  //find max frequency
  string jones="";
  frequ=0;
  for(map<string,double>::iterator it=histogram_jones.begin();it!=histogram_jones.end();it++)
    {
      if(it->second >= frequ)
	{
	  frequ=it->second;
	  jones=it->first;
	}
    }

  return jones;
}

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
