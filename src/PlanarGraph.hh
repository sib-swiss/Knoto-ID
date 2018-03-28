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


#ifndef PLANARGRAPH_HH
#define PLANARGRAPH_HH

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>
#include <cmath>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <Random.hh>


#ifndef ZERO
#define ZERO 1e-10
#endif




//************************************************************
//**********************   PlanarGraph   **********************
//************************************************************
class PlanarGraph
{
public:
  PlanarGraph(bool flag_cyclic,bool flag_debug=false);

  //add edge from node1 to node2.
  //if draw>=0 => edge will be drawn, by increasing order of "to_draw"
  //if draw<0 =>  edge will not be drawn
  //z: z1=z value at node1, z2=z value at node2
  //WARNING: edges must be added in counter-clockwise order
  void add_edge(std::string node1_name,std::string node1_label,bool node1_external,int z1,std::string node2_name,std::string node2_label,bool node2_external,int z2,int draw);

  //triangulate regions, add dummy external nodes around external perimeter, check topology
  //WARNING:for every edge the opposite edge must exist, all nodes must have at least one outgoing edge
  void triangulate();

  //save xyz file (diagram in plane xy, z1>z2 z1 above z2)
  //if smooth>=0: use "smooth" points on cubic spline, if smooth<0 => plot circle centers & tangent points
  //if nb_iterations_relaxation>0: relax the graph.
  //if check_balanced=true, check that the diagram is balanced and quit with error if it is not balanced.
  //For relaxation:
  //  - decrease temperature from temperature_start to temperature_end
  //  - stops after nb_iterations_relaxation iterations or when distance_min/distance_max>=distance_ratio_threshold (distances=edges lengths, taking into account only edges with edges_draw>=0)
  void save_to_file_xyz(std::string filename,int smooth=0,long nb_iterations_relaxation=0,double distance_ratio_threshold=0.3,double temperature_start=1,double temperature_end=1e-3,bool check_balanced=true,bool debug_output=false);
  void save_to_file_xyz(std::ostream& file,int smooth=0,long nb_iterations_relaxation=0,double distance_ratio_threshold=0.3,double temperature_start=1,double temperature_end=1e-3,bool check_balanced=true,bool debug_output=false);
  
  //save list of edges (debug)
  void save_to_file_list(std::string filename);
  void save_to_file_list(std::ostream& file);

private:
  //create a node (if it does not exist) and return node index
  int add_node(std::string name,std::string label,bool is_external);

  //find, (m,j) such that such that edges_target_node[n][i]=m and edges_target_node[m][j]=n.
  //return j. If opposite edge does not exist => j<0;
  inline int get_opposite_edge(int n,int i){
    int m=edges_target_node[n][i];
    std::vector<int>::iterator it=find(edges_target_node[m].begin(),edges_target_node[m].end(),n);
    if(it!=edges_target_node[m].end())
      {
	return it-edges_target_node[m].begin();
      }
    std::cerr<<"*********************************************************"<<std::endl;
    std::cerr<<"ERROR PlanarGraph::get_opposite_edge() opposite edge missing."<<std::endl;
    std::cerr<<"*********************************************************"<<std::endl;
    exit(1);
    return -1;
  }
  
  //method from  C. R. Collins and K. Stephenson, "A Circle Packing Algorithm", Comp. Geom. Theory and Appl. (2003).
  //based on http://www.ics.uci.edu/~eppstein/PADS/CirclePack.py (by David Eppstein, University of California, Irvine.)
  void circle_packing();  
  //for a triangle with side lengths a, b, c, return angle opposite to side of length c.
  double get_angle(double a,double b,double c);
  //only used by circle_packing().
  //return true if valid, false if failed
  bool circle_packing_bfs_placement(double tolerance=1e-3);

  //nodes_x, nodes_y, nodes_r should first be created with circle_packing()
  //will modify nodes_x, nodes_y, nodes_r for non-dummy nodes
  //WARNING leave nodes_x, nodes_y, nodes_r in an incoherent states for dummy nodes
  //stops after nb_iterations_relaxation iterations or when distance_min/distance_max>=distance_ratio_threshold (distances=edges lengths, taking into account only edges with edges_draw>=0)
  void relax(long nb_iterations_relaxation,double distance_ratio_threshold=0.3,double temperature_start=1,double temperature_end=1e-3);

  //return true if valid, false if failed
  bool check_circle_packing(bool flag_check_external=true,double tolerance=1e-3);  

  //return N+2 points on a circle of radius R centered on (Cx,Cy)
  std::vector<std::vector<double> > get_circle(double Cx,double Cy,double R,int N);
  //return N+2 points on cubic spline 
    std::vector<double> get_spline(double x1,double x2,double x3,int N,double k=1.655);

  //return distance between point (x1,y1) and segment from (x2,y2) to (x3,y3)
  double distance_point_segment(double x1,double y1,double x2,double y2,double x3,double y3);


  bool flag_debug;
  bool flag_cyclic;
  //nodes
  std::vector<std::string> nodes_name;
  std::vector<std::string> nodes_label;
  std::vector<bool> nodes_external;
  std::map<std::string,int> nodes_name_to_index;
  //created by circle_packing();
  std::vector<double> nodes_x;
  std::vector<double> nodes_y;
  std::vector<double> nodes_r;

  
  //edges
  std::vector<std::vector<int> > edges_target_node; //edges_target_nodes[n][i]=m => i-th outgoing edge from node nodes_name[n] is connected to node nodes_name[m] (assumed to be ordered counterclockwise)
  std::vector<std::vector<std::pair<int,int> > > edges_z; //edges_z[n][i]=(z1,z2)  => z1=z value at node nodes_name[n], z2=z value at node nodes_name[edges_target_nodes[n][i]]. 
  std::vector<std::vector<int> > edges_draw; //edges_draw[n][i]=j if j>=0 => i-th outgoing edge from node nodes_name[n] will be drawn. WARNING: each value of j>=0 should appear only once.
  std::vector<std::vector<bool> > edges_external; //edges_external[n][i]=true => region between  edges_target_node[n][i] and  edges_target_node[n][(i+1)%N] is exterior. Initialized after calling triangulate();
  
  time_t t0;
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif
