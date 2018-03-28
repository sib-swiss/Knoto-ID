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


#include "PlanarDiagram.hh"


using namespace std;
using namespace boost;

//************************************************************
//**********************   PlanarDiagram   **********************
//************************************************************
PlanarDiagram::PlanarDiagram(bool flag_planar,bool flag_debug):flag_planar(flag_planar),flag_debug(flag_debug)
{

}
//---------------------------------------------------------------------------

PlanarDiagram::PlanarDiagram(vector<Crossing> & crossings,bool flag_planar,bool flag_debug):crossings(crossings),flag_planar(flag_planar),flag_debug(flag_debug)
{
  initialize();
  if(!check())
    {
      cerr<<"**************************************"<<endl;
      cerr<<"ERROR PlanarDiagram not valid!"<<endl;
      cerr<<"**************************************"<<endl;
      if(flag_debug)
	save_to_file_list(cerr);
      exit(1);
    }
}
//---------------------------------------------------------------------------
void PlanarDiagram::save_to_file_list(string filename)
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
	  cerr<<"ERROR problem with file "<<filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      save_to_file_list(file);
      file.close();
    }
}

//---------------------------------------------------------------------------


void PlanarDiagram::save_to_file_list(ostream& file)
{
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//empty (removed) crossing
	file<<"Crossing "<<n<<" removed"<<endl;
      else if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
	{
	  file<<"Endpoint "<<n<<" arcs: "<<crossings[n].arc0;
	  file<<", connected to crossing: "<<crossings[n].connected_crossing0;
	  file<<", is exterior(1=true,-1=unset): "<<crossings[n].region01;
	  file<<endl;
	}
      else
	{
	  file<<"Crossing "<<n<<" arcs: "<<crossings[n].arc0<<" "<<crossings[n].arc1<<" "<<crossings[n].arc2<<" "<<crossings[n].arc3;
	  file<<", connected to crossings: "<<crossings[n].connected_crossing0<<" "<<crossings[n].connected_crossing1<<" "<<crossings[n].connected_crossing2<<" "<<crossings[n].connected_crossing3;
	  file<<", is exterior boundary(1=true,-1=unset): 1-2:"<<crossings[n].region01<<" 2-3:"<<crossings[n].region12<<" 3-4:"<<crossings[n].region23<<" 4-1:"<<crossings[n].region30;
	  file<<endl;
	}
    }
}

//---------------------------------------------------------------------------
void PlanarDiagram::save_to_file_KnotTheory(string filename,bool flag_one_line)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_KnotTheory(cout,flag_one_line);
  else if(filename=="stderr")
    save_to_file_KnotTheory(cerr,flag_one_line);
  else
    {
      ofstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR problem with file "<<filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      save_to_file_KnotTheory(file,flag_one_line);
      file.close();
    }
}

//---------------------------------------------------------------------------


void PlanarDiagram::save_to_file_KnotTheory(ostream& file,bool flag_one_line)
{
  string sep;
  if(flag_one_line==false)
    sep="\n";
  else
    sep="";
    
  int nbcrossings=0;
  for(int n=0;n<crossings.size();n++)
    if(!(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0))//empty
      nbcrossings++;
  stringstream comments;
  if(nbcrossings==0)
    {
      file<<"PD[];";
      if(flag_one_line==false)file<<endl;
    }
  else
    {
      file<<"PD[";  
      for(int n=0;n<crossings.size();n++)
	{
	  if(crossings[n].arc0>=0&&crossings[n].arc1>=0&&crossings[n].arc2>=0&&crossings[n].arc3>=0)//regular crossing
	    {
	      file<<sep<<"X[";
	      if(flag_planar&&(crossings[n].region30==1||crossings[n].region01==1))
	      	file<<"r["<<crossings[n].arc0<<"]";
	      else
		file<<crossings[n].arc0;
	      if(flag_planar&&(crossings[n].region01==1||crossings[n].region12==1))
	      	file<<",r["<<crossings[n].arc1<<"]";
	      else
		file<<","<<crossings[n].arc1;
	      if(flag_planar&&(crossings[n].region12==1||crossings[n].region23==1))
	      	file<<",r["<<crossings[n].arc2<<"]";
	      else
		file<<","<<crossings[n].arc2;
	      if(flag_planar&&(crossings[n].region23==1||crossings[n].region30==1))
	      	file<<",r["<<crossings[n].arc3<<"]]";
	      else
		file<<","<<crossings[n].arc3<<"]";
	      if(flag_one_line==false)
		sep=",\n";
	      else
		sep=",";
	    }
	}
      if(flag_one_line==false)
	file<<"\n];";
      else
	file<<"];";
      if(flag_one_line==false)file<<endl;
      if(flag_one_line==false)file<<comments.str();
    }
}

//---------------------------------------------------------------------------
void PlanarDiagram::save_to_file_extended_gauss_code(string filename,bool flag_one_line)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_extended_gauss_code(cout,flag_one_line);
  else if(filename=="stderr")
    save_to_file_extended_gauss_code(cerr,flag_one_line);
  else
    {
      ofstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR problem with file "<<filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      save_to_file_extended_gauss_code(file,flag_one_line);
      file.close();
    }
}

//---------------------------------------------------------------------------


void PlanarDiagram::save_to_file_extended_gauss_code(ostream& file,bool flag_one_line)
{

  //search for first arc
  int min_arc=-1;
  int max_arc=-1;
  int input_arc=-1;
  int input_crossing=-1;
  int input_pos=-1;
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0>=0&&crossings[n].arc1>=0&&crossings[n].arc2>=0&&crossings[n].arc3>=0)//ignore empty crossing and endpoints
	{
	  for(int i=0;i<4;i++)
	    {
	      int arc1=crossings[n].get_arc(i);
	      int arc2=crossings[n].get_arc((i+2)%4);
	      if(arc1>=0)
		{
		  if(arc1<min_arc||min_arc<0)
		    {
		      min_arc=arc1;
		    }
		  if((arc2==arc1+1)&&(arc1<input_arc||input_arc<0))
		    {
		      input_arc=arc1;
		      input_crossing=n;
		      input_pos=i;		  
		    }
		  if(arc1>max_arc||max_arc<0)
		    max_arc=arc1;
		}
	      
	    }
	}
    }

  if(input_crossing<0)//no non-endpoint crossing => empty gauss code
    {
      file<<"empty_diagram";
      if(flag_one_line==false)
	file<<endl;
      return;
    }
  //follow the curve
  int output_pos;
  int output_arc;
  int output_crossing;
  vector<string> crossing_signs;
  set<int> exterior_arcs;
  map<int,int> map_crossing_index_to_output_index;
  map<int,int> map_arc_index_to_output_index;
  vector<int> crossing_passage_count(crossings.size(),0);
  int output_index;
  while(true)
    {
      output_pos=(input_pos+2)%4;
      output_arc=crossings[input_crossing].get_arc(output_pos);
      output_crossing=crossings[input_crossing].get_connected_crossing(output_pos);
      if(output_arc<0||output_crossing<0)
	break;
      crossing_passage_count[input_crossing]++;
      if(crossing_passage_count[input_crossing]>2)
	break;

      map<int,int>::iterator it;
      it =map_crossing_index_to_output_index.find(input_crossing);
      if(it==map_crossing_index_to_output_index.end())
	{
	  output_index=map_crossing_index_to_output_index.size();
	  map_crossing_index_to_output_index[input_crossing]=output_index;
	  if(crossings[input_crossing].arc1==max_arc&&crossings[input_crossing].arc3==min_arc)
	    {
		crossing_signs.push_back("-");
	    }
	  else if(crossings[input_crossing].arc1==min_arc&&crossings[input_crossing].arc3==max_arc)
	    {
		crossing_signs.push_back("+");
	    }
	  else
	    {
	      if(crossings[input_crossing].arc1<crossings[input_crossing].arc3)
		crossing_signs.push_back("-");
	      else if(crossings[input_crossing].arc1>crossings[input_crossing].arc3)
		crossing_signs.push_back("+");
	      else
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR invalid diagram"<<endl;
		  cerr<<"*********************************************************"<<endl;
		  exit(1);
		}
	    }
	}
      it =map_arc_index_to_output_index.find(input_arc);
      if(it==map_arc_index_to_output_index.end())
	{
	  output_index=map_arc_index_to_output_index.size();
	  map_arc_index_to_output_index[input_arc]=output_index;
	}
      //exterior arcs
      if(crossings[input_crossing].get_region(input_pos,(input_pos+1)%4)==1||crossings[input_crossing].get_region(input_pos,(input_pos+3)%4)==1)
	 exterior_arcs.insert(map_arc_index_to_output_index[input_arc]);

      //outgoing arc
      it =map_arc_index_to_output_index.find(output_arc);
      if(it==map_arc_index_to_output_index.end())
	{
	  output_index=map_arc_index_to_output_index.size();
	  map_arc_index_to_output_index[output_arc]=output_index;
	}

      //exterior arcs
      if(crossings[input_crossing].get_region((input_pos+2)%4,(input_pos+1)%4)==1||crossings[input_crossing].get_region((input_pos+2)%4,(input_pos+3)%4)==1)
	 exterior_arcs.insert(map_arc_index_to_output_index[output_arc]);
	
      if(input_pos%2==0)//below
	file<<"-";
      file<<map_crossing_index_to_output_index[input_crossing]+1<<" ";// 1 based indexing

      //next crossing
      input_crossing=output_crossing;
      input_arc=output_arc;
      input_pos=-1;
      for(int i=0;i<4;i++)
	{
	  int arc1=crossings[input_crossing].get_arc(i);
	  int arc2=crossings[input_crossing].get_arc((i+2)%4);
	  if(arc1==input_arc&&(arc2==arc1+1||(arc1==max_arc&&arc2==min_arc)))
	    {
	      input_pos=i;
	      break;
	    }
	}
      if(input_pos<0)
	break;
    }

  for(int i=0;i<crossing_signs.size();i++)
    file<<crossing_signs[i];

  for(set<int>::iterator it=exterior_arcs.begin();it!=exterior_arcs.end();it++)
    file<<" "<<(*it);

  if(flag_one_line==false)
    file<<endl;

}

//---------------------------------------------------------------------------
void PlanarDiagram::save_to_file_xyz(string filename,int smooth,long nb_iterations_relaxation,double distance_ratio_threshold,double temperature_start,double temperature_end,bool check_balanced,bool flag_debug_info,int nb_additional_nodes_per_arc_noloop,int nb_additional_nodes_per_arc_empty_loop,int nb_additional_nodes_per_arc_enclosing_loop)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_xyz(cout,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,flag_debug_info,nb_additional_nodes_per_arc_noloop,nb_additional_nodes_per_arc_empty_loop,nb_additional_nodes_per_arc_enclosing_loop);
  else if(filename=="stderr")
    save_to_file_xyz(cerr,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,flag_debug_info,nb_additional_nodes_per_arc_noloop,nb_additional_nodes_per_arc_empty_loop,nb_additional_nodes_per_arc_enclosing_loop);
  else
    {
      ofstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR problem with file "<<filename <<"."<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      save_to_file_xyz(file,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,flag_debug_info,nb_additional_nodes_per_arc_noloop,nb_additional_nodes_per_arc_empty_loop,nb_additional_nodes_per_arc_enclosing_loop);
      file.close();
    }
}

//---------------------------------------------------------------------------


void PlanarDiagram::save_to_file_xyz(ostream& file,int smooth,long nb_iterations_relaxation,double distance_ratio_threshold,double temperature_start,double temperature_end,bool check_balanced,bool flag_debug_info,int nb_additional_nodes_per_arc_noloop,int nb_additional_nodes_per_arc_empty_loop,int nb_additional_nodes_per_arc_enclosing_loop)
{
  PlanarGraph graph=get_planar_graph(nb_additional_nodes_per_arc_noloop,nb_additional_nodes_per_arc_empty_loop,nb_additional_nodes_per_arc_enclosing_loop);

  graph.save_to_file_xyz(file,smooth,nb_iterations_relaxation,distance_ratio_threshold,temperature_start,temperature_end,check_balanced,flag_debug_info);
}


//---------------------------------------------------------------------------
void PlanarDiagram::load_from_file_KnotTheory(string filename)
{
  if(filename=="stdin"||filename=="-")
    load_from_file_KnotTheory(cin);
  else
    {
      ifstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR problem with file "<<filename <<"."<<endl;
	  cerr<<"Does the file exist?"<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      load_from_file_KnotTheory(file);
      file.close();
    }
}

//---------------------------------------------------------------------------

void PlanarDiagram::load_from_file_KnotTheory(istream& file)
{
  stringstream file_str;
  string str;
  file_str<<file.rdbuf();
  str=file_str.str();
  load_from_string_KnotTheory(str);
}
//---------------------------------------------------------------------------

void PlanarDiagram::load_from_string_KnotTheory(string str)
{
  crossings.clear();
  vector<int> exterior_arcs;
  int minarc=-1;
  int minarc_count=0;
  int maxarc=-1;
  int maxarc_count=0;

  //clean: erase ignored char
  string toremove=" \t;\n\r";
  string::size_type pos1;
  pos1=str.find_first_of(toremove);
   while(pos1 != string::npos)
     {
       str.erase(pos1,1);
       pos1=str.find_first_of(toremove);
     }
   
  if(str.size()==0)
    {
      cerr<<"WARNING: empty diagram."<<endl;
    }
  else if(str=="Loop[1]"||str=="PD[]")
    {
      //empty diagram
    }
  else
    {
      //check that it starts with PD[
      if(str.substr (0,3)!="PD[")
	{
	  cerr<<"ERROR: invalid PD file format (does not start with \"PD[\")."<<endl;
	  exit(1);
	}
      
      
      int flag_status=0;//0=outside X[], 1=just passed "X" (i.e. inside X[] ),2 just passed "r" (i.e. inside X[r[]])
      int brace_level=0;//
      Crossing crossing_tmp;
      int arcpos=0;
      for(int i=0;i<str.size();i++)
	{
	  if(flag_debug)cerr<<"i="<<i<<" str[i]="<<str[i]<<" flag_status="<<flag_status<<" brace_level="<<brace_level<<" arcpos="<<arcpos<<endl;
	  if(str[i]=='X')
	    {
	      //new crossing
	      flag_status=1;
	      arcpos=0;
	    }
	  else if(str[i]=='r')
	    {
	      if(flag_status==1)
		{
		  flag_status=2;
		}
	      else
		{
		  cerr<<"ERROR: invalid PD file format (r[] outside of X[])."<<endl;
		  exit(1);
		}
	    }
	  else if(str[i]=='[')
	    {
	      brace_level++;
	      if(flag_status==1&&brace_level==2)//entering X[]
		arcpos=0;
	    }
	  else if(str[i]==']')
	    {
	      if(flag_status==1&&brace_level==2)//leaving X[]
		{
		  crossings.push_back(crossing_tmp);
		  arcpos=0;
		}
	      brace_level--;
	      flag_status--;
	    }
	  else if(str[i]==',')
	    {
	      if(flag_status==1&&brace_level==2)//inside X[]
		arcpos++;
	    }
	  else if(str[i]=='P'||str[i]=='D')
	    {
	      //ignore
	    }
	  else//try to extract arc number
	    {
	      if((flag_status==1&&brace_level==2)||(flag_status==2&&brace_level==3))//inside X[] or X[r[]]
		{
		  size_t pos=str.find_first_of(",]",i);
		  if(pos==string::npos)
		    {
		      cerr<<"ERROR: invalid PD file format."<<endl;
		      exit(1);
		    }		      
		  string strtmp=str.substr(i,pos-i);
		  //check only 0-9 chars
		  if(string::npos!=strtmp.find_first_not_of("0123456789"))
		    {
		      cerr<<"ERROR: invalid PD file format (invalid arc: "<<strtmp<<")."<<endl;
		      exit(1);
		    }		      
		  
		  char* p_end;
		  int arc=strtoul(strtmp.c_str(),&p_end,10);
		  if(p_end-strtmp.c_str()!=strtmp.size())//strtmp is not an integer
		    {
		      cerr<<"ERROR: invalid PD file format (invalid arc: "<<strtmp<<")."<<endl;
		      exit(1);
		    }
		  //found a valid arc number 
		  crossing_tmp.set_arc(arcpos,arc);
		  if(flag_status==2&&brace_level==3)//inside r[]
		    exterior_arcs.push_back(arc);
		  if(minarc<0||arc<minarc)
		    {
		      minarc=arc;
		      minarc_count=1;
		    }
		  else if(arc==minarc)
		    {
		      minarc_count++;
		    }
		  if(maxarc<0||arc>maxarc)
		    {
		      maxarc=arc;
		      maxarc_count=1;
		    }
		  else if(arc==maxarc)
		    {
		      maxarc_count++;
		    }
		  i=pos-1;

		}
	      else
		{
		  cerr<<"ERROR: invalid PD file format."<<endl;
		  exit(1);
		}
	    }
	}

      if(minarc_count==1&&maxarc_count==1)//not cyclic => add endpoints
	{
	  crossings.insert(crossings.begin(),Crossing(minarc));
	  crossings.push_back(Crossing(maxarc));
	}
    }
  //for the very special case (ill defined): crossing X[1,0,0,1] => replace by X[0,1,1,0]
  if(crossings.size()==1)
    {
      if(crossings[0].arc0==1&&crossings[0].arc1==0&&crossings[0].arc2==0&&crossings[0].arc3==1)
	{
	  cerr<<"WARNING: PD[X[1,0,0,1]] is not valid. Replacing by PD[X[0,1,1,0]]"<<endl;
	  crossings[0].arc0=0;
	  crossings[0].arc1=1;
	  crossings[0].arc2=1;
	  crossings[0].arc3=0;
	}
    }

  if(flag_planar&&exterior_arcs.size()==0&&crossings.size()>0)
    {
      cerr<<"ERROR for planar knot(oid) diagrams, at least one arc must be on the exterior (r[])."<<endl;
      exit(1);
    }
  if(flag_debug)
    {
      cerr<<"PlanarDiagram::load_from_string_KnotTheory():"<<endl;
      save_to_file_list(cerr);
    }

  initialize();

  if(flag_planar&&exterior_arcs.size()>0)
    {
      //check each arc in exterior_arcs appears exactly twice, except endpoints
      //for endpoint, add them again to exterior boundary
      vector<int> count_arc_is_exterior(maxarc+1,0);//count_arc_is_exterior[arc]=nb times arc appears as exterior (region=1)
      for(int n=0;n<exterior_arcs.size();n++)
	{
	  count_arc_is_exterior[exterior_arcs[n]]++;
	}
      for(int n=0;n<count_arc_is_exterior.size();n++)
	{
	  if(count_arc_is_exterior[n]!=2&&count_arc_is_exterior[n]!=0)
	    {
	      if((n==minarc||n==maxarc)&&count_arc_is_exterior[n]==1)//endpoint arc
		{
		  //add the endpoint again, so that it appears twice, like all other arcs
		  exterior_arcs.push_back(n);
		  count_arc_is_exterior[n]=2;
		}
	      else
		{
		  cerr<<"ERROR: arc "<<n<<" not always within r[]."<<endl;
		  exit(1);
		}
	    }
	}
      create_regions();
      if(flag_debug)
	{
	  cerr<<"PlanarDiagram::load_from_string_KnotTheory() after create regions:"<<endl;
	  save_to_file_list(cerr);
	}

      vector<int> nb_exterior_arcs;//nb_exterior_arcs[region]=nb exterior arc that appears in region 
      vector<int> nb_interior_arcs;//nb_interior_arcs[region]=nb interior arc that appears in region 
      
      //find region corresponding to exterior_arcs
      for(int n=0;n<crossings.size();n++)
	{
	  int multiplicity=1;
	  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
	    continue;
	  //multiplicity=2;//endpoint are counted twice, since only one connected to one region
	  
	  if(flag_debug)cerr<<"n="<<n<<endl;
	  for(int i=0;i<4;i++)
	    {
	      int arc=crossings[n].get_arc(i);
	      if(flag_debug)cerr<<" arc="<<arc<<endl;
	      if(arc>=0)
		{
		  int region1=crossings[n].get_region(i,(i+1)%4);
		  int region2=crossings[n].get_region(i,(i+3)%4);
		  if(flag_debug)cerr<<" region1="<<region1<<endl;
		  if(flag_debug)cerr<<" region2="<<region2<<endl;
		  if(region1>=0&&region1>=nb_exterior_arcs.size())nb_exterior_arcs.resize(region1+1,0);
		  if(region1>=0&&region1>=nb_interior_arcs.size())nb_interior_arcs.resize(region1+1,0);
		  if(region2>=0&&region2>=nb_exterior_arcs.size())nb_exterior_arcs.resize(region2+1,0);
		  if(region2>=0&&region2>=nb_interior_arcs.size())nb_interior_arcs.resize(region2+1,0);
		  if(count_arc_is_exterior[arc]>0)
		    {
		      if(flag_debug&&region1>=0)cerr<<" region  "<<region1<<" is exterior (mult "<<multiplicity<<")"<<endl;
		      if(flag_debug&&region2>=0)cerr<<" region  "<<region2<<" is exterior (mult "<<multiplicity<<")"<<endl;
		      if(region1>=0)nb_exterior_arcs[region1]+=multiplicity;
		      if(region2>=0)nb_exterior_arcs[region2]+=multiplicity;
		    }
		  else
		    {
		      if(flag_debug&&region1>=0)cerr<<" region  "<<region1<<" is interior (mult "<<multiplicity<<")"<<endl;
		      if(flag_debug&&region2>=0)cerr<<" region  "<<region2<<" is interior (mult "<<multiplicity<<")"<<endl;
		      if(region1>=0)nb_interior_arcs[region1]+=multiplicity;
		      if(region2>=0)nb_interior_arcs[region2]+=multiplicity;
		    }
		}
	    }
	}
      int exterior_region=-1;
      for(int r=0;r<nb_exterior_arcs.size();r++)
	{
	  if(nb_exterior_arcs[r]>=exterior_arcs.size()&&nb_interior_arcs[r]==0)
	    {
	      if(exterior_region<0)
		{
		  exterior_region=r;
		}
	      else
		{
		  cerr<<"ERROR: more than one exterior regions."<<endl;
		  exit(1);
		}	      
	    }
	      
	  if(flag_debug)cerr<<" nb_exterior_arcs["<<r<<"]="<<nb_exterior_arcs[r]<<" exterior_arcs.size()="<<exterior_arcs.size()<<endl;
	  if(flag_debug)cerr<<" nb_interior_arcs["<<r<<"]="<<nb_interior_arcs[r]<<" exterior_arcs.size()="<<exterior_arcs.size()<<endl;
	}
      if(exterior_region<0)
	{
	  cerr<<"ERROR: could not find exterior regions (mandatory for planar knot(oid) diagrams)."<<endl;
	  exit(1);
	}
      //update region
      for(int n=0;n<crossings.size();n++)
	{
	  for(int i=0;i<4;i++)
	    {
	      if(crossings[n].get_region(i,(i+1)%4)==exterior_region)
		crossings[n].set_region(i,(i+1)%4,1);
	    }
	}
      
    }
  clean();
  if(!check())
    {
      if(flag_debug)
	save_to_file_list(cerr);
      throw range_error("knot(oid) diagram not valid.");
    }

  
}

//---------------------------------------------------------------------------
void PlanarDiagram::load_from_file_extended_gauss_code(string filename,bool flag_cyclic)
{
  if(filename=="stdin"||filename=="-")
    load_from_file_extended_gauss_code(cin,flag_cyclic);
  else
    {
      ifstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR: problem with file "<<filename <<"."<<endl;
	  cerr<<"Does the file exist?"<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      load_from_file_extended_gauss_code(file,flag_cyclic);
      file.close();
    }
}

//---------------------------------------------------------------------------

void PlanarDiagram::load_from_file_extended_gauss_code(istream& file,bool flag_cyclic)
{
  stringstream file_str;
  string str;
  file_str<<file.rdbuf();
  str=file_str.str();
  load_from_string_extended_gauss_code(str,flag_cyclic);
}
//---------------------------------------------------------------------------

//convert to PD code and call load_from_string_KnotTheory()
void PlanarDiagram::load_from_string_extended_gauss_code(string str,bool flag_cyclic)
{
  //clean: replace [](){},;\t\n\r by " "
  string toremove="[](){},\t;\n\r";
  string::size_type pos1;
  pos1=str.find_first_of(toremove);
   while(pos1 != string::npos)
     {
       str[pos1]=' ';
       pos1=str.find_first_of(toremove);
     }

  //insert " " between consecutive + or -
  string::size_type pos2=string::npos;
  pos1=str.find_first_of("+-");
  if(pos1 != string::npos)
      pos2=str.find_first_of("+-",pos1+1);
  while(pos2 != string::npos)
    {
      if(pos2-pos1==1)
	{
	  str.insert(pos2," ");
	  pos1=pos2+1;
	}
      else
	  pos1=pos2;
      pos2=str.find_first_of("+-",pos1+1);
    }

  stringstream sstr(str);
  int flag_status=0;//0=crossing passages, =1 crossing signs, =2 exterior arcs 
  vector<int> crossing_passages_label;//contain the crossing label
  vector<int> crossing_passages_sign;//contain the sign (-=below, += above)
  vector<int> crossing_signs;
  vector<int> exterior_arcs;
  map<int,int> crossing_label_to_index;//label in the gauss code, index in the vector crossings (before adding endpoints).
  int label,last_first_passage_label=-1;
  while(sstr.good())
    {
      string tmp;
      sstr>>tmp;
      if(tmp=="")
	  continue;
      if(tmp=="empty_diagram")
	{
	  if(!flag_cyclic)//add endpoints
	    {
	      crossings.push_back(Crossing());
	      crossings.push_back(Crossing());
	      crossings[0].arc0=0;
	      if(flag_planar)
		crossings[0].region01=1;
	      crossings[0].connected_crossing0=1;
	      crossings[1].arc0=0;
	      if(flag_planar)
		crossings[1].region01=1;
	      crossings[1].connected_crossing0=0;

	    }
	  return;
	}
      if(tmp=="+"||tmp=="-")
	{
	  flag_status=1;
	}
      if(flag_status==1&&(tmp!="+"&&tmp!="-"))
	{
	  flag_status=2;
	}

      if(flag_status==0)
	{	  
	  if(tmp[0]=='-'||tmp[0]=='+')
	    {
	      //split in sign/label
	      label=atoi(tmp.substr(1).c_str());
	      crossing_passages_label.push_back(label);
	      crossing_label_to_index[label]++;//=0
	      if(tmp[0]=='-')
		crossing_passages_sign.push_back(-1);
	      else
		crossing_passages_sign.push_back(+1);
	    }
	  else
	    {
	      label=atoi(tmp.c_str());
	      crossing_passages_label.push_back(label);
	      crossing_label_to_index[label]++;//=0;
	      crossing_passages_sign.push_back(+1);
	    }
	  if(crossing_label_to_index[label]==1)//first passage
	    {
	      if(!(last_first_passage_label<label))
		{
		  throw range_error("crossings must labeled with strictly increasing numbers when following the curve.");
		}
	      last_first_passage_label=label;
	    }
	     
	}
      else if(flag_status==1)
	{
	  if(tmp=="-")
	    crossing_signs.push_back(-1);
	  else
	    crossing_signs.push_back(+1);
	}
      else if(flag_status==2)
	{
	  exterior_arcs.push_back(atoi(tmp.c_str()));
	}
    }

  if(flag_planar&&exterior_arcs.size()==0&&crossing_passages_label.size()>0)
    {
      cerr<<"ERROR: for planar knot(oid) diagrams, at least one arc must be on the exterior."<<endl;
      exit(1);
    }

  //quick checks:
  int nb_crossings=crossing_signs.size();
  if(crossing_passages_label.size()!=2*nb_crossings)
    {
      cerr<<"ERROR: invalid extended gauss code file format."<<endl;
      exit(1);
    }

  //map crossing to index
  int n=0;
  for(map<int,int>::iterator it=crossing_label_to_index.begin();it!=crossing_label_to_index.end();it++)
    {
      it->second=n;
      n++;
    }

  vector<bool> arc_is_exterior;
  if(flag_cyclic)    
    arc_is_exterior.assign(2*nb_crossings,false);
  else
    arc_is_exterior.assign(2*nb_crossings+1,false);
  for(int i=0;i<exterior_arcs.size();i++)
    {
      if(exterior_arcs[i]>=arc_is_exterior.size())
	{
	  cerr<<"ERROR: invalid extended gauss code file format (arc index too high)."<<endl;
	  exit(1);
	}
      arc_is_exterior[exterior_arcs[i]]=true;
    }

  if(crossing_passages_label.size()==0)
    {
      cerr<<"WARNING: empty diagram."<<endl;
    }
  else
    {
      //build diagram
      vector<Crossing> crossings_tmp(nb_crossings);
      int arc=0;
      for(int i=0;i<crossing_passages_label.size();i++)
	{
	  int crossing_index=crossing_label_to_index[crossing_passages_label[i]];
	  if(crossing_passages_sign[i]<0)//below =>
	    {
	      crossings_tmp[crossing_index].set_arc(0,arc);
	      arc++;
	      if(flag_cyclic&&i==crossing_passages_label.size()-1)
		arc=0;
	      crossings_tmp[crossing_index].set_arc(2,arc);
	    }
	  else//above
	    {
	      if(crossing_signs[crossing_index]>0)
		{
		  crossings_tmp[crossing_index].set_arc(3,arc);
		  arc++;
		  if(flag_cyclic&&i==crossing_passages_label.size()-1)
		    arc=0;
		  crossings_tmp[crossing_index].set_arc(1,arc);
		}
	      else
		{
		  crossings_tmp[crossing_index].set_arc(1,arc);
		  arc++;
		  if(flag_cyclic&&i==crossing_passages_label.size()-1)
		    arc=0;
		  crossings_tmp[crossing_index].set_arc(3,arc);
		}
	    }
	}

      stringstream PD_code;
      PD_code<<"PD[";
      string sep1="";
      for(int n=0;n<crossings_tmp.size();n++)
	{
	  string sep2="";
	  PD_code<<sep1<<"X[";
	  for(int i=0;i<4;i++)
	    {
	      int a=crossings_tmp[n].get_arc(i);
	      if(a<0||a>=arc_is_exterior.size())
		{
		  cerr<<"ERROR: invalid extended gauss code file format (arc index "<<a<<")."<<endl;
		  exit(1);
		}
	      if(arc_is_exterior[a])
		PD_code<<sep2<<"r["<<a<<"]";
	      else
		PD_code<<sep2<<a;
	      sep2=",";
	    } 
	  PD_code<<"]";
	  sep1=",";
	}
      PD_code<<"]";

      load_from_string_KnotTheory(PD_code.str());

    }

}
//---------------------------------------------------------------------------


PlanarGraph PlanarDiagram::get_planar_graph(int nb_additional_nodes_per_arc_noloop,int nb_additional_nodes_per_arc_empty_loop,int nb_additional_nodes_per_arc_enclosing_loop)
{
 if(nb_additional_nodes_per_arc_empty_loop<1)
    {
      if(flag_debug)
	cerr<<"WARNING PlanarDiagram::get_planar_graph(): forcing nb_additional_nodes_per_arc_empty_loop=1"<<endl;
      nb_additional_nodes_per_arc_empty_loop=1;
    }
  if(nb_additional_nodes_per_arc_enclosing_loop<1)
    {
      if(flag_debug)
	cerr<<"WARNING PlanarDiagram::get_planar_graph(): forcing nb_additional_nodes_per_arc_enclosing_loop=1"<<endl;
      nb_additional_nodes_per_arc_enclosing_loop=1;
    }
  PlanarGraph graph(is_cyclic(),flag_debug);
  clean();
  create_regions();
  int exterior_region=1;
  if(!flag_planar)//choose regions with highest number of crossing 
    {
      map<int,int> map_region_to_nbcrossings;
      for(int n=0;n<crossings.size();n++)
	{
	  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// removed
	    continue;
	  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint ignore
	    continue;	  
	  for(int i=0;i<4;i++)    
	    {
	      if(crossings[n].get_arc(i)>=0)
		{
		  map_region_to_nbcrossings[crossings[n].get_region(i,(i+1)%4)]++;
		}
	      
	    }
	}
      int max_nbcrossings=-1;
      exterior_region=2;
      for(map<int,int>::iterator it=map_region_to_nbcrossings.begin();it!=map_region_to_nbcrossings.end();it++)
	{
	  if(it->second > max_nbcrossings)
	    {
	      max_nbcrossings=it->second ;
	      exterior_region=it->first;
	    }

	}
    }


  int minarcindex=-1;
  int maxarcindex=0;
  set<int> arcs;
  for(int j=0;j<crossings.size();j++)
    for(int i=0;i<4;i++)    
      {
	maxarcindex=max(maxarcindex,crossings[j].get_arc(i));
	if(crossings[j].get_arc(i)>=0)
	  {
	    arcs.insert(crossings[j].get_arc(i));
	    if(minarcindex<0)
	      minarcindex=crossings[j].get_arc(i);
	    else
	      minarcindex=min(minarcindex,crossings[j].get_arc(i));
	  }
      }
  int nbarcs=arcs.size();

  //note: edges are listed in counter-clockwise order
  stringstream node1_name,node2_name;
  stringstream node1_label,node2_label;
  int draw_order;
  int max_nb_segments_per_arcs=2*max(nb_additional_nodes_per_arc_empty_loop,max(nb_additional_nodes_per_arc_enclosing_loop,nb_additional_nodes_per_arc_noloop))+1;
  if(max_nb_segments_per_arcs==1)max_nb_segments_per_arcs=2;
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// removed
	continue;
      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint ignore
	continue;

      bool flag_crossing_exterior=false;//if crossing n is exterior
      for(int i=0;i<4;i++)
	if(crossings[n].get_region(i,(i+1)%4)==exterior_region)
	  flag_crossing_exterior=true;

      //search for self loops
      vector<bool> arc_loop(4,false);// if(arc_loop[i]) => arc form a self loop
      vector<bool> arc_enclosing_loop(4,false);// if(arc_loop[i]) => arc form a self loop enclosing the diagram
      for(int i=0;i<4;i++)
	for(int j=i+1;j<4;j++)
	  {
	    if(crossings[n].get_arc(i)==crossings[n].get_arc(j))
	      {
		if(((i+1)%4==j||(j+1)%4==i)&&crossings[n].get_region(i,j)==exterior_region)
		  {
		    arc_enclosing_loop[i]=true;
		    arc_enclosing_loop[j]=true;
		  }
		else
		  {
		    arc_loop[i]=true;
		    arc_loop[j]=true;
		  }
	      }
	  }
      ///Crossings - arcs
      for(int i=0;i<4;i++)
	{
	  int nb_additional_nodes_on_arc=nb_additional_nodes_per_arc_noloop;
	  if(arc_loop[i])
	    nb_additional_nodes_on_arc=nb_additional_nodes_per_arc_empty_loop;
	  else if(arc_enclosing_loop[i])
	    nb_additional_nodes_on_arc=nb_additional_nodes_per_arc_enclosing_loop;

	  bool flag_arc_exterior=false;
	  if(crossings[n].get_region(i,(i+1)%4)==exterior_region||crossings[n].get_region(i,(i+3)%4)==exterior_region)
	    flag_arc_exterior=true;
	  bool flag_above=false;
	  if(i%2==1)
	    flag_above=true;
	  if(nbarcs==2)
	    {
	      if(crossings[n].get_arc(1)!=crossings[n].get_arc(2)&&(i==1||i==2))
		draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i); 
	      else if(crossings[n].get_arc(2)!=crossings[n].get_arc(3)&&(i==2||i==3))
		draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i); 
	      else
		draw_order=-1;	      
	    }
	  else
	    {
	      if((crossings[n].get_arc(i)>crossings[n].get_arc((i+2)%4) && !(crossings[n].get_arc(i)==maxarcindex&&crossings[n].get_arc((i+2)%4)==minarcindex) )||(crossings[n].get_arc(i)==minarcindex&&crossings[n].get_arc((i+2)%4)==maxarcindex)) 
		draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i);
	      else
		draw_order=-1;
	    }
	  
	  //crossing-arc
	  node1_name.str("");
	  node2_name.str("");
	  node1_name<<"Crossing_"<<n;
	  if(nb_additional_nodes_on_arc>0)
	    node2_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_0";
	  else
	    node2_name<<"Arc_"<<crossings[n].get_arc(i);
	  node1_label.str("");
	  node2_label.str("");
	  node1_label<<"Crossing_"<<n;
	  node2_label<<"Arc_"<<crossings[n].get_arc(i);
	  if(flag_above)	    
	    graph.add_edge(node1_name.str(),node1_label.str(),flag_crossing_exterior,1,node2_name.str(),node2_label.str(),flag_arc_exterior,0,draw_order);
	  else
	    graph.add_edge(node1_name.str(),node1_label.str(),flag_crossing_exterior,-1,node2_name.str(),node2_label.str(),flag_arc_exterior,0,draw_order);

	}
      ///Arcs - crossings/arcs
      for(int i=0;i<4;i++)
	{
	  int nb_additional_nodes_on_arc=nb_additional_nodes_per_arc_noloop;
	  if(arc_loop[i])
	    nb_additional_nodes_on_arc=nb_additional_nodes_per_arc_empty_loop;
	  else if(arc_enclosing_loop[i])
	    nb_additional_nodes_on_arc=nb_additional_nodes_per_arc_enclosing_loop;

	  int m=crossings[n].get_connected_crossing(i);

	  bool flag_connected_to_endpoint=false;//connected to an endpoint?
	  if(crossings[m].arc0>=0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)//connected to endpoint
	    flag_connected_to_endpoint=true;

	  bool flag_arc_exterior=false;
	  if(crossings[n].get_region(i,(i+1)%4)==exterior_region||crossings[n].get_region(i,(i+3)%4)==exterior_region)
	    flag_arc_exterior=true;

	  //arc -crossing
	  node1_name.str("");
	  node2_name.str("");
	  if(nb_additional_nodes_on_arc>0)
	    node1_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_0";
	  else
	    node1_name<<"Arc_"<<crossings[n].get_arc(i);
	  node2_name<<"Crossing_"<<n;
	  node1_label.str("");
	  node2_label.str("");
	  node1_label<<"Arc_"<<crossings[n].get_arc(i);
	  node2_label<<"Crossing_"<<n;
	  if(nbarcs==2)
	    {
	      if(crossings[n].get_arc(1)!=crossings[n].get_arc(2)&&(i==0||i==3))
		draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+max_nb_segments_per_arcs-1;//2; 
	      else if(crossings[n].get_arc(2)!=crossings[n].get_arc(3)&&(i==0||i==1))
		draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+max_nb_segments_per_arcs-1;//2; 	      
	      else
		draw_order=-1;	      
	    }
	  else
	    {
	      if((crossings[n].get_arc(i)<crossings[n].get_arc((i+2)%4) && !(crossings[n].get_arc(i)==minarcindex&&crossings[n].get_arc((i+2)%4)==maxarcindex) ) ||(crossings[n].get_arc(i)==maxarcindex&&crossings[n].get_arc((i+2)%4)==minarcindex)) 
		draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+max_nb_segments_per_arcs-1;//2;
	      else
		draw_order=-1;
	    }
	  bool flag_above=false;
	  if(i%2==1)
	    flag_above=true;

	  if(flag_above)
	    graph.add_edge(node1_name.str(),node1_label.str(),flag_arc_exterior,0,node2_name.str(),node2_label.str(),flag_crossing_exterior,1,draw_order);
	  else
	    graph.add_edge(node1_name.str(),node1_label.str(),flag_arc_exterior,0,node2_name.str(),node2_label.str(),flag_crossing_exterior,-1,draw_order);

	  //find position on crossing m
	  int j=0;
	  for(j=0;j<4;j++)
	    if(crossings[m].get_arc(j)==crossings[n].get_arc(i)&&crossings[n].get_region(i,(i+1)%4)==crossings[m].get_region(j,(j+3)%4)&&crossings[n].get_region(i,(i+3)%4)==crossings[m].get_region(j,(j+1)%4))
	      break;


	  //arc -arc (for split arc)
	  if(!flag_connected_to_endpoint)
	    {
	      if(nb_additional_nodes_on_arc>0)//connect arc-arc 
		{
		  for(int a=0;a<nb_additional_nodes_on_arc-1;a++)
		    {
		      //arc_(a) arc_(a+1)
		      node1_name.str("");
		      node2_name.str("");
		      node1_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_"<<a;
		      node2_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_"<<a+1;
		      node1_label.str("");
		      node2_label.str("");
		      node1_label<<"Arc_"<<crossings[n].get_arc(i);
		      node2_label<<"Arc_"<<crossings[n].get_arc(i);
		      if(nbarcs==2)
			{
			  if(crossings[n].get_arc(1)!=crossings[n].get_arc(2)&&(i==1||i==2))
			    draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+a+1; 
			  else if(crossings[n].get_arc(2)!=crossings[n].get_arc(3)&&(i==2||i==3))
			    draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+a+1; 
			  else
			    draw_order=-1;	      
			}
		      else
			{
			  if((crossings[n].get_arc(i)>crossings[n].get_arc((i+2)%4) && !(crossings[n].get_arc(i)==maxarcindex&&crossings[n].get_arc((i+2)%4)==minarcindex) )||(crossings[n].get_arc(i)==minarcindex&&crossings[n].get_arc((i+2)%4)==maxarcindex)) 
			    draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+a+1;
			  else
			    draw_order=-1;	     
			}
		      graph.add_edge(node1_name.str(),node1_label.str(),flag_arc_exterior,0,node2_name.str(),node2_label.str(),flag_arc_exterior,0,draw_order);
		      //arc_(a+1) arc_(a)
		      node1_name.str("");
		      node2_name.str("");
		      node1_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_"<<a+1;
		      node2_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_"<<a;
		      node1_label.str("");
		      node2_label.str("");
		      node1_label<<"Arc_"<<crossings[n].get_arc(i);
		      node2_label<<"Arc_"<<crossings[n].get_arc(i);
		      if(nbarcs==2)
			{
			  if(crossings[n].get_arc(1)!=crossings[n].get_arc(2)&&(i==1||i==2))
			    draw_order=-1;//draw_order=3*crossings[n].get_arc(i)+1; 
			  else if(crossings[n].get_arc(2)!=crossings[n].get_arc(3)&&(i==2||i==3))
			    draw_order=-1;//3*crossings[n].get_arc(i)+1; 
			  else
			    draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+max_nb_segments_per_arcs-(a+2);//1;//-1;	      
			}
		      else
			{
			  if((crossings[n].get_arc(i)>crossings[n].get_arc((i+2)%4) && !(crossings[n].get_arc(i)==maxarcindex&&crossings[n].get_arc((i+2)%4)==minarcindex) )||(crossings[n].get_arc(i)==minarcindex&&crossings[n].get_arc((i+2)%4)==maxarcindex)) 
			    draw_order=-1;	     
			  else
			    draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+max_nb_segments_per_arcs-(a+2);
			}
		      graph.add_edge(node1_name.str(),node1_label.str(),flag_arc_exterior,0,node2_name.str(),node2_label.str(),flag_arc_exterior,0,draw_order);
		    }
		  
		  //middle arc
		  node1_name.str("");
		  node2_name.str("");
		  node1_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<n<<"_"<<i<<"_"<<nb_additional_nodes_on_arc-1;
		  node2_name<<"Arc_"<<crossings[n].get_arc(i)<<"_"<<m<<"_"<<j<<"_"<<nb_additional_nodes_on_arc-1;
		  node1_label.str("");
		  node2_label.str("");
		  node1_label<<"Arc_"<<crossings[n].get_arc(i);
		  node2_label<<"Arc_"<<crossings[n].get_arc(i);
		  if(nbarcs==2)
		    {
		      if(crossings[n].get_arc(1)!=crossings[n].get_arc(2)&&(i==1||i==2))
			draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+nb_additional_nodes_on_arc;//1; 
		      else if(crossings[n].get_arc(2)!=crossings[n].get_arc(3)&&(i==2||i==3))
			draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+nb_additional_nodes_on_arc;//1; 
		      else
			draw_order=-1;	      
		    }
		  else
		    {
		      if((crossings[n].get_arc(i)>crossings[n].get_arc((i+2)%4) && !(crossings[n].get_arc(i)==maxarcindex&&crossings[n].get_arc((i+2)%4)==minarcindex) )||(crossings[n].get_arc(i)==minarcindex&&crossings[n].get_arc((i+2)%4)==maxarcindex)) 
			draw_order=max_nb_segments_per_arcs*crossings[n].get_arc(i)+nb_additional_nodes_on_arc;//1;
		      else
			draw_order=-1;	     
		    }
		  graph.add_edge(node1_name.str(),node1_label.str(),flag_arc_exterior,0,node2_name.str(),node2_label.str(),flag_arc_exterior,0,draw_order);
		}
	    }


	}
    }
  graph.triangulate();

  return graph;
}


