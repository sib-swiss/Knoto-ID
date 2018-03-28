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

#ifndef POLYGON_HH
#define POLYGON_HH

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <PlanarDiagram.hh>
#include <Random.hh>
#include <stdexcept>

#ifndef ZERO
#define ZERO 1e-10
#endif

//************************************************************
//**********************   Polygon   **********************
//************************************************************
class Polygon
{
public:
  //empty polygon
  Polygon(bool flag_debug=false);
  //load from file with
  Polygon(std::string filename,bool flag_cyclic,bool flag_debug=false);
  //create from coordinates
  Polygon(const std::vector<double> & x,const std::vector<double> & y,const std::vector<double> & z,bool flag_cyclic,bool flag_debug=false);

  void set_debug(bool flag_debug_t){flag_debug=flag_debug_t;}
  
  void load_from_file_xyz(std::string filename,bool flag_cyclic);
  void load_from_file_xyz(std::istream& file,bool flag_cyclic);
  void save_to_file_xyz(std::string filename);
  void save_to_file_xyz(std::ostream& file);
  void save_to_file_hoomd(std::string filename,int nb_helper_beads_per_segment=0,double dx=0,double dy=0,double dz=0);//add axis of projection dx,dy,dz if |(dx,dy,dz)|>0 and flag_cyclic=false
  void save_to_file_hoomd(std::ostream& file,int nb_helper_beads_per_segment=0,double dx=0,double dy=0,double dz=0);//add axis of projection dx,dy,dz if |(dx,dy,dz)|>0 and flag_cyclic=false
  void save_to_file_vmdxyz(std::string filename);
  void save_to_file_vmdxyz(std::ostream& file);

  //if flag_cyclic=false => does nothing
  //if flag_cyclic=true && method="direct"
  // => does nothing
  //if flag_cyclic=true && method="rays"
  // => add points p1 and p2 in first and last position respectively. p1 and p2 are obtained by adding L*(dx,dy,dz) to the position of the first and last point respectively. The new polygon is then closed with a straight line.
  //WARNING: do not call twice with method="rays"
  void set_closure(double dx,double dy,double dz,std::string method="direct");
  
  int get_nb_points(){return polygon_x.size();}
  
  //dx,dy,dz is only used if flag_cyclic=false. In this case infinite straight lines parallel to dx,dy,dz are added to each end point, and these lines cannot be crossed by any segment of the polygon
  void simplify_polygon(double dx=0,double dy=0,double dz=1);

  //create planar diagram, projecting on the plane perpendicular to (dx,dy,dz).
  PlanarDiagram get_planar_diagram(double dx,double dy,double dz,bool flag_planar);

  //only for debuging: project polygon the plane perpendicular to (dx,dy,dz)
  void project(double dx=0,double dy=0,double dz=1);

  //return the polygon (piecewise linear curve) obtained by keeping points with index i: start_index<=start_index<=end_index
  //and flag_cyclic
  //start_index=0, end_index=get_nb_points()-1 => full polygon
  //if flag_cyclic and start_index>end_index => all points from start_index to end, and from beginning to end_index.
  Polygon get_polygon(int start_index,int end_index);
  Polygon get_polygon(int start_index,int end_index,bool flag_cyclic);
private:
  //sep_list=interpreted as list of char used as separators
  std::vector<std::string> split_string(std::string str,std::string sep_list);
  bool Intersection_segment_triangle(double x1,double y1,double z1,double x2,double y2,double z2,double X1,double Y1,double Z1,double X2,double Y2,double Z2,double X3,double Y3,double Z3,double tolerance=ZERO);//return true if the segment (x1,y1,z1),(x2,y2,z2) and the triangle (X1,Y1,Z1),(X2,Y2,Z2),(X3,Y3,Z3) intersect, return false if there is no intersection.
  bool Intersection_line_triangle(double x1,double y1,double z1,double x2,double y2,double z2,double X1,double Y1,double Z1,double X2,double Y2,double Z2,double X3,double Y3,double Z3,double tolerance=ZERO);//return true if the line passing through (x1,y1,z1) and (x2,y2,z2) and the triangle (X1,Y1,Z1),(X2,Y2,Z2),(X3,Y3,Z3) intersect, return false if there is no intersection.

  std::vector<double> polygon_x;
  std::vector<double> polygon_y;
  std::vector<double> polygon_z;
  bool flag_debug;
  bool flag_cyclic;
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif
