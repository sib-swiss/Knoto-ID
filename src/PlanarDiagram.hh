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

#ifndef PLANARDIAGRAM_HH
#define PLANARDIAGRAM_HH

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <PlanarGraph.hh>
#include <Random.hh>
#include <stdexcept>

//for boost graphs
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/biconnected_components.hpp>

//************************************************************
//**********************   crossing   ************************
//************************************************************
class Crossing
{
public:
  //create empty (invalid) crossing
  Crossing()
  {
    arc0=-1;arc1=-1;arc2=-1;arc3=-1;
    connected_crossing0=-1;connected_crossing1=-1;connected_crossing2=-1;connected_crossing3=-1;
    region01=-1;region12=-1;region23=-1;region30=-1;
  }
  //create crossing
  Crossing(int a1,int a2,int a3,int a4)
  {
    arc0=a1;arc1=a2;arc2=a3;arc3=a4;
    connected_crossing0=-1;connected_crossing1=-1;connected_crossing2=-1;connected_crossing3=-1;
    region01=-1;region12=-1;region23=-1;region30=-1;
  }
  //create endpoint
  Crossing(int arc)
  {
    arc0=arc;arc1=-1;arc2=-1;arc3=-1;
    connected_crossing0=-1;connected_crossing1=-1;connected_crossing2=-1;connected_crossing3=-1;
    region01=-1;region12=-1;region23=-1;region30=-1;
  }

  inline int get_arc(int n)
  {
    if(n==0)return arc0;
    else if(n==1) return arc1;
    else if(n==2) return arc2;
    else if(n==3) return arc3;
    std::cerr<<"ERROR: get_arc("<<n<<")"<<std::endl;
    exit(1);
  }
  
  inline int get_connected_crossing(int n)
  {
    if(n==0)return connected_crossing0;
    else if(n==1) return connected_crossing1;
    else if(n==2) return connected_crossing2;
    else if(n==3) return connected_crossing3;
    std::cerr<<"ERROR: get_connected_crossing("<<n<<")"<<std::endl;
    exit(1);
  }
  
  inline int get_region(int n,int m)
  {
    if((n==0&&m==1)||(n==1&&m==0))return region01;
    else if((n==1&&m==2)||(n==2&&m==1)) return region12;
    else if((n==2&&m==3)||(n==3&&m==2)) return region23;
    else if((n==3&&m==0)||(n==0&&m==3)) return region30;
    std::cerr<<"ERROR: get_region("<<n<<","<<m<<")"<<std::endl;
    exit(1);
  }
  
  inline void set_arc(int n,int arc)
  {
    if(n==0) arc0=arc;
    else if(n==1) arc1=arc;
    else if(n==2) arc2=arc;
    else if(n==3) arc3=arc;
    else
      {
	std::cerr<<"ERROR: set_arc(n="<<n<<",arc="<<arc<<")"<<std::endl;
	exit(1);
      }
  }
  inline void set_connected_crossing(int n,int connected_crossing)
  {
    if(n==0) connected_crossing0=connected_crossing;
    else if(n==1) connected_crossing1=connected_crossing;
    else if(n==2) connected_crossing2=connected_crossing;
    else if(n==3) connected_crossing3=connected_crossing;
    else
      {
	std::cerr<<"ERROR: set_connected_crossing(n="<<n<<",arc="<<connected_crossing<<")"<<std::endl;
	exit(1);
      }
  }
  
  inline void set_region(int n,int m,int region)
  {
    if((n==0&&m==1)||(n==1&&m==0)) region01=region;
    else if((n==1&&m==2)||(n==2&&m==1)) region12=region;
    else if((n==2&&m==3)||(n==3&&m==2)) region23=region;
    else if((n==3&&m==0)||(n==0&&m==3)) region30=region;
    else
      {
	std::cerr<<"ERROR: set_region(n="<<n<<","<<m<<",arc="<<region<<")"<<std::endl;
	exit(1);
      }
  }
  
  //Convention: (cf http://katlas.org/wiki/Planar_Diagrams)
  //incoming arcs, numbered counterclockwise, with segment a1-a3 below a2-a4 and a1<a3
  //for end points (knotoids) arc1=arc2=arc3=-1; arc1 contains the unique incoming (or outgoing) arc
  //For empty (i.e. removed) crossings arc0=arc1=arc2=arc3=-1
  int arc0; 
  int arc1;
  int arc2;
  int arc3; 
  int connected_crossing0;//arc0 connect to crossings with index connected_crossing0
  int connected_crossing1;//arc1 connect to crossings with index connected_crossing1
  int connected_crossing2;//arc2 connect to crossings with index connected_crossing2
  int connected_crossing3; //arc3 connect to crossings with index connected_crossing3
  int region01; //if =1: region between arc0 and arc1 is exterior of the diagram, otherwise: interior. For endpoint: =1 => endpoint is exterior.
  int region12; //if =1: region between arc1 and arc2 is exterior of the diagram, otherwise: interior. For endpoint: ignored
  int region23; //if =1: region between arc2 and arc3 is exterior of the diagram, otherwise: interior. For endpoint: ignored
  int region30; //if =1: region between arc3 and arc0 is exterior of the diagram, otherwise: interior. For endpoint: ignored  
};



//************************************************************
//**********************   PlanarDiagram   *******************
//************************************************************
class PlanarDiagram
{
public:
  //empty diagram
  PlanarDiagram(bool flag_planar,bool flag_debug=false);
  PlanarDiagram(std::vector<Crossing> & crossings,bool flag_planar,bool flag_debug=false);

  void set_debug(bool flag_debug_t){flag_debug=flag_debug_t;}
  int get_nb_crossings(bool flag_ignore_endpoints=false){
      int nbcrossings=0;
      for(int n=0;n<crossings.size();n++)
	{
	  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//ignore empty crossing
	    continue;
	  if(flag_ignore_endpoints&&(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0))//ignore endpoint
	    continue;
	  nbcrossings++;
	}
      return nbcrossings;
  }
  int get_nb_arcs(){
    std::set<int> arcs;
    for(int n=0;n<crossings.size();n++)
      for(int i=0;i<4;i++)
	if(crossings[n].get_arc(i)>=0)
	  arcs.insert(crossings[n].get_arc(i));
    return arcs.size();
  }
  
  std::vector<Crossing> get_crossings(){return crossings;}
  
  void save_to_file_list(std::string filename);
  void save_to_file_list(std::ostream& file);
  //format for mathematica package KnotTheory (http://katlas.org/wiki/Planar_Diagrams)
  //if flag_one_line=true => do not use "\n"
  void save_to_file_KnotTheory(std::string filename,bool flag_one_line=false);
  void save_to_file_KnotTheory(std::ostream& file,bool flag_one_line=false);

  //if flag_one_line=true => do not use "\n"
  void save_to_file_extended_gauss_code(std::string filename,bool flag_one_line=false);
  void save_to_file_extended_gauss_code(std::ostream& file,bool flag_one_line=false);

  //to draw the diagram.
  //If smooth>=0, interpolates each arc using smooth points on a spline.
  //If flag_debug_info=true: output circles and segments used to build the xyz coordinates.
  //min(1,2*nb_additional_nodes_per_arc_noloop) nodes are placed per arc (not forming loop)
  //min(2,2*nb_additional_nodes_per_arc_empty_loop) nodes are placed per arc (forming a loop inside the diagram).
  //min(2,2*nb_additional_nodes_per_arc_enclosing_loop) nodes are placed per arc (forming a loop enclosing the diagram).
  //Note: nb_additional_nodes_per_arc_loop must be greater or equal to 1.
  //For relaxation:
  //  - decrease temperature from temperature_start to temperature_end
  //  - stops after nb_iterations_relaxation iterations or when distance_min/distance_max>=distance_ratio_threshold (distances=edges lengths, taking into account only edges with edges_draw>=0)
  void save_to_file_xyz(std::string filename,int smooth=10,long nb_iterations_relaxation=0,double distance_ratio_threshold=0.3,double temperature_start=1,double temperature_end=1e-3,bool check_balanced=true,bool flag_debug_info=false,int nb_additional_nodes_per_arc_noloop=0,int nb_additional_nodes_per_arc_empty_loop=1,int nb_additional_nodes_per_arc_enclosing_loop=3);
  void save_to_file_xyz(std::ostream& file,int smooth=10,long nb_iterations_relaxation=0,double distance_ratio_threshold=0.3,double temperature_start=1,double temperature_end=1e-3,bool check_balanced=true,bool flag_debug_info=false,int nb_additional_nodes_per_arc_noloop=0,int nb_additional_nodes_per_arc_empty_loop=1,int nb_additional_nodes_per_arc_enclosing_loop=3);
  
  //format for mathematica package KnotTheory (http://katlas.org/wiki/Planar_Diagrams)
  //e.g. PD[X[0,3,1,4],X[4,1,5,2],X[2,5,3,6]] 
  //e.g. PD[X[0,3,1,4],X[4,1,5,2],X[2,5,3,6]] 
  //ignore everything except [X\[\],0-9] (e.g. "\n", "space" ;)
  //Throw a range_error if the diagram is not valid
  void load_from_file_KnotTheory(std::string filename);
  void load_from_file_KnotTheory(std::istream& file);
  void load_from_string_KnotTheory(std::string str);
  //extended gauss code format:
  //e.g.  1 2 3 -1 -2 -3 -+-
  //e.g.  1 -2 3 -1 2 -3 +++
  //e.g.  1 -2 3 -4 2 -1 4 -3, --++
  //e.g.  {1 -2 3 -4 2 -1 4 -3}, {--++}
  //e.g.  {1,-2,3,-4,2,-1,4,-3}, {-,-,+,+}
  //e.g.  (1 -2 3 -4 2 -1 4 -3),(- - + +)
  // can optionally be followed by and additional list of arcs touching the exterior of the diagram(0 based indexing) 
  //all "( ,{[)}]" are interpreted as separators
  //see B. Gabrovsek, "Tabulation of prime knots in lens spaces", Mediterr. J. Math. 14:88 (2017)
  //if flag_cyclic=true: assume that the diagram is closed (knot), error if not possible.
  //if flag_cyclic=false: open diagram (knotoid).
  //Throw a range_error if the diagram is not valid
  void load_from_file_extended_gauss_code(std::string filename,bool flag_cyclic);
  void load_from_file_extended_gauss_code(std::istream& file,bool flag_cyclic);
  void load_from_string_extended_gauss_code(std::string str,bool flag_cyclic);

