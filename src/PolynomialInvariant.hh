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

#ifndef POLYNOMIALINVARIANT_HH
#define POLYNOMIALINVARIANT_HH

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ctime>
#include <map>
#include <algorithm>
#include <sstream>
#include <PlanarDiagram.hh>
#include <Polynomial.hh>
#include <stdexcept>

//for boost graphs
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>


//************************************************************
//**********************   PolynomialInvariant   **********************
//************************************************************
//classical Jones polynomial for knots, Jones polynomial for knotoids (on the sphere), Turaev loop bracket for knotoids (on the plane)
class PolynomialInvariant
{
public:
  //empty diagram
  PolynomialInvariant(PlanarDiagram & diagram,bool flag_planar,bool flag_debug=false);

  void set_timeout(time_t t){timeout=t;}//timeout=0 => no timeout
  time_t get_timeout(){return timeout;}
  Polynomial get_polynomial_simple();
  Polynomial get_polynomial_recursive(std::string method="default",bool flag_silent=false);//method=default, crossing_order, arc_order, region_order


private:
  Polynomial jones_recursive(std::string method,std::vector<int> & crossing_type,std::set<int> & remaining_crossings,int last_smoothed_crossing=-1);
  void combination_first(std::vector<int> & bits);
  bool combination_next(std::vector<int> & bits);//if not cyclic, ignore first and last bit
  bool flag_debug;
  bool flag_planar;
  bool flag_cyclic;
  
  PlanarDiagram diagram;
  std::vector<Crossing> crossings;

  //for boost graphs
  struct NodeProperties
  {
    int region;
  };
  struct EdgeProperties//each edge is attached to a crossing (multiple edges are attached to each crossing)
  {
    int length;//number of diagram arcs to cross to reach target region (vertex) from source region (vertex)
    int sign;//sign=+1 if region is between crossing.arc0-crossing.arc1 or crossing.arc2-crossing.arc3. sign=-1 if region is between crossing.arc1-crossing.arc2 or crossing.arc3-crossing.arc0. NOTE: only defined for two regions on opposite side of the crossing
    int multiplicity;//count the number of time this edge appears in the diagram (with same length and sign)
    int crossing;//index of corresponding crossing.  NOTE: only defined for two regions on opposite side of the crossing
  };
  typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,NodeProperties, EdgeProperties > graph_t; //edges vecS => allow parallel edges
  typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;
  //to filter out edges with length>0
  class edge_predicate_zero_length {
  public:
    edge_predicate_zero_length() : graph_m(0) {}
    edge_predicate_zero_length(graph_t& graph) : graph_m(&graph) {}
    bool operator()(const edge_descriptor& edge_id) const {
      return (*graph_m)[edge_id].length==0;
    }
  private:
    graph_t* graph_m;
  };
  typedef boost::filtered_graph<graph_t, edge_predicate_zero_length> graph_t_zero_length;


  graph_t graph_regions;
  std::vector<std::pair<edge_descriptor,bool> > map_crossing_to_edge_1;  //map_crossing_to_edge_1[n]=pair<e1,flag1> such that if flag1==true => graph_regions[e1].length=2*crossing_type[n]
  std::vector<std::pair<edge_descriptor,bool> > map_crossing_to_edge_2;  //map_crossing_to_edge_2[n]=pair<e2,flag2> such that if flag2==true => graph_regions[e2].length=2-2*crossing_type[n]
  std::vector<vertex_descriptor> map_region_to_vertex;//map_region_to_vertex[r]=i => graph_regions[i].region=r
  int region_endpoint1;//endpoint 1 is in region region_endpoint1
  int region_endpoint2;//endpoint 2 is in region region_endpoint2

  //debug, used with get_polynomial_recursive:
  int recursion_level;
  time_t t0;
  time_t t1;
  double count_max;
  long long count_real;

  time_t timeout;//in seconds
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif
