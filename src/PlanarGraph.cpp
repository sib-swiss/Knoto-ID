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

#include "PlanarGraph.hh"


using namespace std;

//************************************************************
//**********************   PlanarGraph   **********************
//************************************************************

PlanarGraph::PlanarGraph(bool flag_cyclic,bool flag_debug):flag_cyclic(flag_cyclic),flag_debug(flag_debug)
{
    t0=time(NULL);

}
//---------------------------------------------------------------------------
void PlanarGraph::add_edge(string node1_name,string node1_label,bool node1_external,int z1,string node2_name,string node2_label,bool node2_external,int z2,int draw)
{
  int i1=add_node(node1_name,node1_label,node1_external);
  int i2=add_node(node2_name,node2_label,node2_external);
  edges_target_node[i1].push_back(i2);
  edges_z[i1].push_back(pair<int,int>(z1,z2));
  edges_draw[i1].push_back(draw);
  
}

//---------------------------------------------------------------------------
void PlanarGraph::triangulate()
{
  cerr<<"triangulating ("<<nodes_name.size()<<" nodes)"<<endl;
  int nb_nodes_added=0;
  string added_nodes_basename="dummy";

  if(nodes_name.size()==0)//empty diagram. creating ring or open segment
    {
      if(flag_cyclic)
	{
	  add_edge("Arc_0_0","Arc_0",true,0,"Arc_0_1","Arc_0",true,0,1);
	  add_edge("Arc_0_0","Arc_0",true,0,"Arc_0_2","Arc_0",true,0,-1);
	  add_edge("Arc_0_1","Arc_0",true,0,"Arc_0_2","Arc_0",true,0,2);
	  add_edge("Arc_0_1","Arc_0",true,0,"Arc_0_0","Arc_0",true,0,-1);
	  add_edge("Arc_0_2","Arc_0",true,0,"Arc_0_0","Arc_0",true,0,3);
	  add_edge("Arc_0_2","Arc_0",true,0,"Arc_0_1","Arc_0",true,0,-1);
	}
      else
	{
	  add_edge("Arc_0_0","Arc_0",true,0,"Arc_0_1","Arc_0",true,0,1);
	  add_edge("Arc_0_1","Arc_0",true,0,"Arc_0_0","Arc_0",true,0,-1);
	  add_edge("Arc_0_1","Arc_0",true,0,"Arc_0_2","Arc_0",true,0,2);
	  add_edge("Arc_0_2","Arc_0",true,0,"Arc_0_1","Arc_0",true,0,-1);
	}

    }
  //check all nodes have at least one outgoing edge:
  for(int n=0;n<nodes_name.size();n++)
    {
      if(edges_target_node[n].size()==0)
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR PlanarGraph::triangulate() isolated node."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
    }    

  //triangulate regions and add dummy exterior nodes
  bool flag_done=false;
  int nb_external_nodes=0;
  for(int n=0;n<nodes_external.size();n++)
    if(nodes_external[n])nb_external_nodes++;
  bool flag_dummy_external_nodes_added=false;
  int n_external=-1;//store the position of one external node (to avoid searching the full graph again and again...)
  while(!flag_done)
    {
      vector<vector<bool> > done(edges_target_node.size());//done[n][i]=true if region between outgoing edges edges_target_node[n][i] and edges_target_node[n][(i+1)%N] (N is edges_target_node[n].size() was tested, done[n][i]=false otherwise.
      for(int n=0;n<edges_target_node.size();n++)
	done[n].assign(edges_target_node[n].size(),false);
      int nb_external_regions=0;
      flag_done=true;
      for(int n=0;n<edges_target_node.size();n++)
	{
	  if(flag_debug)
	    cerr<<"n="<<n<<" name:"<<nodes_name[n]<<endl;
	  for(int i=0;i<edges_target_node[n].size();i++)
	    {
	      if(done[n][i])
		{
		  continue;
		}
	      bool flag_all_external=true;
	      //check triangle
	      int n1=n;
	      int i1_outgoing=i;
	      int i1_incoming=(i1_outgoing+1)%edges_target_node[n1].size();//i.e. next edge counter-clockwise
	      done[n1][i1_outgoing]=true;
	      if(!nodes_external[n1])flag_all_external=false;
	  
	      int n2=edges_target_node[n1][i1_outgoing];
	  
	      int i2_incoming=get_opposite_edge(n1,i1_outgoing);
	  
	      int i2_outgoing=(i2_incoming+edges_target_node[n2].size()-1)%edges_target_node[n2].size();//i.e. next edge clockwise
	      done[n2][i2_outgoing]=true;
	      if(!nodes_external[n2])flag_all_external=false;

	      int n3=edges_target_node[n2][i2_outgoing];
	      int i3_incoming=get_opposite_edge(n2,i2_outgoing);
	      int i3_outgoing=(i3_incoming+edges_target_node[n3].size()-1)%edges_target_node[n3].size();//i.e. next edge clockwise
	      done[n3][i3_outgoing]=true;
	      if(!nodes_external[n3])flag_all_external=false;

	      int n4=edges_target_node[n3][i3_outgoing];
	      int i4_incoming=get_opposite_edge(n3,i3_outgoing);
	      int i4_outgoing=(i4_incoming+edges_target_node[n4].size()-1)%edges_target_node[n4].size();//i.e. ndext edge clockwise
	      done[n4][i4_outgoing]=true;
	      if(!nodes_external[n4])flag_all_external=false;

	      if(n4==n1&&i1_incoming==i4_incoming)//triangle
		{
		  if(flag_debug)
		    cerr<<" => triangle  (all_external="<<flag_all_external<<")"<<endl;
		  if(!(flag_all_external&&nb_external_nodes==3))//not external region
		    continue;
		}

	      if(flag_debug)
		cerr<<" follow region"<<endl;
	      //follow region counterclockwise (clockwise for external region) 
	      int n0=n1;
	      int i0_incoming=i1_incoming;
	      int i0_outgoing=i1_outgoing;
	      vector<pair<int,int> > region_boundary_edges_list;//region_boundary_edges_list[m]=(n,i) add edge from node n, insert it after  edges_target_node[n][i]
	      map<int,int> node_count;//node_count[n]=m => node n appeared m times, to detect duplicates;
	      if(flag_debug)
		cerr<<"  n0="<<n0<<" name="<<nodes_name[n0]<<" i0_in="<<i0_incoming<<" i0_out="<<i0_outgoing<<endl;
	      while(true)
		{
		  n2=edges_target_node[n1][i1_outgoing];
		  i2_incoming=get_opposite_edge(n1,i1_outgoing);
		  i2_outgoing=(i2_incoming+edges_target_node[n2].size()-1)%edges_target_node[n2].size();//i.e. next edge clockwise
		  if(!nodes_external[n2])flag_all_external=false;
		  done[n2][i2_outgoing]=true;
		  region_boundary_edges_list.push_back(pair<int,int>(n2,i2_outgoing));
		  node_count[n2]++;
	      
		  if(n2==n0&&i2_outgoing==i0_outgoing)
		    break;
		  if(flag_debug)
		    cerr<<"  n2="<<n2<<" name="<<nodes_name[n2]<<" i2_in="<<i2_incoming<<" i2_out="<<i2_outgoing<<" all_external="<<flag_all_external<<endl;
	      
		  n1=n2;
		  i1_outgoing=i2_outgoing;
		}
	  
	      if(flag_all_external&&node_count.size()==nb_external_nodes)
		{
		  if(flag_debug)
		    cerr<<"  found external region: adding external nodes"<<endl;
		  nb_external_regions++;
		  if(nb_external_regions>1)
		    {
		      cerr<<"*********************************************************"<<endl;
		      cerr<<"ERROR PlanarGraph::triangulate() multiple external regions."<<endl;
		      cerr<<"*********************************************************"<<endl;
		      exit(1);
		    }
		  if(!flag_dummy_external_nodes_added)//add dummy external nodes only once
		    {
		      map<int,map<int,vector<int> > > edges_to_add;//edges_to_add[n1][i1]=vector of n2 => edges to add from n1 to n2 after position  edges_target_node[n1][i1]. 
		      int edges_to_add_first_n1=-1;
		      int edges_to_add_first_i1=-1;
		      map<pair<int,int>,int >::iterator it_index;
		      int newnode1;
		      int newnode2;
		      int newnode1_first=-1;
		      int newnode2_last=-1;
		      for(int m=0;m<region_boundary_edges_list.size();m++)
			{
			  n1=region_boundary_edges_list[m].first;
			  i1_outgoing=region_boundary_edges_list[m].second;
			  i1_incoming=(i1_outgoing+1)%edges_target_node[n1].size();//i.e. next edge counter clockwise

			  {

			  
			    int n_prev=edges_target_node[n1][i1_incoming];
			    int i_prev_outgoing=get_opposite_edge(n1,i1_incoming);
			    int n_next=edges_target_node[n1][i1_outgoing];
			    int i_next_incoming=get_opposite_edge(n1,i1_outgoing);
			    int i_next_outgoing=(i_next_incoming+edges_target_node[n_next].size()-1)%edges_target_node[n_next].size();//i.e. next edge clockwise
			    bool newnode1_new=false;
			    bool newnode2_new=false;
			    if(flag_debug)
			      cerr<<"    n_prev="<<nodes_name[n_prev]<<" n1="<<nodes_name[n1]<<" n_next="<<nodes_name[n_next]<<endl;
			    //newnode1 between n_prev and n1
			    if(m==0)//node does not exist, only first iteration
			      {
				stringstream target_node;
				target_node<<added_nodes_basename<<"_exterior"<<"_"<<nb_nodes_added;
				nb_nodes_added++;
				newnode1=add_node(target_node.str(),added_nodes_basename,true);//exterior
				nb_external_nodes++;
				done.push_back(vector<bool>());
				newnode1_first=newnode1;
				newnode1_new=true;
				n_external=newnode1;
				if(flag_debug)
				  cerr<<"     add newnode1 ="<<nodes_name[newnode1]<<endl;
			      }
			    else//newnode1 exists
			      {
				newnode1=newnode2_last;
			      }
			    //newnode2 between n1 and n_next
			    if(m==region_boundary_edges_list.size()-1)//last iteration, node exists
			      {
				newnode2=newnode1_first;
			      }
			    else//newnode2 does not exist
			      {
				stringstream target_node;
				target_node<<added_nodes_basename<<"_exterior"<<"_"<<nb_nodes_added;
				nb_nodes_added++;
				newnode2=add_node(target_node.str(),added_nodes_basename,true);//exterior
				nb_external_nodes++;
				done.push_back(vector<bool>());
				newnode2_new=true;
				if(flag_debug)
				  cerr<<"     add newnode2 ="<<nodes_name[newnode2]<<endl;
			      }
			    newnode2_last=newnode2;

			    if(newnode1_new)
			      {
				//to add later: edge from n_prev to newnode1 (insert after i_prev_outgoing)
				edges_to_add[n_prev][i_prev_outgoing].push_back(newnode1);
				if( edges_to_add_first_n1<0)
				  {
				    edges_to_add_first_n1=n_prev;
				    edges_to_add_first_i1=i_prev_outgoing;
				  }
				//add opposite edge from newnode1 to n_prev
				edges_target_node[newnode1].push_back(n_prev);
				edges_z[newnode1].push_back(pair<int,int>(0,0));
				edges_draw[newnode1].push_back(-1);
				done[newnode1].push_back(true);

				//to add later: edge from n1 to newnode1 (insert after i1_outgoing)
				edges_to_add[n1][i1_outgoing].push_back(newnode1);
				//add opposite edge from newnode1 to n1
				edges_target_node[newnode1].push_back(n1);
				edges_z[newnode1].push_back(pair<int,int>(0,0));
				edges_draw[newnode1].push_back(-1);
				done[newnode1].push_back(true);
			      }
			    //to add later: edge from newnode1 to newnode2 (insert in last position)
			    edges_target_node[newnode1].push_back(newnode2);
			    edges_z[newnode1].push_back(pair<int,int>(0,0));
			    edges_draw[newnode1].push_back(-1);
			    done[newnode1].push_back(true);
			    //add opposite edge from newnode2 to newnode1 
			    edges_target_node[newnode2].push_back(newnode1);
			    edges_z[newnode2].push_back(pair<int,int>(0,0));
			    edges_draw[newnode2].push_back(-1);
			    done[newnode2].push_back(true);
			    if(newnode2_new)
			      {
				//to add later: edge from n1 to newnode2 (insert after i1_outgoing)
				edges_to_add[n1][i1_outgoing].push_back(newnode2);
				if( edges_to_add_first_n1<0)
				  {
				    edges_to_add_first_n1=n1;
				    edges_to_add_first_i1=i1_outgoing;
				  }
				//add opposite edge from newnode2 to n1
				edges_target_node[newnode2].push_back(n1);
				edges_z[newnode2].push_back(pair<int,int>(0,0));
				edges_draw[newnode2].push_back(-1);
				done[newnode2].push_back(true);

				//to add later: edge from n_next to newnode2 (insert after i_next_outgoing)
				edges_to_add[n_next][i_next_outgoing].push_back(newnode2);
				//add opposite edge from newnode2 to n_next
				edges_target_node[newnode2].push_back(n_next);
				edges_z[newnode2].push_back(pair<int,int>(0,0));
				edges_draw[newnode2].push_back(-1);
				done[newnode2].push_back(true);

			      }
			    if(nodes_external[n1])
			      {
				nodes_external[n1]=false;
				nb_external_nodes--;
			      }
			  }
			}
		      
		      if(edges_to_add_first_n1>=0)
			{
			  //move first element of edges_to_add[edges_to_add_first_n1][edges_to_add_first_i1] to last position 
			  edges_to_add[edges_to_add_first_n1][edges_to_add_first_i1].push_back(edges_to_add[edges_to_add_first_n1][edges_to_add_first_i1][0]);
			  edges_to_add[edges_to_add_first_n1][edges_to_add_first_i1].erase(edges_to_add[edges_to_add_first_n1][edges_to_add_first_i1].begin());
			}
		  
		      //add missing edges (order is important)
		      int n1,i1,n2;
		      for(map<int,map<int,vector<int> > >::iterator it1=edges_to_add.begin();it1!=edges_to_add.end();it1++)
			{
			  n1=it1->first;
			  for(map<int,vector<int> >::reverse_iterator it2=it1->second.rbegin();it2!=it1->second.rend();it2++)
			    {
			      i1=it2->first;
			      for(int l=0;l<it2->second.size();l++)
				{
				  n2=it2->second[l];
				  //add edges from n1 to n2 (insert after i1)
				  edges_target_node[n1].insert(edges_target_node[n1].begin()+i1+1, n2);
				  edges_z[n1].insert(edges_z[n1].begin()+i1+1,pair<int,int>(0,0));
				  edges_draw[n1].insert(edges_draw[n1].begin()+i1+1,-1);
				  done[n1].insert(done[n1].begin()+i1+1,true);
				}
			    }
			}
		      flag_dummy_external_nodes_added=true;
		    }
		}
	      else//internal region: add dummy node and edges to triangulate
		{
		  stringstream target_node;
		  target_node<<added_nodes_basename<<"_"<<nb_nodes_added;
		  nb_nodes_added++;
		  int newnode=add_node(target_node.str(),added_nodes_basename,false);
		  if(flag_debug)
		    cerr<<"   add internal node: "<<nodes_name[newnode]<<endl;
		  done.push_back(vector<bool>());
		  for(int m=0;m<region_boundary_edges_list.size();m++)
		    {
		      n1=region_boundary_edges_list[m].first;
		      i1_outgoing=region_boundary_edges_list[m].second;
		      i1_incoming=(i1_outgoing+1)%edges_target_node[n1].size();
		      if(node_count[n1]==1)
			{
			  if(flag_debug)
			    cerr<<"    add internal edge: "<<nodes_name[n1]<<"  n1="<<n1<<" i1_incoming="<<i1_incoming<<endl;
			  //add edges from n1
			  edges_target_node[n1].insert(edges_target_node[n1].begin()+i1_incoming, newnode);
			  edges_z[n1].insert(edges_z[n1].begin()+i1_incoming,pair<int,int>(0,0));
			  edges_draw[n1].insert(edges_draw[n1].begin()+i1_incoming,-1);
			  done[n1].insert(done[n1].begin()+i1_incoming,true);
		      
			  //add edges from newnode
			  edges_target_node[newnode].push_back(n1);
			  edges_z[newnode].push_back(pair<int,int>(0,0));
			  edges_draw[newnode].push_back(-1);
			  done[newnode].push_back(true);
			}
		      else
			{
			  if(flag_debug)
			    cerr<<"    IGNORE eddge: "<<nodes_name[n1]<<"  n1="<<n1<<" i1_incoming="<<i1_incoming<<endl;
			  //TODO:  save specific regions to triangulate (=> no need to parse the full graph again)
			  flag_done=false;
				
			}
		    
		    }
		  if(flag_debug)
		    cerr<<" done"<<endl;
		}
	  
	    }
	}
      if(nb_external_regions==0)
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR PlanarGraph::triangulate() external region missing."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      if(nb_external_regions>1)
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR PlanarGraph::triangulate() multiple external regions."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
    }
  cerr<<"done ("<<nodes_name.size()<<" nodes)"<<endl;


  if(flag_debug)
    cerr<<"searching new external boundary"<<endl;
  //find external region to set edges_external;
  edges_external=vector<vector<bool> >(edges_target_node.size()); //edges_external[n][i]=true => region between  edges_target_node[n][i] and  edges_target_node[n][(i+1)%N] is exterior
  for(int n0=0;n0<edges_target_node.size();n0++)
      edges_external[n0].assign(edges_target_node[n0].size(),false);

  bool flag_all_external=false;
  int n1,i1_outgoing,i1_incoming;
  int n2,i2_outgoing,i2_incoming;
  int n0=n_external;

  if(nodes_external[n0])
    {
      for(int i0_outgoing=0;i0_outgoing<edges_target_node[n0].size()&&!flag_all_external;i0_outgoing++)
	{
	  //follow external region clockwise
	  n1=n0;
	  i1_outgoing=i0_outgoing;
	  i1_incoming=(i1_outgoing+1)%edges_target_node[n1].size();//next edge counter clockwise
	  set<int> nodes;
	  flag_all_external=true;
	  if(flag_debug)
	    cerr<<" n1:"<<nodes_name[n1]<<" i1_in="<<i1_incoming<<" ("<<nodes_name[edges_target_node[n1][i1_incoming]]<<")"<<" i1_out="<<i1_outgoing<<" ("<<nodes_name[edges_target_node[n1][i1_outgoing]]<<")"<<endl;
	  while(true)
	    {
	      n2=edges_target_node[n1][i1_outgoing];
	      i2_incoming=get_opposite_edge(n1,i1_outgoing);
	      i2_outgoing=(i2_incoming+edges_target_node[n2].size()-1)%edges_target_node[n2].size();//next edge clockwise
	      if(flag_debug)
		cerr<<" n2:"<<nodes_name[n2]<<" i2_in="<<i2_incoming<<" ("<<nodes_name[edges_target_node[n2][i2_incoming]]<<")"<<" i2_out="<<i2_outgoing<<" ("<<nodes_name[edges_target_node[n2][i2_outgoing]]<<")"<<endl;
	      nodes.insert(n2);
	      if(!nodes_external[n2])
		{
		  flag_all_external=false;
		  break;
		}
	      if(n2==n0&&i2_outgoing==i0_outgoing)
		break;		  
	      n1=n2;
	      i1_outgoing=i2_outgoing;
	    }
	  if(flag_all_external&&nodes.size()==nb_external_nodes)//found the extrior => set edges_external to true for all edges
	    {
	      n1=n0;
	      i1_outgoing=i0_outgoing;
	      flag_all_external=true;
	      while(true)
		{
		  if(flag_debug)
		    cerr<<" external: "<<nodes_name[n1]<<" i1_out="<<i1_outgoing<<"  (i.e. -> "<<nodes_name[edges_target_node[n1][i1_outgoing]]<<")"<<endl;
		  edges_external[n1][i1_outgoing]=true;
		  n2=edges_target_node[n1][i1_outgoing];
		  i2_incoming=get_opposite_edge(n1,i1_outgoing);
		  i2_outgoing=(i2_incoming+edges_target_node[n2].size()-1)%edges_target_node[n2].size();//i.e. ~i2_incoming-1 modulo nb edges.
		  if(n2==n0&&i2_outgoing==i0_outgoing)
		    break;		  
		  n1=n2;
		  i1_outgoing=i2_outgoing;
		}
	    }
	  else
	    flag_all_external=false;
	}
    }


  
  //////check that all regions (except exterior) are triangles (and with correct edge order)  
  cerr<<"checking topology"<<endl;
  //check all regions are triangles (except exterior)
  vector<vector<bool> > done(edges_target_node.size());//done[n][i]=true if region between outgoing edges edges_target_node[n][i] and edges_target_node[n][(i+1)%N] (N is edges_target_node[n].size() was tested, done[n][i]=false otherwise.
  for(int n=0;n<edges_target_node.size();n++)
    done[n].assign(edges_target_node[n].size(),false);
  for(int n=0;n<edges_target_node.size();n++)
    {
      if(flag_debug)
	cerr<<" n="<<n<<" name:"<<nodes_name[n]<<endl;
      for(int i=0;i<edges_target_node[n].size();i++)
	    {
	      if(edges_external[n][i])
		continue;//external region

	      if(done[n][i])
		{
		  continue;
		}


	      //check triangle
	      int n1=n;
	      int i1_outgoing=i;
	      int i1_incoming=(i1_outgoing+1)%edges_target_node[n1].size();//i.e. ~i1_outgoing+1 modulo nb edges.;
	      done[n1][i1_outgoing]=true;
	  
	      int n2=edges_target_node[n1][i1_outgoing];	  
	      int i2_incoming=get_opposite_edge(n1,i1_outgoing);//=edges_opposite[n1][i1_outgoing].second;	  
	      int i2_outgoing=(i2_incoming+edges_target_node[n2].size()-1)%edges_target_node[n2].size();//i.e. ~i2_incoming-1 modulo nb edges.
	      done[n2][i2_outgoing]=true;

	      int n3=edges_target_node[n2][i2_outgoing];
	      int i3_incoming=get_opposite_edge(n2,i2_outgoing);//edges_opposite[n2][i2_outgoing].second;
	      int i3_outgoing=(i3_incoming+edges_target_node[n3].size()-1)%edges_target_node[n3].size();//i.e. ~i3_incoming-1 modulo nb edges.
	      done[n3][i3_outgoing]=true;

	      int n4=edges_target_node[n3][i3_outgoing];
	      int i4_incoming=get_opposite_edge(n3,i3_outgoing);//edges_opposite[n3][i3_outgoing].second;
	      int i4_outgoing=(i4_incoming+edges_target_node[n4].size()-1)%edges_target_node[n4].size();//i.e. ~i4_incoming-1 modulo nb edges.
	      done[n4][i4_outgoing]=true;

	      if(n4!=n1||i1_incoming!=i4_incoming)//triangle
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR PlanarGraph::triangulate() not a triangle."<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);
		}
	    }
    }
  if(flag_debug)
    cerr<<"done"<<endl;

  
}


//---------------------------------------------------------------------------
void PlanarGraph::save_to_file_xyz(string filename,int smooth,long nb_iterations_relaxation,double distance_ratio_threshold,double temperature_start,double temperature_end,bool check_balanced,bool debug_output)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_xyz(cout,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,debug_output);
  else if(filename=="stderr")
    save_to_file_xyz(cerr,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,debug_output);
  else
    {
      ofstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR PlanarGraph::save_to_file_xyz() problem with file "<<filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      save_to_file_xyz(file,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,debug_output);
      file.close();
    }
}

//---------------------------------------------------------------------------


void PlanarGraph::save_to_file_xyz(ostream& file,int smooth,long nb_iterations_relaxation,double distance_ratio_threshold,double temperature_start,double temperature_end,bool check_balanced,bool debug_output)
{
  streamsize precisiontmp=file.precision();
  file.precision(15);
  file<<"#x y z label"<<endl;
  if(nodes_name.size()==0)
    return;

  circle_packing();

  if(nb_iterations_relaxation>0)
    relax(nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end);

  
  int min_draw_order=-1,max_draw_order=-1;
  map<int,pair<int,int > > ordered_edges_to_draw;//ordered_edges_to_draw[i]=(n1,l) edge with edges_draw=i connects ndoes_name[n1] to nodes_name[n2] where  edges_target_node[n1][l]=n2 and edges_draw[n1][l]=i). ERROR if more than one edge with same edges_draw value.
  int i,n2;
  double distance_max=0,distance_min=-1;
  double distance;
  for(int n1=0;n1<edges_draw.size();n1++)
    for(int l=0;l<edges_draw[n1].size();l++)
      {
	i=edges_draw[n1][l];
	if(i>=0)
	  {
	    if(i<min_draw_order||min_draw_order<0)
	      min_draw_order=i;
	    if(i>max_draw_order)
	      max_draw_order=i;
	    if(ordered_edges_to_draw.find(i)!=ordered_edges_to_draw.end())
	      {
		cerr<<"*********************************************************"<<endl;
		cerr<<"ERROR PlanarGraph::save_to_file_xyz(): multiple edges with same drawing order."<<endl;
		cerr<<"*********************************************************"<<endl;
		exit(1);
	      }

	    n2=edges_target_node[n1][l];
	    ordered_edges_to_draw[i]=pair<int,int>(n1,l);

	    distance=sqrt((nodes_x[n1]-nodes_x[n2])*(nodes_x[n1]-nodes_x[n2])+(nodes_y[n1]-nodes_y[n2])*(nodes_y[n1]-nodes_y[n2]));
	    if(distance>distance_max)
	      distance_max=distance;
	    if(distance_min<0||distance<distance_min)
	      distance_min=distance;

	  }
      }
  double distance_ratio_threshold_error=min(0.05,distance_ratio_threshold);
  if(distance_min/distance_max<distance_ratio_threshold_error)
    {
      cerr<<"*********************************************************"<<endl;
      if(check_balanced)
	cerr<<"ERROR: ";
      else
	cerr<<"WARNING: ";
      cerr<<"Unbalanced circle packing."<<endl;
      if(flag_debug)cerr<<"(min distance/max distance="<<distance_min/distance_max<<")."<<endl;
      if(nb_iterations_relaxation>0)
	cerr<<"Try to increase --nb-iterations-relaxation."<<endl;
      else
	cerr<<"Use option --nb-iterations-relaxation to relax the knot(oid) diagram."<<endl;
      if(check_balanced)
	cerr<<"Or use option --force to save the unbalanced diagram."<<endl;
      cerr<<"*********************************************************"<<endl;
      if(check_balanced)
	exit(1);
    }

  //generate control points
  vector<double> control_points_x1;
  vector<double> control_points_x2;
  vector<double> control_points_x3;
  vector<double> control_points_y1;
  vector<double> control_points_y2;
  vector<double> control_points_y3;
  vector<double> control_points_z1;  
  vector<double> control_points_z2;  
  vector<double> control_points_z3;  
  vector<string> labels;  
  int last_n1=-1;
  int last_n2=-1;
  int last_l=-1;
  int n1;
  bool flag_first=true;
  for(map<int,pair<int,int>  >::iterator it=ordered_edges_to_draw.begin();it!=ordered_edges_to_draw.end();it++)
      {
	double x1,y1,z1;
	double x2,y2,z2;
	double x3,y3,z3;
	double x4,y4;
	n1=it->second.first;
	int l=it->second.second;
	n2=edges_target_node[n1][l];
	if(last_n1>=0&&last_n2>=0)
	  {
	    if(last_n2==n1)//output (last_n1,n1,n2)
	      {
		if(flag_first&&!flag_cyclic)//add segment from first point (last_n1, n1)
		  {
		    x1=nodes_x[last_n1];
		    y1=nodes_y[last_n1];
		    distance=sqrt((nodes_x[last_n1]-nodes_x[n1])*(nodes_x[last_n1]-nodes_x[n1])+(nodes_y[last_n1]-nodes_y[n1])*(nodes_y[last_n1]-nodes_y[n1]));
		    x2=nodes_x[last_n1]+nodes_r[last_n1]*(nodes_x[n1]-nodes_x[last_n1])/distance;
		    y2=nodes_y[last_n1]+nodes_r[last_n1]*(nodes_y[n1]-nodes_y[last_n1])/distance;

		    z1=edges_z[last_n1][last_l].first;
		    //choose the z with min |z|
		    if(fabs(edges_z[last_n1][last_l].first)<fabs(edges_z[last_n1][last_l].second))
		      z2=edges_z[last_n1][last_l].first;
		    else
		      z2=edges_z[last_n1][last_l].second;
		    control_points_x1.push_back(x1);
		    control_points_x2.push_back((x1+x2)/2.0);
		    control_points_x3.push_back(x2);
		    control_points_y1.push_back(y1);
		    control_points_y2.push_back((y1+y2)/2.0);
		    control_points_y3.push_back(y2);
		    control_points_z1.push_back(z1);
		    control_points_z2.push_back((z1+z2)/2.0);
		    control_points_z3.push_back(z2);
		    labels.push_back(nodes_label[last_n1]);
		    if(distance>(1+1e-3)*(nodes_r[n1]+nodes_r[last_n1]))//disk not in contact => add control points to join them
		      {
			x4=nodes_x[n1]+nodes_r[n1]*(nodes_x[last_n1]-nodes_x[n1])/distance;
			y4=nodes_y[n1]+nodes_r[n1]*(nodes_y[last_n1]-nodes_y[n1])/distance;
			control_points_x1.push_back(x2);
			control_points_x2.push_back((x2+x4)/2.0);
			control_points_x3.push_back(x4);
			control_points_y1.push_back(y2);
			control_points_y2.push_back((y2+y4)/2.0);
			control_points_y3.push_back(y4);
			control_points_z1.push_back(z2);
			control_points_z2.push_back(z2);
			control_points_z3.push_back(z2);
			if(nodes_label[n1]==nodes_label[last_n1])
			  labels.push_back(nodes_label[n1]);
			else
			  labels.push_back("junction-"+nodes_label[last_n1]+"-"+nodes_label[n1]);
		      }
		  }
		// (last_n1,n1,n2)
		distance=sqrt((nodes_x[last_n1]-nodes_x[n1])*(nodes_x[last_n1]-nodes_x[n1])+(nodes_y[last_n1]-nodes_y[n1])*(nodes_y[last_n1]-nodes_y[n1]));
		x1=nodes_x[n1]+nodes_r[n1]*(nodes_x[last_n1]-nodes_x[n1])/distance;
		y1=nodes_y[n1]+nodes_r[n1]*(nodes_y[last_n1]-nodes_y[n1])/distance;
		//x1=nodes_x[last_n1]+nodes_r[last_n1]*(nodes_x[n1]-nodes_x[last_n1])/distance;
		//y1=nodes_y[last_n1]+nodes_r[last_n1]*(nodes_y[n1]-nodes_y[last_n1])/distance;

		x2=nodes_x[n1];
		y2=nodes_y[n1];
		
		distance=sqrt((nodes_x[n2]-nodes_x[n1])*(nodes_x[n2]-nodes_x[n1])+(nodes_y[n2]-nodes_y[n1])*(nodes_y[n2]-nodes_y[n1]));
		x3=nodes_x[n1]+nodes_r[n1]*(nodes_x[n2]-nodes_x[n1])/distance;
		y3=nodes_y[n1]+nodes_r[n1]*(nodes_y[n2]-nodes_y[n1])/distance;
		//x3=nodes_x[n2]+nodes_r[n2]*(nodes_x[n1]-nodes_x[n2])/(nodes_r[n2]+nodes_r[n1]);
		//y3=nodes_y[n2]+nodes_r[n2]*(nodes_y[n1]-nodes_y[n2])/(nodes_r[n2]+nodes_r[n1]);

		//choose the z with min |z|
		if(fabs(edges_z[last_n1][last_l].first)<fabs(edges_z[last_n1][last_l].second))
		  z1=edges_z[last_n1][last_l].first;
		else
		  z1=edges_z[last_n1][last_l].second;
		z2=edges_z[n1][l].first;
		//choose the z with min |z|
		if(fabs(edges_z[n1][l].first)<fabs(edges_z[n1][l].second))
		  z3=edges_z[n1][l].first;
		else
		  z3=edges_z[n1][l].second;

		control_points_x1.push_back(x1);
		control_points_x2.push_back(x2);
		control_points_x3.push_back(x3);
		control_points_y1.push_back(y1);
		control_points_y2.push_back(y2);
		control_points_y3.push_back(y3);
		control_points_z1.push_back(z1);
		control_points_z2.push_back(z2);
		control_points_z3.push_back(z3);
		labels.push_back(nodes_label[n1]);
		if(distance>(1+1e-3)*(nodes_r[n2]+nodes_r[n1]))//disk not in contact => add control points to join them
		  {
		    x4=nodes_x[n2]+nodes_r[n2]*(nodes_x[n1]-nodes_x[n2])/distance;
		    y4=nodes_y[n2]+nodes_r[n2]*(nodes_y[n1]-nodes_y[n2])/distance;
		    control_points_x1.push_back(x3);
		    control_points_x2.push_back((x3+x4)/2.0);
		    control_points_x3.push_back(x4);
		    control_points_y1.push_back(y3);
		    control_points_y2.push_back((y3+y4)/2.0);
		    control_points_y3.push_back(y4);
		    control_points_z1.push_back(z3);
		    control_points_z2.push_back(z3);
		    control_points_z3.push_back(z3);
		    if(nodes_label[n1]==nodes_label[last_n1])
		      labels.push_back(nodes_label[n1]);
		    else
		      labels.push_back("junction-"+nodes_label[n1]+"-"+nodes_label[n2]);
		  }

		if(it->first==max_draw_order&&!flag_cyclic)//add segment for last point (n1, n2)
		  {
		    distance=sqrt((nodes_x[n2]-nodes_x[n1])*(nodes_x[n2]-nodes_x[n1])+(nodes_y[n2]-nodes_y[n1])*(nodes_y[n2]-nodes_y[n1]));
		    x2=nodes_x[n2]+nodes_r[n2]*(nodes_x[n1]-nodes_x[n2])/distance;
		    y2=nodes_y[n2]+nodes_r[n2]*(nodes_y[n1]-nodes_y[n2])/distance;
		    
		    x3=nodes_x[n2];
		    y3=nodes_y[n2];

		    //choose the z with min |z|
		    if(fabs(edges_z[n1][l].first)<fabs(edges_z[n1][l].second))
		      z2=edges_z[n1][l].first;
		    else
		      z2=edges_z[n1][l].second;
		    z3=edges_z[n1][l].second;
		    
		    if(distance>(1+1e-3)*(nodes_r[n2]+nodes_r[n1]))//disk not in contact => add control points to join them
		      {
			x1=nodes_x[n1]+nodes_r[n1]*(nodes_x[n2]-nodes_x[n1])/distance;
			y1=nodes_y[n1]+nodes_r[n1]*(nodes_y[n2]-nodes_y[n1])/distance;
			control_points_x1.push_back(x1);
			control_points_x2.push_back((x1+x1)/2.0);
			control_points_x3.push_back(x1);
			control_points_y1.push_back(y1);
			control_points_y2.push_back((y1+y1)/2.0);
			control_points_y3.push_back(y1);
			control_points_z1.push_back(z2);
			control_points_z2.push_back(z2);
			control_points_z3.push_back(z2);
			if(nodes_label[n1]==nodes_label[last_n1])
			  labels.push_back(nodes_label[n1]);
			else
			  labels.push_back("junction-"+nodes_label[n1]+"-"+nodes_label[n2]);
		      }
		    control_points_x1.push_back(x2);
		    control_points_x2.push_back((x2+x3)/2.0);
		    control_points_x3.push_back(x3);
		    control_points_y1.push_back(y2);
		    control_points_y2.push_back((y2+y3)/2.0);
		    control_points_y3.push_back(y3);
		    control_points_z1.push_back(z2);
		    control_points_z2.push_back((z2+z3)/2.0);
		    control_points_z3.push_back(z3);
		    labels.push_back(nodes_label[n2]);

		  }
	      }
	    else
	      {
		cerr<<"*********************************************************"<<endl;
		cerr<<"ERROR: PlanarGraph::save_to_file_xyz(): last_n2!=n1."<<endl;
		cerr<<"*********************************************************"<<endl;
		exit(1);
	      }
	    flag_first=false;		
	  }
	last_n1=n1;
  	last_n2=n2;
	last_l=l;
      }
  if(flag_cyclic)
    {
      n1=ordered_edges_to_draw[min_draw_order].first;
      int l=ordered_edges_to_draw[min_draw_order].second;
      n2=edges_target_node[n1][l];
      if(last_n2==n1)//output (last_n1,n1,n2)
	{
	  double x1,y1,z1;
	  double x2,y2,z2;
	  double x3,y3,z3;
	  double x4,y4;
	  double distance;
	  distance=sqrt((nodes_x[last_n1]-nodes_x[n1])*(nodes_x[last_n1]-nodes_x[n1])+(nodes_y[last_n1]-nodes_y[n1])*(nodes_y[last_n1]-nodes_y[n1]));
	  x1=nodes_x[n1]+nodes_r[n1]*(nodes_x[last_n1]-nodes_x[n1])/distance;
	  y1=nodes_y[n1]+nodes_r[n1]*(nodes_y[last_n1]-nodes_y[n1])/distance;
	  //x1=nodes_x[last_n1]+nodes_r[last_n1]*(nodes_x[n1]-nodes_x[last_n1])/(nodes_r[last_n1]+nodes_r[n1]);
	  //y1=nodes_y[last_n1]+nodes_r[last_n1]*(nodes_y[n1]-nodes_y[last_n1])/(nodes_r[last_n1]+nodes_r[n1]);

	  x2=nodes_x[n1];
	  y2=nodes_y[n1];

	  distance=sqrt((nodes_x[n2]-nodes_x[n1])*(nodes_x[n2]-nodes_x[n1])+(nodes_y[n2]-nodes_y[n1])*(nodes_y[n2]-nodes_y[n1]));
	  x3=nodes_x[n1]+nodes_r[n1]*(nodes_x[n2]-nodes_x[n1])/distance;
	  y3=nodes_y[n1]+nodes_r[n1]*(nodes_y[n2]-nodes_y[n1])/distance;
	  //x3=nodes_x[n2]+nodes_r[n2]*(nodes_x[n1]-nodes_x[n2])/(nodes_r[n2]+nodes_r[n1]);
	  //y3=nodes_y[n2]+nodes_r[n2]*(nodes_y[n1]-nodes_y[n2])/(nodes_r[n2]+nodes_r[n1]);


	  //choose the z with min |z|
	  if(fabs(edges_z[last_n1][last_l].first)<fabs(edges_z[last_n1][last_l].second))
	    z1=edges_z[last_n1][last_l].first;
	  else
	    z1=edges_z[last_n1][last_l].second;
	  z2=edges_z[n1][l].first;
	  //choose the z with min |z|
	  if(fabs(edges_z[n1][l].first)<fabs(edges_z[n1][l].second))
	    z3=edges_z[n1][l].first;
	  else
	    z3=edges_z[n1][l].second;

	  control_points_x1.push_back(x1);
	  control_points_x2.push_back(x2);
	  control_points_x3.push_back(x3);
	  control_points_y1.push_back(y1);
	  control_points_y2.push_back(y2);
	  control_points_y3.push_back(y3);
	  control_points_z1.push_back(z1);
	  control_points_z2.push_back(z2);
	  control_points_z3.push_back(z3);
	  labels.push_back(nodes_label[n1]);
	  if(distance>(1+1e-3)*(nodes_r[n2]+nodes_r[n1]))//disk not in contact => add control points to join them
	    {
	      x4=nodes_x[n2]+nodes_r[n2]*(nodes_x[n1]-nodes_x[n2])/distance;
	      y4=nodes_y[n2]+nodes_r[n2]*(nodes_y[n1]-nodes_y[n2])/distance;
	      control_points_x1.push_back(x3);
	      control_points_x2.push_back((x3+x4)/2.0);
	      control_points_x3.push_back(x4);
	      control_points_y1.push_back(y3);
	      control_points_y2.push_back((y3+y4)/2.0);
	      control_points_y3.push_back(y4);
	      control_points_z1.push_back(z3);
	      control_points_z2.push_back(z3);
	      control_points_z3.push_back(z3);
	      if(nodes_label[n1]==nodes_label[n2])
		labels.push_back(nodes_label[n1]);
	      else
		labels.push_back("junction-"+nodes_label[n1]+"-"+nodes_label[n2]);
	    }
	  
	}
      else
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR: PlanarGraph::save_to_file_xyz(): last_n2!=n1."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
    }

  //smooth z control points
  int last_extremum_l=-1;
  int last_extremum_value=0;
  int first_extremum_l=-1;
  int first_extremum_value=0;
  int value=0;
  for(int l=0;l<control_points_x1.size();l++)
    {
      //we only consider x2,y2,z2 wich are the only control points placed on the crossings and can reach z=+1 or -1.
      if(control_points_z2[l]>=1-ZERO||control_points_z2[l]<=-1+ZERO)
	{
	  if(control_points_z2[l]>0)
	    value=1;
	  else
	    value=-1;	  

	  if(last_extremum_l<0)
	    {
	      first_extremum_l=l;
	      first_extremum_value=value;	      
	    }
	  if(last_extremum_l<0&&!flag_cyclic)//first extremum, set all previous z to extremum value
	    {
	      control_points_z1[l]=value;
	      for(int l2=0;l2<l;l2++)
		{
		  control_points_z1[l2]=value;
		  control_points_z2[l2]=value;
		  control_points_z3[l2]=value;
		}	      
	    }
	  else if(last_extremum_l>=0&&last_extremum_value==value)//flatten
	    {
	      control_points_z3[last_extremum_l]=value;
	      for(int l2=last_extremum_l+1;l2<l;l2++)
		{
		  control_points_z1[l2]=value;
		  control_points_z2[l2]=value;
		  control_points_z3[l2]=value;
		}	      
	      control_points_z1[l]=value;
	    }
	  else if(last_extremum_l>=0&&last_extremum_value!=value)//smooth
	    {
	      vector<double> tmp=get_spline(last_extremum_value,(last_extremum_value+value)/2.0,value,2*(l-last_extremum_l)-1);	      
	      control_points_z3[last_extremum_l]=tmp[1];
	      for(int l2=last_extremum_l+1;l2<l;l2++)
		{
		  control_points_z1[l2]=tmp[2*(l2-last_extremum_l)-1];
		  control_points_z2[l2]=tmp[2*(l2-last_extremum_l)];
		  control_points_z3[l2]=tmp[2*(l2-last_extremum_l)+1];
		  
		}
	      control_points_z1[l]=tmp[2*(l-last_extremum_l)-1];	      
	    }
	  last_extremum_l=l;
	  last_extremum_value=value;

	}

      
    }
  if(flag_cyclic)//consider last_extremum -> first_extremum
    {
      if(last_extremum_l>=0&&last_extremum_value==first_extremum_value)//flatten 
	{
	  control_points_z3[last_extremum_l]=first_extremum_value;
	  for(int l2=last_extremum_l+1;l2<first_extremum_l+control_points_z1.size();l2++)
	    {
	      control_points_z1[l2%control_points_z1.size()]=first_extremum_value;
	      control_points_z2[l2%control_points_z2.size()]=first_extremum_value;
	      control_points_z3[l2%control_points_z3.size()]=first_extremum_value;
	    }	      
	  control_points_z1[first_extremum_l]=first_extremum_value;

	}
      else if(last_extremum_l>=0&&last_extremum_value!=first_extremum_value)//smooth
	{
	  vector<double> tmp=get_spline(last_extremum_value,(last_extremum_value+first_extremum_value)/2.0,first_extremum_value,2*(first_extremum_l+control_points_z1.size()-last_extremum_l)-1);	      
	  control_points_z3[last_extremum_l]=tmp[1];
	  for(int l2=last_extremum_l+1;l2<first_extremum_l+control_points_z1.size();l2++)
	    {
	      control_points_z1[l2%control_points_z1.size()]=tmp[2*(l2-last_extremum_l)-1];
	      control_points_z2[l2%control_points_z1.size()]=tmp[2*(l2-last_extremum_l)];
	      control_points_z3[l2%control_points_z1.size()]=tmp[2*(l2-last_extremum_l)+1];
	      
	    }
	  control_points_z1[first_extremum_l]=tmp[2*(first_extremum_l+control_points_z1.size()-last_extremum_l)-1];	      
	}      
    }
  else//last endpoint
    {
      if(last_extremum_l>=0)
	{
	  control_points_z3[last_extremum_l]=last_extremum_value;
	  for(int l2=last_extremum_l+1;l2<control_points_x1.size();l2++)
	    {
	      control_points_z1[l2]=last_extremum_value;
	      control_points_z2[l2]=last_extremum_value;
	      control_points_z3[l2]=last_extremum_value;
	    }
	}
    }

  
	//output
	if(debug_output)
	  {
	    //draw circles
	    for(int n=0;n<nodes_name.size();n++)
	      {	  
		if(nb_iterations_relaxation<=0||nodes_name[n].substr(0,6)!="dummy_")
		  {
		    vector<vector<double> > points=get_circle(nodes_x[n],nodes_y[n],nodes_r[n],30);
		    for(int i=0;i<points.size();i++)
		      {
			for(int j=0;j<points[i].size();j++)
			  file<<points[i][j]<<" ";
			file<<" Circle|"<<nodes_name[n];
			if(nodes_external[n])
			  file<<"|is.exterior=TRUE"<<endl;
			else
			  file<<"|is.exterior=FALSE"<<endl;
		      }
		  }
	      }
	    //draw edges
	    for(int n=0;n<edges_target_node.size();n++)
	      {
		if(nb_iterations_relaxation<=0||nodes_name[n].substr(0,6)!="dummy_")
		  {
		    for(int i=0;i<edges_target_node[n].size();i++)
		      {
			int m=edges_target_node[n][i];
			if(nb_iterations_relaxation<=0||nodes_name[m].substr(0,6)!="dummy_")
			  {
			    file<<nodes_x[n]<<" "<<nodes_y[n]<<" "<<0<<" Segment|"<<nodes_name[n]<<"->"<<nodes_name[m]<<endl;
			    file<<nodes_x[m]<<" "<<nodes_y[m]<<" "<<0<<" Segment|"<<nodes_name[n]<<"->"<<nodes_name[m]<<endl;
			  }
		      }
		  }
	      }
	  }
  for(int l=0;l<control_points_x1.size();l++)
    {
      if(smooth<0)
	{
	  file<<control_points_x1[l]<<" "<<control_points_y1[l]<<" "<<control_points_z1[l]<<" "<<labels[l]<<endl;
	  file<<control_points_x2[l]<<" "<<control_points_y2[l]<<" "<<control_points_z2[l]<<" "<<labels[l]<<endl;
	  file<<control_points_x3[l]<<" "<<control_points_y3[l]<<" "<<control_points_z3[l]<<" "<<labels[l]<<endl;
	}
      else
	{
	  vector<double> points_x=get_spline(control_points_x1[l],control_points_x2[l],control_points_x3[l],smooth);
	  vector<double> points_y=get_spline(control_points_y1[l],control_points_y2[l],control_points_y3[l],smooth);
	  vector<double> points_z=get_spline(control_points_z1[l],control_points_z2[l],control_points_z3[l],smooth);
	  for(int i=0;i<points_x.size();i++)
	    {
	      file<<points_x[i]<<" "<<points_y[i]<<" "<<points_z[i];
	      file<<" "<<labels[l]<<endl;
	    }
	  
	}

    }

  file.precision(precisiontmp);

}


//---------------------------------------------------------------------------
void PlanarGraph::save_to_file_list(string filename)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_list(cout);
  else if(filename=="stderr")
    save_to_file_list(cerr);
  else
    {
      ofstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR PlanarGraph::save_to_file_list() problem with file "<<filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      save_to_file_list(file);
      file.close();
    }
}

//---------------------------------------------------------------------------


void PlanarGraph::save_to_file_list(ostream& file)
{
  file<<"#node1 node2 node1_label node2_label node1_is_external node2_is_external z1 z2 draw_order"<<endl;
  for(int n=0;n<edges_target_node.size();n++)
    {
      for(int i=0;i<edges_target_node[n].size();i++)
  	{
	  int m=edges_target_node[n][i];
	  
	  file<<nodes_name[n]<<" "<<nodes_name[m]<<" "<<nodes_label[n]<<" "<<nodes_label[m];
	  if(nodes_external[n])
	    file<<" "<<"TRUE";
	  else
	    file<<" "<<"FALSE";
	  if(nodes_external[m])
	    file<<" "<<"TRUE";
	  else
	    file<<" "<<"FALSE";
	  file<<" "<<edges_z[n][i].first<<" "<<edges_z[n][i].second;
	  file<<" "<<edges_draw[n][i]<<endl;
  	}
    }
}

//////////////////////////////////////////////
//Private
//////////////////////////////////////////////

int PlanarGraph::add_node(string name,string label,bool is_external)
{
  int i;
  map<string,int>::iterator it=nodes_name_to_index.find(name);
  if(it==nodes_name_to_index.end())
    {
      //create
      i=nodes_name.size();
      nodes_name_to_index[name]=i;
      nodes_name.push_back(name);
      nodes_label.push_back(label);
      nodes_external.push_back(is_external);

      edges_target_node.push_back(vector<int>()); 
      edges_z.push_back(vector<pair<int,int> >()); 
      edges_draw.push_back(vector<int>()); 

      edges_external.push_back(vector<bool>()); 
      //edges_external.clear();//should be reinitialized by triangulate()
    }  
  else
    {
      //check
      i=it->second;
      if(nodes_name[i]!=name||nodes_external[i]!=is_external||nodes_label[i]!=label)
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR PlanarGraph::add_node(): node already exists with different attributes."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
    }
  return i;
}


//---------------------------------------------------------------------------

void PlanarGraph::circle_packing()
{
  nodes_x.assign(nodes_name.size(),0);
  nodes_y.assign(nodes_name.size(),0);
  nodes_r.assign(nodes_name.size(),1);
  if(nodes_name.size()<2)
    return;
  int external_radius=1;//radius of external nodes
  for(int n=0;n<nodes_external.size();n++)
    {
      if(nodes_external[n])
	{
	  nodes_r[n]=external_radius;
	}
      else
	{
	  if(edges_target_node[n].size()==0)
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR PlanarGraph::circle_packing(): disconnected internal node."<<endl;
	      cerr<<nodes_name[n]<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);	      
	    }
	}
    }
  
  t0=time(NULL);
  time_t t1=time(NULL);
  double last_change = 2;
  double theta,newrad,kc,s1,s2;
  double tolerance=1e-15,tolerance_tmp=1e-4;
  bool flag_valid=false;
  while(tolerance_tmp>=tolerance)
    {
      cerr<<"circle packing tolerance="<<tolerance_tmp<<endl;
      while(last_change > 1+tolerance_tmp)
	{
	  last_change=1;
	  for(int n=0;n<edges_target_node.size();n++)
	    {
	      if(nodes_external[n])continue;
	      //angle sum around internal circle
	      theta=0;
	      for(int i=0;i<edges_target_node[n].size();i++)
		{
		  int N=edges_target_node[n].size();
		  int i1=edges_target_node[n][(i-1+N)%N];
		  int i2=edges_target_node[n][i];
		  theta+= get_angle(nodes_r[n]+nodes_r[i1],nodes_r[n]+nodes_r[i2],nodes_r[i1]+nodes_r[i2]);
		}
	      s1=sin(theta/(2*edges_target_node[n].size()));
	      s2=sin(M_PI/edges_target_node[n].size());
	      if(fabs(s1)<ZERO||fabs(s2)<ZERO) 
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR circle packing failed (radii)."<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);	      	      
		}
	      if(fabs(1-s1)<ZERO||fabs(1-s2)<ZERO) 
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR circle packing failed (radii)."<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);	      	      
		}
	      newrad =  nodes_r[n]*(1.0-s2)*s1/((1-s1)*s2);	  
	      if(newrad>1/ZERO) 
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR circle packing failed (radii)."<<endl;	      
		  cerr<<"*********************************************************"<<endl;
		  exit(1);	      	      
		}
	      kc = max(newrad/nodes_r[n],nodes_r[n]/newrad);
	      last_change = max(last_change,kc);
	      nodes_r[n] = newrad;
	    }
	  if(time(NULL)-t1>=2)
	    {
	      cerr<<"\r"<<"circle packing (radii): last change:"<<last_change-1<<" elapsed time: "<<time(NULL)-t0<<" s                    ";
	      t1=time(NULL);	  
	    }
	}
      if(time(NULL)-t0>=2)
	cerr<<endl;

      cerr<<"placing circles"<<endl;

      if(circle_packing_bfs_placement())
	{
	  cerr<<"checking circle packing"<<endl;
	  if(check_circle_packing(false))
	    {
	      flag_valid=true;
	      break;
	    }
	}
      tolerance_tmp=tolerance_tmp/10.0;
    }
  //check circle packing, with external region
  if(flag_valid)
    {
      cerr<<"checking circle packing"<<endl;
      if(!check_circle_packing())
	  flag_valid=false;
    }
  if(!flag_valid)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR  circle packing failed."<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }
}

//---------------------------------------------------------------------------

double PlanarGraph::get_angle(double a,double b,double c)
{
  double angle;
  double x=a*a + b*b - c*c;
  double y=2.0*a*b;

  if(fabs(y)<1e-4)
    {
      double ratio_xy=exp(log(fabs(x))-log(2.0)-log(a)-log(b));
      if(x<0)
	ratio_xy=-ratio_xy;
      angle=acos(ratio_xy);
    }
  else if(fabs(x)>fabs(y))
    {
      if(x<0)
	angle=M_PI;
      else
	angle=0;
    }
  else
    {
      angle=acos(x/y);
    }
  return angle;
	
}

//---------------------------------------------------------------------------

bool PlanarGraph::circle_packing_bfs_placement(double tolerance)
{
  t0=time(NULL);
  time_t t1=time(NULL);
  vector<bool> finished(nodes_name.size(),false);//true if already finished

  std::queue<pair<int,int> > to_test;//(n,l) s.t. edges_target_node[n][l]
  //find an internal node
  for(int n1=0;n1<nodes_external.size()&&to_test.size()==0;n1++)
    {
      if(!nodes_external[n1])
	{
         for(int i1=0;i1<edges_target_node[n1].size();i1++)
           if(!edges_external[n1][i1])
             {
               nodes_x[n1]=0;
               nodes_y[n1]=0;
               int n2=edges_target_node[n1][i1];
               nodes_x[n2]=nodes_r[n1]+nodes_r[n2];
               nodes_y[n2]=0;
               to_test.push(pair<int,int>(n1,i1));
               break;
             }
	}
    }

  int n,n1,n2,i1,i2,l;
  double offset_x,offset_y,tmp_x,tmp_y,theta,norm;
  long int nb_iterations=0;
  while(to_test.size()>0)
    {
      n=to_test.front().first;
      l=to_test.front().second;
      to_test.pop();
      if(finished[n])
	continue;

      nb_iterations++;
      for(int i=0;i<edges_target_node[n].size();i++)
	{
	  i1=(l+i+edges_target_node[n].size())%edges_target_node[n].size();
	  i2=(l+i+1+edges_target_node[n].size())%edges_target_node[n].size();
	  n1=edges_target_node[n][i1];
	  n2=edges_target_node[n][i2];
	  if(!finished[n1]&&!finished[n2])
	    {
	      theta= get_angle(nodes_r[n]+nodes_r[n1],nodes_r[n]+nodes_r[n2],nodes_r[n1]+nodes_r[n2]);
	      norm=sqrt((nodes_x[n1]-nodes_x[n])*(nodes_x[n1]-nodes_x[n])+(nodes_y[n1]-nodes_y[n])*(nodes_y[n1]-nodes_y[n]));	      
	      tmp_x = (nodes_x[n1]-nodes_x[n])/(norm);
	      tmp_y = (nodes_y[n1]-nodes_y[n])/(norm);	  	      
	      offset_x=tmp_x*cos(theta)-tmp_y*sin(theta);
	      offset_y=tmp_x*sin(theta)+tmp_y*cos(theta);
	      nodes_x[n2] = nodes_x[n] + offset_x*(nodes_r[n2]+nodes_r[n]);
	      nodes_y[n2] = nodes_y[n] + offset_y*(nodes_r[n2]+nodes_r[n]);	      
	      //check distances:	  
	      if(fabs(sqrt((nodes_x[n1]-nodes_x[n2])*(nodes_x[n1]-nodes_x[n2])+(nodes_y[n1]-nodes_y[n2])*(nodes_y[n1]-nodes_y[n2]))-(nodes_r[n1]+nodes_r[n2]))>tolerance||
		 fabs(sqrt((nodes_x[n]-nodes_x[n2])*(nodes_x[n]-nodes_x[n2])+(nodes_y[n]-nodes_y[n2])*(nodes_y[n]-nodes_y[n2]))-(nodes_r[n]+nodes_r[n2]))>tolerance)
		{
		  return false;
	      	}
	      if(!nodes_external[n1])
		{
		  //find j such that edges_target_node[n1][j]=n2;
		  
		  for(int j=0;j<edges_target_node[n1].size();j++)
		    {
		      if(edges_target_node[n1][j]==n2)
			{
			  to_test.push(pair<int,int>(n1,j));
			  break;
			}
		    }
		}
	    }	  
	}
      finished[n]=true;
      if(time(NULL)-t1>=2)
	{
	  cerr<<"\r"<<"circle packing (centers): "<<nb_iterations<<"/"<<edges_target_node.size()<<" elapsed time: "<<time(NULL)-t0<<" s                    ";
	  t1=time(NULL);	  
	}
      
    }
  if(time(NULL)-t0>=2)
    cerr<<endl;
  return true;
  
}

//---------------------------------------------------------------------------
void PlanarGraph::relax(long nb_iterations_relaxation,double distance_ratio_threshold,double temperature_start,double temperature_end)
{
  t0=time(NULL);
  time_t t1=time(NULL);
  cerr<<"relax diagram"<<endl;
  ////////////////////relax ///////////////////////
  vector<int>  nodes_to_relax;//list of nodes index to change during relaxation (i.e. non-dummy nodes)
  vector<bool>  torelax(nodes_name.size(),false);//if torelax[n]=true -> kept node with index n
  vector<vector<int> > direct_neighbors_list(nodes_name.size());//neighbors_list[n][i]=m  m is the i-th direct neighbor neighbor of n (i.e. directly connected by an edge edges_target_node[n][j]). Ordered counter clockwise. 
  vector<vector<int> > neighbors_list(nodes_name.size());//neighbors_list[n]=set of m such that m is neighbor of n (to check for intersections) (including direct neighbor)
  vector<vector<pair<int,int> > > neighbors_edges_list(nodes_name.size());//neighbors_edges_list[n]=set of (m,j) such that edges_target_node[m][j]  is a neighbor edge of n (to check for intersections & Eline) (without edges connecting n).
  vector<vector<pair<int,int> > > neighbors_edge_nodes_list(nodes_name.size());//neighbors_edges_list[n1]=set of (n2,n3) such that node n3 is  neighbor of edge n1-n2 (to update Eline_trial) (without edges connecting m). Note: all n1,n2 (n1<n2 and n1>=n2);
  for(int n=0;n<nodes_name.size();n++)
    {
      if(nodes_name[n].substr(0,6)!="dummy_")
	{
	  nodes_to_relax.push_back(n);
	  torelax[n]=true;
	}
    }
  for(int l=0;l<nodes_to_relax.size();l++)//follow regions, ignoring nodes not in nodes_to_relax
    {
      int n=nodes_to_relax[l];
      for(int i=0;i<edges_target_node[n].size();i++)
	{
	  if(!torelax[edges_target_node[n][i]])
	    continue;
	  int n1=n;
	  int i1_outgoing=i;
	  //find next to relax	  
	  int i1_incoming=(i1_outgoing+1)%edges_target_node[n1].size();//i.e. next edge counter-clockwise
	  while(!torelax[edges_target_node[n1][i1_incoming]])
	    {
	       i1_incoming=(i1_incoming+1)%edges_target_node[n1].size();//i.e. next edge counter-clockwise
	    }
	  direct_neighbors_list[n].push_back(edges_target_node[n1][i1_incoming]);
	  int n0=n1;
	  int i0_outgoing=i1_outgoing;
	  int n2;
	  int i2_incoming;
	  int i2_outgoing;
	  while(true)
	    {
	      n2=edges_target_node[n1][i1_outgoing];
	      i2_incoming=get_opposite_edge(n1,i1_outgoing);
	      i2_outgoing=(i2_incoming+edges_target_node[n2].size()-1)%edges_target_node[n2].size();//i.e. next edge clockwise
	      while(!torelax[edges_target_node[n2][i2_outgoing]])
		{
		  i2_outgoing=(i2_outgoing+2*edges_target_node[n2].size()-1)%edges_target_node[n2].size();//i.e. next edge counter-clockwise
		}

	      if(n2!=n0)
		{
		  if(find(neighbors_list[n0].begin(),neighbors_list[n0].end(),n2)==neighbors_list[n0].end())
		    neighbors_list[n0].push_back(n2);
		}
	      if(edges_target_node[n2][i2_outgoing]!=n0&&n2!=n0)
		{
		  if(n2<edges_target_node[n2][i2_outgoing])
		    {
		      if(find(neighbors_edges_list[n0].begin(),neighbors_edges_list[n0].end(),pair<int,int>(n2,i2_outgoing))==neighbors_edges_list[n0].end())
			neighbors_edges_list[n0].push_back(pair<int,int>(n2,i2_outgoing));
		    }
		  else
		    {
		      if(find(neighbors_edges_list[n0].begin(),neighbors_edges_list[n0].end(),pair<int,int>(edges_target_node[n2][i2_outgoing],get_opposite_edge(n2,i2_outgoing)))==neighbors_edges_list[n0].end())
			neighbors_edges_list[n0].push_back(pair<int,int>(edges_target_node[n2][i2_outgoing],get_opposite_edge(n2,i2_outgoing)));
		    }
		  if(find(neighbors_edge_nodes_list[n2].begin(),neighbors_edge_nodes_list[n2].end(),pair<int,int>(edges_target_node[n2][i2_outgoing],n0))==neighbors_edge_nodes_list[n2].end())
		    neighbors_edge_nodes_list[n2].push_back(pair<int,int>(edges_target_node[n2][i2_outgoing],n0));
		  if(find(neighbors_edge_nodes_list[edges_target_node[n2][i2_outgoing]].begin(),neighbors_edge_nodes_list[edges_target_node[n2][i2_outgoing]].end(),pair<int,int>(n2,n0))==neighbors_edge_nodes_list[edges_target_node[n2][i2_outgoing]].end())
		    neighbors_edge_nodes_list[edges_target_node[n2][i2_outgoing]].push_back(pair<int,int>(n2,n0));

		}
	      
	      if(n2==n0&&i2_outgoing==i0_outgoing)
		break;
	      
	      n1=n2;
	      i1_outgoing=i2_outgoing;
	    }
	  
	}
    }

  if(flag_debug)
    {
      cerr<<"neighbors (all):"<<endl; 
      for(int n=0;n<neighbors_list.size();n++)
	{
	  if(!torelax[n])
	    continue;
	  cerr<<" "<<nodes_name[n]<<endl;
	  for(vector<int>::iterator it=neighbors_list[n].begin();it!=neighbors_list[n].end();it++)
	    {
	      cerr<<"  "<<nodes_name[(*it)]<<endl;
	    }
	}
      cerr<<"direct neighbors:"<<endl; 
      for(int n=0;n<direct_neighbors_list.size();n++)
	{
	  if(!torelax[n])
	    continue;
	  cerr<<" "<<nodes_name[n]<<endl;
	  for(vector<int>::iterator it=direct_neighbors_list[n].begin();it!=direct_neighbors_list[n].end();it++)
	    {
	      cerr<<"  "<<nodes_name[(*it)]<<endl;
	    }
	}
      cerr<<"nodes_to_relax:"<<endl; 
      for(vector<int>::size_type n=0;n<nodes_to_relax.size();n++)
	  cerr<<" "<<nodes_name[nodes_to_relax[n]]<<endl;
      cerr<<"neighbors_edge_list:"<<endl;
      for(int n=0;n<direct_neighbors_list.size();n++)
	{
	  if(!torelax[n])
	    continue;
	  cerr<<" "<<nodes_name[n]<<endl;
	  for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n].begin();it!=neighbors_edges_list[n].end();it++)
	    {
	      int n2=it->first;
	      int n3=edges_target_node[n2][it->second];
	      cerr<<"  "<<nodes_name[n2]<<" - "<<nodes_name[n3]<<endl;
	    }
	}
	      
      cerr<<"neighbors_edge_nodes_list:"<<endl;
      for(int n=0;n<direct_neighbors_list.size();n++)
	{
	  if(!torelax[n])
	    continue;
	  for(vector<pair<int,int> >::iterator it=neighbors_edge_nodes_list[n].begin();it!=neighbors_edge_nodes_list[n].end();it++)
	    {
	      int n2=it->first;
	      int n3=it->second;
	      cerr<<" segment "<<nodes_name[n]<<" - "<<nodes_name[n2]<<"  point "<<nodes_name[n3]<<endl;
	    }
	}

      
    }

  //eval max dist
  double distance_max_initial=0;
  double distance_max=0,distance_min=-1;
  for(int i=0;i<nodes_to_relax.size();i++)
    {
      int n1=nodes_to_relax[i];
      for(vector<int>::iterator it=direct_neighbors_list[n1].begin();it!=direct_neighbors_list[n1].end();it++)
	{
	  int n2=(*it);
	  double distance=sqrt((nodes_x[n1]-nodes_x[n2])*(nodes_x[n1]-nodes_x[n2])+(nodes_y[n1]-nodes_y[n2])*(nodes_y[n1]-nodes_y[n2]));
	  if(distance>distance_max_initial)
	    distance_max_initial=distance;
	  if(distance_min<0||distance<distance_min)
	    distance_min=distance;
	}
    }
  distance_max=distance_max_initial;
  
  int n1;
  double x1,y1;
  double x1_trial,y1_trial;
  double dx,dy;
  double distance;
  double distance_trial;
  double temperature=temperature_start;//start
  
  double dT=exp((log(temperature_end)-log(temperature))/round(nb_iterations_relaxation));

  double Energy,Energy_trial;
  //dist(ri,line(rj,rk))^2=((yk-yj)*xi-(xk-xj)*yi+xk*yj-yk*xj)^2/((yk-yj)^2+(xk-xj)^2)


  //eval initial energy:
  Energy=0;
  for(vector<int>::size_type i=0;i<nodes_to_relax.size();i++)
    {
      n1=nodes_to_relax[i];
      x1=nodes_x[n1];
      y1=nodes_y[n1];
      for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end();it++)
	{
	  int n2=it->first;
	  int n3=edges_target_node[n2][it->second];
	  double x2=nodes_x[n2];
	  double y2=nodes_y[n2];
	  double x3=nodes_x[n3];
	  double y3=nodes_y[n3];
	  //distance between point (x1,y1) and segment (x2,y2)-(x3,y3) (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
	  distance=distance_point_segment(x1,y1,x2,y2,x3,y3);
	  Energy+=1/(distance*distance*nodes_to_relax.size());	    
	}
      
    }
  if(flag_debug)
    cerr<<"Initial Energy="<<Energy<<endl;
  
  int changes_move_accepted=0;
  int changes_move_total=0;
  double factor_move=0.5;
  double gradient_contribution=0.5;//To choose trial position: 0 => only random, Inf => only gradient.
  for(long iteration=0;iteration<nb_iterations_relaxation&&distance_min/distance_max<distance_ratio_threshold;iteration++)
    {
      if(time(NULL)-t1>=2)
	{
	  cerr<<"\r"<<"relax diagram: "<<round(10*100*iteration/(double)nb_iterations_relaxation)/10.0<<"% elapsed time: "<<time(NULL)-t0<<" s                      ";
	  t1=time(NULL);	  
	}
      distance_max=0;
      distance_min=-1;

      for(vector<int>::size_type i=0;i<nodes_to_relax.size();i++)
	{
	  n1=nodes_to_relax[i];
	  x1=nodes_x[n1];
	  y1=nodes_y[n1];

	  if(gradient_contribution>0)
	    {
	      ////////energy gradient///////
	      double dEdx=0;
	      double dEdy=0;
	      // point n1, segments n2,n3
	      for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end();it++)
		{
		  int n2=it->first;
		  int n3=edges_target_node[n2][it->second];
		  double x2=nodes_x[n2];
		  double y2=nodes_y[n2];
		  double x3=nodes_x[n3];
		  double y3=nodes_y[n3];

		  //gradient of distance between point (x1,y1) and segment (x2,y2)-(x3,y3) (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
		  double tmp=((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
		  if(fabs(tmp)<ZERO) //(x2,y2)~(x3,y3)
		    {
		      double tmp2=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
		      dEdx+=2*(x2-x1)/(tmp2*tmp2*nodes_to_relax.size());
		      dEdy+=2*(y2-y1)/(tmp2*tmp2*nodes_to_relax.size());
		      //distance=sqrt((x1-0.5*(x2+x3))*(x1-0.5*(x2+x3))+(y1-0.5*(y2+y3))*(y1-0.5*(y2+y3)));
		    }
		  else
		    {
		      double u=((x3-x2)*(x1-x2)+(y3-y2)*(y1-y2))/tmp;
		      if(u<0)
			{
			  double tmp2=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
			  dEdx+=2*(x2-x1)/(tmp2*tmp2*nodes_to_relax.size());
			  dEdy+=2*(y2-y1)/(tmp2*tmp2*nodes_to_relax.size());
			}
		      else if(u>=1)
			{
			  double tmp2=(x1-x3)*(x1-x3)+(y1-y3)*(y1-y3);
			  dEdx+=2*(x3-x1)/(tmp2*tmp2*nodes_to_relax.size());
			  dEdy+=2*(y3-y1)/(tmp2*tmp2*nodes_to_relax.size());
			}
		      else
			{
			  double tmp2=x3*(y2-y1)+x2*(y1-y3)+x1*(y3-y2);
			  dEdx+=2*(y2-y3)*tmp/(tmp2*tmp2*tmp2*nodes_to_relax.size());
			  dEdy+=2*(x3-x2)*tmp/(tmp2*tmp2*tmp2*nodes_to_relax.size());
			}
		    }
		}
	      //segment n1-n2 points n3	  
	      for(int l=0;l<neighbors_edge_nodes_list[n1].size();l++)
		{
	      
		  int n2=neighbors_edge_nodes_list[n1][l].first;
		  double x2=nodes_x[n2];
		  double y2=nodes_y[n2];
		  int n3=neighbors_edge_nodes_list[n1][l].second;
		  double x3=nodes_x[n3];
		  double y3=nodes_y[n3];
	      

		  //gradient of distance between point (x3,y3) and segment (x1,y1)-(x2,y2) (x1+u*(x2-x1),y1+u*(y2-y1)) with 0<=u<=1
		  double tmp=((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
		  if(fabs(tmp)<ZERO) //(x1,y1)~(x1,y1)
		    {
		      double tmp2=(x1-x3)*(x1-x3)+(y1-y3)*(y1-y3);
		      dEdx+=2*(x3-x1)/(tmp2*tmp2*nodes_to_relax.size());
		      dEdy+=2*(y3-y1)/(tmp2*tmp2*nodes_to_relax.size());
		    }
		  else
		    {
		      double u=((x2-x1)*(x3-x1)+(y2-y1)*(y3-y1))/tmp;
		      if(u<0)
			{
			  double tmp2=(x1-x3)*(x1-x3)+(y1-y3)*(y1-y3);
			  dEdx+=2*(x3-x1)/(tmp2*tmp2*nodes_to_relax.size());
			  dEdy+=2*(y3-y1)/(tmp2*tmp2*nodes_to_relax.size());
			}
		      else if(u>=1)
			{
			  dEdx+=0;
			  dEdy+=0;
			}
		      else
			{
			  double tmp1=(x1-x2)*(x2-x3)+(y1-y2)*(y2-y3);
			  double tmp2=x3*(y2-y1)+x2*(y1-y3)+x1*(y3-y2);
			  dEdx+=2*(y1-y2)*tmp1/(tmp2*tmp2*tmp2*nodes_to_relax.size());
			  dEdy+=2*(x2-x1)*tmp1/(tmp2*tmp2*tmp2*nodes_to_relax.size());
			}
		    }
		}
	      
	      double dE=sqrt(dEdx*dEdx+dEdy*dEdy);
	      if(dE>ZERO)
		{
		  dx=-gradient_contribution*dEdx/dE;
		  dy=-gradient_contribution*dEdy/dE;
		  //add random noise
		  double a=2*M_PI*random01();
		  dx+=cos(a);
		  dy+=sin(a);
		  //normalize
		  double norm=sqrt(dx*dx+dy*dy);
		  dx=dx/norm;
		  dy=dy/norm;
		}
	      else
		{	  
		  //choose random direction:
		  double a=2*M_PI*random01();
		  dx=cos(a);
		  dy=sin(a);
		}
	    }
	  else
	    {	  
	      //choose random direction:
	      double a=2*M_PI*random01();
	      dx=cos(a);
	      dy=sin(a);
	    }
	  
	  double max_t=1e100;
	  //find max allowed move;
	  //find max t>0 such that:
	  //1) segments (x2,y2) to (x1+t*dx,y1+t*dy) do not touch any neigboring point
	  //  find t,u such that segment (with 0<u<1)
	  //  x=x2+u*(x1t-x2);
	  //  y=y2+u*(y1t-y2);
	  //  contains point :
	  //  x3,y3
	  //2) points (x1t,y1t)=(x1+t*dx,y1+t*dy) do not touch any neigboring segment
	  //  find t,u such that segment (with 0<u<1)
	  //  x=x2+u*(x3-x2);
	  //  y=y2+u*(y3-y2);
	  //  intersects:
	  //  x=x1+t*dx
	  //  y=y1+t*dy
	  //3) distance (x1t,y1t)=(x1+t*dx,y1+t*dy) to all direct neighbors is < distance_max_initial
	  for(vector<int>::iterator it=direct_neighbors_list[n1].begin();it!=direct_neighbors_list[n1].end()&&max_t>0;it++)
	    {
	      //(3) distance between (x1+t*dx,y1+t*dy) and (x2,y2) < distance_max_initial
	      int n2=(*it);
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      distance=sqrt((nodes_x[n1]-nodes_x[n2])*(nodes_x[n1]-nodes_x[n2])+(nodes_y[n1]-nodes_y[n2])*(nodes_y[n1]-nodes_y[n2]));
	      if(distance>distance_max)
		distance_max=distance;
	      if(distance_min<0||distance<distance_min)
		distance_min=distance;
		  
	      if(distance>distance_max_initial+ZERO)
		{
		  cerr<<" iter="<<iteration<<" n1="<<nodes_name[n1]<<" n2="<<nodes_name[n2]<<" distance="<<distance<<" > maxdistance="<<distance_max_initial<<" i="<<i<<endl;
		  exit(1);
		}
	      //check distance
	      double tmp=distance_max_initial*distance_max_initial*(dx*dx+dy*dy)-(dy*(x1-x2)-dx*(y1-y2))*(dy*(x1-x2)-dx*(y1-y2));
	      if(tmp>0)
		{
		  double t1=(-dx*(x1-x2)-dy*(y1-y2)-sqrt(tmp))/(dx*dx+dy*dy);
		  double t2=(-dx*(x1-x2)-dy*(y1-y2)+sqrt(tmp))/(dx*dx+dy*dy);
		  if(t1>=-ZERO&&t1<max_t)max_t=t1;
		  if(t2>=-ZERO&&t2<max_t)max_t=t2;
		}
	      else//should not happen
		{
		  max_t=0;
		}
	    }
	  //(1) intersect segments (x2,y2) to (x1+t*dx,y1+t*dy) with neighboring points 
	  for(vector<int>::iterator it=neighbors_list[n1].begin();it!=neighbors_list[n1].end()&&max_t>0;it++)
	    {
	      int n2=(*it);
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      for(vector<int>::iterator it3=neighbors_list[n1].begin();it3!=neighbors_list[n1].end()&&max_t>0;it3++)
		{
		  int n3=(*it3);
		  if(n2==n3)continue;
		  double x3=nodes_x[n3];
		  double y3=nodes_y[n3];
		  //x3=x2+u*(x1+t*dx-x2);
		  //y3=y2+u*(y1+t*dy-y2);
		  double tmp1=dy*(x1-x2)-dx*(y1-y2);
		  if(fabs(tmp1)<ZERO)//dx parallel to n1-n2 => avoid move
		    {
		      max_t=0;
		      break;
		    }		  
		  double tmp2=dy*(x3-x2)-dx*(y3-y2);
		  if(fabs(tmp2)<ZERO)//dx parallel to n2-n3 => t can be infinite OR segment n2-n3 has length 3 => avoid move
		    {
		      max_t=0;
		      break;
		    }
		  double u=(dy*(x3-x2)-dx*(y3-y2))/tmp1;
		  if(u>=-ZERO&&u<=1+ZERO)//intersection
		    {
		      double t=(x3*(y2-y1)+x2*(y1-y3)+x1*(y3-y2))/tmp2;
		      if(t>=-ZERO&&t<max_t)max_t=t;
		    }
		}
	    }
	  //(2) intersect points (x1t,y1t)=(x1+t*dx,y1+t*dy)with neigboring segments
	  for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end()&&max_t>0;it++)
	    {
	      int n2=it->first;
	      int n3=edges_target_node[n2][it->second];
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      double x3=nodes_x[n3];
	      double y3=nodes_y[n3];
	      //find u, t such that
	      //x1 + t*dx == x2 + u (x3 - x2)
	      //y1 + t*dy == y2 + u (y3 - y2)
	      double tmp=dy*(x2-x3)-dx*(y2-y3);
	      if(fabs(tmp)<ZERO)//dx parallel to n2-n3 => safer to  move
		{
		  max_t=0;
		  break;
		}		  
	      double u=(dy*(x2-x1)-dx*(y2-y1))/tmp;
	      if(u>=-ZERO&&u<=1+ZERO)//intersection
		{
		  double t=(x3*(y1-y2)+x2*(y3-y1)+x1*(y2-y3))/tmp;
		  if(t>=-ZERO&&t<max_t)max_t=t;
		}		  	      
	    }
	  if(max_t<0)max_t=0;

	  if(max_t<ZERO)
	    continue;
	  changes_move_total++;

	  double t=0.5*factor_move*random01();
	  dx=t*max_t*dx;
	  dy=t*max_t*dy;

	  x1_trial=x1+dx;
	  y1_trial=y1+dy;

	  ////////energy_trial///////
	  //direct neighbors:
	  Energy_trial=Energy;	  
	  //correction to Energy: point n1, segments n2,n3
	  for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end();it++)
	    {
	      int n2=it->first;
	      int n3=edges_target_node[n2][it->second];
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      double x3=nodes_x[n3];
	      double y3=nodes_y[n3];
	      //distance between point (x1,y1) and segment (x2,y2)-(x3,y3) (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
	      distance=distance_point_segment(x1,y1,x2,y2,x3,y3);
	      distance_trial=distance_point_segment(x1_trial,y1_trial,x2,y2,x3,y3);
	      Energy_trial=Energy_trial-1/(distance*distance*nodes_to_relax.size());	    
	      Energy_trial=Energy_trial+1/(distance_trial*distance_trial*nodes_to_relax.size());	    
	    }
	  //correction to Energy: segment n1-n2 points n3	  
	  for(int l=0;l<neighbors_edge_nodes_list[n1].size();l++)
	    {
	      
	      int n2=neighbors_edge_nodes_list[n1][l].first;
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      int n3=neighbors_edge_nodes_list[n1][l].second;
	      double x3=nodes_x[n3];
	      double y3=nodes_y[n3];
	      

	      //distance between point (x3,y3) and segment (x1,y1)-(x2,y2) (x1+u*(x2-x1),y1+u*(y2-y1)) with 0<=u<=1
	      distance=distance_point_segment(x3,y3,x1,y1,x2,y2);
	      distance_trial=distance_point_segment(x3,y3,x1_trial,y1_trial,x2,y2);
	      Energy_trial=Energy_trial-1/(distance*distance*nodes_to_relax.size());	    
	      Energy_trial=Energy_trial+1/(distance_trial*distance_trial*nodes_to_relax.size());	    
			      
	    }

	  
	  if(Energy_trial<Energy||(temperature>0&&random01()<exp(-(Energy_trial-Energy)/temperature)))//accept
	    {
	      nodes_x[n1]=x1_trial;
	      nodes_y[n1]=y1_trial;
	      Energy=Energy_trial;
	      changes_move_accepted++;		  
	    }
	  //randomize order in nodes_to_relax (swap edges_list[j] and  edges_list[j2])
	  int i2=random01()*i;
	  nodes_to_relax[i]=nodes_to_relax[i2];
	  nodes_to_relax[i2]=n1;	  
	}
      if(flag_debug)
	cout<<"Iteration= "<<iteration<<" Temperature= "<<temperature<<" factor_move= "<<factor_move<<" Energy= "<<Energy<<" distance_ratio= "<<distance_min/distance_max<<" accepted_move= "<<changes_move_accepted/(double)changes_move_total<<" changes_move_total= "<<changes_move_total<<endl;
      if(changes_move_total>20)
	{
	  if(changes_move_accepted/(double)changes_move_total>0.8)
	    factor_move=factor_move*1.1;
	  if(changes_move_accepted/(double)changes_move_total<0.2)
	    factor_move=factor_move/1.1;
	  if(factor_move>1)factor_move=1;
	  changes_move_total=0;
	  changes_move_accepted=0;
	}
      temperature=temperature*dT;

      //exit(1);
      if(flag_debug)
	cerr<<"iteration="<<iteration<<" distance_ratio="<<distance_min/distance_max<<endl;

    }

  if(flag_debug)
    {
      cerr<<" last Energy="<<Energy<<" elapsed time: "<<time(NULL)-t0<<" s"<<endl;
      //eval final energy:
      Energy=0;
      for(vector<int>::size_type i=0;i<nodes_to_relax.size();i++)
	{
	  n1=nodes_to_relax[i];
	  x1=nodes_x[n1];
	  y1=nodes_y[n1];
	  for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end();it++)
	    {
	      int n2=it->first;
	      int n3=edges_target_node[n2][it->second];
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      double x3=nodes_x[n3];
	      double y3=nodes_y[n3];
	      //distance between point (x1,y1) and segment (x2,y2)-(x3,y3) (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
	      distance=distance_point_segment(x1,y1,x2,y2,x3,y3);
	      Energy+=1/(distance*distance*nodes_to_relax.size());	    
	    }
      
	}
      cerr<<"Final Energy="<<Energy<<endl;
    }
  ///////////////adapt radius//////////////
  for(vector<int>::size_type i=0;i<nodes_to_relax.size();i++)
    {
      n1=nodes_to_relax[i];
      //search max possible radius
      nodes_r[n1]=-1;
      x1=nodes_x[n1];
      y1=nodes_y[n1];
      for(vector<int>::iterator it=neighbors_list[n1].begin();it!=neighbors_list[n1].end();it++)
	{
	  int n2=(*it);
	  distance=sqrt((nodes_x[n1]-nodes_x[n2])*(nodes_x[n1]-nodes_x[n2])+(nodes_y[n1]-nodes_y[n2])*(nodes_y[n1]-nodes_y[n2]));
	  if(nodes_r[n1]<0||nodes_r[n1]>(distance)/2.0)
	    nodes_r[n1]=(distance)/2.0;
	}
      for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end();it++)
	{
	  int n2=it->first;
	  int n3=edges_target_node[n2][it->second];
	  double x2=nodes_x[n2];
	  double y2=nodes_y[n2];
	  double x3=nodes_x[n3];
	  double y3=nodes_y[n3];
	  //distance between point (x1,y1) and segment (x2,y2)-(x3,y3) (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
	  distance=distance_point_segment(x1,y1,x2,y2,x3,y3);
	  if(nodes_r[n1]<0||nodes_r[n1]>distance)
	    nodes_r[n1]=distance;
	}      
    }
  //TODO: optimize Radius?
  for(int iteration=0;iteration<nb_iterations_relaxation;iteration++)
    {
      double last_change=0;
      for(vector<int>::size_type i=0;i<nodes_to_relax.size();i++)
	{
	  n1=nodes_to_relax[i];
	  x1=nodes_x[n1];
	  y1=nodes_y[n1];
	  //search max possible radius
	  double maxR=-1;
	  for(vector<int>::iterator it=neighbors_list[n1].begin();it!=neighbors_list[n1].end();it++)
	    {
	      int n2=(*it);
	      distance=sqrt((nodes_x[n1]-nodes_x[n2])*(nodes_x[n1]-nodes_x[n2])+(nodes_y[n1]-nodes_y[n2])*(nodes_y[n1]-nodes_y[n2]));
	      if(maxR<0||maxR>(distance-nodes_r[n2]))
		maxR=(distance-nodes_r[n2]);
	    }
	  for(vector<pair<int,int> >::iterator it=neighbors_edges_list[n1].begin();it!=neighbors_edges_list[n1].end();it++)
	    {
	      int n2=it->first;
	      int n3=edges_target_node[n2][it->second];
	      double x2=nodes_x[n2];
	      double y2=nodes_y[n2];
	      double x3=nodes_x[n3];
	      double y3=nodes_y[n3];
	      //distance between point (x1,y1) and segment (x2,y2)-(x3,y3) (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
	      distance=distance_point_segment(x1,y1,x2,y2,x3,y3);
	      if(maxR<0||maxR>distance)
		maxR=distance;
	    }
	  
	  if(maxR>nodes_r[n1])
	    {
	      if(last_change<0.1*(maxR-nodes_r[n1])/nodes_r[n1])
		last_change=0.1*(maxR-nodes_r[n1])/nodes_r[n1];
	      nodes_r[n1]=nodes_r[n1]+0.1*(maxR-nodes_r[n1]);
	    }
	}
      if(last_change<1e-5)
	break;      
    }

  if(flag_debug)
    cerr<<" distance_ratio="<<distance_min/distance_max<<endl;

  if(time(NULL)-t0>=2)
    cerr<<endl;


}


//---------------------------------------------------------------------------
bool PlanarGraph::check_circle_packing(bool flag_check_external,double tolerance)
{
  if(flag_debug)
    cerr<<"check packing()"<<endl;
  if(nodes_x.size()!=nodes_name.size()||nodes_y.size()!=nodes_name.size()||nodes_r.size()!=nodes_name.size())
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"WARNING PlanarGraph::check_circle_packing() positions and radii not initialized."<<endl;
      cerr<<"*********************************************************"<<endl;
      return false;
    }
  for(int n=0;n<nodes_x.size();n++)
    {
      if(isnan(nodes_x[n])||isnan(nodes_y[n])||isnan(nodes_r[n]))
	{
	  if(flag_debug)
	    cerr<<"PlanarGraph::check_circle_packing() nan"<<endl;
	  return false;
	}
    }
  double delta_r;
  double delta_theta;
  double x0,y0,x1,y1,x2,y2;
  double r0,r1,r2;
  double d1x,d1y,d2x,d2y,d1,d2;
  double theta,theta_sum;
  vector<int> external_nodes;
  for(int n=0;n<edges_target_node.size();n++)
    {
      if(flag_check_external&&nodes_external[n])
	external_nodes.push_back(n);
      x0=nodes_x[n];
      y0=nodes_y[n];
      r0=nodes_r[n];
      theta_sum=0;
      for(int i=0;i<edges_target_node[n].size();i++)
	{	  
	  int m1=edges_target_node[n][i];
	  int m2=edges_target_node[n][(i+1)%edges_target_node[n].size()];
	  x1=nodes_x[m1];
	  y1=nodes_y[m1];
	  r1=nodes_r[m1];
	  x2=nodes_x[m2];
	  y2=nodes_y[m2];
	  d1x=x1-x0;
	  d1y=y1-y0;
	  d2x=x2-x0;
	  d2y=y2-y0;
	  d1=sqrt(d1x*d1x+d1y*d1y);
	  d2=sqrt(d2x*d2x+d2y*d2y);

	  //check distances
	  delta_r=fabs((r0+r1)-sqrt(d1x*d1x+d1y*d1y))/((r0+r1));
	  if(delta_r>tolerance)
	    {
	      if(flag_debug)
		cerr<<"PlanarGraph::check_circle_packing() delta_r="<<delta_r<<" "<<nodes_name[n]<<" "<<nodes_name[m1]<<endl;
	      return false;
	    }

	  //sum angles
	  theta=acos((d1x*d2x+d1y*d2y)/(d1*d2));// between 0 and pi
	  if((d1x*d2y-d1y*d2x)<0)//=> theta between 0 and 2*PI
	    theta=2*M_PI-theta;
	  theta_sum+=theta;
	}
      if(edges_target_node[n].size()>0)
	{
	  delta_theta=fabs(2*M_PI-theta_sum);
	  if(delta_theta>tolerance)
	    {
	      if(flag_debug)
		{
		  cerr<<"PlanarGraph::check_circle_packing() delta_theta="<<delta_theta<<" "<<nodes_name[n]<<endl;
		  for(int i=0;i<edges_target_node[n].size();i++)
		    {
		      int m1=edges_target_node[n][i];
		      int m2=edges_target_node[n][(i+1)%edges_target_node[n].size()];
		      x1=nodes_x[m1];
		      y1=nodes_y[m1];
		      x2=nodes_x[m2];
		      y2=nodes_y[m2];
		      d1x=x1-x0;
		      d1y=y1-y0;
		      d2x=x2-x0;
		      d2y=y2-y0;
		      d1=sqrt(d1x*d1x+d1y*d1y);
		      d2=sqrt(d2x*d2x+d2y*d2y);		  
		      theta=acos((d1x*d2x+d1y*d2y)/(d1*d2));// between 0 and pi
		      if((d1x*d2y-d1y*d2x)<0)//=> theta between 0 and 2*PI
			theta=2*M_PI-theta;
		      cerr<<" theta("<<nodes_name[m1]<<"-"<<nodes_name[n]<<"-"<<nodes_name[m2]<<")="<<theta<<" ("<<nodes_x[m1]<<","<<nodes_y[m1]<<")-("<<nodes_x[n]<<","<<nodes_y[n]<<")-("<<nodes_x[m2]<<","<<nodes_y[m2]<<")  radii="<<nodes_r[m1]<<","<<nodes_r[n]<<","<<nodes_r[m2]<<endl;
		    }
		}      
	      return false;
	    }
	}
    }

  //check external nodes do not intersect (it could happen, according to C. R. Collins and K. Stephenson)
  if(flag_check_external)
    {
      for(int n1=0;n1<external_nodes.size();n1++)
	{
	  x1=nodes_x[n1];
	  y1=nodes_y[n1];
	  r1=nodes_r[n1];
	  for(int n2=n1+1;n2<external_nodes.size();n2++)
	    {
	      x2=nodes_x[n2];
	      y2=nodes_y[n2];
	      r2=nodes_r[n2];
	      if(sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))<(r2+r1)*(1-tolerance))
		{
		  if(flag_debug)
		    cerr<<"PlanarGraph::check_circle_packing() external region intersect "<<nodes_name[n1]<<" "<<nodes_name[n2]<<endl;
		  return false;
		}

	    }
	}
    }  
  if(flag_debug)
    cerr<<"OK"<<endl;
  return true;
}


//---------------------------------------------------------------------------

vector<vector<double> > PlanarGraph::get_circle(double Cx,double Cy,double R,int N)
{
  N=N+2;//add endpoints

  double t;
  double x,y,z;
  vector<vector<double> >  points;
  vector<double> tmp;
  for(int i=0;i<N;i++)
    {
      t=i*2*M_PI/(double)(N-1);

      x=Cx+R*cos(t);
      y=Cy+R*sin(t);
      z=0;
      
      tmp.clear();
      tmp.push_back(x);
      tmp.push_back(y);
      tmp.push_back(z);
      points.push_back(tmp);
    }
  return points;
  
  
}


//---------------------------------------------------------------------------

vector<double> PlanarGraph::get_spline(double x1,double x2,double x3,int N,double k)
{
  N=N+2;//add endpoints

  double d1x=x2-x1;
  double d3x=x3-x2;
  vector<double>  result;

  double k1x=k*d1x;
  double k3x=k*d3x;
  double ax=k1x-(x3-x1);
  double bx=-k3x+(x3-x1);
  double t;
  for(int i=0;i<N;i++)
    {
      t=i/(double)(N-1);
      result.push_back((1-t)*x1+t*x3+t*(1-t)*(ax*(1-t)+bx*t));
    }
  return result;

}

//------------------------------------------------------------------------------

double PlanarGraph::distance_point_segment(double x1,double y1,double x2,double y2,double x3,double y3)
{
  double distance;
  //distance between point (x1,y1)
  //and segment (x2,y2)-(x3,y3) with parametric equation (x2+u*(x3-x2),y2+u*(y3-y2)) with 0<=u<=1
  double tmp=((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
  if(fabs(tmp)<ZERO) //(x2,y2)~(x3,y3)
    {
      distance=sqrt((x1-0.5*(x2+x3))*(x1-0.5*(x2+x3))+(y1-0.5*(y2+y3))*(y1-0.5*(y2+y3)));
    }
  else
    {
      double u=((x3-x2)*(x1-x2)+(y3-y2)*(y1-y2))/tmp;
      if(u<0)
	distance=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
      else if(u>=1)
	distance=sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
      else
	distance=sqrt((x2+u*(x3-x2)-x1)*(x2+u*(x3-x2)-x1)+(y2+u*(y3-y2)-y1)*(y2+u*(y3-y2)-y1));
    }
  return distance;
}