  //evaluate PlanarGraph (used to draw the diagram)
  //min(1,2*nb_additional_nodes_per_arc_noloop) nodes are placed per arc (not forming loop)
  //min(2,2*nb_additional_nodes_per_arc_empty_loop) nodes are placed per arc (forming a loop inside the diagram).
  //min(2,2*nb_additional_nodes_per_arc_enclosing_loop) nodes are placed per arc (forming a loop enclosing the diagram).
  //Note: nb_additional_nodes_per_arc_loop must be greater or equal to 1.
  PlanarGraph get_planar_graph(int nb_additional_nodes_per_arc_noloop=0,int nb_additional_nodes_per_arc_empty_loop=1,int nb_additional_nodes_per_arc_enclosing_loop=3);

  //check if it is cyclic by checking if it has "Endpoint" crossings
  bool is_cyclic(){
    bool flag_cyclic=true;
    for(int n=0;n<crossings.size();n++)
      {
	if((crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0))//endpoint
	  {
	    flag_cyclic=false;
	    break;
	  }
      }
    return flag_cyclic;
  }

  //invert arcs (and crossing) numbering: replace 0, 1, 2, 3... by ..., 3, 2, 1, 0
  void change_orientation();
  //reorder crossings with order of appearance when following the curve
  void reorder_crossings();
  //if flag_cyclic=false: close the diagram by adding crossings if needed.
  //if flag_cyclic=true: do nothing
  //if flag_overpass=true: close diagram using overpass closure
  //if flag_overpass=false: close diagram using underpass closure
  void close(bool flag_overpass=false);
  
  //iteratively remove crossings with Reidemeister moves I and II until no changes.
  //assumes that crossings are ordered along the polygon, otherwise, will fail to detect Reidemeister moves II
  void simplify();
  //iteratively remove crossings with Reidemeister moves I and II until no changes.
  //assumes that crossings are ordered along the polygon, otherwise, will fail to detect Reidemeister moves II
  //then apply 1 random reidemeister move III and iterates during at most maxiterations iterations.
  //Stop after maxiterations_unsuccessfull consecutive iterations without simplification.
  void simplify_with_random_reidemeister_moves_III( long maxiterations,long maxiterations_unsuccessfull);

  void create_regions();//evaluate regions in the diagram, and store in crossings[n].region01/12/23/30 (1=outside, 2, 3,... are other regions). Only region 1 (outside) is kept, all other regions are revaluated. If flag_planar=false => start region numbering at 2, i.e. crossings[n].region01/12/23/30=2,3,4,5,6....

  int get_writhe();
  
  bool check();//test validity of the planar diagram. Return true if valid


private:

  void initialize();//Evaluate connected_crossing0,1,2,3
  void clean();//remove empty crossings and renumber arcs and renumber regions
  bool reidemeister_move_I(int n);//try reidemeister move I on crossing n, if it simplifies the diagram, erase crossing and return true. return false otherwise.
  bool reidemeister_move_II(int n,int m);//try reidemeister move II on crossing n and m, if it simplifies the diagram, erase crossings n,m and return true. return false otherwise. WARNING: first try reidemeister move I on each crossing. 
  bool reidemeister_move_III(int n,int m,int l);//try reidemeister move III on crossings n, m and l, if done =>  return true. return false otherwise.  WARNING: first try reidemeister move I on each crossing

  //check if diagram is planar, using algorithm described in
  //Vijayan and Wigderson (1982). "Planarity of Edge Ordered Graphs". Technical Report 307, Department of Electrical Engineering and Computer Science, Princeton University.
  bool check_planar();//test validity of the planar diagram. Return true if valid
  
  std::vector<Crossing> crossings;
  bool flag_debug;
  bool flag_planar;
  
  //definitions for boost graph
  struct NodeProperties_region
  {
    int region;
  };
  struct EdgeProperties_region
  {
    int arc;
    int crossing;
    int pos;
    int region;
  };
  struct NodeProperties_component
  {
    int crossing;
  };
  struct EdgeProperties_component
  {
    int component;
  };

};



////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif
