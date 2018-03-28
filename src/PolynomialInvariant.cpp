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


#include "PolynomialInvariant.hh"


using namespace std;
using namespace boost;

//************************************************************
//**********************   PolynomialInvariant   **********************
//************************************************************
PolynomialInvariant::PolynomialInvariant(PlanarDiagram & diagram_original,bool flag_planar,bool flag_debug):flag_planar(flag_planar),flag_debug(flag_debug),diagram(diagram_original)
{

  //NOTE: region 1 corresponds to exterior, and is only used if flag_planar=true. Otherwise, region numbering starts at 2.
  //NOTE: crossings will be split in two ways (crossing_type): 0 or 1
  //
  //    |         |    
  //  -----  by --/ /--   (case crossing_type=0)
  //    |           |
  //    
  //
  //    |           |  
  //  -----  by --\ \--   (case crossing_type=1)
  //    |         |

  //additionaly, we define two tables mapping crossing_number to edges:
  //map_crossing_to_edge_1[n]=pair<e1,flag1> such that
  //map_crossing_to_edge_2[n]=pair<e2,flag2> such that
  // if flag1: graph_regions[e1].length=2*crossing_type[n]
  // if flag2: graph_regions[e2].length=2-2*crossing_type[n]
  //Note: if flag1=false: e1 is not defined
  //Note: if flag2=false: e2 is not defined (no edge corresponding to this crossing
  
  // For the writhe of the diagram, we use the following sign convention:
  //
  //     ^
  //     |
  //  <------   => -1
  //     | 
  //    
  //     ^
  //     |
  //  ------>   => +1
  //     | 
  //    

  timeout=0;
  //create graph_region
  diagram.create_regions();
  if(flag_debug)
    {
      cerr<<"PD_code"<<endl;
      diagram.save_to_file_list(cerr);
    }
  //create "distance" graph with nodes=regions, edges = nb arcs to cross (with property d=nb arcs to cross)
  crossings=diagram.get_crossings();
  if(crossings.size()==0&&flag_planar)
    {
      //problem when using flag_planar=true with empty diagram. Since for empty diagrams there is no need for inside/outside information, we can as well set flag_planar=false
      flag_planar=false;
    }
  int r,ri,rj,d;
  region_endpoint1=-1;//endpoint 1 is in region region_endpoint1
  region_endpoint2=-1;//endpoint 2 is in region region_endpoint2
  vertex_descriptor vid,vid_i,vid_j;
  edge_descriptor eid;
  bool flag_found;
  EdgeProperties edge_prop;  
  map_crossing_to_edge_1.resize(crossings.size());
  map_crossing_to_edge_2.resize(crossings.size());
  //initialize to false
  for(int i=0;i<map_crossing_to_edge_1.size();i++)
    {
      map_crossing_to_edge_1[i].second=false;
      map_crossing_to_edge_2[i].second=false;
    }
  //vertex: property region -> vertex correspond to this region
  //edges: property length -> length = number of arcs to cross from source vertex (region) to target vertex (region), i.e. distance
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// removed
	continue;
      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint => store region
	{
	  if(crossings[n].arc0==0)//first endpoint
	    region_endpoint1=crossings[n].region01;
	  else
	    region_endpoint2=crossings[n].region01;
	}
      //add nodes
      for(int i=0;i<4;i++)
	{
	  if(crossings[n].get_arc(i)<0)// removed or endpoint
	    continue;
	  r=crossings[n].get_region(i,(i+1)%4);
	  if(map_region_to_vertex.size()<r+1)
	    map_region_to_vertex.resize(r+1,graph_traits<graph_t>::null_vertex());
	  if(map_region_to_vertex[r]==graph_traits<graph_t>::null_vertex())//does not exist yet
	    {
	      vid=add_vertex(graph_regions);
	      graph_regions[vid].region=r;
	      map_region_to_vertex[r]=vid;
	    }	  
	}
      //add edges
      for(int i=0;i<4;i++)
	{
	  if(crossings[n].get_arc(i)<0)// removed or endpoint
	    continue;
	  ri=crossings[n].get_region(i,(i+1)%4);
	  vid_i=map_region_to_vertex[ri];
	  for(int j=i+1;j<4;j++)
	      {
		if(crossings[n].get_arc(j)<0)// removed or endpoint
		  continue;
		rj=crossings[n].get_region(j,(j+1)%4);
		vid_j=map_region_to_vertex[rj];
		map<pair<int,int>,vector<int> >::iterator it;
		d=1;
		if(j==i+2)//j>i always
		  {
		    d=2;//corresponds to original crossing configuration-n;//temporary (for debug), it can as will be left uninitialized (AND !=1) since it will be replaced later when type of crossing is known
		  }
		//ni -> nj
		edge_prop.length=d;
		if(edge_prop.length!=1)
		  {
		    edge_prop.sign=1-2*(i%2);
		    edge_prop.crossing=n;

		  }
		else
		  {
		    edge_prop.sign=0;
		    edge_prop.crossing=-1;
		  }

		edge_prop.multiplicity=1;
		//check if edge vid_i-vid_j exist with same length and sign. If yes, simply increment multiplicity. otherwise add edge
		//WARNING: if edge_prop.length!=1, i.e.  j==i+2 => always add
		bool flag_add=true;
		if(edge_prop.length==1)
		  {
		    for(graph_traits<graph_t>::out_edge_iterator it = out_edges(vid_i,graph_regions).first;it!=out_edges(vid_i,graph_regions).second;it++)
		      {
			if(target((*it),graph_regions)==vid_j&&graph_regions[*it].length==edge_prop.length&&graph_regions[*it].sign==edge_prop.sign)
			  {
			    flag_add=false;
			    //increment multiplicity
			    graph_regions[*it].multiplicity+=1;
			    break;
			  }
		      }
		  }
		if(flag_add)
		  {
		    tie(eid,flag_found)=add_edge(vid_i,vid_j, edge_prop,graph_regions);
		    if(edge_prop.length!=1)//=> j==i+2
		      {
			if(i%2==0)
			  map_crossing_to_edge_2[n]=make_pair(eid,true);
			else
			  map_crossing_to_edge_1[n]=make_pair(eid,true);
		      }
		  }
		    
	      }
	}
    }
  //check
  if(flag_planar&&map_region_to_vertex[1]==graph_traits<graph_t>::null_vertex())
    {
      cerr<<"ERROR PolynomialInvariant() flag_planar=true but exterior region not found"<<endl;
      exit(1);
    }
  if((region_endpoint1<0&&region_endpoint2>=0)||(region_endpoint1>=0&&region_endpoint2<0))
    {
      if(diagram.get_nb_crossings(true)==0)//only one arc
	{
	  if(region_endpoint1>=0)
	    region_endpoint2=region_endpoint1;
	  else if(region_endpoint2>=0)
	    region_endpoint1=region_endpoint2;
	}
      else
	{
      cerr<<"ERROR PolynomialInvariant() found only one endpoint."<<endl;
	  exit(1);
	}
    }
  flag_cyclic=true;
  if(region_endpoint1>=0)
    flag_cyclic=false;


  /////////debug graph//////////
  if(flag_debug)
    {
      cerr<<"--------------------------------------------"<<endl;      
      cerr<<"Jones polynomial: Distance Graph "<<endl;
      for(int i=0;i<map_region_to_vertex.size();i++)
	cerr<<" map_region_to_vertex["<<i<<"]="<<map_region_to_vertex[i]<<endl;
      for(int i=0;i<map_crossing_to_edge_1.size();i++)
	{
	  if(map_crossing_to_edge_1[i].second)
	    cerr<<" map_crossing_to_edge_1["<<i<<"]="<<map_crossing_to_edge_1[i].first<<" (r"<<graph_regions[source(map_crossing_to_edge_1[i].first,graph_regions)].region<<"-r"<< graph_regions[target(map_crossing_to_edge_1[i].first,graph_regions)].region<<")"<<endl;
	}
      for(int i=0;i<map_crossing_to_edge_2.size();i++)
	{
	  if(map_crossing_to_edge_2[i].second)
	    cerr<<" map_crossing_to_edge_2["<<i<<"]="<<map_crossing_to_edge_2[i].first<<" (r"<<graph_regions[source(map_crossing_to_edge_2[i].first,graph_regions)].region<<"-r"<< graph_regions[target(map_crossing_to_edge_2[i].first,graph_regions)].region<<")"<<endl;;
	}
      int n=0;
      for (graph_traits<graph_t>::vertex_iterator it = vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
	{
	  cerr<<" vertex "<<n<<": graph_regions["<<(*it)<<"].region="<<graph_regions[*it].region<<endl;
	  n++;
	}
      n=0;
      for (graph_traits<graph_t>::edge_iterator it = edges(graph_regions).first; it!=edges(graph_regions).second;it++)
	{      
	  cerr<<" edge "<<n<<": "<<source((*it),graph_regions)<<"-"<<target((*it),graph_regions)<<" (r"<<graph_regions[source((*it),graph_regions)].region<<"-r"<<graph_regions[target((*it),graph_regions)].region<<")"<<" i.e. graph_regions["<<(*it)<<"].length="<<graph_regions[*it].length<<" sign="<<graph_regions[*it].sign<<" multiplicity="<<graph_regions[*it].multiplicity<<endl;
	  n++;
	}
      if(flag_planar)
	cerr<<"Region exterior=1 node index: map_region_to_vertex[1]="<<map_region_to_vertex[1]<<endl;
      cerr<<"Region endpoint1="<<region_endpoint1<<" node index: map_region_to_vertex["<<region_endpoint1<<"]=";
      if(region_endpoint1>=0)
	cerr<<map_region_to_vertex[region_endpoint1]<<endl;
      else
	cerr<<"NA"<<endl;
      cerr<<"Region endpoint2="<<region_endpoint2<<" node index: map_region_to_vertex["<<region_endpoint2<<"]=";
      if(region_endpoint2>=0)
	cerr<<map_region_to_vertex[region_endpoint2]<<endl;
      else
	cerr<<"NA"<<endl;

    }


}

//---------------------------------------------------------------------------

Polynomial PolynomialInvariant::get_polynomial_simple()
{
  if(diagram.get_nb_crossings(true)==0)
    {
      Polynomial polynomial_jones;
      polynomial_jones.set_variable_names("A","v");
      polynomial_jones.add_monomial(1,0,0);
      return polynomial_jones;
    }
  //we define a bit vector to store crossing types:
  // crossing_type[n]=0,1 if crossings[n] is set to type 0 or 1.
  // Note: it is also defined for n corresponding to endpoints, but ignored.

  //In this function, we evaluate the jones polynomial as:
  //jones(A,v)=(-A^3)^(-writhe)*sum_s (-A^2-A^(-2))^p(s)*v^q(s)*A^sigma(s)
  //were the sum runs over all realizations of the bit vector crossing_type (one realization of crossing_type is denoted s) WARNING: ignoring endpoints
  // sigma(s)=(number of crossing_type 0) - (number of crossing_type 1)  (WARNING: ignoring endpoints)
  // for knotoids:
  //   p(s)=number of rings disjoint from the segment joining the two endpoints
  //   q(s)=number of rings containing the segment joining the two endpoints (=0 on the sphere)
  // for closed knots:
  //   p(s)=number of rings - 1
  //   q(s)=0
  // writhe=writhe of the diagram, with following sign convention:
  //
  //     ^
  //     |
  //  <------   => -1
  //     | 
  //    
  //     ^
  //     |
  //  ------>   => +1
  //     | 
  //    
  
  ////////////loop over all combinations of crossing_type/////////////

  Polynomial polynomial_jones;
  polynomial_jones.set_variable_names("A","v");
  Polynomial polynomial_A;//A
  polynomial_A.add_monomial(1,1,0);
  polynomial_A.set_variable_names("A","v");
  Polynomial polynomial_A_inv;//A^-1
  polynomial_A_inv.add_monomial(1,-1,0);
  polynomial_A_inv.set_variable_names("A","v");
  Polynomial polynomial_v;//v
  polynomial_v.add_monomial(1,0,1);
  polynomial_v.set_variable_names("A","v");
  Polynomial polynomial_delta;//-A^2-A^-2
  polynomial_delta.add_monomial(-1,2,0);
  polynomial_delta.add_monomial(-1,-2,0);
  polynomial_delta.set_variable_names("A","v");
  vector<int> crossing_type(crossings.size());
  combination_first(crossing_type);

  t0=time(NULL);
  t1=time(NULL);
  count_real=0;
  bool flag_output_status=false;
  do
    {
      count_real+=1;
      if(!flag_debug&&time(NULL)-t1>=2)
	{
	  time_t t_tot=time(NULL)-t0;
	  if(timeout>0&&t_tot>timeout)
	    {
          throw runtime_error("PolynomialInvariant: Timeout.");
	    }
	  if(flag_cyclic)
	    cerr<<"\r"<<"Polynomial "<<int(0.5+100*count_real*100.0/pow(2.0,(double)crossing_type.size()))/100.0<<" %"<<" elapsed time: "<<t_tot<<" s, remaining time: "<<lround(t_tot*pow(2.0,(double)crossing_type.size())/(double)count_real-t_tot)<<" s          ";
	  else
	    cerr<<"\r"<<"Polynomial "<<int(0.5+100*count_real*100/pow(2.0,(double)crossing_type.size()-2))/100.0<<" %"<<" elapsed time: "<<t_tot<<" s, remaining time: "<<lround(t_tot*pow(2.0,(double)crossing_type.size()-2)/(double)count_real-t_tot)<<" s          ";
	  t1=time(NULL);
	  flag_output_status=true;
	}
      if(flag_debug)
	{
	  cerr<<"crossing_type: ";
	  for(int i=0;i<crossing_type.size();i++)
	    cerr<<crossing_type[i];
	  cerr<<endl;
	}
      //assign length to each edge
      for(int n=0;n<map_crossing_to_edge_1.size();n++)
	if(map_crossing_to_edge_1[n].second)
	  graph_regions[map_crossing_to_edge_1[n].first].length=2*crossing_type[n];
      for(int n=0;n<map_crossing_to_edge_2.size();n++)
	if(map_crossing_to_edge_2[n].second)
	  graph_regions[map_crossing_to_edge_2[n].first].length=2-2*crossing_type[n];

      if(flag_debug)
	{
	  ///output graph
	  cerr<<" Distance Graph: "<<endl;
	  int n=0;
	  for (graph_traits<graph_t>::vertex_iterator it = vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
	    {
	      cerr<<"  vertex "<<n<<": graph_regions["<<(*it)<<"].region="<<graph_regions[*it].region<<endl;
	      n++;
	    }
	  n=0;
	  for (graph_traits<graph_t>::edge_iterator it = edges(graph_regions).first; it!=edges(graph_regions).second;it++)
	    {      
	      cerr<<"  edge "<<n<<": "<<source((*it),graph_regions)<<"-"<<target((*it),graph_regions)<<" (r"<<graph_regions[source((*it),graph_regions)].region<<"-r"<<graph_regions[target((*it),graph_regions)].region<<")"<<" i.e. graph_regions["<<(*it)<<"].length="<<graph_regions[*it].length<<endl;
	      n++;
	    }
	}

      //shortest path
      int shortest_path_length=-1;
      if(flag_planar&&!flag_cyclic)
	{
	  vector<int> distances(num_vertices(graph_regions));
	  dijkstra_shortest_paths(graph_regions,map_region_to_vertex[1], 
				  weight_map(get(&EdgeProperties::length, graph_regions))
				  .distance_map(make_iterator_property_map(distances.begin(),get(vertex_index, graph_regions))));
	  shortest_path_length=distances[map_region_to_vertex[region_endpoint2]];
	}
      
      //filter graph (keep only edges of length 0):
      graph_t_zero_length graph_regions_filtered(graph_regions, edge_predicate_zero_length(graph_regions)); 
      //nb connected components
      vector<int> vertex_component(num_vertices(graph_regions_filtered));
      int ncomponents = connected_components(graph_regions_filtered, &vertex_component[0]);

      //////eval polynomial
      int nrings=ncomponents-1;
      int q=0;//nb rings containing the segment => =0 if not planar (on the sphere)
      if(shortest_path_length>=0)
	q=shortest_path_length;//nb rings containing the segment
      int p=nrings-q;//nb ring disjoints from the segment;
      if(flag_cyclic)
	p=p-1;//
      int sigma=0;//nb0-nb1
      for(int i=0;i<crossing_type.size();i++)
	{
	  if(!flag_cyclic&&i==0)continue;// (ignore endpoints)
	  if(!flag_cyclic&&i==crossing_type.size()-1)continue;// (ignore endpoints)
	  if(crossing_type[i]==0)
	    sigma+=1;
	  else
	    sigma-=1;
	}
      Polynomial tmp;
      tmp.add_monomial(1,0,0);//1	    
      //(-A^2-A^-2)^p
      for(int i=0;i<p;i++)//p always positive
	tmp.multiply(polynomial_delta);
      //multiply by v^q
      for(int i=0;i<q;i++)//q always positive
	tmp.multiply(polynomial_v);
      //multiply by A^sigma
      if(sigma>0)
	{
	  for(int i=0;i<sigma;i++)
	    tmp.multiply(polynomial_A);
	}
      else//sigma<0
	{
	  for(int i=0;i<-sigma;i++)
	    tmp.multiply(polynomial_A_inv);
	}
      polynomial_jones.add(tmp);
      if(flag_debug)
	{
	  cerr<<" Distance graph: shortest_path_length="<<shortest_path_length<<endl;
	  cerr<<" Distance graph (only length 0 edges): nb components="<<ncomponents<<endl;
	  cerr<<" nrings="<<nrings<<endl;
	  cerr<<" q="<<q<<endl;
	  cerr<<" p="<<p<<endl;
	  cerr<<" sigma="<<sigma<<endl;
	  tmp.set_variable_names("A","v");
	  cerr<<" contribution to polynomial: "<<tmp<<endl;
	}      
    }
  while(combination_next(crossing_type));

  if(flag_output_status)
    cerr<<endl;

  //evaluate writhe
  int writhe=diagram.get_writhe();
  //multiply polynomial_jones by  (-A^3)^-writhe =(-A^-sign(writhe)*3)^|writhe|
  if(writhe>0)
    {
      Polynomial tmp;
      tmp.add_monomial(-1,-3,0);//-A^-3
      for(int i=0;i<writhe;i++)
	polynomial_jones.multiply(tmp);
    }
  else//writhe<0
    {
      Polynomial tmp;      
      tmp.add_monomial(-1,3,0);//-A^3
      for(int i=0;i<-writhe;i++)
	polynomial_jones.multiply(tmp);
    }
  

  if(flag_debug)
    {
      cerr<<"polynomial_jones="<<polynomial_jones<<endl;
      cerr<<"writhe="<<writhe<<endl;
      cerr<<"PD_code"<<endl;
      diagram.save_to_file_list(cerr);
    }
  return polynomial_jones;
}


//---------------------------------------------------------------------------

Polynomial PolynomialInvariant::get_polynomial_recursive(string method,bool flag_silent)
{
  if(flag_debug)
    {
      cerr<<"PolynomialInvariant::get_polynomial_recursive() constructor PD_code"<<endl;
      diagram.save_to_file_list(cerr);
    }
  if(diagram.get_nb_crossings(true)==0)
    {
      Polynomial polynomial_jones;
      polynomial_jones.set_variable_names("A","v");
      polynomial_jones.add_monomial(1,0,0);
      return polynomial_jones;
    }

  
  //NOTE: region 1 corresponds to exterior, and is only used if flag_planar=true. Otherwise, region numbering starts at 2.
  //NOTE: crossings will be split in two ways (crossing_type): 0 or 1 (or 2=not original crossing)
  //
  //    |         |    
  //  -----  by --/ /--   (case crossing_type=0)
  //    |           |
  //    
  //
  //    |           |  
  //  -----  by --\ \--   (case crossing_type=1)
  //    |         |
  //we define a bit vector to store crossing types:
  // crossing_type[n]=0,1 if crossings[n] is set to type 0 or 1.
  // Note: it is also defined for n corresponding to endpoints, but ignored.

  
  Polynomial polynomial_jones;


  set<int> remaining_crossings;
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0>=0&&crossings[n].arc1>=0&&crossings[n].arc2>=0&&crossings[n].arc3>=0)//not endpoint, not removed
	remaining_crossings.insert(n);
    }
  vector<int> crossing_type(crossings.size(),2);//2 => original crossing
  recursion_level=0;
  count_max=0;
  count_real=0;
  t0=time(NULL);
  t1=t0;
  polynomial_jones=jones_recursive(method,crossing_type,remaining_crossings);
  if(t1!=t0)
    cerr<<endl;
  if(flag_debug&&!flag_silent)
    {
      if(flag_cyclic)
	cerr<<"fraction of evaluations done: "<<count_real*100.0/pow(2.0,(double)crossing_type.size()) <<"%"<<endl;
      else
	cerr<<"fraction of evaluations done: "<<count_real*100.0/pow(2.0,(double)crossing_type.size()-2) <<"%"<<endl;
    }
  //evaluate writhe
  int writhe=diagram.get_writhe();
  //multiply polynomial_jones by  (-A^3)^-writhe =(-A^-sign(writhe)*3)^|writhe|
  if(writhe>0)
    {
      Polynomial tmp;
      tmp.add_monomial(-1,-3,0);//-A^-3
      for(int i=0;i<writhe;i++)
	polynomial_jones.multiply(tmp);
    }
  else//writhe<0
    {
      Polynomial tmp;      
      tmp.add_monomial(-1,3,0);//-A^3
      for(int i=0;i<-writhe;i++)
	polynomial_jones.multiply(tmp);
    }
  
  if(flag_debug)
    {
      cerr<<"writhe="<<writhe<<endl;
      cerr<<"PD_code"<<endl;
      diagram.save_to_file_list(cerr);

    }
  return  polynomial_jones;
}






//////////////////////////////////////////////////////////////////////
//Private
//////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------

Polynomial PolynomialInvariant::jones_recursive(string method,vector<int> & crossing_type,set<int> &remaining_crossings,int last_smoothed_crossing)
{
  if(!flag_debug&&time(NULL)-t1>=2)
    {
      time_t t_tot=time(NULL)-t0;
      if(timeout>0&&t_tot>timeout)
	{
      throw runtime_error("PolynomialInvariant: Timeout.");
	}
      
      if(flag_cyclic)
	cerr<<"\r"<<"Polynomial "<<int(0.5+100*count_max*100.0/pow(2.0,(double)crossing_type.size()))/100.0<<" %"<<" elapsed time: "<<t_tot<<" s          "; //, remaining time: "<<lround(t_tot*pow(2.0,(double)crossing_type.size())/(double)count_max-t_tot)<<" s          "; 
      else
	cerr<<"\r"<<"Polynomial "<<int(0.5+100*count_max*100/pow(2.0,(double)(crossing_type.size()-2)))/100.0<<" %"<<" elapsed time: "<<t_tot<<" s          "; //, remaining time: "<<lround(t_tot*pow(2.0,(double)(crossing_type.size()-2))/(double)count_max-t_tot)<<" s          ";
      t1=time(NULL);
    }
  string debug_spacer;
  if(flag_debug)
    {
      recursion_level++;
      
      for(int i=1;i<recursion_level;i++)
	debug_spacer+="| ";
      cerr<<debug_spacer<<"start remaining_crossings.size()="<<remaining_crossings.size()<<" last_crossing="<<last_smoothed_crossing<<" recursion level="<<recursion_level<<endl;
    }
  if(flag_debug)
    {
      cerr<<debug_spacer<<"| current crossing_type ";
      for(int i=0;i<crossing_type.size();i++)
	cerr<<crossing_type[i];
      cerr<<endl;
    }

  vector<int> smoothed_crossings;//store crossings smoothed in this function call
  Polynomial polynomial_result;
  polynomial_result.set_variable_names("A","v");
  Polynomial polynomial_tmp;
  int total_typeI_removal=0;//=(nb positive type I removal)-(nb negative type I removal) => to evaluate correction (-A^3)^total_typeI_removal to the jones polynomial
  vector<int> total_multiplicity;//total_multiplicity[r]=sum of multiplicity in region r. If regions r1 and r2 are merged, it will contain the same informatoin for both regions. Only used if method="region_order"
  vector<int> total_sign;//total_sign[r]=sum of sign in region r. only used if method="region_order"
  if(remaining_crossings.size()>0)//not finished => try simplifications
    {
      //////////////////////////try to simplify diagram with type I and II reidemeister moves/////////////////
      //keep only zero length edges
      graph_t_zero_length graph_regions_filtered(graph_regions, edge_predicate_zero_length(graph_regions)); 
      //nb connected components (each connected component correspond to a group of regions that are merged)
      vector<int> vertex_component(num_vertices(graph_regions_filtered));
      int ncomponents = connected_components(graph_regions_filtered, &vertex_component[0]);      
      vector<int> component_multiplicity(ncomponents,0);//component_multiplicity[n]=total multiplicity of component n (i.e. vertices such that vertex_components[v]=n)
      vector<int> component_sign(ncomponents,0);
      vector<int> component_exterior(ncomponents,false);//component_exterior[n]==true if component n contains exterior regions (i.e. region 1) OR endpoints regions (region_endpoint1/2)
      vector<bool> component_removed(ncomponents,false);//component_removed[n]==true if the component has been removed (merged with another component)
      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
	{
	  if(graph_regions[*it].region==1||graph_regions[*it].region==region_endpoint1||graph_regions[*it].region==region_endpoint2)
	    component_exterior[vertex_component[*it]]=true;
	  for(graph_traits<graph_t>::out_edge_iterator e = out_edges(*it,graph_regions).first;e!=out_edges(*it,graph_regions).second;e++)
	    {
	      if(graph_regions[*e].length==2&&graph_regions[*e].crossing>=0&&crossing_type[graph_regions[*e].crossing]==2)//only edges of length 2 are considered that not yet smoothed
		{
		  component_sign[vertex_component[*it]]+=graph_regions[*e].sign;
		  component_multiplicity[vertex_component[*it]]+=graph_regions[*e].multiplicity;
		}
	    }
	}
      if(flag_debug)
	{
	  for(int i=0;i<ncomponents;i++)
	    {
	      if(component_removed[i])
		continue;
	      cerr<<debug_spacer<<"| component["<<i<<"] ext="<<component_exterior[i]<<" sign="<<component_sign[i]<<" mult="<<component_multiplicity[i]<<" (regions:";
	      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
		{
		  if(vertex_component[*it]==i)
		    cerr<<" "<<graph_regions[*it].region;
		}
	      cerr<<")"<<endl;
	      
	    }
	}
      //search for possible simplifications
      int nb_simplifications=1;
      do//loop while nb_simplications>0
	{
	  nb_simplifications=0;
	  for(int i=0;i<ncomponents;i++)
	    {
	      if(component_removed[i])
		continue;

	      //type II
	      bool flag_remove_typeII=false;
	      bool flag_remove_typeI=false;
	      if(component_exterior[i]==false&&component_sign[i]==0&&component_multiplicity[i]==2)
		{
		  flag_remove_typeII=true;
		  if(flag_debug)
		    {
		      cerr<<debug_spacer<<"| test component "<<i<<" (type II): regions";
		      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
			{
			  if(vertex_component[*it]==i)
			    cerr<<" "<<graph_regions[*it].region;
			}
		      cerr<<" ************"<<endl;
		    }
		}
	      //type I
	      if(component_exterior[i]==false&&component_multiplicity[i]==1)
		{
		  flag_remove_typeI=true;
		  if(flag_debug)
		    {
		      cerr<<debug_spacer<<"| test component "<<i<<" (type I): regions";
		      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
			{
			  if(vertex_component[*it]==i)
			    cerr<<" "<<graph_regions[*it].region;
			}
		      cerr<<" ************"<<endl;
		    }
		}
	  
	      if(flag_remove_typeII||flag_remove_typeI)
		{
		  int n=-10;
		  //find edges correspondoing to crossings to be smoothed. For type II, check that both edges are not on the same crossing.
		  //Check that the region has a unique boundary (i.e. no holes in the region), i.e. build a graph (graph_boundary) with vertex=arcs and edge=two arcs are connected at a crossings => nb boundary=nb component of this graph (note: we reuse graph_t, but store arc index in the "region" node property
		  graph_t graph_boundary;
		  map<int,vertex_descriptor> map_arc_to_vertex;//map_arc_to_vertex[a]=v <=> vertice graph_boundary[v] corresponds to arc a
		  vector<edge_descriptor> edges;//contain edges (from graph_regions) corresponding to crossings to be smoothed
		  for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
		    {
		      if(vertex_component[*it]==i)
			{
			  for(graph_traits<graph_t>::out_edge_iterator e = out_edges(*it,graph_regions).first;e!=out_edges(*it,graph_regions).second;e++)
			    {
			      //if more that one edge, check that both edges are not on the same crossing:
			      if(graph_regions[*e].length==2&&graph_regions[*e].crossing>=0&&crossing_type[graph_regions[*e].crossing]==2)//only edges of length 2 are considered that not yet smoothed
				{
				  edges.push_back(*e);
				  if(n==graph_regions[*e].crossing)
				    {
				      flag_remove_typeII=false;
				      break;
				    }
				  n=graph_regions[*e].crossing;
				}
			      //check that the region has a unique boundary (i.e. no holes in the region) => build graph of arcs connections
			      if((graph_regions[*e].length==2||graph_regions[*e].length==0)&&graph_regions[*e].crossing>=0)//only edges of length 2 or 0 (i.e. corresponding to opposite sides of the crossing)
				{
				  int c=graph_regions[*e].crossing;
				  int a;
				  if(crossings[c].arc0<0||crossings[c].arc1<0||crossings[c].arc2<0||crossings[c].arc3<0)//endpoint or empty
				    continue;
				  //add vertices & edges
				  for(int k=0;k<4;k++)
				    {
				      if(graph_regions[*it].region==crossings[c].get_region(k,(k+1)%4))
					{
					  if(crossing_type[c]==2)
					    {
					      a=crossings[c].get_arc(k);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      a=crossings[c].get_arc((k+1)%4);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      add_edge(map_arc_to_vertex[crossings[c].get_arc(k)],map_arc_to_vertex[crossings[c].get_arc((k+1)%4)],graph_boundary);
					    }
					  else if((k%2==1&&crossing_type[c]==0)||(k%2==0&&crossing_type[c]==1))
					    {					  
					      a=crossings[c].get_arc(k);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      a=crossings[c].get_arc((k+3)%4);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      add_edge(map_arc_to_vertex[crossings[c].get_arc(k)],map_arc_to_vertex[crossings[c].get_arc((k+3)%4)],graph_boundary);
					      a=crossings[c].get_arc((k+1)%4);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      a=crossings[c].get_arc((k+2)%4);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      add_edge(map_arc_to_vertex[crossings[c].get_arc((k+1)%4)],map_arc_to_vertex[crossings[c].get_arc((k+2)%4)],graph_boundary);
					    }
					  else if((k%2==0&&crossing_type[c]==0)||(k%2==1&&crossing_type[c]==1))
					    {
					      a=crossings[c].get_arc(k);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      a=crossings[c].get_arc((k+1)%4);
					      if(map_arc_to_vertex.find(a)==map_arc_to_vertex.end())
						map_arc_to_vertex[a]=add_vertex(graph_boundary);
					      add_edge(map_arc_to_vertex[crossings[c].get_arc(k)],map_arc_to_vertex[crossings[c].get_arc((k+1)%4)],graph_boundary);
					    }
					}
				    }
				}

			    }
			}
		    }
		  if(flag_remove_typeII||flag_remove_typeI)
		    {
		      //check that the boundary of the (union of) region has only one component
		      vector<int> vertex_component_tmp(num_vertices(graph_boundary));
		      int ncomponents = connected_components(graph_boundary, &vertex_component_tmp[0]);
		      if(ncomponents!=1)
			{
			  flag_remove_typeII=false;
			  flag_remove_typeI=false;
			}		      
		      if(flag_remove_typeII&&edges.size()!=2)
			{
			  cerr<<"OUPS edges.size()!=2 ("<<edges.size()<<")"<<endl;
			  exit(1);
			}			    
		      if(flag_remove_typeI&&edges.size()!=1)
			{
			  cerr<<"OUPS edges.size()!=1 ("<<edges.size()<<")"<<endl;
			  exit(1);
			}			    
		    }
		  //simplify type I/II: update crossing_type, graph_regions edge length
		  if(flag_remove_typeII||flag_remove_typeI)
		    {
		      if(flag_debug)
			{
			  if(flag_remove_typeII)
			    cerr<<debug_spacer<<"| remove component "<<i<<" (type II): regions";
			  if(flag_remove_typeI)
			    cerr<<debug_spacer<<"| remove component "<<i<<" (type I): regions";		      
			  for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
			    {
			      if(vertex_component[*it]==i)
				cerr<<" "<<graph_regions[*it].region;
			    }
			  cerr<<" ***********************"<<endl;
			}
		      //find corresponding edges & crossing and set crossing_type, graph_regions edge length
		      for(int j=0;j<edges.size();j++)
			{
			  int n=graph_regions[edges[j]].crossing;
			  //add to smoothed crossings
			  smoothed_crossings.push_back(n);
			  //update crossing_type
			  if(graph_regions[edges[j]].sign==+1)			
			    crossing_type[n]=1;
			  if(graph_regions[edges[j]].sign==-1)
			    crossing_type[n]=0;
			  total_typeI_removal+=graph_regions[edges[j]].sign;//we also sum for type, but it does not contribute since total sign in type=0
			  //assign length to edge n in graph_region
			  if(map_crossing_to_edge_1[n].second)
			    graph_regions[map_crossing_to_edge_1[n].first].length=2*crossing_type[n];
			  if(map_crossing_to_edge_2[n].second)
			    graph_regions[map_crossing_to_edge_2[n].first].length=2-2*crossing_type[n];  
			  //erase n from remaining_crossings
			  remaining_crossings.erase(n);
			  count_max+=pow(2.0,(double)remaining_crossings.size());
			}
		    }
		  if(flag_remove_typeII)
		    {
		      //merge components n2,n3 to n1
		      int n1=vertex_component[source(edges[0],graph_regions)];
		      int n2=vertex_component[target(edges[0],graph_regions)];
		      int n3=vertex_component[target(edges[1],graph_regions)];
		      if(n1==n2||n1==n3)
			{
			  cerr<<"OUPS n1==n2||n1==n3"<<endl;
			  exit(1);
			}
		      //rename component in vertex_component
		      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
			{
			  if(vertex_component[*it]==n2||vertex_component[*it]==n3)
			    vertex_component[*it]=n1;
			}
		      //set components n2,n3 as removed
		      component_removed[n2]=true;
		      component_removed[n3]=true;
		    }
		  //type I simplification
		  else if(flag_remove_typeI)
		    {
		      //merge components n2 to n1
		      int n1=vertex_component[source(edges[0],graph_regions)];
		      int n2=vertex_component[target(edges[0],graph_regions)];
		      if(n1==n2)
			{
			  cerr<<"OUPS n1==n2"<<endl;
			  exit(1);
			}
		      //rename component in vertex_component
		      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
			{
			  if(vertex_component[*it]==n2)
			    vertex_component[*it]=n1;
			}
		      //set components n2 as removed
		      component_removed[n2]=true;
		    }

		  //reevaluate component_multiplicity/sign/exterior
		  if(flag_remove_typeII||flag_remove_typeI)
		    {
		      nb_simplifications++;
		      for(int j=0;j<ncomponents;j++)
			{
			  component_sign[j]=0;
			  component_multiplicity[j]=0;
			  component_exterior[j]=false;
			}
		      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
			{
			  if(graph_regions[*it].region==1||graph_regions[*it].region==region_endpoint1||graph_regions[*it].region==region_endpoint2)
			    component_exterior[vertex_component[*it]]=true;
			  for(graph_traits<graph_t>::out_edge_iterator e = out_edges(*it,graph_regions).first;e!=out_edges(*it,graph_regions).second;e++)
			    {
			      if(graph_regions[*e].length==2&&graph_regions[*e].crossing>=0&&crossing_type[graph_regions[*e].crossing]==2)//only edges of length 2 are considered that not yet smoothed
				{
				  component_sign[vertex_component[*it]]+=graph_regions[*e].sign;
				  component_multiplicity[vertex_component[*it]]+=graph_regions[*e].multiplicity;
				}
			    }
			}

		      if(flag_debug)
			{
			  cerr<<debug_spacer<<"| current crossing_type ";
			  for(int i=0;i<crossing_type.size();i++)
			    cerr<<crossing_type[i];
			  cerr<<endl;
			  for(int i=0;i<ncomponents;i++)
			    {
			      if(component_removed[i])
				continue;
			      cerr<<debug_spacer<<"| component["<<i<<"] ext="<<component_exterior[i]<<" sign="<<component_sign[i]<<" mult="<<component_multiplicity[i]<<" (regions:";
			      for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
				{
				  if(vertex_component[*it]==i)
				    cerr<<" "<<graph_regions[*it].region;
				}
			      cerr<<")"<<endl;
			  
			    }
			}
		  
		    }
		}
	      //set crossing_type to 0 or 1 for removed crossings...
	      //add to smoothed_crossings
	      //update graph_region.length?
	      //warning: type I => modify polynomial.... *(-A)^+/-3
	      //warning erase from remaining_crossings
	      //=> update
	      //vector<int> vertex_component[v]
	      //int ncomponents
	      //vector<int> component_multiplicity[n]
	      //vector<int> component_sign[n]
	      //vector<int> component_exterior[n]
	  
	    }
	  if(flag_debug)
	    {
	      cerr<<debug_spacer<<"| total_typeI_removal="<<total_typeI_removal<<endl;
	    }
	}while(nb_simplifications>0);
      //only used in method="region_order"
      total_multiplicity.clear();
      total_sign.clear();
      int r;
      for(int i=0;i<ncomponents;i++)
	{
	  for (graph_traits<graph_t>::vertex_iterator it=vertices(graph_regions).first; it!=vertices(graph_regions).second;it++)
	    {
	      if(vertex_component[*it]==i)
		{
		  r=graph_regions[*it].region;
		  if(total_multiplicity.size()<=r)
		    {
		      total_multiplicity.resize(r+1,0);
		      total_sign.resize(r+1,0);
		    }
		  total_multiplicity[r]=component_multiplicity[i];
		  total_sign[r]=component_sign[i];
		}
	    }
	}
    }
  
  
  if(remaining_crossings.size()==0)//finished
    {
      count_real+=1;
      count_max+=1;
      if(flag_debug)
	{
	  cerr<<debug_spacer<<"| current crossing_type ";
	  for(int i=0;i<crossing_type.size();i++)
	    cerr<<crossing_type[i];
	  cerr<<" -> finished "<<endl;
	}
      //shortest path
      int shortest_path_length=-1;
      if(flag_planar&&!flag_cyclic)
	{
	  vector<int> distances(num_vertices(graph_regions));
	  //dijkstra_shortest_paths(graph, Source, predecessor_map(&parents[0]).distance_map(&distances[0]));
	  dijkstra_shortest_paths(graph_regions,map_region_to_vertex[1], 
				  weight_map(get(&EdgeProperties::length, graph_regions))
				  .distance_map(make_iterator_property_map(distances.begin(),get(vertex_index, graph_regions)))
				  );
	  shortest_path_length=distances[map_region_to_vertex[region_endpoint2]];
	}
      //filter graph (keep only edges of length 0):
      graph_t_zero_length graph_regions_filtered(graph_regions, edge_predicate_zero_length(graph_regions)); 
      //nb connected components
      vector<int> vertex_component(num_vertices(graph_regions_filtered));
      int ncomponents = connected_components(graph_regions_filtered, &vertex_component[0]);

      //////eval polynomial
      int nrings=ncomponents-1;
      int q=0;//nb rings containing the segment => =0 if not planar (on the sphere)
      if(shortest_path_length>=0)
	q=shortest_path_length;//nb rings containing the segment
      int p=nrings-q;//nb ring disjoints from the segment;
      if(flag_cyclic)
	p=p-1;//
      

      polynomial_result.add_monomial(1,0,0);//1	    
      //(-A^2-A^-2)^p
      if(p>0)
	{
	  Polynomial polynomial_delta;//-A^2-A^-2
	  polynomial_delta.add_monomial(-1,2,0);
	  polynomial_delta.add_monomial(-1,-2,0);
	  for(int i=0;i<p;i++)//p always positive
	    polynomial_result.multiply(polynomial_delta);
	}
      //multiply by v^q
      if(q>0)
	{
	  Polynomial polynomial_v;//v
	  polynomial_v.add_monomial(1,0,1);
	  for(int i=0;i<q;i++)//q always positive
	    polynomial_result.multiply(polynomial_v);
	}
      //multiply by (-A^3)^total_typeI_removal
      if(total_typeI_removal>0)
	{
	  Polynomial polynomial_A3;//-A^3
	  polynomial_A3.add_monomial(-1,3,0);
	  for(int i=0;i<total_typeI_removal;i++)
	    polynomial_result.multiply(polynomial_A3);
	}
      else if(total_typeI_removal<0)
	{
	  Polynomial polynomial_A3;//-A^-3
	  polynomial_A3.add_monomial(-1,-3,0);
	  for(int i=0;i<-total_typeI_removal;i++)
	    polynomial_result.multiply(polynomial_A3);
	}
      
      if(flag_debug)
	{
	  cerr<<debug_spacer<<"| Distance graph: shortest_path_length="<<shortest_path_length<<endl;
	  cerr<<debug_spacer<<"| Distance graph (only length 0 edges): nb components="<<ncomponents<<endl;
	  cerr<<debug_spacer<<"| nrings="<<nrings<<endl;
	  cerr<<debug_spacer<<"| p="<<p<<endl;
	  cerr<<debug_spacer<<"| q="<<q<<endl;
	  cerr<<debug_spacer<<"| contribution to jones polynomial: "<<polynomial_result<<endl;
	}      
      if(flag_debug)
	{
	  cerr<<debug_spacer<<"end  recursion level="<<recursion_level<<endl;
	  recursion_level--;
	}
      //add back smoothed crossings to remaining_crossings
      for(int i=0;i<smoothed_crossings.size();i++)
	{
	  int n=smoothed_crossings[i];
	  remaining_crossings.insert(n);
	  crossing_type[n]=2;//only necessary if passed by reference
	  //assign length 2 to each edge n in graph_region
	  if(map_crossing_to_edge_1[n].second)
	    graph_regions[map_crossing_to_edge_1[n].first].length=2;
	  if(map_crossing_to_edge_2[n].second)
	    graph_regions[map_crossing_to_edge_2[n].first].length=2;
	}      
      return polynomial_result;
    }
  
  if(flag_debug)
    {
      cerr<<debug_spacer<<"| current crossing_type ";
      for(int i=0;i<crossing_type.size();i++)
	cerr<<crossing_type[i];
      cerr<<endl;
    }

  //choose next crossing & remove from remaining_crossings
  int n;
  set<int>::iterator it;
  if(method=="crossing_order")
    {
      //pick next crossing
      it=remaining_crossings.begin();
    }
  else if(method=="arc_order")
    {
      //find next crossing with lowest input arc
      int best_arc=-1,arc;
      for(set<int>::iterator it_tmp=remaining_crossings.begin();it_tmp!=remaining_crossings.end();it_tmp++)
	{
	  for(int i=0;i<4;i++)
	    {
	      arc=crossings[*it_tmp].get_arc(i);
	      if(arc>=0)
		{
		  if(best_arc<0||arc<best_arc)
		    {
		      best_arc=arc;
		      it=it_tmp;
		    }
		}
	    }
	}
    }
  else if(method=="default"||method=="region_order")
    {
      //find next crossing with lowest input arc
      //order by considering opposite regions for each crossings (2 pairs):
      //for pair 1: (min multiplicity between 2 regions), (sum multiplicy), (|sum sign|). Same for pair2
      vector<int> score_best;
      int c,r1,r2;
      it=remaining_crossings.begin();//just in case;
      for(set<int>::iterator it_tmp=remaining_crossings.begin();it_tmp!=remaining_crossings.end();it_tmp++)
	{
	  c=(*it_tmp);
	  if(crossings[c].arc0<0||crossings[c].arc1<0||crossings[c].arc2<0||crossings[c].arc3<0)//endpoint or empty
	    continue;
	  if(map_crossing_to_edge_1[c].second==false)
	    continue;
	  int pair=0;
	  int min_multiplicity1;
	  int sum_multiplicity1;
	  int sum_sign1;
	  r1=crossings[c].get_region((pair)%4,(pair+1)%4);
	  r2=crossings[c].get_region((pair+2)%4,(pair+3)%4);
	  min_multiplicity1=min(total_multiplicity[r1],total_multiplicity[r2]);
	  sum_multiplicity1=total_multiplicity[r1]+total_multiplicity[r2];
	  sum_sign1=fabs(total_sign[r1])+fabs(total_sign[r2]);
	  pair=1;
	  int min_multiplicity2;
	  int sum_multiplicity2;
	  int sum_sign2;
	  r1=crossings[c].get_region((pair)%4,(pair+1)%4);
	  r2=crossings[c].get_region((pair+2)%4,(pair+3)%4);
	  min_multiplicity2=min(total_multiplicity[r1],total_multiplicity[r2]);
	  sum_multiplicity2=total_multiplicity[r1]+total_multiplicity[r2];
	  sum_sign2=fabs(total_sign[r1])+fabs(total_sign[r2]);
	  //find best pair:
	  if(min_multiplicity2<min_multiplicity1||
	     (min_multiplicity2==min_multiplicity1&&sum_multiplicity2<sum_multiplicity1)||
	     (min_multiplicity2==min_multiplicity1&&sum_multiplicity2==sum_multiplicity1&&sum_sign2<sum_sign1))
	    {
	      int tmp=min_multiplicity1;
	      min_multiplicity1=min_multiplicity2;
	      min_multiplicity2=tmp;
	      tmp=sum_multiplicity1;
	      sum_multiplicity1=sum_multiplicity2;
	      sum_multiplicity2=tmp;
	      tmp=sum_sign1;
	      sum_sign1=sum_sign2;
	      sum_sign2=tmp;
	    }
	  if(score_best.size()==0)
	    {
	      score_best.push_back(min_multiplicity1);
	      score_best.push_back(sum_multiplicity1);
	      score_best.push_back(sum_sign1);
	      score_best.push_back(min_multiplicity2);
	      score_best.push_back(sum_multiplicity2);
	      score_best.push_back(sum_sign2);
	    }
	  if(score_best[0]>min_multiplicity1||
	     (score_best[0]==min_multiplicity1&&score_best[1]>sum_multiplicity1)||
	     (score_best[0]==min_multiplicity1&&score_best[1]==sum_multiplicity1&&score_best[2]>sum_sign1)||
	     (score_best[0]==min_multiplicity1&&score_best[1]==sum_multiplicity1&&score_best[2]==sum_sign1&&score_best[3]>min_multiplicity2)||
	     (score_best[0]==min_multiplicity1&&score_best[1]==sum_multiplicity1&&score_best[2]==sum_sign1&&score_best[3]==min_multiplicity2&&score_best[4]>sum_multiplicity2)||
	     (score_best[0]==min_multiplicity1&&score_best[1]==sum_multiplicity1&&score_best[2]==sum_sign1&&score_best[3]==min_multiplicity2&&score_best[4]==sum_multiplicity2&&score_best[5]>sum_sign2))
	    {
	      it=it_tmp;
	      score_best[0]=min_multiplicity1;
	      score_best[1]=sum_multiplicity1;
	      score_best[2]=sum_sign1;
	      score_best[3]=min_multiplicity2;
	      score_best[4]=sum_multiplicity2;
	      score_best[5]=sum_sign2;
	    }
	  if(flag_debug)
	    {
	      cerr<<" region-order: ("<<min_multiplicity1<<","<<sum_multiplicity1<<","<<sum_sign1<<","<<min_multiplicity2<<","<<sum_multiplicity2<<","<<sum_sign2<<")  best:";
	      cerr<<" best: ("<<score_best[0]<<","<<score_best[1]<<","<<score_best[2]<<","<<score_best[3]<<","<<score_best[4]<<","<<score_best[5]<<") (crossing "<<(*it_tmp)<<", best crossing "<<(*it)<<")"<<endl;
	    }
	}
    }
  else
    {
      cerr<<"ERROR PolynomialInvariant::jones_recursive(): unkown method "<<method<<endl;
      exit(1);
    }
  n=(*it);
  remaining_crossings.erase(it);
  smoothed_crossings.push_back(n);
  
  crossing_type[n]=0;
  //assign length to edge n in graph_region
  if(map_crossing_to_edge_1[n].second)
    graph_regions[map_crossing_to_edge_1[n].first].length=2*crossing_type[n];
  if(map_crossing_to_edge_2[n].second)
    graph_regions[map_crossing_to_edge_2[n].first].length=2-2*crossing_type[n];  
  polynomial_result.add_monomial(1,1,0);//A
  polynomial_result.multiply(jones_recursive(method,crossing_type,remaining_crossings,n));//A*jones_recursive(crossing_type=0)
  crossing_type[n]=1;
  //assign length to edge n in graph_region
  if(map_crossing_to_edge_1[n].second)
    graph_regions[map_crossing_to_edge_1[n].first].length=2*crossing_type[n];
  if(map_crossing_to_edge_2[n].second)
    graph_regions[map_crossing_to_edge_2[n].first].length=2-2*crossing_type[n];  
  polynomial_tmp.add_monomial(1,-1,0);//A^-1      
  polynomial_tmp.multiply(jones_recursive(method,crossing_type,remaining_crossings,n));//A^-1*jones_recursive(crossing_type=1)

  polynomial_result.add(polynomial_tmp);//A*jones_recursive(crossing_type=0)+A^-1*jones_recursive(crossing_type=1)
  //multiply by (-A^3)^total_typeI_removal
  if(total_typeI_removal>0)
    {
      Polynomial polynomial_A3;//-A^3
      polynomial_A3.add_monomial(-1,3,0);
      for(int i=0;i<total_typeI_removal;i++)
	polynomial_result.multiply(polynomial_A3);
    }
  else if(total_typeI_removal<0)
    {
      Polynomial polynomial_A3;//-A^-3
      polynomial_A3.add_monomial(-1,-3,0);
      for(int i=0;i<-total_typeI_removal;i++)
	polynomial_result.multiply(polynomial_A3);
    }


  //add back smoothed crossings to remaining_crossings
  for(int i=0;i<smoothed_crossings.size();i++)
    {
      int n=smoothed_crossings[i];
      remaining_crossings.insert(n);
      crossing_type[n]=2;//only necessary if passed by reference
      //assign length 2 to each edge n in graph_region
      if(map_crossing_to_edge_1[n].second)
	graph_regions[map_crossing_to_edge_1[n].first].length=2;
      if(map_crossing_to_edge_2[n].second)
	graph_regions[map_crossing_to_edge_2[n].first].length=2;
    }
  if(flag_debug)
    {
      cerr<<debug_spacer<<"end n="<<n<<" recursion level="<<recursion_level<<endl;
      recursion_level--;
    }
  return polynomial_result;

}

//---------------------------------------------------------------------------

void PolynomialInvariant::combination_first(vector<int> & bits)
{
  for(int i=0;i<bits.size();i++)
    bits[i]=0;
}

//---------------------------------------------------------------------------

bool PolynomialInvariant::combination_next(vector<int> & bits)
{
  int i1=0;
  int i2=bits.size()-1;
  if(!flag_cyclic)
    {
      i1=i1+1;
      i2=i2-1;
    }
  for(int i=i2;i>=i1;i--)
    {
      bits[i]=1-bits[i];
      if(bits[i]==1)
	{
	  return true;
	}
      else if(i==i1)
	{
	  return false;
	}
    }
  return true;
}