//--------------------------------------------------------------------------
void PlanarDiagram::change_orientation()  
{
  if(flag_debug)
    cerr<<"inverting orientation"<<endl;
  map<int,int> arcs_map;
  map<int,int> crossings_map;
  vector<Crossing> crossings_new;
  //invert crossings & create arcs_map
  for(int n=crossings.size()-1;n>=0;n--)
    {
      for(int i=0;i<4;i++)
	{
	  int a=crossings[n].get_arc(i);
	  if(a>=0)
	    arcs_map[a]=a;
	}
      crossings_map[n]=crossings_new.size();
      crossings_new.push_back(crossings[n]);
    }
  map<int,int>::iterator it1=arcs_map.begin();
  map<int,int>::reverse_iterator it2=arcs_map.rbegin();
  while(it1!=arcs_map.end()&&it2!=arcs_map.rend())
    {
      it1->second=it2->first;
      it1++;
      it2++;
    }
  //add -1
  arcs_map[-1]=-1;
  //adapt crossings: change arc number and change ordering to keep arc0<arc2
  for(int n=0;n<crossings_new.size();n++)
    {
      if(crossings_new[n].arc0>=0&&crossings_new[n].arc1<0&&crossings_new[n].arc2<0&&crossings_new[n].arc3<0)//endpoint. do not change ordering
	{
	  crossings_new[n].set_arc(0,arcs_map[crossings_new[n].get_arc(0)]);
	  crossings_new[n].set_connected_crossing(0,crossings_map[crossings_new[n].get_connected_crossing(0)]);
	}
      else
	{
	  int a1=crossings_new[n].arc2;
	  int a2=crossings_new[n].arc3;
	  int a3=crossings_new[n].arc0;
	  int a4=crossings_new[n].arc1;
	  crossings_new[n].arc0=arcs_map[a1];
	  crossings_new[n].arc1=arcs_map[a2];
	  crossings_new[n].arc2=arcs_map[a3];
	  crossings_new[n].arc3=arcs_map[a4];	  
	  a1=crossings_new[n].connected_crossing2;
	  a2=crossings_new[n].connected_crossing3;
	  a3=crossings_new[n].connected_crossing0;
	  a4=crossings_new[n].connected_crossing1;
	  crossings_new[n].connected_crossing0=crossings_map[a1];
	  crossings_new[n].connected_crossing1=crossings_map[a2];
	  crossings_new[n].connected_crossing2=crossings_map[a3];
	  crossings_new[n].connected_crossing3=crossings_map[a4];	  
	  a1=crossings_new[n].region23;
	  a2=crossings_new[n].region30;
	  a3=crossings_new[n].region01;
	  a4=crossings_new[n].region12;
	  crossings_new[n].region01=a1;
	  crossings_new[n].region12=a2;
	  crossings_new[n].region23=a3;
	  crossings_new[n].region30=a4;
	}
    }
  crossings=crossings_new;
  if(!check())
    {
      if(flag_debug)
	save_to_file_list(cerr);
      throw range_error("knot(oid) diagram not valid.");
    }
  
}
//--------------------------------------------------------------------------
void PlanarDiagram::reorder_crossings()  
{
  if(crossings.size()<2)
    return;

  clean();//to be sure to have consecutive arcs numbered be steps of 1

  //find starting position (crossing with outgoing min arc index)
  int n1=-1;
  int i1=-1;
  int n2=-1;
  int i2=-1;
  int minarcindex=-1,maxarcindex=-1;
  for(int n=0;n<crossings.size();n++)
    {
      for(int i=0;i<4;i++)    
	{
	  if(crossings[n].get_arc(i)>=0)
	    {
	      if(maxarcindex<crossings[n].get_arc(i))
		maxarcindex=crossings[n].get_arc(i);
	      if(minarcindex<0)//initialization
		{
		  minarcindex=crossings[n].get_arc(i);
		  n1=n;
		  i1=i;
		}
	      else if(minarcindex>crossings[n].get_arc(i))//first crossing
		{
		  n1=n;
		  i1=i;
		  n2=n;
		  i2=i;
		}
	      else if(minarcindex==crossings[n].get_arc(i))//second crossing
		{
		  n2=n;
		  i2=i;
		}
	    }
	}
    }
  //choose n1 or n2 as first crossing
  int n0,i0;
  if(crossings[n1].arc0>=0&&crossings[n1].arc1<0&&crossings[n1].arc2<0&&crossings[n1].arc3<0)//n1 is endpoint
    {
      n0=n1;
      i0=i1;
    }
  else if(crossings[n2].arc0>=0&&crossings[n2].arc1<0&&crossings[n2].arc2<0&&crossings[n2].arc3<0)//n2 is endpoint
    {
      n0=n2;
      i0=i2;
    }
  else if(crossings[n1].get_arc(i1)<crossings[n1].get_arc((i1+2)%4))
    {
      n0=n1;
      i0=i1;
    }
  else if(crossings[n2].get_arc(i1)<crossings[n2].get_arc((i1+2)%4))
    {
      n0=n2;
      i0=i2;
    }
  else
    {
      cerr<<"*******************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::renumber_crossings()"<<endl;
      cerr<<"*******************************************"<<endl;
      exit(1); 
    }
  
  map<int,int> crossings_map;//crossings_map[n1]=n2 => old crossing n1 will be renamed to n2
  vector<Crossing> crossings_new;


  //follow the curve
  n1=n0;
  i1=i0;
  int arc;
  while(true)
    {
      //add crossing only if not yet discovered
      if(crossings_map.find(n1)==crossings_map.end())
	{
	  crossings_map[n1]=crossings_new.size();
	  crossings_new.push_back(crossings[n1]);
	}
      
      if(i1<0)//endpoint or error
	break;

      //get next crossing
      arc=crossings[n1].get_arc(i1);
      n2=crossings[n1].get_connected_crossing(i1);

      if(crossings[n2].arc0<0&&crossings[n2].arc1<0&&crossings[n2].arc2<0&&crossings[n2].arc3<0)//empty (removed) crossing
	break;//error

      i2=-1;
      for(int i=0;i<4;i++)    
	{	  
	  if(crossings[n2].arc0==arc&&crossings[n2].arc1<0&&crossings[n2].arc2<0&&crossings[n2].arc3<0)//endpoint
	    {
	      i2=-1;
	      break;
	    }
	  if(crossings[n2].get_arc(i)==arc&&crossings[n2].get_arc((i+2)%4)==arc+1)//normal
	    {
	      i2=(i+2)%4;
	      break;
	    }
	  if(crossings[n2].get_arc(i)==maxarcindex&&crossings[n2].get_arc((i+2)%4)==minarcindex)//
	    {
	      i2=(i+2)%4;
	      break;
	    }
	}
      if(n2==n0&&i2==i0)
	break;

      n1=n2;
      i1=i2;
    }

  if(crossings.size()!=crossings_new.size())
    {
      cerr<<"*******************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::renumber_crossings()"<<endl;
      cerr<<"*******************************************"<<endl;
      exit(1); 
    }

  
  for(int n=0;n<crossings_new.size();n++)
    {
      for(int i=0;i<4;i++)    
	{
	  crossings_new[n].set_connected_crossing(i,crossings_map[crossings_new[n].get_connected_crossing(i)]);
	}
    }
  crossings=crossings_new;

  if(!check())
    {
      cerr<<"**************************************"<<endl;
      cerr<<"ERROR knot(oid) diagram not valid!"<<endl;
      cerr<<"**************************************"<<endl;
      if(flag_debug)
	save_to_file_list(cerr);
      exit(1);
    }

    
}

//--------------------------------------------------------------------------
void PlanarDiagram::close(bool flag_overpass)  
{

  
  bool flag_cyclic=true;
  
  create_regions();	
  int minarcindex=-1;
  int maxarcindex=-1;
  int maxregion=-1;
  for(int n=0;n<crossings.size();n++)
    {
      for(int i=0;i<4;i++)
	{
	  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// removed
	    continue;
	  if(crossings[n].get_arc(i)>=0)
	    {
	      if(maxarcindex<crossings[n].get_arc(i))
		maxarcindex=crossings[n].get_arc(i);
	      if(minarcindex<0||minarcindex>crossings[n].get_arc(i))
		minarcindex=crossings[n].get_arc(i);
	      if(maxregion<crossings[n].get_region(i,(i+1)%4))
		maxregion=crossings[n].get_region(i,(i+1)%4);
	    }
	}
    }
	  

  //definitions for boost graph
  typedef adjacency_list < setS, vecS, undirectedS,NodeProperties_region, EdgeProperties_region > graph_t; //edges setS => no parallel edges
  typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;

  //create graph_region and find endpoints
  vector<vertex_descriptor> map_region_to_vertex;//map_region_to_vertex[r]=i => graph_regions[i].region=r
  graph_t graph_regions;
  vertex_descriptor vid;
  edge_descriptor eid;
  bool flag_new;
  int crossing_endpoint1=-1;//crossing crossing_endpoint1 is first endpoint
  int crossing_endpoint2=-1;//crossing crossing_endpoint2 is first endpoint
  int region_endpoint1=-1;//endpoint 1 is in region region_endpoint1
  int region_endpoint2=-1;//endpoint 2 is in region region_endpoint2
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// removed
	continue;
      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint => store region
	{
	  flag_cyclic=false;
	  if(crossings[n].arc0==maxarcindex)//first endpoint
	    {
	      crossing_endpoint2=n;
	      region_endpoint2=crossings[n].region01;
	    }
	  else
	    {
	      crossing_endpoint1=n;
	      region_endpoint1=crossings[n].region01;
	    }
	  continue;
	}

      //add vertex
      for(int i=0;i<4;i++)    
	{
	  int r=crossings[n].get_region(i,(i+1)%4);
	  if(map_region_to_vertex.size()<r+1)
	    map_region_to_vertex.resize(r+1,graph_traits<graph_t>::null_vertex());
	  if(map_region_to_vertex[r]==graph_traits<graph_t>::null_vertex())//does not exist yet
	    {
	      vid=add_vertex(graph_regions);
	      graph_regions[vid].region=r;
	      map_region_to_vertex[r]=vid;
	    }
	}
      //add edge
      for(int i=0;i<4;i++)
	{
	  int r1=crossings[n].get_region(i,(i+1)%4);
	  int r2=crossings[n].get_region(i,(i+3)%4);
	  if(r1!=r2&&crossings[n].get_arc(i)>crossings[n].get_arc((i+2)%4))//store only outgoing arcs (we only consider non cyclic graphs)
	    {
		  tie(eid,flag_new)=add_edge(map_region_to_vertex[r1],map_region_to_vertex[r2],graph_regions);
		  graph_regions[eid].arc=crossings[n].get_arc(i);
		  graph_regions[eid].crossing=n;
		  graph_regions[eid].pos=i;
		  graph_regions[eid].region=crossings[n].get_region(i,(i+1)%4);
	    }
	}
    }
  if(flag_cyclic)//nothing to do
    {
      return;
    }

  if(crossings.size()==2)//two endpoints
    {
      crossings.clear();
      return;
    }

    
  vector<vertex_descriptor> predecessors(num_vertices(graph_regions));
  breadth_first_search(graph_regions,map_region_to_vertex[region_endpoint1],visitor(make_bfs_visitor(record_predecessors(&predecessors[0],on_tree_edge()))));
  //reconstruct shortest path from endpoint2 to region_endpoint1
  vertex_descriptor v=map_region_to_vertex[region_endpoint2];
  vertex_descriptor vlast=graph_traits<graph_t>::null_vertex();
  int last_crossing_n=crossings[crossing_endpoint2].get_connected_crossing(0);
  int last_crossing_i;
  int last_maxarcindex=maxarcindex;
  int last_maxregion=maxregion;
  for(int i=0;i<4;i++)
    if(crossings[last_crossing_n].get_arc(i)==crossings[crossing_endpoint2].get_arc(0))
      {
	last_crossing_i=i;
	break;
      }
  while (v!=map_region_to_vertex[region_endpoint1])
    {
      vlast=v;
      v = predecessors[v];
      if(vlast!=graph_traits<graph_t>::null_vertex())
	{
	  pair<edge_descriptor, bool> e=edge(vlast,v,graph_regions);
	  if(e.second)
	    {
	      //add crossing
	      Crossing crossing;
	      int a=graph_regions[e.first].arc;
	      int n=graph_regions[e.first].crossing;
	      int i=graph_regions[e.first].pos;
	      int r=graph_regions[e.first].region;
	      //renumber arcs
	      for(int m=0;m<crossings.size();m++)
		for(int j=0;j<4;j++)
		  {
		    if(crossings[m].get_arc(j)<0)
		      continue;
		    if(crossings[m].get_arc(j)==crossings[n].get_arc(i)&&crossings[m].get_arc(j)<crossings[m].get_arc((j+2)%4))
		      crossings[m].set_arc(j,crossings[m].get_arc(j)+1);
		    else if(crossings[m].get_arc(j)>crossings[n].get_arc(i))
		      crossings[m].set_arc(j,crossings[m].get_arc(j)+1);
		  }
	      
	      if(flag_overpass)
		{
		  crossing.set_arc(0,crossings[n].get_arc(i));
		  crossing.set_connected_crossing(0,n);
		  crossing.set_arc(2,crossings[n].get_arc(i)+1);		  
		  crossing.set_connected_crossing(2,crossings[n].get_connected_crossing(i));
		  if(r==graph_regions[vlast].region)
		    {
		      crossing.set_arc(3,last_maxarcindex+1);		      
		      crossing.set_connected_crossing(3,last_crossing_n);
		      crossing.set_arc(1,last_maxarcindex+2);		      
		      crossing.set_connected_crossing(1,crossing_endpoint2);
		    }
		  else
		    {
		      crossing.set_arc(1,last_maxarcindex+1);		      
		      crossing.set_connected_crossing(1,last_crossing_n);
		      crossing.set_arc(3,last_maxarcindex+2);		      
		      crossing.set_connected_crossing(3,crossing_endpoint2);
		    }
		  crossing.set_region(0,1,crossings[n].get_region(i,(i+3)%4));
		  crossing.set_region(1,2,crossings[n].get_region(i,(i+3)%4));
		  crossing.set_region(2,3,crossings[n].get_region(i,(i+1)%4));
		  crossing.set_region(3,0,crossings[n].get_region(i,(i+1)%4));
		  //update neighboring crossings
		  for(int j=0;j<4;j++)
		    if(crossings[crossings[n].get_connected_crossing(i)].get_connected_crossing(j)==n&&crossings[crossings[n].get_connected_crossing(i)].get_arc(j)==crossings[n].get_arc(i)+1)
		      crossings[crossings[n].get_connected_crossing(i)].set_connected_crossing(j,crossings.size());
		  crossings[n].set_connected_crossing(i,crossings.size());
		  crossings[last_crossing_n].set_connected_crossing(last_crossing_i,crossings.size());
		  crossings[crossing_endpoint2].set_connected_crossing(0,crossings.size());
		  crossings[crossing_endpoint2].set_arc(0,crossings[crossing_endpoint2].get_arc(0)+1);

		}
	      else//underpass
		{
		  crossing.set_arc(0,last_maxarcindex+1);		      
		  crossing.set_connected_crossing(0,last_crossing_n);
		  crossing.set_arc(2,last_maxarcindex+2);		      
		  crossing.set_connected_crossing(2,crossing_endpoint2);
		  if(r==graph_regions[vlast].region)
		    {
		      crossing.set_arc(1,crossings[n].get_arc(i));
		      crossing.set_connected_crossing(1,n);
		      crossing.set_arc(3,crossings[n].get_arc(i)+1);		  
		      crossing.set_connected_crossing(3,crossings[n].get_connected_crossing(i));
		      crossing.set_region(0,1,crossings[n].get_region(i,(i+1)%4));
		      crossing.set_region(1,2,crossings[n].get_region(i,(i+3)%4));
		      crossing.set_region(2,3,crossings[n].get_region(i,(i+3)%4));
		      crossing.set_region(3,0,crossings[n].get_region(i,(i+1)%4));
		    }
		  else
		    {
		      crossing.set_arc(3,crossings[n].get_arc(i));
		      crossing.set_connected_crossing(3,n);
		      crossing.set_arc(1,crossings[n].get_arc(i)+1);		  
		      crossing.set_connected_crossing(1,crossings[n].get_connected_crossing(i));
		      crossing.set_region(0,1,crossings[n].get_region(i,(i+3)%4));
		      crossing.set_region(1,2,crossings[n].get_region(i,(i+1)%4));
		      crossing.set_region(2,3,crossings[n].get_region(i,(i+1)%4));
		      crossing.set_region(3,0,crossings[n].get_region(i,(i+3)%4));
		    }
		  //update neighboring crossings
		  for(int j=0;j<4;j++)
		    if(crossings[crossings[n].get_connected_crossing(i)].get_connected_crossing(j)==n&&crossings[crossings[n].get_connected_crossing(i)].get_arc(j)==crossings[n].get_arc(i)+1)
		      crossings[crossings[n].get_connected_crossing(i)].set_connected_crossing(j,crossings.size());
		  crossings[n].set_connected_crossing(i,crossings.size());
		  crossings[last_crossing_n].set_connected_crossing(last_crossing_i,crossings.size());
		  crossings[crossing_endpoint2].set_connected_crossing(0,crossings.size());
		  crossings[crossing_endpoint2].set_arc(0,crossings[crossing_endpoint2].get_arc(0)+1);

		}

	      crossings.push_back(crossing);
	      last_maxregion++;
	      last_maxarcindex+=2;
	      last_crossing_n=crossings.size()-1;
	      if(flag_overpass)
		{
		  if(r==graph_regions[vlast].region)
		    last_crossing_i=1;
		  else
		    last_crossing_i=3;
		}
	      else
		{
		  last_crossing_i=2;
		}

	      vector<vector<pair<int,int> > > regions(2);
	      int n0,i0,n1,i1,n2,i2;
	      for(int l=0;l<2;l++)
		{
		  if(l==0)
		    {
		      n0=last_crossing_n;
		      i0=(last_crossing_i+1)%4;
		    }
		  else
		    {
		      n0=last_crossing_n;
		      i0=(last_crossing_i+2)%4;
		    }
		  n1=n0;
		  i1=i0;
		  n2=n0;
		  i2=i0;
		  while(true)
		    {
		      regions[l].push_back(pair<int,int>(n1,i1));
		      n2=crossings[n1].get_connected_crossing(i1);
		      i2=-1;
		      for(i2=0;i2<4;i2++)
			{
			  if(crossings[n2].get_arc(i2)==crossings[n1].get_arc(i1)&&crossings[n2].get_connected_crossing(i2)==n1&&(n1!=n2||i1!=i2))
			    {
			      if(crossings[n2].arc0>=0&&crossings[n2].arc1<0&&crossings[n2].arc2<0&&crossings[n2].arc3<0)//endpoint
				{
				  i2=0;
				  break;				  
				}
			      else
				{
				  i2=(i2+3)%4;
				  break;
				}
			    }
			}
		      if(i2==-1)
			{
			  cerr<<"ERROR: i2==-1"<<endl;
			  exit(1);
			}
		      n1=n2;
		      i1=i2;
		      if(n1==n0&&i1==i0)
			break;
		    }
		}
	      //split region (rename smaller region, important when region is outside => keep largest region as outside)
	      r=0;
	      if(regions[1].size()<regions[0].size())
		r=1;
	      for(int l=0;l<regions[r].size();l++)
		crossings[regions[r][l].first].set_region(regions[r][l].second,(regions[r][l].second+1)%4,last_maxregion);
	    }
	  
	}
    }

  //remove endpoints
  int n01=crossings[crossing_endpoint1].get_connected_crossing(0);
  int i01=-1;
  for(int j=0;j<4;j++)
    if(crossings[n01].get_connected_crossing(j)==crossing_endpoint1)
      i01=j;
  int n02=crossings[crossing_endpoint2].get_connected_crossing(0);
  int i02=-1;
  for(int j=0;j<4;j++)
    if(crossings[n02].get_connected_crossing(j)==crossing_endpoint2)
      i02=j;
  crossings[n01].set_connected_crossing(i01,n02);
  crossings[n02].set_connected_crossing(i02,n01);
  crossings[n02].set_arc(i02,crossings[n01].get_arc(i01));
  for(int j=0;j<4;j++)
    {
      crossings[crossing_endpoint1].set_arc(j,-1);
      crossings[crossing_endpoint2].set_arc(j,-1);
    }
  //search for smaller region (rename smaller region, important when region is outside => keep largest region as outside)
  vector<vector<pair<int,int> > > regions(2);
  int n0,i0,n1,i1,n2,i2;
  for(int l=0;l<2;l++)
    {
      if(l==0)
	{
	  n0=n01;
	  i0=i01;
	}
      else
	{
	  n0=n02;
	  i0=i02;
	}
      n1=n0;
      i1=i0;
      n2=n0;
      i2=i0;
      while(true)
	{
	  regions[l].push_back(pair<int,int>(n1,i1));
	  n2=crossings[n1].get_connected_crossing(i1);
	  i2=-1;
	  for(i2=0;i2<4;i2++)
	    {
	      if(crossings[n2].get_arc(i2)==crossings[n1].get_arc(i1)&&crossings[n2].get_connected_crossing(i2)==n1&&(n1!=n2||i1!=i2))
		{
		  if(crossings[n2].arc0>=0&&crossings[n2].arc1<0&&crossings[n2].arc2<0&&crossings[n2].arc3<0)//endpoint
		    {
		      i2=0;
		      break;				  
		    }
		  else
		    {
		      i2=(i2+3)%4;
		      break;
		    }
		}
	    }
	  if(i2==-1)
	    {
	      cerr<<"ERROR: i2==-1"<<endl;
	      exit(1);
	    }
	  n1=n2;
	  i1=i2;
	  if(n1==n0&&i1==i0)
	    break;
	}
    }
  //split region (rename smaller region, important when region is outside => keep largest region as outside)
  last_maxregion++;
  int r=0;
  if(regions[1].size()<regions[0].size())
    r=1;
  for(int l=0;l<regions[r].size();l++)
    crossings[regions[r][l].first].set_region(regions[r][l].second,(regions[r][l].second+1)%4,last_maxregion);


  clean();
  //reorder to have get_arc(0)<get_arc(2)
  if(crossings.size()==1)
    {
      if(crossings[0].get_arc(0)>crossings[0].get_arc(2))
	{
	  Crossing crossing=crossings[0];
	  crossings[0].set_arc(0,crossing.get_arc(2));
	  crossings[0].set_arc(1,crossing.get_arc(3));
	  crossings[0].set_arc(2,crossing.get_arc(0));
	  crossings[0].set_arc(3,crossing.get_arc(1));
	  crossings[0].set_connected_crossing(0,crossing.get_connected_crossing(2));
	  crossings[0].set_connected_crossing(1,crossing.get_connected_crossing(3));
	  crossings[0].set_connected_crossing(2,crossing.get_connected_crossing(0));
	  crossings[0].set_connected_crossing(3,crossing.get_connected_crossing(1));
	  crossings[0].set_region(0,1,crossing.get_region(2,3));
	  crossings[0].set_region(1,2,crossing.get_region(3,0));
	  crossings[0].set_region(2,3,crossing.get_region(0,1));
	  crossings[0].set_region(3,0,crossing.get_region(1,2));
	}
    }  

  if(!check())
    {
      cerr<<"**************************************"<<endl;
      cerr<<"ERROR PlanarDiagram not valid!"<<endl;
      cerr<<"**************************************"<<endl;
      if(flag_debug)
	save_to_file_list(cerr);
      exit(1);
    }
}

//--------------------------------------------------------------------------
void PlanarDiagram::simplify()  
{
  int nbchanges=1;
  bool flag_has_changed=false;
  while(nbchanges>0)
    {
      if(crossings.size()==0)
	return;
      nbchanges=0;
      for(int n=0;n<crossings.size();n++)	
	{
	  //only test connected crossings
	  if(reidemeister_move_II(n,crossings[n].connected_crossing0))
	    {
	      nbchanges++;
	      flag_has_changed=true;
	      if(flag_debug)
		cerr<<" nbcrossings="<<get_nb_crossings()<<endl;
	      break;
	    }
	  if(reidemeister_move_II(n,crossings[n].connected_crossing1))
	    {
	      nbchanges++;
	      flag_has_changed=true;
	      if(flag_debug)
		cerr<<" nbcrossings="<<get_nb_crossings()<<endl;
	      break;
	    }
	  if(reidemeister_move_II(n,crossings[n].connected_crossing2))
	    {
	      nbchanges++;
	      flag_has_changed=true;
	      if(flag_debug)
		cerr<<" nbcrossings="<<get_nb_crossings()<<endl;
	      break;
	    }
	  if(reidemeister_move_II(n,crossings[n].connected_crossing3))
	    {
	      nbchanges++;
	      flag_has_changed=true;
	      if(flag_debug)
		cerr<<" nbcrossings="<<get_nb_crossings()<<endl;
	      break;
	    }
	  if(reidemeister_move_I(n))//only usefull when we have a unique non-endpoint crossing
	    {
	      nbchanges++;
	      flag_has_changed=true;
	      if(flag_debug)
		cerr<<" nbcrossings="<<get_nb_crossings()<<endl;
	      break;
	    }
	}
    }
  /////////renumber arcs and remove empty crossings (all arcs=-1)///////////
  if(flag_has_changed)
    {
      if(flag_debug)
	cerr<<"Cleaning"<<endl;
      clean();
    }

  if(!check())
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR knot(oid) diagram not valid!"<<endl;
      cerr<<"*********************************************************"<<endl;
      if(flag_debug)
	save_to_file_list(cerr);
      exit(1);
    }

}

//--------------------------------------------------------------------------
void PlanarDiagram::simplify_with_random_reidemeister_moves_III( long maxiterations)  
{
  if(maxiterations==0)
    return;
  if(flag_debug)cerr<<"DEBUG: try reidemeister moves III, nb crossings="<<get_nb_crossings()<<endl;
  time_t t0=time(NULL);
  bool flag_output_status=false;
  for( long iter=0;iter<maxiterations;iter++)
    {
      if(crossings.size()<3)
	return;

      //find n-l-m  non-removed-crossings      
      int n=random01()*crossings.size();
      while(crossings[n].arc3<0)
	{
	  n=random01()*crossings.size();
	}
      int m=crossings[n].get_connected_crossing(int(random01()*3.999));
      int l=crossings[m].get_connected_crossing(int(random01()*3.999));
      if(m<0)
	{
	  continue;
	}
      if(l<0)
	{
	  continue;
	}
      if(!reidemeister_move_III( n, m, l))//not done
	{
	  if(time(NULL)-t0>1)
	    {
	      t0=time(NULL);
	      cerr<<"\r"<<" reidemeister moves III: "<<(iter*100)/maxiterations<<"%"<<" (nb crossings="<<get_nb_crossings()<<")";//".";
	      flag_output_status=true;
	    }
	  continue;
	}
      if(flag_output_status)
	cerr<<endl;
      flag_output_status=false;
      if(flag_debug)
	{
	  save_to_file_list(cerr);
	}


      bool flag_has_changed=false;
      //only try reidemeister moves II (and I) on crossings n,m,l 
      if(reidemeister_move_II(n,l))flag_has_changed=true;
      if(reidemeister_move_II(l,m))flag_has_changed=true;
      if(reidemeister_move_II(m,n))flag_has_changed=true;
      for(int i=0;i<4;i++)
	{
	  if(crossings[n].get_connected_crossing(i)!=l&&crossings[n].get_connected_crossing(i)!=m&&crossings[n].get_connected_crossing(i)!=n)
	    if(reidemeister_move_II(n,crossings[n].get_connected_crossing(i)))
	      flag_has_changed=true;
	  if(crossings[m].get_connected_crossing(i)!=l&&crossings[m].get_connected_crossing(i)!=m&&crossings[m].get_connected_crossing(i)!=n)
	    if(reidemeister_move_II(m,crossings[m].get_connected_crossing(i)))
	      flag_has_changed=true;
	  if(crossings[l].get_connected_crossing(i)!=l&&crossings[l].get_connected_crossing(i)!=m&&crossings[l].get_connected_crossing(i)!=n)
	    if(reidemeister_move_II(l,crossings[l].get_connected_crossing(i)))
	      flag_has_changed=true;
	}
      if(reidemeister_move_I(n))flag_has_changed=true;
      if(reidemeister_move_I(l))flag_has_changed=true;
      if(reidemeister_move_I(m))flag_has_changed=true;

      if(flag_has_changed)
	{
	  if(flag_debug)
	      cerr<<"Cleaning"<<endl;
	  clean();
	  if(flag_debug)
	    {
	      if(!check())
		{
		  cerr<<"*********************************************************"<<endl;
		  cerr<<"ERROR knot(oid) diagram not valid!"<<endl;
		  cerr<<"*********************************************************"<<endl;
		  save_to_file_list(cerr);
		  exit(1);
		}
	    }
	}
    }
  if(flag_output_status)
    cerr<<endl;

  clean();
  if(!check())
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR knot(oid) diagram not valid!"<<endl;
      cerr<<"*********************************************************"<<endl;
      save_to_file_list(cerr);
      exit(1);
    }

}

//---------------------------------------------------------------------------
//Private
//---------------------------------------------------------------------------

void PlanarDiagram::initialize()
{
  //find max arc index
  int maxarcindexe=0;
  for(int n=0;n<crossings.size();n++)
    {
      maxarcindexe=max(maxarcindexe,crossings[n].arc0);
      maxarcindexe=max(maxarcindexe,crossings[n].arc1);
      maxarcindexe=max(maxarcindexe,crossings[n].arc2);
      maxarcindexe=max(maxarcindexe,crossings[n].arc3);
    }
  map<int,vector<int> > arc_to_connected_crossings;//arc_to_connected_crossings[arc]=set of crossing indices connected by this arc (should be of size 2)
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//removed crossing
	{
	  if(flag_debug)
	    cerr<<" initialize diagram: crossing "<<n<<" to remove (all arcs=-1)"<<endl;
	}
      else if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
	{
	  arc_to_connected_crossings[crossings[n].arc0].push_back(n);
	}
      else//general crossing
	{
	  arc_to_connected_crossings[crossings[n].arc0].push_back(n);
	  arc_to_connected_crossings[crossings[n].arc1].push_back(n);
	  arc_to_connected_crossings[crossings[n].arc2].push_back(n);
	  arc_to_connected_crossings[crossings[n].arc3].push_back(n);
	}
    }
  int arc,n1,n2;
  for(map<int,vector<int> >::iterator it=arc_to_connected_crossings.begin();it!=arc_to_connected_crossings.end();it++)
    {
      arc=it->first;
      if(it->second.size()!=2)
	{
	  cerr<<"ERROR: arc "<<arc<<" is connected to "<<it->second.size()<<" crossings instead of 2."<<endl;
	  exit(1);
	}
      n1=it->second[0];
      n2=it->second[1];
      if(crossings[n1].arc0==arc)crossings[n1].connected_crossing0=n2;
      if(crossings[n1].arc1==arc)crossings[n1].connected_crossing1=n2;
      if(crossings[n1].arc2==arc)crossings[n1].connected_crossing2=n2;
      if(crossings[n1].arc3==arc)crossings[n1].connected_crossing3=n2;
      if(crossings[n2].arc0==arc)crossings[n2].connected_crossing0=n1;
      if(crossings[n2].arc1==arc)crossings[n2].connected_crossing1=n1;
      if(crossings[n2].arc2==arc)crossings[n2].connected_crossing2=n1;
      if(crossings[n2].arc3==arc)crossings[n2].connected_crossing3=n1;
    }
  
}
//---------------------------------------------------------------------------
void PlanarDiagram::clean()
{
      if(flag_debug)
	cerr<<"PlanarDiagram::clean()"<<endl;
      //remove empty crossings
      int nbcrossings=0;
      vector<int> old_to_new_crossingindex(crossings.size(),-1);//old_to_new_crossingindex[oldindex]=newindex. newindex=-1 if crossing with oldindex was removed
      for(int n=0;n<crossings.size();n++)
	{
	  if(!(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0))//not empty crossing
	    {
	      crossings[nbcrossings].arc0=crossings[n].arc0;
	      crossings[nbcrossings].arc1=crossings[n].arc1;
	      crossings[nbcrossings].arc2=crossings[n].arc2;
	      crossings[nbcrossings].arc3=crossings[n].arc3;
	      crossings[nbcrossings].connected_crossing0=crossings[n].connected_crossing0;
	      crossings[nbcrossings].connected_crossing1=crossings[n].connected_crossing1;
	      crossings[nbcrossings].connected_crossing2=crossings[n].connected_crossing2;
	      crossings[nbcrossings].connected_crossing3=crossings[n].connected_crossing3;
	      crossings[nbcrossings].region01=crossings[n].region01;
	      crossings[nbcrossings].region12=crossings[n].region12;
	      crossings[nbcrossings].region23=crossings[n].region23;
	      crossings[nbcrossings].region30=crossings[n].region30;
	      old_to_new_crossingindex[n]=nbcrossings;
	      nbcrossings++;
	    }
	  else if(flag_debug)
	    cerr<<" removing crossing "<<n<<endl;
	}
      crossings.resize(nbcrossings);

      //renumber arcs & connected_crossing
      if(flag_debug)
	cerr<<" renumbering arcs"<<endl;
      set<int> arcindexes;
      for(int n=0;n<crossings.size();n++)
	{
	  arcindexes.insert(crossings[n].arc0);
	  arcindexes.insert(crossings[n].arc1);
	  arcindexes.insert(crossings[n].arc2);
	  arcindexes.insert(crossings[n].arc3);
	}
      int count=0;
      vector<int> newarcindexes(*max_element(arcindexes.begin(),arcindexes.end())+1,-1);
      for(set<int>::iterator it=arcindexes.begin();it!=arcindexes.end();it++)
	{
	  if(*it>=0)
	    {
	      newarcindexes[*it]=count;
	      count++;
	    }
	}
      for(int n=0;n<crossings.size();n++)
	{
	  if(crossings[n].arc0>=0)
	    crossings[n].arc0=newarcindexes[crossings[n].arc0];
	  if(crossings[n].arc1>=0)
	    crossings[n].arc1=newarcindexes[crossings[n].arc1];
	  if(crossings[n].arc2>=0)
	    crossings[n].arc2=newarcindexes[crossings[n].arc2];
	  if(crossings[n].arc3>=0)
	    crossings[n].arc3=newarcindexes[crossings[n].arc3];

	  if(crossings[n].connected_crossing0>=0)
	    {
	      if(old_to_new_crossingindex[crossings[n].connected_crossing0]<0)
		{
		  cerr<<"ERROR: PlanarDiagram::clean() old_to_new_crossingindex["<<crossings[n].connected_crossing0<<"]="<<old_to_new_crossingindex[crossings[n].connected_crossing0]<<endl;
		  exit(1);
		}	      
	      crossings[n].connected_crossing0=old_to_new_crossingindex[crossings[n].connected_crossing0];
	    }
	  if(crossings[n].connected_crossing1>=0)
	    {
	      if(old_to_new_crossingindex[crossings[n].connected_crossing1]<0)
		{
		  cerr<<"ERROR: PlanarDiagram::clean() old_to_new_crossingindex["<<crossings[n].connected_crossing1<<"]="<<old_to_new_crossingindex[crossings[n].connected_crossing1]<<endl;
		  exit(1);
		}	      
	      crossings[n].connected_crossing1=old_to_new_crossingindex[crossings[n].connected_crossing1];
	    }
	  if(crossings[n].connected_crossing2>=0)
	    {
	      if(old_to_new_crossingindex[crossings[n].connected_crossing2]<0)
		{
		  cerr<<"ERROR: PlanarDiagram::clean() old_to_new_crossingindex["<<crossings[n].connected_crossing2<<"]="<<old_to_new_crossingindex[crossings[n].connected_crossing2]<<endl;
		  exit(1);
		}	      
	      crossings[n].connected_crossing2=old_to_new_crossingindex[crossings[n].connected_crossing2];
	    }
	  if(crossings[n].connected_crossing3>=0)
	    {
	      if(old_to_new_crossingindex[crossings[n].connected_crossing3]<0)
		{
		  cerr<<"ERROR: PlanarDiagram::clean() old_to_new_crossingindex["<<crossings[n].connected_crossing3<<"]="<<old_to_new_crossingindex[crossings[n].connected_crossing3]<<endl;
		  exit(1);
		}	      
	      crossings[n].connected_crossing3=old_to_new_crossingindex[crossings[n].connected_crossing3];
	    }

	}
      

}

//---------------------------------------------------------------------------

bool PlanarDiagram::reidemeister_move_I(int n)
{

  bool flag_remove=false;
  if(n<0||n>=crossings.size())
    return flag_remove;
  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
    return flag_remove;
  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//already removed
    return flag_remove;
  if(crossings[n].arc0<0||crossings[n].arc1<0||crossings[n].arc2<0||crossings[n].arc3<0)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::reidemeister_move_I() crossings["<<n<<"] has an incoming arc <0"<<endl;
      cerr<<"*********************************************************"<<endl;
      return flag_remove;
    }
  //NOTE: reidemeister move I does not affect region.
  int arc_to_remove=-1;
  int arc_to_merge1=-1;
  int arc_to_merge2=-1;//replace arc_to_merge2 by arc_to_merge1
  int input_pos_1=-1;  //crossing[n].get_arc(input_pos_1)=arc_to_remove, only used for region
  int input_pos_2=-1;  //crossing[n].get_arc(input_pos_2)=arc_to_remove, only used for region

  int connected_crossing1=-1;//crossing connected to crossing n
  int connected_crossing2=-1;//crossing connected to crossing n
  if(crossings[n].arc0==crossings[n].arc1)
    {
      arc_to_remove=crossings[n].arc0;
      arc_to_merge1=crossings[n].arc2;
      arc_to_merge2=crossings[n].arc3;
      input_pos_1=0;
      input_pos_2=1;
      connected_crossing1=crossings[n].connected_crossing2;
      connected_crossing2=crossings[n].connected_crossing3;
      flag_remove=true;
    }
  else if(crossings[n].arc0==crossings[n].arc2)//do not remove, not a reidemeister move I
    {
    }
  else if(crossings[n].arc0==crossings[n].arc3)
    {
      arc_to_remove=crossings[n].arc0;
      arc_to_merge1=crossings[n].arc1;
      arc_to_merge2=crossings[n].arc2;
      input_pos_1=0;
      input_pos_2=3;
      connected_crossing1=crossings[n].connected_crossing1;
      connected_crossing2=crossings[n].connected_crossing2;
      flag_remove=true;
    }
  else if(crossings[n].arc1==crossings[n].arc2)
    {
      arc_to_remove=crossings[n].arc1;
      arc_to_merge1=crossings[n].arc0;
      arc_to_merge2=crossings[n].arc3;
      input_pos_1=1;
      input_pos_2=2;
      connected_crossing1=crossings[n].connected_crossing0;
      connected_crossing2=crossings[n].connected_crossing3;
      flag_remove=true;
    }
  else if(crossings[n].arc1==crossings[n].arc3)//do not remove, not a reidemeister move I
    {
    }
  else if(crossings[n].arc2==crossings[n].arc3)
    {
      arc_to_remove=crossings[n].arc2;
      arc_to_merge1=crossings[n].arc0;
      arc_to_merge2=crossings[n].arc1;
      input_pos_1=2;
      input_pos_2=3;
      connected_crossing1=crossings[n].connected_crossing0;
      connected_crossing2=crossings[n].connected_crossing1;
      flag_remove=true;
    }

  
  if(flag_remove)
    {
      //check that the loop is not exterior
      if(flag_planar)
	{
	  if(crossings[n].get_region(input_pos_1,input_pos_2)==1)//exterior loop
	    {
	      flag_remove=false;
	      return flag_remove;
	    }
	}
      
      if(flag_debug)
	cerr<<" Reidemeister move I: Removing crossing "<<n<<", removing arc "<<arc_to_remove<<", merging arcs "<<arc_to_merge1<<" and "<<arc_to_merge2<<" (connected to crossings "<<connected_crossing1<<" and "<<connected_crossing2<<" resp.)"<<endl;

      //modify connected_crossing
      if(crossings[connected_crossing1].arc0==arc_to_merge1)crossings[connected_crossing1].connected_crossing0=connected_crossing2;
      if(crossings[connected_crossing1].arc1==arc_to_merge1)crossings[connected_crossing1].connected_crossing1=connected_crossing2;
      if(crossings[connected_crossing1].arc2==arc_to_merge1)crossings[connected_crossing1].connected_crossing2=connected_crossing2;
      if(crossings[connected_crossing1].arc3==arc_to_merge1)crossings[connected_crossing1].connected_crossing3=connected_crossing2;
      if(crossings[connected_crossing2].arc0==arc_to_merge2)crossings[connected_crossing2].connected_crossing0=connected_crossing1;
      if(crossings[connected_crossing2].arc1==arc_to_merge2)crossings[connected_crossing2].connected_crossing1=connected_crossing1;
      if(crossings[connected_crossing2].arc2==arc_to_merge2)crossings[connected_crossing2].connected_crossing2=connected_crossing1;
      if(crossings[connected_crossing2].arc3==arc_to_merge2)crossings[connected_crossing2].connected_crossing3=connected_crossing1;
      //renumber arcs
      for(int i=0;i<crossings.size();i++)
	{
	  //replace arc_to_merge2 by arc_to_merge1
	  if(crossings[i].arc0==arc_to_merge2)
	    crossings[i].arc0=arc_to_merge1;
	  //replace arc_to_merge2 by arc_to_merge1
	  if(crossings[i].arc1==arc_to_merge2)
	    crossings[i].arc1=arc_to_merge1;
	  //replace arc_to_merge2 by arc_to_merge1
	  if(crossings[i].arc2==arc_to_merge2)
	    crossings[i].arc2=arc_to_merge1;
	  //replace arc_to_merge2 by arc_to_merge1
	  if(crossings[i].arc3==arc_to_merge2)
	    crossings[i].arc3=arc_to_merge1;	  
	}
      //remove crossing
      crossings[n].arc0=-1;
      crossings[n].arc1=-1;
      crossings[n].arc2=-1;
      crossings[n].arc3=-1;
      
      //for the very special case: cyclic with one crossing, to avoid X[1,0,0,1] (cyclic)
      if(get_nb_crossings()==1)
	{
	  if(flag_debug)cerr<<" checking for reordering arcs in crossing arc0="<<crossings[0].arc0<<" arc1="<<crossings[0].arc0<<endl;
	  //////reorder arcs to satisfy arc0 < arc2 (BUT not arc0=maxindex=1 and arc2=0); 
	  for(int n=0;n<crossings.size();n++)
	    {
	      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//ignore empty crossing
		continue;
	      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//ignore endpoint
		continue;
	      if(crossings[n].arc0>crossings[n].arc2)
		{
		  if(flag_debug)cerr<<" reordering arcs in crossing"<<endl;
		  int a1=crossings[n].arc2;
		  int a2=crossings[n].arc3;
		  int a3=crossings[n].arc0;
		  int a4=crossings[n].arc1;
		  crossings[n].arc0=a1;
		  crossings[n].arc1=a2;
		  crossings[n].arc2=a3;
		  crossings[n].arc3=a4;	  
		  a1=crossings[n].connected_crossing2;
		  a2=crossings[n].connected_crossing3;
		  a3=crossings[n].connected_crossing0;
		  a4=crossings[n].connected_crossing1;
		  crossings[n].connected_crossing0=a1;
		  crossings[n].connected_crossing1=a2;
		  crossings[n].connected_crossing2=a3;
		  crossings[n].connected_crossing3=a4;	  
		  a1=crossings[n].region23;
		  a2=crossings[n].region30;
		  a3=crossings[n].region01;
		  a4=crossings[n].region12;
		  crossings[n].region01=a1;
		  crossings[n].region12=a2;
		  crossings[n].region23=a3;
		  crossings[n].region30=a4;	  
		}
	    }
	}
      
    }
  return flag_remove;
}

//---------------------------------------------------------------------------

bool PlanarDiagram::reidemeister_move_II(int n,int m)
{

  bool flag_remove=false;
  //check
  if(n==m)
    return flag_remove;
  if(n<0||n>=crossings.size())
    return flag_remove;
  if(m<0||m>=crossings.size())
    return flag_remove;
  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
    return flag_remove;
  if(crossings[m].arc0>=0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)//endpoint
    return flag_remove;
  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//already removed
    return flag_remove;
  if(crossings[m].arc0<0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)//already removed
    return flag_remove;
  
  if(crossings[n].arc0<0||crossings[n].arc1<0||crossings[n].arc2<0||crossings[n].arc3<0)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::reidemeister_move_II() crossings["<<n<<"] has an incoming arc <0"<<endl;
      cerr<<"*********************************************************"<<endl;
      return flag_remove;
    }	
  if(crossings[m].arc0<0||crossings[m].arc1<0||crossings[m].arc2<0||crossings[m].arc3<0)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::reidemeister_move_II() crossings["<<m<<"] has an incoming arc <0"<<endl;
      cerr<<"*********************************************************"<<endl;
      return flag_remove;
    }
  ///////////////////
  //WARNING: our implementation of reidemeister_move_II will fail if a crossing has twice the same arc (fail when merging)
  //  =>If it is the case Reidemeister_move_I may be applied
  if(crossings[n].arc0==crossings[n].arc1||crossings[n].arc0==crossings[n].arc2||crossings[n].arc0==crossings[n].arc3||
     crossings[n].arc1==crossings[n].arc2||crossings[n].arc1==crossings[n].arc3||crossings[n].arc2==crossings[n].arc3)
    {
      return reidemeister_move_I(n);
    }
  if(crossings[m].arc0==crossings[m].arc1||crossings[m].arc0==crossings[m].arc2||crossings[m].arc0==crossings[m].arc3||
     crossings[m].arc1==crossings[m].arc2||crossings[m].arc1==crossings[m].arc3||crossings[m].arc2==crossings[m].arc3)
    {
      return reidemeister_move_I(m);
    }
      

  int arc_to_remove1=-1;
  int arc_to_remove2=-1;
  int arc_to_merge_a1=-1;
  int arc_to_merge_a2=-1;//replace arc_to_merge_a2 by arc_to_merge_a1
  int arc_to_merge_b1=-1;
  int arc_to_merge_b2=-1;//replace arc_to_merge_b2 by arc_to_merge_b1
  int connected_crossing_a1=-1;//crossing connected to crossing n by arc arc_to_merge_a1
  int connected_crossing_a2=-1;//crossing connected to crossing m by arc arc_to_merge_a2
  int connected_crossing_b1=-1;//crossing connected to crossing n by arc arc_to_merge_b1
  int connected_crossing_b2=-1;//crossing connected to crossing m by arc arc_to_merge_b2
  int input_pos_out_a1=-1;  //crossing[n].get_arc(input_pos_out_a1)=arc_to_merge_a1, only used for region
  int input_pos_out_a2=-1;  //crossing[m].get_arc(input_pos_out_a2)=arc_to_merge_a2, only used for region
  int input_pos_out_b1=-1;  //crossing[n].get_arc(input_pos_out_b1)=arc_to_merge_b1, only used for region
  int input_pos_out_b2=-1;  //crossing[m].get_arc(input_pos_out_b2)=arc_to_merge_b2, only used for region


  //Notes:
  // crossing.arc0 and  crossing.arc2 are below
  // crossing.arc1 and  crossing.arc3 are above
  // arcs above: arc_to_merge_a1(=incoming arc to crossing n) - arc_to_remove1(=arc between crossings n,m) - arc_to_merge_a2(=incoming arc to crossing m)
  // arcs below: arc_to_merge_b1(=incoming arc to crossing n) - arc_to_remove2(=arc between crossings n,m) - arc_to_merge_b2(=incoming arc to crossing m)

    if(crossings[n].arc0==crossings[m].arc0&&crossings[n].arc1==crossings[m].arc3)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region01==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc0;
      arc_to_remove2=crossings[n].arc1;
      arc_to_merge_a1=crossings[n].arc2;
      arc_to_merge_a2=crossings[m].arc2;
      arc_to_merge_b1=crossings[n].arc3;
      arc_to_merge_b2=crossings[m].arc1;
      connected_crossing_a1=crossings[n].connected_crossing2;
      connected_crossing_a2=crossings[m].connected_crossing2;
      connected_crossing_b1=crossings[n].connected_crossing3;
      connected_crossing_b2=crossings[m].connected_crossing1;
      input_pos_out_a1=2;
      input_pos_out_a2=2;
      input_pos_out_b1=3;
      input_pos_out_b2=1;
      flag_remove=true;
    }
  else if(crossings[n].arc0==crossings[m].arc0&&crossings[n].arc3==crossings[m].arc1)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region30==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc0;
      arc_to_remove2=crossings[n].arc3;
      arc_to_merge_a1=crossings[n].arc2;
      arc_to_merge_a2=crossings[m].arc2;
      arc_to_merge_b1=crossings[n].arc1;
      arc_to_merge_b2=crossings[m].arc3;
      connected_crossing_a1=crossings[n].connected_crossing2;
      connected_crossing_a2=crossings[m].connected_crossing2;
      connected_crossing_b1=crossings[n].connected_crossing1;
      connected_crossing_b2=crossings[m].connected_crossing3;           
      input_pos_out_a1=2;
      input_pos_out_a2=2;
      input_pos_out_b1=1;
      input_pos_out_b2=3;
      flag_remove=true;
    }
  else if(crossings[n].arc0==crossings[m].arc2&&crossings[n].arc1==crossings[m].arc1)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region01==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc0;
      arc_to_remove2=crossings[n].arc1;
      arc_to_merge_a1=crossings[n].arc2;
      arc_to_merge_a2=crossings[m].arc0;
      arc_to_merge_b1=crossings[n].arc3;
      arc_to_merge_b2=crossings[m].arc3;
      connected_crossing_a1=crossings[n].connected_crossing2;
      connected_crossing_a2=crossings[m].connected_crossing0;
      connected_crossing_b1=crossings[n].connected_crossing3;
      connected_crossing_b2=crossings[m].connected_crossing3;            
      input_pos_out_a1=2;
      input_pos_out_a2=0;
      input_pos_out_b1=3;
      input_pos_out_b2=3;
      flag_remove=true;
    }
  else if(crossings[n].arc0==crossings[m].arc2&&crossings[n].arc3==crossings[m].arc3)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region30==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc0;
      arc_to_remove2=crossings[n].arc3;
      arc_to_merge_a1=crossings[n].arc2;
      arc_to_merge_a2=crossings[m].arc0;
      arc_to_merge_b1=crossings[n].arc1;
      arc_to_merge_b2=crossings[m].arc1;
      connected_crossing_a1=crossings[n].connected_crossing2;
      connected_crossing_a2=crossings[m].connected_crossing0;
      connected_crossing_b1=crossings[n].connected_crossing1;
      connected_crossing_b2=crossings[m].connected_crossing1;            
      input_pos_out_a1=2;
      input_pos_out_a2=0;
      input_pos_out_b1=1;
      input_pos_out_b2=1;
      flag_remove=true;
    }
  else if(crossings[n].arc2==crossings[m].arc0&&crossings[n].arc1==crossings[m].arc1)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region12==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc2;
      arc_to_remove2=crossings[n].arc1;
      arc_to_merge_a1=crossings[n].arc0;
      arc_to_merge_a2=crossings[m].arc2;
      arc_to_merge_b1=crossings[n].arc3;
      arc_to_merge_b2=crossings[m].arc3;
      connected_crossing_a1=crossings[n].connected_crossing0;
      connected_crossing_a2=crossings[m].connected_crossing2;
      connected_crossing_b1=crossings[n].connected_crossing3;
      connected_crossing_b2=crossings[m].connected_crossing3;                  
      input_pos_out_a1=0;
      input_pos_out_a2=2;
      input_pos_out_b1=3;
      input_pos_out_b2=3;
      flag_remove=true;
    }
  else if(crossings[n].arc2==crossings[m].arc0&&crossings[n].arc3==crossings[m].arc3)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region23==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc2;
      arc_to_remove2=crossings[n].arc3;
      arc_to_merge_a1=crossings[n].arc0;
      arc_to_merge_a2=crossings[m].arc2;
      arc_to_merge_b1=crossings[n].arc1;
      arc_to_merge_b2=crossings[m].arc1;
      connected_crossing_a1=crossings[n].connected_crossing0;
      connected_crossing_a2=crossings[m].connected_crossing2;
      connected_crossing_b1=crossings[n].connected_crossing1;
      connected_crossing_b2=crossings[m].connected_crossing1;
      input_pos_out_a1=0;
      input_pos_out_a2=2;
      input_pos_out_b1=1;
      input_pos_out_b2=1;
      flag_remove=true;
    }
  else if(crossings[n].arc2==crossings[m].arc2&&crossings[n].arc1==crossings[m].arc3)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region12==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc2;
      arc_to_remove2=crossings[n].arc1;
      arc_to_merge_a1=crossings[n].arc0;
      arc_to_merge_a2=crossings[m].arc0;
      arc_to_merge_b1=crossings[n].arc3;
      arc_to_merge_b2=crossings[m].arc1;
      connected_crossing_a1=crossings[n].connected_crossing0;
      connected_crossing_a2=crossings[m].connected_crossing0;
      connected_crossing_b1=crossings[n].connected_crossing3;
      connected_crossing_b2=crossings[m].connected_crossing1;
      input_pos_out_a1=0;
      input_pos_out_a2=0;
      input_pos_out_b1=3;
      input_pos_out_b2=1;
      flag_remove=true;
    }
  else if(crossings[n].arc2==crossings[m].arc2&&crossings[n].arc3==crossings[m].arc1)//OK
    {
      //check that the loop (between arc_to_remove1 and arc_to_remove2) is not exterior
      if(flag_planar&&crossings[n].region23==1)
	{
	  flag_remove=false;
	  return flag_remove;
	}
      arc_to_remove1=crossings[n].arc2;
      arc_to_remove2=crossings[n].arc3;
      arc_to_merge_a1=crossings[n].arc0;
      arc_to_merge_a2=crossings[m].arc0;
      arc_to_merge_b1=crossings[n].arc1;
      arc_to_merge_b2=crossings[m].arc3;
      connected_crossing_a1=crossings[n].connected_crossing0;
      connected_crossing_a2=crossings[m].connected_crossing0;
      connected_crossing_b1=crossings[n].connected_crossing1;
      connected_crossing_b2=crossings[m].connected_crossing3;
      input_pos_out_a1=0;
      input_pos_out_a2=0;
      input_pos_out_b1=1;
      input_pos_out_b2=3;
      flag_remove=true;
    }

  //WARNING: our current implementation of reidemeister_move_II will fail if any of the connected crossings (connected_crossing_a1/a2/b1/b2) is n or m
  //  =>we quit
  if(connected_crossing_a1==n||connected_crossing_a1==m)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_a2==n||connected_crossing_a2==m)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_b1==n||connected_crossing_b1==m)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_b2==n||connected_crossing_b2==m)
    {
      flag_remove=false;
      return flag_remove;
    }

  if(flag_remove)
    {
      if(flag_debug)
	{
	  cerr<<" Reidemeister move II: Removing crossings "<<n<<" "<<m<<", removing arcs "<<arc_to_remove1<<" "<<arc_to_remove2<<", merging arcs "<<arc_to_merge_a2<<" and "<<arc_to_merge_a1<<", merging arcs "<<arc_to_merge_b2<<" and "<<arc_to_merge_b1<<" (old connected crossings: a1="<<connected_crossing_a1<<" b1="<<connected_crossing_b1<<" a2="<<connected_crossing_a2<<" b2="<<connected_crossing_b2<<")"<<endl;
	  if(flag_planar)
	    {
	      cerr<<" Crossing "<<n<<" arcs: "<<crossings[n].arc0<<" "<<crossings[n].arc1<<" "<<crossings[n].arc2<<" "<<crossings[n].arc3;
	      cerr<<", connected to crossings: "<<crossings[n].connected_crossing0<<" "<<crossings[n].connected_crossing1<<" "<<crossings[n].connected_crossing2<<" "<<crossings[n].connected_crossing3;
	      cerr<<", is exterior boundary(1=true): 1-2:"<<crossings[n].region01<<" 2-3:"<<crossings[n].region12<<" 3-4:"<<crossings[n].region23<<" 4-1:"<<crossings[n].region30;
	      cerr<<endl;

	      cerr<<" Crossing "<<m<<" arcs: "<<crossings[m].arc0<<" "<<crossings[m].arc1<<" "<<crossings[m].arc2<<" "<<crossings[m].arc3;
	      cerr<<", connected to crossings: "<<crossings[m].connected_crossing0<<" "<<crossings[m].connected_crossing1<<" "<<crossings[m].connected_crossing2<<" "<<crossings[m].connected_crossing3;
	      cerr<<", is exterior boundary(1=true): 1-2:"<<crossings[m].region01<<" 2-3:"<<crossings[m].region12<<" 3-4:"<<crossings[m].region23<<" 4-1:"<<crossings[m].region30;
	      cerr<<endl;

	    }

	}
      //adapt region, i.e. follow the bondary
      if(flag_planar)
	{
	  if(crossings[n].get_region(input_pos_out_a1,input_pos_out_b1)!=crossings[m].get_region(input_pos_out_a2,input_pos_out_b2))
	    {
	      int arc0=-10;//incoming arc
	      int arc1=-10;//outgoing arc
	      int n0=-10;//current crossing
	      int n1=-10;//next crossing
	      int pos_arc0=-10;//such that crossings[n0].get_arc(pos_arc0)=arc0;
	      int pos_arc1=-10;//such that crossings[n0].get_arc(pos_arc1)=arc1;
	      int region_to_propagate=-10;
	      if(crossings[n].get_region(input_pos_out_a1,input_pos_out_b1)==1)//propagate region on the side of m
		{
		  region_to_propagate=1;
		  n0=m;
		  crossings[n0].set_region(input_pos_out_a2,input_pos_out_b2,1);	      
		  if(input_pos_out_b2==(input_pos_out_a2+3)%4)//
		    {
		      arc0=arc_to_merge_a2;
		      arc1=arc_to_merge_b2;
		      //pos_arc0=input_pos_out_a2;
		      pos_arc1=input_pos_out_b2;
		      n1=connected_crossing_b2;
		    }
		  else if(input_pos_out_a2==(input_pos_out_b2+3)%4)//
		    {
		      arc0=arc_to_merge_b2;
		      arc1=arc_to_merge_a2;
		      //pos_arc0=input_pos_out_b2;
		      pos_arc1=input_pos_out_a2;
		      n1=connected_crossing_a2;
		    }
		  else
		    {
		      cerr<<"*********************************************************"<<endl;
		      cerr<<"ERROR PlanarDiagram::reidemeister_move_II() error with input_pos"<<endl;
		      cerr<<"*********************************************************"<<endl;
		      exit(1);		      
		    }
		}
	      else //if(crossings[m].get_region(input_pos_out_a2,input_pos_out_b2)==1)//propagate region on the side of n
		{
		  region_to_propagate=crossings[m].get_region(input_pos_out_a2,input_pos_out_b2);
		  n0=n;
		  crossings[n0].set_region(input_pos_out_a1,input_pos_out_b1,region_to_propagate);	      
		  if(input_pos_out_b1==(input_pos_out_a1+3)%4)//
		    {
		      arc0=arc_to_merge_a1;
		      arc1=arc_to_merge_b1;
		      //pos_arc0=input_pos_out_a1;
		      pos_arc1=input_pos_out_b1;
		      n1=connected_crossing_b1;
		    }
		  else if(input_pos_out_a1==(input_pos_out_b1+3)%4)//
		    {
		      arc0=arc_to_merge_b1;
		      arc1=arc_to_merge_a1;
		      //pos_arc0=input_pos_out_b1;
		      pos_arc1=input_pos_out_a1;
		      n1=connected_crossing_a1;
		    }
		  else
		    {
		      cerr<<"*********************************************************"<<endl;
		      cerr<<"ERROR PlanarDiagram::reidemeister_move_II() error with input_pos"<<endl;
		      cerr<<"*********************************************************"<<endl;
		      exit(1);		      
		    }
		}
	      if(flag_debug)
		cerr<<"move II (propagating region). initial n0="<<n0<<" next crossing n1="<<n1<<" arc0="<<arc0<<" arc1="<<arc1<<" crossing["<<n0<<"].region: 0-1="<<crossings[n0].region01<<" 1-2="<<crossings[n0].region12<<" 2-3="<<crossings[n0].region23<<" 3-0="<<crossings[n0].region30<<" region_to_propagate="<<region_to_propagate<<endl;	      
	      while(true)
		{
		  bool flag_stop=false;
		  for(int i=0;i<4;i++)
		    {

		      if((n1!=n0&&(crossings[n1].get_arc(i)==arc1&&crossings[n1].get_connected_crossing(i)==n0))||
			 (n1==n0&&(crossings[n1].get_arc(i)==arc1&&crossings[n1].get_connected_crossing(i)==n0&&i!=pos_arc1)))
			{
			  //check if crossing is an endpoint
			  if(crossings[n1].arc0>=0&&crossings[n1].arc1<0&&crossings[n1].arc2<0&&crossings[n1].arc3<0)//endpoint
			    {
			      n0=n1;
			      arc0=arc1;
			      if(crossings[n1].region01==region_to_propagate)//already done
				flag_stop=true;
			      crossings[n1].region01=region_to_propagate;
			      arc1=crossings[n1].arc0;
			      //pos_arc0=0;
			      pos_arc1=0;
			      n1=crossings[n1].connected_crossing0;
			      break;
			    }
			  else //or if not endpoint but outgoing arc = (arc1+1) if not-cyclic %nbarcs
			    {
			      n0=n1;
			      arc0=arc1;
			      if(crossings[n1].get_region(i,(i+3)%4)==region_to_propagate)//already done
				flag_stop=true;
			      crossings[n1].set_region(i,(i+3)%4,region_to_propagate);
			      arc1=crossings[n1].get_arc((i+3)%4);//next arc clockwise		    
			      //pos_arc0=i;
			      pos_arc1=(i+3)%4;
			      n1=crossings[n1].get_connected_crossing((i+3)%4);//next arc clockwise		    
			      break;
			    }
			}
		    }
		  if(flag_stop)
		    break;
		  if(flag_debug)
		    cerr<<"move II (propagating region). additional n0="<<n0<<" next crossing n1="<<n1<<" arc0="<<arc0<<" arc1="<<arc1<<" crossing["<<n0<<"].region: 0-1="<<crossings[n0].region01<<" 1-2="<<crossings[n0].region12<<" 2-3="<<crossings[n0].region23<<" 3-0="<<crossings[n0].region30<<endl;
		}
	      
	    }
	}
      
      //modify connected_crossing
      if(crossings[connected_crossing_a1].arc0==arc_to_merge_a1)crossings[connected_crossing_a1].connected_crossing0=connected_crossing_a2;
      if(crossings[connected_crossing_a1].arc1==arc_to_merge_a1)crossings[connected_crossing_a1].connected_crossing1=connected_crossing_a2;
      if(crossings[connected_crossing_a1].arc2==arc_to_merge_a1)crossings[connected_crossing_a1].connected_crossing2=connected_crossing_a2;
      if(crossings[connected_crossing_a1].arc3==arc_to_merge_a1)crossings[connected_crossing_a1].connected_crossing3=connected_crossing_a2;

      if(crossings[connected_crossing_b1].arc0==arc_to_merge_b1)crossings[connected_crossing_b1].connected_crossing0=connected_crossing_b2;
      if(crossings[connected_crossing_b1].arc1==arc_to_merge_b1)crossings[connected_crossing_b1].connected_crossing1=connected_crossing_b2;
      if(crossings[connected_crossing_b1].arc2==arc_to_merge_b1)crossings[connected_crossing_b1].connected_crossing2=connected_crossing_b2;
      if(crossings[connected_crossing_b1].arc3==arc_to_merge_b1)crossings[connected_crossing_b1].connected_crossing3=connected_crossing_b2;

      if(crossings[connected_crossing_a2].arc0==arc_to_merge_a2)crossings[connected_crossing_a2].connected_crossing0=connected_crossing_a1;
      if(crossings[connected_crossing_a2].arc1==arc_to_merge_a2)crossings[connected_crossing_a2].connected_crossing1=connected_crossing_a1;
      if(crossings[connected_crossing_a2].arc2==arc_to_merge_a2)crossings[connected_crossing_a2].connected_crossing2=connected_crossing_a1;
      if(crossings[connected_crossing_a2].arc3==arc_to_merge_a2)crossings[connected_crossing_a2].connected_crossing3=connected_crossing_a1;

      if(crossings[connected_crossing_b2].arc0==arc_to_merge_b2)crossings[connected_crossing_b2].connected_crossing0=connected_crossing_b1;
      if(crossings[connected_crossing_b2].arc1==arc_to_merge_b2)crossings[connected_crossing_b2].connected_crossing1=connected_crossing_b1;
      if(crossings[connected_crossing_b2].arc2==arc_to_merge_b2)crossings[connected_crossing_b2].connected_crossing2=connected_crossing_b1;
      if(crossings[connected_crossing_b2].arc3==arc_to_merge_b2)crossings[connected_crossing_b2].connected_crossing3=connected_crossing_b1;

      
      //renumber arcs
      for(int i=0;i<crossings.size();i++)
	{
	  //replace arc_to_merge_a2 by arc_to_merge_a1
	  if(crossings[i].arc0==arc_to_merge_a2)
	    crossings[i].arc0=arc_to_merge_a1;
	  //replace arc_to_merge_b2 by arc_to_merge_b1
	  if(crossings[i].arc0==arc_to_merge_b2)
	    crossings[i].arc0=arc_to_merge_b1;
	  //replace arc_to_merge_a2 by arc_to_merge_a1
	  if(crossings[i].arc1==arc_to_merge_a2)
	    crossings[i].arc1=arc_to_merge_a1;
	  //replace arc_to_merge_b2 by arc_to_merge_b1
	  if(crossings[i].arc1==arc_to_merge_b2)
	    crossings[i].arc1=arc_to_merge_b1;
	  //replace arc_to_merge_a2 by arc_to_merge_a1
	  if(crossings[i].arc2==arc_to_merge_a2)
	    crossings[i].arc2=arc_to_merge_a1;
	  //replace arc_to_merge_b2 by arc_to_merge_b1
	  if(crossings[i].arc2==arc_to_merge_b2)
	    crossings[i].arc2=arc_to_merge_b1;
	  //replace arc_to_merge_a2 by arc_to_merge_a1
	  if(crossings[i].arc3==arc_to_merge_a2)
	    crossings[i].arc3=arc_to_merge_a1;
	  //replace arc_to_merge_b2 by arc_to_merge_b1
	  if(crossings[i].arc3==arc_to_merge_b2)
	    crossings[i].arc3=arc_to_merge_b1;
	}
      //remove crossing
      crossings[n].arc0=-1;
      crossings[n].arc1=-1;
      crossings[n].arc2=-1;
      crossings[n].arc3=-1;
      crossings[m].arc0=-1;
      crossings[m].arc1=-1;
      crossings[m].arc2=-1;
      crossings[m].arc3=-1;

      //for the very special case: cyclic with one crossing, to avoid X[1,0,0,1] (cyclic)
      if(get_nb_crossings()==1)
	{
	  if(flag_debug)cerr<<" checking for reordering arcs in crossing arc0="<<crossings[0].arc0<<" arc1="<<crossings[0].arc1<<endl;
	  //////reorder arcs to satisfy arc0 < arc2 (BUT not arc0=maxindex=1 and arc2=0);
	  for(int n=0;n<crossings.size();n++)
	    {
	      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//ignore empty crossing
		continue;
	      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//ignore endpoint
		continue;
	      if(crossings[n].arc0>crossings[n].arc2)
		{
		  if(flag_debug)cerr<<" reordering arcs in crossing"<<endl;
		  int a1=crossings[n].arc2;
		  int a2=crossings[n].arc3;
		  int a3=crossings[n].arc0;
		  int a4=crossings[n].arc1;
		  crossings[n].arc0=a1;
		  crossings[n].arc1=a2;
		  crossings[n].arc2=a3;
		  crossings[n].arc3=a4;	  
		  a1=crossings[n].connected_crossing2;
		  a2=crossings[n].connected_crossing3;
		  a3=crossings[n].connected_crossing0;
		  a4=crossings[n].connected_crossing1;
		  crossings[n].connected_crossing0=a1;
		  crossings[n].connected_crossing1=a2;
		  crossings[n].connected_crossing2=a3;
		  crossings[n].connected_crossing3=a4;	  
		  a1=crossings[n].region23;
		  a2=crossings[n].region30;
		  a3=crossings[n].region01;
		  a4=crossings[n].region12;
		  crossings[n].region01=a1;
		  crossings[n].region12=a2;
		  crossings[n].region23=a3;
		  crossings[n].region30=a4;	  
		}
	    }
	}
    }
  return flag_remove;
}

//---------------------------------------------------------------------------

bool PlanarDiagram::reidemeister_move_III(int n,int m,int l)
{
  bool flag_remove=false;
  //check
  if(n==m||n==l||m==l)
    return flag_remove;
  if(n<0||n>=crossings.size())
    return flag_remove;
  if(m<0||m>=crossings.size())
    return flag_remove;
  if(l<0||l>=crossings.size())
    return flag_remove;
  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
    return flag_remove;
  if(crossings[m].arc0>=0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)//endpoint
    return flag_remove;
  if(crossings[l].arc0>=0&&crossings[l].arc1<0&&crossings[l].arc2<0&&crossings[l].arc3<0)//endpoint
    return flag_remove;
  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//already removed
    return flag_remove;
  if(crossings[m].arc0<0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)//already removed
    return flag_remove;
  if(crossings[l].arc0<0&&crossings[l].arc1<0&&crossings[l].arc2<0&&crossings[l].arc3<0)//already removed
    return flag_remove;
  
  if(crossings[n].arc0<0||crossings[n].arc1<0||crossings[n].arc2<0||crossings[n].arc3<0)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::reidemeister_move_III() crossings["<<n<<"] has an incoming arc <0"<<endl;
      cerr<<"*********************************************************"<<endl;
      return flag_remove;
    }	
  if(crossings[m].arc0<0||crossings[m].arc1<0||crossings[m].arc2<0||crossings[m].arc3<0)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::reidemeister_move_III() crossings["<<m<<"] has an incoming arc <0"<<endl;
      cerr<<"*********************************************************"<<endl;
      return flag_remove;
    }
  if(crossings[l].arc0<0||crossings[l].arc1<0||crossings[l].arc2<0||crossings[l].arc3<0)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR PlanarDiagram::reidemeister_move_III() crossings["<<m<<"] has an incoming arc <0"<<endl;
      cerr<<"*********************************************************"<<endl;
      return flag_remove;
    }
  //WARNING: our implementation of reidemeister_move_III will fail if a crossing has twice the same arc (fail when merging)
  //  =>If it is the case Reidemeister_move_I may be applied
  if(crossings[n].arc0==crossings[n].arc1||crossings[n].arc0==crossings[n].arc2||crossings[n].arc0==crossings[n].arc3||
     crossings[n].arc1==crossings[n].arc2||crossings[n].arc1==crossings[n].arc3||crossings[n].arc2==crossings[n].arc3)
    {
      return reidemeister_move_I(n);
    }
  if(crossings[m].arc0==crossings[m].arc1||crossings[m].arc0==crossings[m].arc2||crossings[m].arc0==crossings[m].arc3||
     crossings[m].arc1==crossings[m].arc2||crossings[m].arc1==crossings[m].arc3||crossings[m].arc2==crossings[m].arc3)
    {
      return reidemeister_move_I(m);
    }
  if(crossings[l].arc0==crossings[l].arc1||crossings[l].arc0==crossings[l].arc2||crossings[l].arc0==crossings[l].arc3||
     crossings[l].arc1==crossings[l].arc2||crossings[l].arc1==crossings[l].arc3||crossings[l].arc2==crossings[l].arc3)
    {
      return reidemeister_move_I(l);
    }


  //we need it below
  int minarcindexe=-1;
  int maxarcindexe=0;
  for(int j=0;j<crossings.size();j++)
    for(int i=0;i<4;i++)    
      {
	maxarcindexe=max(maxarcindexe,crossings[j].get_arc(i));
	if(crossings[j].get_arc(i)>=0)
	  {
	    if(minarcindexe<0)
	      minarcindexe=crossings[j].get_arc(i);
	    else
	      minarcindexe=min(minarcindexe,crossings[j].get_arc(i));
	  }
      }
  int crossing_1=-1;//crossing that does not change
  int crossing_2=-1;//crossing that are moved
  int crossing_3=-1;//crossing that are moved
  int arc_shared_23=-1; //arc between crossing_2 and crossing_3, that will move below (or above) crossing 1
  int arc_shared_12=-1; //arc between crossing_1 and crossing_2.
  int arc_shared_13=-1; //arc between crossing_1 and crossing_3.
  int arc_out_23=-1; //arc going out of crossing 2, connected to arc arc_shared_23
  int arc_out_32=-1; //arc going out of crossing 3, connected to arc arc_shared_23
  int arc_out_12=-1; //arc going out of crossing 1, connected to arc arc_shared_12
  int arc_out_21=-1; //arc going out of crossing 2, connected to arc arc_shared_12
  int arc_out_13=-1; //arc going out of crossing 1, connected to arc arc_shared_13
  int arc_out_31=-1; //arc going out of crossing 3, connected to arc arc_shared_13
  int connected_crossing_out_23=-1; //connected crossing by arc going out of crossing 2, connected to arc arc_shared_23
  int connected_crossing_out_32=-1; //connected crossing by arc going out of crossing 3, connected to arc arc_shared_23
  int connected_crossing_out_12=-1; //connected crossing by arc going out of crossing 1, connected to arc arc_shared_12
  int connected_crossing_out_21=-1; //connected crossing by arc going out of crossing 2, connected to arc arc_shared_12
  int connected_crossing_out_13=-1; //connected crossing by arc going out of crossing 1, connected to arc arc_shared_13
  int connected_crossing_out_31=-1; //connected crossing by arc going out of crossing 3, connected to arc arc_shared_13
  int input_pos_shared_23=-1; //crossing[crossing_2].get_arc(input_pos_shared_23)=arc_shared_23
  int input_pos_shared_12=-1;  //crossing[crossing_1].get_arc(input_pos_shared_12)=arc_shared_12
  int input_pos_shared_13=-1;  //crossing[crossing_1].get_arc(input_pos_shared_13)=arc_shared_13
  int input_pos_shared_32=-1;  //crossing[crossing_3].get_arc(input_pos_shared_32)=arc_shared_23
  int input_pos_shared_21=-1;  //crossing[crossing_2].get_arc(input_pos_shared_21)=arc_shared_12
  int input_pos_shared_31=-1;  //crossing[crossing_3].get_arc(input_pos_shared_31)=arc_shared_13
  int input_pos_out_23=-1;  //crossing[crossing_2].get_arc(input_pos_out_23)=arc_out_23
  int input_pos_out_32=-1;  //crossing[crossing_3].get_arc(input_pos_out_32)=arc_out_32
  int input_pos_out_12=-1;  //crossing[crossing_1].get_arc(input_pos_out_12)=arc_out_12
  int input_pos_out_21=-1;  //crossing[crossing_2].get_arc(input_pos_out_21)=arc_out_21
  int input_pos_out_13=-1;  //crossing[crossing_1].get_arc(input_pos_out_13)=arc_out_13
  int input_pos_out_31=-1;  //crossing[crossing_3].get_arc(input_pos_out_31)=arc_out_31

  //find arcs connecting l-n, n-m, m-l
  set<int> arcs_ln;
  set<int> arcs_nm;
  set<int> arcs_ml;
  for(int i=0;i<4;i++)
    {
      if(crossings[l].get_connected_crossing(i)==n)
	arcs_ln.insert(crossings[l].get_arc(i));  
      if(crossings[n].get_connected_crossing(i)==m)
	arcs_nm.insert(crossings[n].get_arc(i));
      if(crossings[m].get_connected_crossing(i)==l)
	arcs_ml.insert(crossings[m].get_arc(i));
    }
  /////////////////////check that if forms a triangle///////////////
  //i.e.input_pos_ln=i <=> connected to crossing[l].get_arc(i)=arc_ln,
  //i.e.input_pos_nl=i <=> connected to crossing[n].get_arc(i)=arc_ln,
  //i.e.input_pos_nm=i <=> connected to crossing[n].get_arc(i)=arc_nm,
  //i.e.input_pos_mn=i <=> connected to crossing[m].get_arc(i)=arc_nm,
  //i.e.input_pos_ml=i <=> connected to crossing[m].get_arc(i)=arc_ml,
  //i.e.input_pos_lm=i <=> connected to crossing[l].get_arc(i)=arc_ml,
  int input_pos_ln; 
  int input_pos_nl; 
  int input_pos_nm; 
  int input_pos_mn; 
  int input_pos_ml; 
  int input_pos_lm;
  int arc_ln;
  int arc_nm;
  int arc_ml;
  bool flag_valid=false;
  for(set<int>::iterator it_ln=arcs_ln.begin();it_ln!=arcs_ln.end()&&flag_valid==false;it_ln++)
    {
      for(int i=0;i<4;i++)
	{
	  if(crossings[l].get_arc(i)==(*it_ln))
	    input_pos_ln=i;
	  if(crossings[n].get_arc(i)==(*it_ln))
	    input_pos_nl=i;
	}
      for(set<int>::iterator it_nm=arcs_nm.begin();it_nm!=arcs_nm.end()&&flag_valid==false;it_nm++)
	{
	  for(int i=0;i<4;i++)
	    {
	      if(crossings[m].get_arc(i)==(*it_nm))
		input_pos_mn=i;
	      if(crossings[n].get_arc(i)==(*it_nm))
		input_pos_nm=i;
	    }
	  for(set<int>::iterator it_ml=arcs_ml.begin();it_ml!=arcs_ml.end()&&flag_valid==false;it_ml++)
	    {
	      for(int i=0;i<4;i++)
		{
		  if(crossings[m].get_arc(i)==(*it_ml))
		    input_pos_ml=i;
		  if(crossings[l].get_arc(i)==(*it_ml))
		    input_pos_lm=i;
		}
	      //check valid
	      if((input_pos_lm==(input_pos_ln+1)%4)&&(input_pos_mn==(input_pos_ml+1)%4)&&(input_pos_nl==(input_pos_nm+1)%4))
		{
		  flag_valid=true;
		  arc_ln=(*it_ln);
		  arc_nm=(*it_nm);
		  arc_ml=(*it_ml);
		  break;
		}
	      else if((input_pos_ln==(input_pos_lm+1)%4)&&(input_pos_ml==(input_pos_mn+1)%4)&&(input_pos_nm==(input_pos_nl+1)%4))
		{
		  flag_valid=true;
		  arc_ln=(*it_ln);
		  arc_nm=(*it_nm);
		  arc_ml=(*it_ml);
		  break;
		}
	      
	    }
	}
    }
  if(flag_valid==false)
    return flag_remove;

  /////////////check that the triangular region inside crossings n,m,l is not exterior
  if(flag_planar)
    {
      if(crossings[l].get_region(input_pos_ln,input_pos_lm)==1)//exterior
	{
	  flag_remove=false;
	  return flag_remove;
	}
    }
  

  
  /////////////chekc under/over pass////////////
  int rand_above_below=int(random01()*1.999999);//rand_above_below=0 => choose arc below, rand_above_below=1 => choose arc above
  //find arc below all others (
  //Note: input_pos_x%2=0 => below, input_pos_x%2=1 => above
  if(input_pos_ln%2==rand_above_below&&input_pos_nl%2==rand_above_below)//n-l below
    {
      crossing_1=m;//crossing that does not change
      crossing_2=n;//crossing that are moved
      crossing_3=l;//crossing that are moved
      arc_shared_23=arc_ln; //arc between crossing_2 and crossing_3, that will move below (or above) crossing 1
      arc_shared_12=arc_nm; //arc between crossing_1 and crossing_2.
      arc_shared_13=arc_ml; //arc between crossing_1 and crossing_3.
      arc_out_12=crossings[crossing_1].get_arc((input_pos_mn+2)%4); //arc going out of crossing 1, connected to arc arc_shared_12
      arc_out_13=crossings[crossing_1].get_arc((input_pos_ml+2)%4); //arc going out of crossing 1, connected to arc arc_shared_13
      arc_out_21=crossings[crossing_2].get_arc((input_pos_nm+2)%4); //arc going out of crossing 2, connected to arc arc_shared_12
      arc_out_23=crossings[crossing_2].get_arc((input_pos_nl+2)%4); //arc going out of crossing 2, connected to arc arc_shared_23
      arc_out_31=crossings[crossing_3].get_arc((input_pos_lm+2)%4); //arc going out of crossing 3, connected to arc arc_shared_13 
      arc_out_32=crossings[crossing_3].get_arc((input_pos_ln+2)%4); //arc going out of crossing 3, connected to arc arc_shared_23
      input_pos_shared_23=input_pos_nl; //crossing[crossing_2].get_arc(input_pos_shared_23)=arc_shared_23
      input_pos_shared_12=input_pos_mn;  //crossing[crossing_1].get_arc(input_pos_shared_12)=arc_shared_12
      input_pos_shared_13=input_pos_ml;  //crossing[crossing_1].get_arc(input_pos_shared_13)=arc_shared_13
      input_pos_shared_32=input_pos_ln;  //crossing[crossing_3].get_arc(input_pos_shared_32)=arc_shared_23
      input_pos_shared_21=input_pos_nm;  //crossing[crossing_2].get_arc(input_pos_shared_21)=arc_shared_12
      input_pos_shared_31=input_pos_lm;  //crossing[crossing_3].get_arc(input_pos_shared_31)=arc_shared_13
      input_pos_out_23=(input_pos_nl+2)%4;  //crossing[crossing_2].get_arc(input_pos_out_23)=arc_out_23
      input_pos_out_32=(input_pos_ln+2)%4;  //crossing[crossing_3].get_arc(input_pos_out_32)=arc_out_32
      input_pos_out_12=(input_pos_mn+2)%4;  //crossing[crossing_1].get_arc(input_pos_out_12)=arc_out_12
      input_pos_out_21=(input_pos_nm+2)%4;  //crossing[crossing_2].get_arc(input_pos_out_21)=arc_out_21
      input_pos_out_13=(input_pos_ml+2)%4;  //crossing[crossing_1].get_arc(input_pos_out_13)=arc_out_13
      input_pos_out_31=(input_pos_lm+2)%4;  //crossing[crossing_3].get_arc(input_pos_out_31)=arc_out_31
      
    }
  else if(input_pos_ml%2==rand_above_below&&input_pos_lm%2==rand_above_below)//l-m below
    {
      crossing_1=n;//crossing that does not change
      crossing_2=l;//crossing that are moved
      crossing_3=m;//crossing that are moved
      arc_shared_23=arc_ml; //arc between crossing_2 and crossing_3, that will move below (or above) crossing 1
      arc_shared_12=arc_ln; //arc between crossing_1 and crossing_2.
      arc_shared_13=arc_nm; //arc between crossing_1 and crossing_3.
      arc_out_12=crossings[crossing_1].get_arc((input_pos_nl+2)%4); //arc going out of crossing 1, connected to arc arc_shared_12
      arc_out_13=crossings[crossing_1].get_arc((input_pos_nm+2)%4); //arc going out of crossing 1, connected to arc arc_shared_13
      arc_out_21=crossings[crossing_2].get_arc((input_pos_ln+2)%4); //arc going out of crossing 2, connected to arc arc_shared_12
      arc_out_23=crossings[crossing_2].get_arc((input_pos_lm+2)%4); //arc going out of crossing 2, connected to arc arc_shared_23
      arc_out_31=crossings[crossing_3].get_arc((input_pos_mn+2)%4); //arc going out of crossing 3, connected to arc arc_shared_13 
      arc_out_32=crossings[crossing_3].get_arc((input_pos_ml+2)%4); //arc going out of crossing 3, connected to arc arc_shared_23

      input_pos_shared_23=input_pos_lm; //crossing[crossing_2].get_arc(input_pos_shared_23)=arc_shared_23
      input_pos_shared_12=input_pos_nl;  //crossing[crossing_1].get_arc(input_pos_shared_12)=arc_shared_12
      input_pos_shared_13=input_pos_nm;  //crossing[crossing_1].get_arc(input_pos_shared_13)=arc_shared_13
      input_pos_shared_32=input_pos_ml;  //crossing[crossing_3].get_arc(input_pos_shared_32)=arc_shared_23
      input_pos_shared_21=input_pos_ln;  //crossing[crossing_2].get_arc(input_pos_shared_21)=arc_shared_12
      input_pos_shared_31=input_pos_mn;  //crossing[crossing_3].get_arc(input_pos_shared_31)=arc_shared_13
      input_pos_out_12=(input_pos_nl+2)%4; 
      input_pos_out_13=(input_pos_nm+2)%4; 
      input_pos_out_21=(input_pos_ln+2)%4; 
      input_pos_out_23=(input_pos_lm+2)%4; 
      input_pos_out_31=(input_pos_mn+2)%4; 
      input_pos_out_32=(input_pos_ml+2)%4; 
    }
  else if(input_pos_mn%2==rand_above_below&&input_pos_nm%2==rand_above_below)//m-n below
    {
      crossing_1=l;//crossing that does not change
      crossing_2=m;//crossing that are moved
      crossing_3=n;//crossing that are moved
      arc_shared_23=arc_nm; //arc between crossing_2 and crossing_3, that will move below (or above) crossing 1
      arc_shared_12=arc_ml; //arc between crossing_1 and crossing_2.
      arc_shared_13=arc_ln; //arc between crossing_1 and crossing_3.
      arc_out_12=crossings[crossing_1].get_arc((input_pos_lm+2)%4); //arc going out of crossing 1, connected to arc arc_shared_12
      arc_out_13=crossings[crossing_1].get_arc((input_pos_ln+2)%4); //arc going out of crossing 1, connected to arc arc_shared_13
      arc_out_21=crossings[crossing_2].get_arc((input_pos_ml+2)%4); //arc going out of crossing 2, connected to arc arc_shared_12
      arc_out_23=crossings[crossing_2].get_arc((input_pos_mn+2)%4); //arc going out of crossing 2, connected to arc arc_shared_23
      arc_out_31=crossings[crossing_3].get_arc((input_pos_nl+2)%4); //arc going out of crossing 3, connected to arc arc_shared_13 
      arc_out_32=crossings[crossing_3].get_arc((input_pos_nm+2)%4); //arc going out of crossing 3, connected to arc arc_shared_23
      input_pos_shared_23=input_pos_mn; //crossing[crossing_2].get_arc(input_pos_shared_23)=arc_shared_23
      input_pos_shared_12=input_pos_lm;  //crossing[crossing_1].get_arc(input_pos_shared_12)=arc_shared_12
      input_pos_shared_13=input_pos_ln;  //crossing[crossing_1].get_arc(input_pos_shared_13)=arc_shared_13
      input_pos_shared_32=input_pos_nm;  //crossing[crossing_3].get_arc(input_pos_shared_32)=arc_shared_23
      input_pos_shared_21=input_pos_ml;  //crossing[crossing_2].get_arc(input_pos_shared_21)=arc_shared_12
      input_pos_shared_31=input_pos_nl;  //crossing[crossing_3].get_arc(input_pos_shared_31)=arc_shared_13
      input_pos_out_12=(input_pos_lm+2)%4; 
      input_pos_out_13=(input_pos_ln+2)%4; 
      input_pos_out_21=(input_pos_ml+2)%4; 
      input_pos_out_23=(input_pos_mn+2)%4; 
      input_pos_out_31=(input_pos_nl+2)%4; 
      input_pos_out_32=(input_pos_nm+2)%4; 
    }
  else//no arc below all other
    return flag_remove;

  connected_crossing_out_12=crossings[crossing_1].get_connected_crossing(input_pos_out_12);
  connected_crossing_out_13=crossings[crossing_1].get_connected_crossing(input_pos_out_13);
  connected_crossing_out_21=crossings[crossing_2].get_connected_crossing(input_pos_out_21);
  connected_crossing_out_23=crossings[crossing_2].get_connected_crossing(input_pos_out_23);
  connected_crossing_out_31=crossings[crossing_3].get_connected_crossing(input_pos_out_31);
  connected_crossing_out_32=crossings[crossing_3].get_connected_crossing(input_pos_out_32);

    //WARNING: our current implementation of reidemeister_move_III will fail if any of the connected crossings (connected_crossing_out_*) is n,m or l
  //  =>we quit
  if(connected_crossing_out_12==n||connected_crossing_out_12==m||connected_crossing_out_12==l)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_out_13==n||connected_crossing_out_13==m||connected_crossing_out_13==l)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_out_21==n||connected_crossing_out_21==m||connected_crossing_out_21==l)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_out_23==n||connected_crossing_out_23==m||connected_crossing_out_23==l)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_out_31==n||connected_crossing_out_31==m||connected_crossing_out_31==l)
    {
      flag_remove=false;
      return flag_remove;
    }
  if(connected_crossing_out_32==n||connected_crossing_out_32==m||connected_crossing_out_32==l)
    {
      flag_remove=false;
      return flag_remove;
    }

  ////////////////debug///////////////
  if(flag_debug)
    {
      cerr<<"Reidemeister III:"<<endl;
      cerr<<" crossing_1="<<crossing_1<<endl;
      cerr<<" crossing_2="<<crossing_2<<endl;
      cerr<<" crossing_3="<<crossing_3<<endl;
      cerr<<" arc_shared_23="<<arc_shared_23<<endl;
      cerr<<" arc_shared_12="<<arc_shared_12<<endl;
      cerr<<" arc_shared_13="<<arc_shared_13<<endl;
      cerr<<" arc_out_23="<<arc_out_23<<endl;
      cerr<<" arc_out_32="<<arc_out_32<<endl;
      cerr<<" arc_out_12="<<arc_out_12<<endl;
      cerr<<" arc_out_21="<<arc_out_21<<endl;
      cerr<<" arc_out_13="<<arc_out_13<<endl;
      cerr<<" arc_out_31="<< arc_out_31<<endl;    
      cerr<<" connected_crossing_out_23="<<connected_crossing_out_23<<endl;
      cerr<<" connected_crossing_out_32="<<connected_crossing_out_32<<endl;
      cerr<<" connected_crossing_out_12="<<connected_crossing_out_12<<endl;
      cerr<<" connected_crossing_out_21="<<connected_crossing_out_21<<endl;
      cerr<<" connected_crossing_out_13="<<connected_crossing_out_13<<endl;
      cerr<<" connected_crossing_out_31="<<connected_crossing_out_31<<endl;
      cerr<<" input_pos_shared_23="<<input_pos_shared_23<<endl;
      cerr<<" input_pos_shared_12="<<input_pos_shared_12<<endl;
      cerr<<" input_pos_shared_13="<<input_pos_shared_13<<endl;
      cerr<<" input_pos_shared_32="<<input_pos_shared_32<<endl;
      cerr<<" input_pos_shared_21="<<input_pos_shared_21<<endl;
      cerr<<" input_pos_shared_31="<<input_pos_shared_31<<endl;
      cerr<<" input_pos_out_12="<<input_pos_out_12   <<endl;
      cerr<<" input_pos_out_13="<<input_pos_out_13   <<endl;
      cerr<<" input_pos_out_21="<<input_pos_out_21	<<endl; 
      cerr<<" input_pos_out_23="<<input_pos_out_23	<<endl; 
      cerr<<" input_pos_out_31="<<input_pos_out_31	<<endl; 
      cerr<<" input_pos_out_32="<<input_pos_out_32   <<endl;
    }

      //adapt region
      if(flag_planar)
	{
	  //7 original regions
	  int region_inside=crossings[crossing_1].get_region(input_pos_shared_12,input_pos_shared_13);
	  int region_outside_shared_23=crossings[crossing_2].get_region(input_pos_shared_23,input_pos_out_21);
	  int region_outside_shared_12=crossings[crossing_1].get_region(input_pos_shared_12,input_pos_out_13);
	  int region_outside_shared_13=crossings[crossing_1].get_region(input_pos_shared_13,input_pos_out_12);
	  int region_outside_3=crossings[crossing_3].get_region(input_pos_out_32,input_pos_out_31);
	  int region_outside_2=crossings[crossing_2].get_region(input_pos_out_23,input_pos_out_21);
	  int region_outside_1=crossings[crossing_1].get_region(input_pos_out_12,input_pos_out_13);
	  //Note: we should also have
	  //  region_inside=crossings[crossing_2].get_region(input_pos_shared_23,input_pos_shared_21);
	  //  region_inside=crossings[crossing_3].get_region(input_pos_shared_31,input_pos_shared_32);
	  //  region_outside_shared_23=crossings[crossing_3].get_region(input_pos_shared_32,input_pos_out_31);
	  //  region_outside_shared_12=crossings[crossing_2].get_region(input_pos_shared_21,input_pos_out_23);
	  //  region_outside_shared_13=crossings[crossing_3].get_region(input_pos_shared_31,input_pos_out_32);
	  if(region_inside!=crossings[crossing_2].get_region(input_pos_shared_23,input_pos_shared_21))
	    {cerr<<"ERROR 1"<<endl;exit(1);}
	  if(region_inside!=crossings[crossing_3].get_region(input_pos_shared_31,input_pos_shared_32))
	    {cerr<<"ERROR 2"<<endl;exit(1);}
	  if(region_outside_shared_23!=crossings[crossing_3].get_region(input_pos_shared_32,input_pos_out_31))
	    {cerr<<"ERROR 3"<<endl;exit(1);}
	  if(region_outside_shared_12!=crossings[crossing_2].get_region(input_pos_shared_21,input_pos_out_23))
	    {cerr<<"ERROR 4"<<endl;exit(1);}
	  if(region_outside_shared_13!=crossings[crossing_3].get_region(input_pos_shared_31,input_pos_out_32))
	    {cerr<<"ERROR 5"<<endl;exit(1);}


	  crossings[crossing_1].set_region(input_pos_shared_12,input_pos_shared_13,region_outside_shared_23);
	  crossings[crossing_1].set_region(input_pos_shared_13,input_pos_out_12,region_outside_3);
	  crossings[crossing_1].set_region(input_pos_out_12,input_pos_out_13,region_inside);
	  crossings[crossing_1].set_region(input_pos_out_13,input_pos_shared_12,region_outside_2);

	  crossings[crossing_2].set_region(input_pos_shared_23,input_pos_shared_21,region_outside_1);
	  crossings[crossing_2].set_region(input_pos_shared_21,input_pos_out_23,region_outside_shared_12);//not changed
	  crossings[crossing_2].set_region(input_pos_out_23,input_pos_out_21,region_outside_2);//not changed
	  crossings[crossing_2].set_region(input_pos_out_21,input_pos_shared_23,region_inside);

	  crossings[crossing_3].set_region(input_pos_shared_32,input_pos_shared_31,region_outside_1);
	  crossings[crossing_3].set_region(input_pos_shared_31,input_pos_out_32,region_outside_shared_13);//not changed
	  crossings[crossing_3].set_region(input_pos_out_32,input_pos_out_31,region_outside_3);//not changed
	  crossings[crossing_3].set_region(input_pos_out_31,input_pos_shared_32,region_inside);

	  
	}
      //change crossing_1
      crossings[crossing_1].set_arc(input_pos_shared_12,arc_out_21);
      crossings[crossing_1].set_connected_crossing(input_pos_shared_12,connected_crossing_out_21);
      crossings[crossing_1].set_arc(input_pos_shared_13,arc_out_31);
      crossings[crossing_1].set_connected_crossing(input_pos_shared_13,connected_crossing_out_31);
      crossings[crossing_1].set_arc(input_pos_out_13,arc_shared_13);
      crossings[crossing_1].set_connected_crossing(input_pos_out_13,crossing_2);
      crossings[crossing_1].set_arc(input_pos_out_12,arc_shared_12);
      crossings[crossing_1].set_connected_crossing(input_pos_out_12,crossing_3);

      //change crossing_2
      crossings[crossing_2].set_arc(input_pos_shared_21,arc_out_13);
      crossings[crossing_2].set_connected_crossing(input_pos_shared_21,connected_crossing_out_13);
      //input_pos_shared_23 => unchanged
      //input_pos_out_23 => unchanged
      crossings[crossing_2].set_arc(input_pos_out_21,arc_shared_13);
      crossings[crossing_2].set_connected_crossing(input_pos_out_21,crossing_1);
      //////reorder arcs to satisfy arc0 < arc2 (or arc0=maxindex and arc2=0) (BUT not arc0=maxindex=1 and arc2=0);
      if((crossings[crossing_2].arc0>crossings[crossing_2].arc2&&(!((crossings[crossing_2].arc0==maxarcindexe&&crossings[crossing_2].arc2==minarcindexe)&&(crossings[crossing_2].arc0!=crossings[crossing_2].arc2-1))))//minarcindex is the first arc
	 ||((crossings[crossing_2].arc0==minarcindexe&&crossings[crossing_2].arc2==maxarcindexe)&&(crossings[crossing_2].arc0!=crossings[crossing_2].arc2-1)))
	{
	  int a1=crossings[crossing_2].arc2;
	  int a2=crossings[crossing_2].arc3;
	  int a3=crossings[crossing_2].arc0;
	  int a4=crossings[crossing_2].arc1;
	  crossings[crossing_2].arc0=a1;
	  crossings[crossing_2].arc1=a2;
	  crossings[crossing_2].arc2=a3;
	  crossings[crossing_2].arc3=a4;	  
	  a1=crossings[crossing_2].connected_crossing2;
	  a2=crossings[crossing_2].connected_crossing3;
	  a3=crossings[crossing_2].connected_crossing0;
	  a4=crossings[crossing_2].connected_crossing1;
	  crossings[crossing_2].connected_crossing0=a1;
	  crossings[crossing_2].connected_crossing1=a2;
	  crossings[crossing_2].connected_crossing2=a3;
	  crossings[crossing_2].connected_crossing3=a4;	  
	  a1=crossings[crossing_2].region23;
	  a2=crossings[crossing_2].region30;
	  a3=crossings[crossing_2].region01;
	  a4=crossings[crossing_2].region12;
	  crossings[crossing_2].region01=a1;
	  crossings[crossing_2].region12=a2;
	  crossings[crossing_2].region23=a3;
	  crossings[crossing_2].region30=a4;	  
	}
      
      //change crossing_3
      crossings[crossing_3].set_arc(input_pos_shared_31,arc_out_12);
      crossings[crossing_3].set_connected_crossing(input_pos_shared_31,connected_crossing_out_12);
      //input_pos_shared_32 => unchanged
      //input_pos_out_32 => unchanged
      crossings[crossing_3].set_arc(input_pos_out_31,arc_shared_12);
      crossings[crossing_3].set_connected_crossing(input_pos_out_31,crossing_1);
      //////reorder arcs to satisfy arc0 < arc2 (or arc0=maxindex and arc2=0) (BUT not arc0=maxindex=1 and arc2=0);
      if((crossings[crossing_3].arc0>crossings[crossing_3].arc2&&(!((crossings[crossing_3].arc0==maxarcindexe&&crossings[crossing_3].arc2==minarcindexe)&&(crossings[crossing_3].arc0!=crossings[crossing_3].arc2-1))))//minarcindex is the first arc
	 ||((crossings[crossing_3].arc0==minarcindexe&&crossings[crossing_3].arc2==maxarcindexe)&&(crossings[crossing_3].arc0!=crossings[crossing_3].arc2-1)))
	{
	  int a1=crossings[crossing_3].arc2;
	  int a2=crossings[crossing_3].arc3;
	  int a3=crossings[crossing_3].arc0;
	  int a4=crossings[crossing_3].arc1;
	  crossings[crossing_3].arc0=a1;
	  crossings[crossing_3].arc1=a2;
	  crossings[crossing_3].arc2=a3;
	  crossings[crossing_3].arc3=a4;	  
	  a1=crossings[crossing_3].connected_crossing2;
	  a2=crossings[crossing_3].connected_crossing3;
	  a3=crossings[crossing_3].connected_crossing0;
	  a4=crossings[crossing_3].connected_crossing1;
	  crossings[crossing_3].connected_crossing0=a1;
	  crossings[crossing_3].connected_crossing1=a2;
	  crossings[crossing_3].connected_crossing2=a3;
	  crossings[crossing_3].connected_crossing3=a4;	  
	  a1=crossings[crossing_3].region23;
	  a2=crossings[crossing_3].region30;
	  a3=crossings[crossing_3].region01;
	  a4=crossings[crossing_3].region12;
	  crossings[crossing_3].region01=a1;
	  crossings[crossing_3].region12=a2;
	  crossings[crossing_3].region23=a3;
	  crossings[crossing_3].region30=a4;	  
	}

      //change crossing connected_crossing_out_12
      //WARNING: if crossings[connected_crossing_out_12].connected_crossing0,1,2,3=n,l,m for more than two outputs => can be problematic
      for(int i=0;i<4;i++)
	  if(crossings[connected_crossing_out_12].get_arc(i)==arc_out_12)
	      crossings[connected_crossing_out_12].set_connected_crossing(i,crossing_3);
      
      //change crossing connected_crossing_out_21
      for(int i=0;i<4;i++)
	  if(crossings[connected_crossing_out_21].get_arc(i)==arc_out_21)
	      crossings[connected_crossing_out_21].set_connected_crossing(i,crossing_1);
      //change crossing connected_crossing_out_13
      for(int i=0;i<4;i++)
	  if(crossings[connected_crossing_out_13].get_arc(i)==arc_out_13)
	      crossings[connected_crossing_out_13].set_connected_crossing(i,crossing_2);
      //change crossing connected_crossing_out_31
      for(int i=0;i<4;i++)
	  if(crossings[connected_crossing_out_31].get_arc(i)==arc_out_31)
	      crossings[connected_crossing_out_31].set_connected_crossing(i,crossing_1);
      //change crossing connected_crossing_out_23//unchanged
      //change crossing connected_crossing_out_32//unchanged
  
  
  return true;
}

//---------------------------------------------------------------------------
void PlanarDiagram::create_regions()
{
  if(flag_debug)
    cerr<<"PlanarDiagram::create_region"<<endl;

  //erase all regions except exterior
  for(int n=0;n<crossings.size();n++)
    for(int i=0;i<4;i++)
      if(crossings[n].get_region(i,(i+1)%4)!=1)
	crossings[n].set_region(i,(i+1)%4,-1);
  
  int r=1;
  if(get_nb_crossings(true)==0)//no real crossing, only endpoints and a unique region
    {
      r++;
      if(flag_debug)
	cerr<<"PlanarDiagram::create_region: 0 crossings (ignoring endpoints)"<<endl;
      for(int n=0;n<crossings.size();n++)
	{
	  for(int i=0;i<4;i++)
	    if(crossings[n].get_arc(i)>=0&&crossings[n].get_region(i,(i+1)%4)<0)//not yet defined
	      {
		crossings[n].set_region(i,(i+1)%4,r);	      				  
	      }
	}
      return;
    }
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// removed
	continue;
      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
	{
	  //do nothing
	}
      else//regular crossing
	{
	  for(int i0=0;i0<4;i0++)
	    {
	      if(crossings[n].get_region(i0,(i0+3)%4)>0)//already found
		continue;

	      r++;
	      /////////////////
	      int arc0;//incoming arc
	      int arc1;//outgoing arc
	      int n0;//current crossing
	      int n1;//next crossing
	      int pos_arc0;//such that crossings[n0].get_arc(pos_arc0)=arc0;
	      int pos_arc1;//such that crossings[n0].get_arc(pos_arc1)=arc1;
	      n0=n;
	      pos_arc0=i0;
	      pos_arc1=(i0+3)%4;
	      arc0=crossings[n0].get_arc(pos_arc0);
	      arc1=crossings[n0].get_arc(pos_arc1);
	      n1=crossings[n0].get_connected_crossing(pos_arc1);	     
	      crossings[n0].set_region(pos_arc0,pos_arc1,r);	      

	      if(flag_debug)
		cerr<<" create region: initial n0="<<n0<<" next crossing n1="<<n1<<" arc0="<<arc0<<" arc1="<<arc1<<" crossing["<<n0<<"].region: 0-1="<<crossings[n0].region01<<" 1-2="<<crossings[n0].region12<<" 2-3="<<crossings[n0].region23<<" 3-0="<<crossings[n0].region30<<endl;	      
	      while(true)
		{
		  bool flag_stop=false;
		  for(int i=0;i<4;i++)
		    {

		      if((n1!=n0&&(crossings[n1].get_arc(i)==arc1&&crossings[n1].get_connected_crossing(i)==n0))||
			 (n1==n0&&(crossings[n1].get_arc(i)==arc1&&crossings[n1].get_connected_crossing(i)==n0&&i!=pos_arc1)))
			{
			  //check if crossing is an endpoint
			  if(crossings[n1].arc0>=0&&crossings[n1].arc1<0&&crossings[n1].arc2<0&&crossings[n1].arc3<0)//endpoint
			    {
			      n0=n1;
			      arc0=arc1;
			      if(crossings[n1].region01>0)//already done
				flag_stop=true;
			      crossings[n1].region01=r;
			      arc1=crossings[n1].arc0;
			      pos_arc1=0;
			      n1=crossings[n1].connected_crossing0;
			      break;
			    }
			  else //regular crossing
			    {
			      n0=n1;
			      arc0=arc1;
			      pos_arc0=i;
			      pos_arc1=(i+3)%4;
			      if(crossings[n1].get_region(pos_arc0,pos_arc1)>0)//already done
				flag_stop=true;
			      crossings[n1].set_region(pos_arc0,pos_arc1,r);
			      arc1=crossings[n1].get_arc(pos_arc1);//next arc clockwise		    
			      n1=crossings[n1].get_connected_crossing(pos_arc1);//next arc clockwise		    
			      break;
			    }
			}
		    }
		  if(flag_stop)
		    break;
		  if(flag_debug)
		    cerr<<" create region: additional n0="<<n0<<" next crossing n1="<<n1<<" arc0="<<arc0<<" arc1="<<arc1<<" crossing["<<n0<<"].region: 0-1="<<crossings[n0].region01<<" 1-2="<<crossings[n0].region12<<" 2-3="<<crossings[n0].region23<<" 3-0="<<crossings[n0].region30<<endl;

		}		  
	    }
	}
    }
  
}
//---------------------------------------------------------------------------
int PlanarDiagram::get_writhe()
{
  //find min/max arc indexes
  int minarcindexe=-1;
  int maxarcindexe=0;
  for(int j=0;j<crossings.size();j++)
    for(int i=0;i<4;i++)    
      {
	maxarcindexe=max(maxarcindexe,crossings[j].get_arc(i));
	if(crossings[j].get_arc(i)>=0)
	  {
	    if(minarcindexe<0)
	      minarcindexe=crossings[j].get_arc(i);
	    else
	      minarcindexe=min(minarcindexe,crossings[j].get_arc(i));
	  }
      }
  int writhe=0;
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0>=0&&crossings[n].arc1>=0&&crossings[n].arc2>=0&&crossings[n].arc3>=0)//ignore empty crossing and endpoints
	{
	  if(crossings[n].arc1==maxarcindexe && crossings[n].arc3==minarcindexe)
	    writhe-=1;
	  else if(crossings[n].arc1==minarcindexe && crossings[n].arc3==maxarcindexe)
	    writhe+=1;
	  else
	    {
	      if(crossings[n].arc1<crossings[n].arc3)
		writhe-=1;
	      else if(crossings[n].arc1>crossings[n].arc3)
		writhe+=1;
	    }
	  if(crossings[n].arc0==crossings[n].arc2+1)//E.g. the special case X[1,0,0,1], which is somewhat ill defined
	    {
	      cerr<<"*******************************************"<<endl;
	      cerr<<"ERROR PlanarDiagram not valid (arc0=arc2+1)!"<<endl;
	      cerr<<"*******************************************"<<endl;
	      if(flag_debug)
		save_to_file_list(cerr);
	      exit(1);
	    }
	}
    }
  return writhe;
}

//---------------------------------------------------------------------------
bool PlanarDiagram::check()
{
  if(flag_debug)
    cerr<<"Checking diagram"<<endl;

  if(crossings.size()==0)
    return true;
  //check that each arc appears exactly twice & each crossing has no -1 arcs nor -1 crossing
  //and that arcs are numbered 0,1,2,3,..,nbarcs-1
  int maxarcindexe=0;
  int nbcrossings=0;//number of crossings (excluding endpoints & empty crossings
  bool flag_cyclic=true;
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//removed crossing
	{
	  if(flag_debug)
	    cerr<<" check diagram: crossing "<<n<<" to remove (all arcs=-1)"<<endl;
	}
      else if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
	{
	  flag_cyclic=false;
	  if(crossings[n].arc0<0)
	    {
	      if(flag_debug)
		cerr<<" check diagram: one  arc=-1"<<endl;
	      return false;
	    }
	  if(crossings[n].connected_crossing0<0)
	    {
	      if(flag_debug)
		cerr<<" check diagram: one  crossing index=-1"<<endl;
	      return false;
	    }
	}
      else//general crossing
	{
	  nbcrossings++;
	  if(crossings[n].arc0<0||crossings[n].arc1<0||crossings[n].arc2<0||crossings[n].arc3<0)//negative arc
	    {
	      if(flag_debug)
		cerr<<" check diagram: one  arc=-1"<<endl;
	      return false;
	    }
	  if(crossings[n].connected_crossing0<0||crossings[n].connected_crossing1<0||crossings[n].connected_crossing2<0||crossings[n].connected_crossing3<0)//negative crossing
	    {
	      if(flag_debug)
		cerr<<" check diagram: one  crossing index=-1"<<endl;
	      return false;
	    }
	}
      maxarcindexe=max(maxarcindexe,crossings[n].arc0);
      maxarcindexe=max(maxarcindexe,crossings[n].arc1);
      maxarcindexe=max(maxarcindexe,crossings[n].arc2);
      maxarcindexe=max(maxarcindexe,crossings[n].arc3);
    }
  vector<int> count(maxarcindexe+1,0);
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0>=0)
	count[crossings[n].arc0]++;
      if(crossings[n].arc1>=0)
	count[crossings[n].arc1]++;
      if(crossings[n].arc2>=0)
	count[crossings[n].arc2]++;
      if(crossings[n].arc3>=0)
	count[crossings[n].arc3]++;
    }
  for(int n=0;n<count.size();n++)
    {
      if(count[n]!=2)
	{
	  if(flag_debug)
	    cerr<<" check diagram: arc="<<n<<" appears "<<count[n]<<" times"<<endl;
	  return false;
	}
    }

  //check that connected_crossing0,1,2,3 are and correct
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//removed crossing
	  continue;
      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// endpoint
	{
	  if(crossings[n].connected_crossing0>=crossings.size())//not valid
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid crossing index"<<endl;
	      return false;
	    }
	  //check that the crossings are indeed connected
	  if(crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc0&&crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc1&&crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc2&&crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc3)//crossings[n].arc0 is not connected to crossings[crossings[n].connected_crossing0]
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid"<<endl;
	      return false;
	    }
	}
      else//general crossing
	{
	  if(crossings[n].connected_crossing0>=crossings.size()||crossings[n].connected_crossing1>=crossings.size()||crossings[n].connected_crossing2>=crossings.size()||crossings[n].connected_crossing3>=crossings.size())//not valid
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid crossing index"<<endl;
	      return false;
	    }
	  //check that the crossings are indeed connected
	  if(crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc0&&crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc1&&crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc2&&crossings[n].arc0!=crossings[crossings[n].connected_crossing0].arc3)//crossings[n].arc0 is not connected to crossings[crossings[n].connected_crossing0]
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid"<<endl;
	      return false;
	    }
	  if(crossings[n].arc1!=crossings[crossings[n].connected_crossing1].arc0&&crossings[n].arc1!=crossings[crossings[n].connected_crossing1].arc1&&crossings[n].arc1!=crossings[crossings[n].connected_crossing1].arc2&&crossings[n].arc1!=crossings[crossings[n].connected_crossing1].arc3)//crossings[n].arc1 is not connected to crossings[crossings[n].connected_crossing1]
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid"<<endl;
	      return false;
	    }
	  if(crossings[n].arc2!=crossings[crossings[n].connected_crossing2].arc0&&crossings[n].arc2!=crossings[crossings[n].connected_crossing2].arc1&&crossings[n].arc2!=crossings[crossings[n].connected_crossing2].arc2&&crossings[n].arc2!=crossings[crossings[n].connected_crossing2].arc3)//crossings[n].arc2 is not connected to crossings[crossings[n].connected_crossing2]
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid"<<endl;
	      return false;
	    }
	  if(crossings[n].arc3!=crossings[crossings[n].connected_crossing3].arc0&&crossings[n].arc3!=crossings[crossings[n].connected_crossing3].arc1&&crossings[n].arc3!=crossings[crossings[n].connected_crossing3].arc2&&crossings[n].arc3!=crossings[crossings[n].connected_crossing3].arc3)//crossings[n].arc3 is not connected to crossings[crossings[n].connected_crossing3]
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid"<<endl;
	      return false;
	    }	   
	}
    }
      
  //check crossings are ordered by order of appearance when following the curve.
  //check that in/out arcs differ by 1: arc0=arc2-1 abs(arc1-arc3)=1, and endpoint have arc 0 or maxarcindex. For cyclic case
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//removed crossing
	continue;
      if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// endpoint
	{
	  if(crossings[n].arc0!=0&&crossings[n].arc0!=maxarcindexe)
	    {
	      if(flag_debug)
		cerr<<" check diagram: invalid, endpoint crossing["<<n<<"] arc0"<<crossings[n].arc0<<" !=0 or "<<maxarcindexe<<endl;
	      return false;
	    }
	  continue;
	}

      if((crossings[n].arc0-crossings[n].arc2)!=-1&&(crossings[n].arc0-crossings[n].arc2)!=maxarcindexe)
	{
	  if(flag_debug)
	    cerr<<" check diagram: invalid, crossing["<<n<<"] arc0="<<crossings[n].arc0<<" and arc2"<<"="<<crossings[n].arc2<<" are not consecutive"<<endl;
	  return false;
	}
      if(crossings[n].arc0==crossings[n].arc2+1)//only for the special case of a unique crossing, e.g. X[1,0,0,1] which should not be valid (should be X[0,1,1,0])
	{
	  if(flag_debug)
	    cerr<<" check diagram: invalid, crossing["<<n<<"] arc0="<<crossings[n].arc0<<" and arc2"<<"="<<crossings[n].arc2<<" are not consecutive"<<endl;
	  return false;
	}
      if(abs(crossings[n].arc1-crossings[n].arc3)!=1&&abs(crossings[n].arc1-crossings[n].arc3)!=maxarcindexe)
	{
	  if(flag_debug)
	    cerr<<" check diagram: invalid, crossing["<<n<<"] arc1="<<crossings[n].arc1<<" and arc3="<<crossings[n].arc3<<" are not consecutive"<<endl;
	  return false;
	}
    }
  /////////check exterior boundary://////
  if(flag_planar)
    {
      int count_exterior=0;
      //check side of exterior, if region between arc(i) and arc(i+1) is exterior (resp. interior), then
      //  for crossing m=connected_crossing(i+1) with crossings[m].arc(j)=crossings[m].arc(i+i), we should have region between arc(j) and arc(j+1) exterior (resp. interior)
      //NOTE: the same should also be true for  crossing m=connected_crossing(i) with crossings[m].arc(j)=crossings[m].arc(i), we should have region between arc(j-1) and arc(j) exterior (resp. interior), but no need to check since it will be done from crossing m
      for(int n=0;n<crossings.size();n++)
	{
	  if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//removed crossing
	    continue;
	  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)// endpoint
	    {
	      if(crossings[n].region01==1)
		count_exterior+=1;
	      int m=crossings[n].connected_crossing0;
	      if(crossings[m].arc0>=0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)// endpoint
		{
		  if(crossings[m].region01!=crossings[n].region01)
		    {
		      if(flag_debug)
			cerr<<" check diagram: crossing "<<n<<" invalid region. region not consistent with connected crossing "<<m<<endl;
		      return false;
		    }
		  continue;
		}
	      for(int j=0;j<4;j++)
		{
		  if(crossings[m].get_arc(j)==crossings[n].arc0)//arc connected to crossings[n]
		    {
		      if(crossings[m].get_region(j,(j+1)%4)!=crossings[n].region01)
			{
			  if(flag_debug)
			    cerr<<" check diagram: crossing "<<n<<" invalid region. region not consistent with connected crossing "<<m<<endl;
			  return false;
			}			    
		    }
		}
	    }
	  else
	    {
	      for(int i=0;i<4;i++)
		{
		  if(crossings[n].get_region(i,(i+1)%4)==1)
		    count_exterior+=1;		  
		  int m=crossings[n].get_connected_crossing((i+1)%4);
		  if(crossings[m].arc0>=0&&crossings[m].arc1<0&&crossings[m].arc2<0&&crossings[m].arc3<0)// endpoint
		    {
		      if(crossings[m].region01!=crossings[n].get_region(i,(i+1)%4))
			{
			  if(flag_debug)
			    cerr<<" check diagram: crossing "<<n<<" invalid region. region not consistent with connected crossing "<<m<<endl;
			  return false;
			}
		      continue;
		    }
		  for(int j=0;j<4;j++)
		    {
		      if(crossings[m].get_arc(j)==crossings[n].get_arc((i+1)%4))//arc connected to crossings[n]
			{
			  if(m==n&&(i+1)%4==j)continue;//to be sure we do not use same side of the arc
			  if(crossings[m].get_region(j,(j+1)%4)!=crossings[n].get_region(i,(i+1)%4))
			    {
			      if(flag_debug)
				cerr<<" check diagram: crossing "<<n<<" invalid region. region not consistent with connected crossing "<<m<<endl;
			      return false;
			    }			    
			}
		    }
		}
	    }
	}
      if(count_exterior==0)
	{
	  if(flag_debug)
	    cerr<<" check diagram: no exterior region (region=1)"<<endl;
	  return false;
	}			    

    }


    //check that nb arcs=2*nb crossings for closed diagram, and nb arcs=2*nb crossings + 1
  int nbarcs=count.size();
  if(flag_cyclic&&nbarcs!=2*nbcrossings)
    {
      if(flag_debug)
	cerr<<" check diagram: cyclic diagram with (nb arcs)!=2*(nb crossings)"<<endl;
      return false;
    }
  if(!flag_cyclic&&nbarcs!=2*nbcrossings+1)
    {
      if(flag_debug)
	cerr<<" check diagram: open diagram with (nb arcs)!=2*(nb crossings)+1"<<endl;
      return false;
    }
  
  if(!check_planar())
    {
      return false;
    }
  return true;
  
}

//---------------------------------------------------------------------------
bool PlanarDiagram::check_planar()
{
  //definitions for boost graph
  typedef adjacency_list < vecS, vecS,undirectedS,NodeProperties_component, EdgeProperties_component > graph_t; //edges setS => no parallel edges
  typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;

  //create graph to evaluate biconnected components with boost
  graph_t graph;
  vertex_descriptor vid;
  edge_descriptor eid;
  bool flag_added;
  vector<vertex_descriptor> map_crossing_to_vertex(crossings.size(),-1);
  for(int n=0;n<crossings.size();n++)
    {
      if(crossings[n].arc0<0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//empty (removed) crossing
	continue;
      vid=add_vertex(graph);
      graph[vid].crossing=n;
      map_crossing_to_vertex[n]=vid;
    }

  for(int n1=0;n1<crossings.size();n1++)
    for(int i=0;i<4;i++)
      {
	if(crossings[n1].get_arc(i)>=0)
	  {
	    int n2=crossings[n1].get_connected_crossing(i);
	    if(n1<n2)
	      {
		if(!edge(map_crossing_to_vertex[n1], map_crossing_to_vertex[n2],graph).second)//do not add multi edges
		  {
		    tie(eid,flag_added)=add_edge( map_crossing_to_vertex[n1], map_crossing_to_vertex[n2],graph);
		    graph[eid].component=-10;
		  }
	      }
	    else if(n1==n2)//ignore self loops (each self loop is a biconnected component)
	      {
		if(crossings[n1].get_arc(i)==crossings[n1].get_arc((i+2)%4))//check the articulation point for the self loop
		  {
		    if(flag_debug)
		      	cerr<<" check diagram planarity: non-planar diagram (self-loop)"<<endl;
		    return false;
		  }
	      }
	  }
      }

    vector<vertex_descriptor> articulation_points;    
    biconnected_components(graph, get(&EdgeProperties_component::component, graph),back_inserter(articulation_points)); 
    
    if(flag_debug)
      {
	cerr<<"biconnected components:"<<endl;
	for(int n1=0;n1<crossings.size();n1++)
	  for(int i=0;i<4;i++)
	    {
	      if(crossings[n1].get_arc(i)<0)
		continue;
	      int n2=crossings[n1].get_connected_crossing(i);
	      if(n1==n2)//ignore self loops
		continue;
	      pair<edge_descriptor,bool> e=edge(map_crossing_to_vertex[n1], map_crossing_to_vertex[n2],graph);
	      if(!e.second)
		{
		  cerr<<"ERROR: crossing "<<n1<<" pos "<<i<<" to crossing "<<n2<<" does not exist"<<endl;
		  exit(1);
		}
	      cerr<<" arc "<<crossings[n1].get_arc(i)<<" (crossings "<<n1<<"-"<<n2<<")  component "<<graph[e.first].component<<endl;
	    }

	cerr<<"articulation points:"<<endl;
	for(int i=0;i<articulation_points.size();i++)
	  cerr<<"  crossing "<<graph[articulation_points[i]].crossing<<endl;
      }

    
    //check articulation points (ignoring self loops)
    for(int l=0;l<articulation_points.size();l++)
      {
	int n=graph[articulation_points[l]].crossing;
	for(int i=0;i<2;i++)
	  if(crossings[n].get_arc(i)==crossings[n].get_arc((i+2)%4)
	     &&crossings[n].get_arc(i)!=crossings[n].get_arc((i+1)%4)
	     &&crossings[n].get_arc(i)!=crossings[n].get_arc((i+3)%4))
	    {
	      if(flag_debug)
		cerr<<" check diagram planarity: non-planar diagram (illegal articulation point)"<<endl;
	      return false;
	    }
      }



    ////////////////
    //generate candidate faces  (ignoring self loops) of biconnected component subragraphs and check that they form simple loops
    vector<bool> done(crossings.size()*4,false);//done[n*4+i]=done if crossings[n].get_arc(i) has already been done
    for(int n0=0;n0<crossings.size();n0++)
      {
	if(crossings[n0].arc0<0&&crossings[n0].arc1<0&&crossings[n0].arc2<0&&crossings[n0].arc3<0)// removed
	  continue;
	if(crossings[n0].arc0>=0&&crossings[n0].arc1<0&&crossings[n0].arc2<0&&crossings[n0].arc3<0)//endpoint
	  continue;
	for(int i0=0;i0<4;i0++)
	  {
	    if(done[n0*4+i0])//already found
	      continue;
	    if(n0==crossings[n0].get_connected_crossing(i0))//self loop
		continue;
	    done[n0*4+i0]=true;

	    /////////////////
	    int arc1;//current outgoing arc
	    int n1;//current crossing
	    int i1;//current outgoing pos
	    int n2;//next crossing
	    int i2;//next outgoing pos
	    n1=n0;
	    i1=i0;
	    set<int> face;
	    int biconnected_component=graph[edge(map_crossing_to_vertex[n0], map_crossing_to_vertex[crossings[n0].get_connected_crossing(i0)],graph).first].component;
	    if(flag_debug)
	      cerr<<" face for biconnected component "<<biconnected_component<<endl;	      
	    while(true)
	      {
		  
		done[n1*4+i1]=true;
		if(flag_debug)
		  cerr<<"   arc "<<crossings[n1].get_arc(i1)<<" (crossings "<<n1<<"-"<<crossings[n1].get_connected_crossing(i1)<<")"<<endl;

		//check if face is a simple cycle (i.e. each point appears only once)
		if(!face.insert(n1).second)
		  {
		    if(flag_debug)
		      cerr<<" face in biconnected component "<<biconnected_component<<" is not a simple cycle"<<endl;
		    return false;
		  }
		//find next 
		n2=crossings[n1].get_connected_crossing(i1);
		i2=-1;
		for(int i=0;i<4;i++)
		  {
		    if(crossings[n2].get_arc(i)==crossings[n1].get_arc(i1)&&crossings[n2].get_connected_crossing(i)==n1&&!(n1==n2&&i1==i))
		      {
			
			if(crossings[n2].arc0>=0&&crossings[n2].arc1<0&&crossings[n2].arc2<0&&crossings[n2].arc3<0)//endpoint
			  i2=0;
			else
			  {
			    i2=(i+3)%4;
			    //ignore self loop & follow arcs on same component
			    while(n2==crossings[n2].get_connected_crossing(i2)||biconnected_component!=graph[edge(map_crossing_to_vertex[n2], map_crossing_to_vertex[crossings[n2].get_connected_crossing(i2)],graph).first].component)
			      {
				i2=(i2+3)%4;
			      }
			  }
			break;
		      }
		  }
		if(i2<0)
		  {
		    cerr<<"ERROR i2<0"<<endl;
		    exit(1);
		  }
		if(n2==n0&&i2==i0)		  
		  break;
		n1=n2;
		i1=i2;		
	      }
	  }
      }

  return true;
  
}
