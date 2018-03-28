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


#include "Polygon.hh"


using namespace std;

//************************************************************
//**********************   Polygon   **********************
//************************************************************
Polygon::Polygon(bool flag_debug):flag_debug(flag_debug)
{

}
//---------------------------------------------------------------------------
Polygon::Polygon(string filename,bool flag_cyclic,bool flag_debug):flag_cyclic(flag_cyclic),flag_debug(flag_debug)
{
  load_from_file_xyz(filename,flag_cyclic);
}

//---------------------------------------------------------------------------
Polygon::Polygon(const vector<double> & x,const vector<double> & y,const vector<double> & z,bool flag_cyclic,bool flag_debug):flag_cyclic(flag_cyclic),flag_debug(flag_debug)
{
  if(!(x.size()==y.size()&&x.size()==z.size()))
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR Polygon::Polygon(x,y,z) x.size()="<<x.size()<<" y.size()="<<y.size()<<" z.size()="<<z.size()<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }
  polygon_x=x;
  polygon_y=y;
  polygon_z=z;
}
//---------------------------------------------------------------------------
void Polygon::load_from_file_xyz(string filename,bool flag_cyclic_t)
{
  flag_cyclic=flag_cyclic_t;
  if(filename=="stdin"||filename=="-")
    load_from_file_xyz(cin,flag_cyclic);
  else
    {
      ifstream file(filename.c_str());
      if(!file.good())
	{
	  cerr<<"*********************************************************"<<endl;
	  cerr<<"ERROR problem with file "<<filename <<endl;
	  cerr<<"Does the file exist?"<<endl;
	  cerr<<"*********************************************************"<<endl;
	  exit(1);
	}
      load_from_file_xyz(file,flag_cyclic);
      file.close();
    }
}

//---------------------------------------------------------------------------

void Polygon::load_from_file_xyz(istream& file,bool flag_cyclic_t)
{
  flag_cyclic=flag_cyclic_t;
  polygon_x.clear();
  polygon_y.clear();
  polygon_z.clear();
  string tmpline;
  vector<string> fields;
  
  while(file.good())
    {
      getline (file,tmpline);
      //remove comments
      size_t pos1 = tmpline.find('#');
      if (pos1!=string::npos)
	{
	  tmpline.erase(pos1);
	}
      if(file.good()&&tmpline.length()>0)
	{
	  fields=split_string(tmpline," \t");	   
	  if(fields.size()!=3)
	    {
	      cerr<<"*********************************************************"<<endl;
	      cerr<<"ERROR: invalid format."<<endl;
	      cerr<<"*********************************************************"<<endl;
	      exit(1);
	    }
	  //remove trailing \r 
	  if(fields[2].size()>0&&fields[2][fields[2].size()-1]=='\r')
	      fields[2].erase(fields[2].end() - 1);
	  polygon_x.push_back(atof(fields[0].c_str()));
	  polygon_y.push_back(atof(fields[1].c_str()));
	  polygon_z.push_back(atof(fields[2].c_str()));	  
	}
    }
  
  
}

//---------------------------------------------------------------------------
void Polygon::save_to_file_xyz(string filename)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_xyz(cout);
  else if(filename=="stderr")
    save_to_file_xyz(cerr);
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
      save_to_file_xyz(file);
      file.close();
    }
}

//---------------------------------------------------------------------------


void Polygon::save_to_file_xyz(ostream& file)
{
  for(int i=0;i<polygon_x.size();i++)
    file<<polygon_x[i]<<" "<<polygon_y[i]<<" "<<polygon_z[i]<<endl;
  
}
//---------------------------------------------------------------------------
void Polygon::save_to_file_hoomd(string filename,int nb_helper_beads_per_segment,double dx,double dy,double dz)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_hoomd(cout,nb_helper_beads_per_segment,dx,dy,dz);
  else if(filename=="stderr")
    save_to_file_hoomd(cerr,nb_helper_beads_per_segment,dx,dy,dz);
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
      save_to_file_hoomd(file,nb_helper_beads_per_segment,dx,dy,dz);
      file.close();
    }
}
//---------------------------------------------------------------------------

void Polygon::save_to_file_hoomd(ostream& file,int nb_helper_beads_per_segment,double dx,double dy,double dz)
{

  bool flag_axis=false;
  double dnorm=sqrt(dx*dx+dy*dy+dz*dz);
  if(sqrt(dx*dx+dy*dy+dz*dz)>1e-4)
    {
      flag_axis=true;
      dx=dx/dnorm;
      dy=dy/dnorm;
      dz=dz/dnorm;
    }
  if(flag_cyclic)flag_axis=false;

  int endpoint1_i;//index of first endpoint in polygon_final_x;
  int endpoint2_i;//index of second endpoint in polygon_final_x;
  vector<double> polygon_final_x;
  vector<double> polygon_final_y;
  vector<double> polygon_final_z;
  vector<string> polygon_final_type;

  ////////////evaluate distance between beads and add helper beads////////////////
  
  ///find separation between consecutive beads
  double d=sqrt((polygon_x[1]-polygon_x[0])*(polygon_x[1]-polygon_x[0])+(polygon_y[1]-polygon_y[0])*(polygon_y[1]-polygon_y[0])+(polygon_z[1]-polygon_z[0])*(polygon_z[1]-polygon_z[0]));
  double dmin=d;
  double dmax=d;
  for(int i=0;i<polygon_x.size()-1;i++)
    {
      d=sqrt((polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])*(polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])+(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])*(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])+(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i])*(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i]));
      if(d>dmax)dmax=d;
      if(d<dmin)dmin=d;
    }
  if(flag_cyclic)
    {
      int i=polygon_x.size()-1;
      d=sqrt((polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])*(polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])+(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])*(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])+(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i])*(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i]));
      if(d>dmax)dmax=d;
      if(d<dmin)dmin=d;
    }
  double scalefactor=nb_helper_beads_per_segment/dmin;
  if(nb_helper_beads_per_segment==0)
     scalefactor=1/dmax;
  //rescale to have dmin=1, and add beads if separation is too high
  endpoint1_i=0;
  for(int i=0;i<polygon_x.size()-1;i++)
    {
      //add bead
      polygon_final_x.push_back(polygon_x[i]);
      polygon_final_y.push_back(polygon_y[i]);
      polygon_final_z.push_back(polygon_z[i]);      
      polygon_final_type.push_back("CA");      
      d=sqrt((polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])*(polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])+(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])*(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])+(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i])*(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i]));
      //add helper beads if necessary
      int N=ceil(d*scalefactor-1e-8);
      for(int j=1;j<N;j++)
	{
	  double t=j/(double)N;
	  polygon_final_x.push_back((polygon_x[i]+t*(polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])));
	  polygon_final_y.push_back((polygon_y[i]+t*(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])));
	  polygon_final_z.push_back((polygon_z[i]+t*(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i])));
	  polygon_final_type.push_back("helper");      	  
	}	
    }
  //add last bead
  endpoint2_i=polygon_final_x.size();
  polygon_final_x.push_back(polygon_x[polygon_x.size()-1]);
  polygon_final_y.push_back(polygon_y[polygon_x.size()-1]);
  polygon_final_z.push_back(polygon_z[polygon_x.size()-1]);      
  polygon_final_type.push_back("CA");      
  if(flag_cyclic)
    {
      //add helper beads to last segment
      int i=polygon_x.size()-1;
      d=sqrt((polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])*(polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])+(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])*(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])+(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i])*(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i]));
      //add helper beads if necessary
      int N=ceil(d*scalefactor-1e-8);
      for(int j=1;j<N;j++)
	{
	  double t=j/(double)N;
	  polygon_final_x.push_back((polygon_x[i]+t*(polygon_x[(i+1)%polygon_x.size()]-polygon_x[i])));
	  polygon_final_y.push_back((polygon_y[i]+t*(polygon_y[(i+1)%polygon_x.size()]-polygon_y[i])));
	  polygon_final_z.push_back((polygon_z[i]+t*(polygon_z[(i+1)%polygon_x.size()]-polygon_z[i])));
	  polygon_final_type.push_back("helper");      	  
	}	
    }
  /////////normalize////////////////
  for(int i=0;i<polygon_final_x.size();i++)
    {      
      polygon_final_x[i]=polygon_final_x[i]*scalefactor;
      polygon_final_y[i]=polygon_final_y[i]*scalefactor;
      polygon_final_z[i]=polygon_final_z[i]*scalefactor;
    }
  //rotate to bring axis parallel to ez
  if(flag_axis)
    {
      double cos_a=dz;//|d|=|ez|=1
      //n=d^ez;
      double nx=dy;
      double ny=-dx;
      double nz=0;
      double sin_a=sqrt(nx*nx+ny*ny+nz*nz);//|d|=|ez|=1
      double normn=sin_a;
      if(normn>ZERO)
	{
	  nx=nx/normn;
	  ny=ny/normn;
	  nz=nz/normn;
	  for(int i=0;i<polygon_final_x.size();i++)
	    {
	      double x=polygon_final_x[i];
	      double y=polygon_final_y[i];
	      double z=polygon_final_z[i];
	      polygon_final_x[i]=(cos_a+nx*nx*(1-cos_a)         )*x + (     +nx*ny*(1-cos_a)         )*y + (     +ny*sin_a)*z;
	      polygon_final_y[i]=(     +nx*ny*(1-cos_a)         )*x + (cos_a+ny*ny*(1-cos_a)         )*y + (     -nx*sin_a)*z;
	      polygon_final_z[i]=(                     -ny*sin_a)*x + (                     +nx*sin_a)*y + (cos_a         )*z;
	    }
	  //rotate axis
	  double x=dx;
	  double y=dy;
	  double z=dz;
	  dx=(cos_a+nx*nx*(1-cos_a)         )*x + (     +nx*ny*(1-cos_a)         )*y + (     +ny*sin_a)*z;
	  dy=(     +nx*ny*(1-cos_a)         )*x + (cos_a+ny*ny*(1-cos_a)         )*y + (     -nx*sin_a)*z;
	  dz=(                     -ny*sin_a)*x + (                     +nx*sin_a)*y + (cos_a         )*z;
	}
    }
  ////////////end normalize///////////  
  double boxLx=2+((*max_element(polygon_final_x.begin(),polygon_final_x.end()))-(*min_element(polygon_final_x.begin(),polygon_final_x.end())));
  double boxLy=2+((*max_element(polygon_final_y.begin(),polygon_final_y.end()))-(*min_element(polygon_final_y.begin(),polygon_final_y.end())));
  double boxLz=2+((*max_element(polygon_final_z.begin(),polygon_final_z.end()))-(*min_element(polygon_final_z.begin(),polygon_final_z.end())));
  double minx=(*min_element(polygon_final_x.begin(),polygon_final_x.end()));
  double miny=(*min_element(polygon_final_y.begin(),polygon_final_y.end()));
  double minz=(*min_element(polygon_final_z.begin(),polygon_final_z.end()));
  
  ///////add axis
  vector<double> axis1_x;
  vector<double> axis1_y;
  vector<double> axis1_z;
  vector<double> axis2_x;
  vector<double> axis2_y;
  vector<double> axis2_z;
  if(flag_axis)
    {
      int nb_axis_beads=ceil(boxLz);
      double step=boxLz/nb_axis_beads;
      double x0=polygon_final_x[endpoint1_i];
      double y0=polygon_final_y[endpoint1_i];
      double z0=polygon_final_z[endpoint1_i];
      double x=x0+step*dx;
      double y=y0+step*dy;
      double z=z0+step*dz;
      for(int i=0;i<nb_axis_beads-1;i++)
	{
	  axis1_x.push_back(x);
	  axis1_y.push_back(y);
	  axis1_z.push_back(z);
	  x=x+step*dx;
	  y=y+step*dy;
	  z=z+step*dz;	  
	}
      x0=polygon_final_x[endpoint2_i];
      y0=polygon_final_y[endpoint2_i];
      z0=polygon_final_z[endpoint2_i];
      x=x0+step*dx;
      y=y0+step*dy;
      z=z0+step*dz;
      for(int i=0;i<nb_axis_beads-1;i++)
	{
	  axis2_x.push_back(x);
	  axis2_y.push_back(y);
	  axis2_z.push_back(z);
	  x=x+step*dx;
	  y=y+step*dy;
	  z=z+step*dz;	  
	}

    }
  
  file<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl;
  file<<"<hoomd_xml version=\"1.4\">"<<endl;
  file<<"<configuration time_step=\"0\" dimensions=\"3\" natoms=\""<<polygon_final_x.size()+axis1_x.size()+axis2_x.size() <<"\">"<<endl;

  file<<"<box lx=\""<<boxLx<<"\" ly=\""<<boxLy<<"\" lz=\""<<boxLz<<"\"/>"<<endl;

  file<<"<position>"<<endl;
  for(int i=0;i<polygon_final_x.size();i++)
    file<<fmod(polygon_final_x[i]+1-minx+10*boxLx,boxLx)-boxLx/2.0<<" "<<fmod(polygon_final_y[i]+1-miny+10*boxLy,boxLy)-boxLy/2.0<<" "<<fmod(polygon_final_z[i]+1-minz+10*boxLz,boxLz)-boxLz/2.0<<endl;
  for(int i=0;i<axis1_x.size();i++)
    file<<fmod(axis1_x[i]+1-minx+10*boxLx,boxLx)-boxLx/2.0<<" "<<fmod(axis1_y[i]+1-miny+10*boxLy,boxLy)-boxLy/2.0<<" "<<fmod(axis1_z[i]+1-minz+10*boxLz,boxLz)-boxLz/2.0<<endl;
  for(int i=0;i<axis2_x.size();i++)
    file<<fmod(axis2_x[i]+1-minx+10*boxLx,boxLx)-boxLx/2.0<<" "<<fmod(axis2_y[i]+1-miny+10*boxLy,boxLy)-boxLy/2.0<<" "<<fmod(axis2_z[i]+1-minz+10*boxLz,boxLz)-boxLz/2.0<<endl;
  file<<"</position>"<<endl;

  file<<"<diameter>"<<endl;
  for(int i=0;i<polygon_final_x.size();i++)
    file<<"1"<<endl;
  for(int i=0;i<axis1_x.size();i++)
    file<<1<<endl;
  for(int i=0;i<axis2_x.size();i++)
    file<<1<<endl;
  file<<"</diameter>"<<endl;
  
  file<<"<type>"<<endl;
  if(flag_axis)
    file<<"axis"<<endl;
  else
    file<<polygon_final_type[0]<<endl;
  for(int i=1;i<polygon_final_x.size()-1;i++)
    file<<polygon_final_type[i]<<endl;
  if(flag_axis)
    file<<"axis"<<endl;
  else
    file<<polygon_final_type[polygon_final_x.size()-1]<<endl;
  for(int i=0;i<axis1_x.size();i++)
    file<<"axis"<<endl;
  for(int i=0;i<axis2_x.size();i++)
    file<<"axis"<<endl;
  file<<"</type>"<<endl;

  file<<"<bond>"<<endl;
  if(flag_axis)
    file<<"polymer_axis_bond "<<0<<" "<<1<<endl;
  else
    file<<"polymer "<<0<<" "<<1<<endl;
  for(int i=1;i<polygon_final_x.size()-2;i++)
    file<<"polymer "<<i<<" "<<(i+1)%polygon_final_x.size()<<endl;
  if(flag_axis)
    file<<"polymer_axis_bond "<<polygon_final_x.size()-2<<" "<<polygon_final_x.size()-1<<endl;
  else
    file<<"polymer "<<polygon_final_x.size()-2<<" "<<polygon_final_x.size()-1<<endl;

  if(flag_cyclic)
    file<<"polymer "<<polygon_final_x.size()-1<<" "<<0<<endl;
  if(flag_axis)
    {
      file<<"axis_bond "<<0<<" "<<polygon_final_x.size()<<endl;//fist endpoint of polygon to first bead of axis
      file<<"axis_bond "<<0<<" "<<polygon_final_x.size()+axis1_x.size()-1<<endl;//fist endpoint of polygon to last bead of axis
      file<<"axis_bond "<<polygon_final_x.size()-1<<" "<<polygon_final_x.size()+axis1_x.size()<<endl;//last endpoint of polygon to first bead of axis2
      file<<"axis_bond "<<polygon_final_x.size()-1<<" "<<polygon_final_x.size()+axis1_x.size()+axis2_x.size()-1<<endl;//last endpoint of polygon to last bead of axis2
      for(int i=0;i<axis1_x.size()-1;i++)
	file<<"axis_bond "<<polygon_final_x.size()+i<<" "<<polygon_final_x.size()+(i+1)<<endl;
      for(int i=0;i<axis2_x.size()-1;i++)
	file<<"axis_bond "<<polygon_final_x.size()+axis1_x.size()+i<<" "<<polygon_final_x.size()+axis1_x.size()+(i+1)<<endl;
    }
  file<<"</bond>"<<endl;
  
  file<<"<angle>"<<endl;
  for(int i=0;i<polygon_final_x.size()-2;i++)
    file<<"CACACA "<<i<<" "<<(i+1)%polygon_final_x.size()<<" "<<(i+2)%polygon_final_x.size()<<endl;
  if(flag_cyclic)
    {
      file<<"CACACA "<<polygon_final_x.size()-2<<" "<<polygon_final_x.size()-1<<" "<<0<<endl;
      file<<"CACACA "<<polygon_final_x.size()-1<<" "<<0<<" "<<1<<endl;
    }
  file<<"</angle>"<<endl;

  file<<"</configuration>"<<endl;
  file<<"</hoomd_xml>"<<endl;  
}


//---------------------------------------------------------------------------
void Polygon::save_to_file_vmdxyz(string filename)
{
  if(filename=="stdout"||filename=="-")
    save_to_file_vmdxyz(cout);
  else if(filename=="stderr")
    save_to_file_vmdxyz(cerr);
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
      save_to_file_vmdxyz(file);
      file.close();
    }
}
//---------------------------------------------------------------------------

void Polygon::save_to_file_vmdxyz(ostream& file)
{
  if(!file.good())
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR Polygon::save_to_file_vmdxyz() problem with file. Does the file exist?"<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }

  file<<polygon_x.size()<<endl;
  file<<"Name"<<endl;

  for(int i=0;i<polygon_x.size();i++)
    file<<"atom"<<i<<" "<<polygon_x[i]<<" "<<polygon_y[i]<<" "<<polygon_z[i]<<endl;
}

//---------------------------------------------------------------------------

void Polygon::set_closure(double dx,double dy,double dz,string method)
{
  double d=sqrt(dx*dx+dy*dy+dz*dz);
  dx=dx/d;
  dy=dy/d;
  dz=dz/d;

  if(flag_cyclic==false)
    return;
  if(polygon_x.size()==0)
    return;
  if(method=="direct"||method=="straight") //"straight" for backward compatitibility
    {
      return;
    }
  else if(method=="rays"||method=="uniform"||method=="parallel")//"parallel" and "uniform" for backward compatitibility
    {
      //find bounding box
      double minx=polygon_x[0],miny=polygon_y[0],minz=polygon_z[0];
      double maxx=polygon_x[0],maxy=polygon_y[0],maxz=polygon_z[0];
      for(int i=0;i<polygon_x.size();i++)
	{
	  if(polygon_x[i]<minx)minx=polygon_x[i];
	  if(polygon_y[i]<miny)miny=polygon_y[i];
	  if(polygon_z[i]<minz)minz=polygon_z[i];
	  if(polygon_x[i]>maxx)maxx=polygon_x[i];
	  if(polygon_y[i]>maxy)maxy=polygon_y[i];
	  if(polygon_z[i]>maxz)maxz=polygon_z[i];
	}
      //estimate a distance large enough to be "outside" the polygon
      double distance=maxx-minx+maxy-miny+maxz-minz;
      double x,y,z;
      x=polygon_x.back()+distance*(dx+random01()*1e-6);
      y=polygon_y.back()+distance*(dy+random01()*1e-6);
      z=polygon_z.back()+distance*(dz+random01()*1e-6);
      polygon_x.push_back(x);
      polygon_y.push_back(y);
      polygon_z.push_back(z);
      x=polygon_x[0]+distance*(dx+random01()*1e-6);
      y=polygon_y[0]+distance*(dy+random01()*1e-6);
      z=polygon_z[0]+distance*(dz+random01()*1e-6);
      polygon_x.push_back(x);
      polygon_y.push_back(y);
      polygon_z.push_back(z);
    }  
  else
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR invalid closure method \""<<method<<"\""<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }

}

//---------------------------------------------------------------------------

void Polygon::simplify_polygon(double dx,double dy,double dz)
{
  vector<int> PointRemoved(polygon_x.size(),0);
  
  int i1,i2,i3,j1,j2;  
  bool ToBeRemoved=true;
  unsigned int nbpointsremoved=1;
  while(nbpointsremoved>0)
    {
      unsigned int nbpoints=polygon_x.size();
      nbpointsremoved=0;
      for(int i=0;(i<nbpoints)&&(nbpoints-nbpointsremoved>3);i++)
	{
	  //find i1<i2<i3 non-removed (triangle)
	  i1=i;
	  while(PointRemoved[i1%nbpoints]==1){i1++;}
	  i2=i1+1;
	  i3=i2+1;
	  ToBeRemoved=true;
	  while((ToBeRemoved)&&(i3-i1<nbpoints)&&(nbpoints-nbpointsremoved>3))//the point is eliminated
	    {
	      ToBeRemoved=true;
	      while(PointRemoved[i2%nbpoints]==1){i2++;}
	      if(flag_cyclic==false&&(i2%nbpoints==0||i2%nbpoints==nbpoints-1))//do not remove end points
		{
		  ToBeRemoved=false;
		}
	      i3=i2+1;
	      while(PointRemoved[i3%nbpoints]==1){i3++;}	      
	      double dist13_squared=(polygon_x[i1%nbpoints]-polygon_x[i3%nbpoints])*(polygon_x[i1%nbpoints]-polygon_x[i3%nbpoints])
		+(polygon_y[i1%nbpoints]-polygon_y[i3%nbpoints])*(polygon_y[i1%nbpoints]-polygon_y[i3%nbpoints])
		+(polygon_z[i1%nbpoints]-polygon_z[i3%nbpoints])*(polygon_z[i1%nbpoints]-polygon_z[i3%nbpoints]);
	      if(dist13_squared<ZERO*ZERO)//point i1 and i3 are on top of each other => degenerate case, dangerous to remove i2.
		ToBeRemoved=false;
	      i=i1;      
	      for(int j=0;(j<nbpoints)&& ToBeRemoved &&(nbpoints-nbpointsremoved>3);j++)
		{
		  //find j1<j2 non-removed (segment)
		  j1=j;while(PointRemoved[j1%nbpoints]==1){ j1++;}
		  j2=j1+1;while(PointRemoved[j2%nbpoints]==1){ j2++;}
		  j=j1;
		  if((j1%nbpoints!=i1%nbpoints)&&(j1%nbpoints!=i2%nbpoints)&&(j1%nbpoints!=i3%nbpoints)
		     &&(j2%nbpoints!=i1%nbpoints)&&(j2%nbpoints!=i2%nbpoints)&&(j2%nbpoints!=i3%nbpoints))//segment is not part of the triangle
		    {
		      if(Intersection_segment_triangle(polygon_x[j1%nbpoints],polygon_y[j1%nbpoints],polygon_z[j1%nbpoints]
						       ,polygon_x[j2%nbpoints],polygon_y[j2%nbpoints],polygon_z[j2%nbpoints]
						       ,polygon_x[i1%nbpoints],polygon_y[i1%nbpoints],polygon_z[i1%nbpoints]
						       ,polygon_x[i2%nbpoints],polygon_y[i2%nbpoints],polygon_z[i2%nbpoints]
						       ,polygon_x[i3%nbpoints],polygon_y[i3%nbpoints],polygon_z[i3%nbpoints],1e-8))
			{
			  //intersection the point cannot be removed
			  ToBeRemoved=false;
			}
		    }
		}
	      /////////check end points 0/////////
	      if(flag_cyclic==false&&Intersection_line_triangle(polygon_x[0],polygon_y[0],polygon_z[0]
								,polygon_x[0]+dx,polygon_y[0]+dy,polygon_z[0]+dz
								,polygon_x[i1%nbpoints],polygon_y[i1%nbpoints],polygon_z[i1%nbpoints]
								,polygon_x[i2%nbpoints],polygon_y[i2%nbpoints],polygon_z[i2%nbpoints]
								,polygon_x[i3%nbpoints],polygon_y[i3%nbpoints],polygon_z[i3%nbpoints],1e-8))
		{
		  //intersection the point cannot be removed
		  ToBeRemoved=false;
		}
	      /////////check end points nbpoints-1/////////
	      if(flag_cyclic==false&&Intersection_line_triangle(polygon_x[nbpoints-1],polygon_y[nbpoints-1],polygon_z[nbpoints-1]
								,polygon_x[nbpoints-1]+dx,polygon_y[nbpoints-1]+dy,polygon_z[nbpoints-1]+dz
								,polygon_x[i1%nbpoints],polygon_y[i1%nbpoints],polygon_z[i1%nbpoints]
								,polygon_x[i2%nbpoints],polygon_y[i2%nbpoints],polygon_z[i2%nbpoints]
								,polygon_x[i3%nbpoints],polygon_y[i3%nbpoints],polygon_z[i3%nbpoints],1e-8))
		{
		  //intersection the point cannot be removed
		  ToBeRemoved=false;
		}
	      if(ToBeRemoved&&(nbpoints-nbpointsremoved>3))// we remove  the point i2
		{
		  nbpointsremoved++;
		  PointRemoved[i2%nbpoints]=1;
		  i2=i3;
		}
	    }
	  // the point i2 has not been removed, we start from there
	  i=i2-1;
	}
      
      ///////////////// remove the points ///////////////
      int   position=0;
      for(int i=0;i<nbpoints;i++)
	{
	  if(PointRemoved[i]==0)//keep this point
	    {
	      polygon_x[position]= polygon_x[i];
	      polygon_y[position]= polygon_y[i];
	      polygon_z[position]= polygon_z[i];
	      position++;
	    }
	  else
	    PointRemoved[i]=0;
	}
      polygon_x.resize(position);
      polygon_y.resize(position);
      polygon_z.resize(position);
    }

 
}
//---------------------------------------------------------------------------

PlanarDiagram Polygon::get_planar_diagram(double dx,double dy,double dz,bool flag_planar)
{
  ////////create perpendicular basis (e1,e2,d)//////////////////////
  double d=sqrt(dx*dx+dy*dy+dz*dz);
  double e1x,e1y,e1z;
  double e2x,e2y,e2z;
  double e1,e2;
  dx=dx/d;
  dy=dy/d;
  dz=dz/d;
  if((fabs(dx) <= fabs(dy))&&(fabs(dx) <= fabs(dz)))
    {
      e1x=1.0;
      e1y=0.0;
      e1z=0.0;
    }
  else if((fabs(dy) <= fabs(dx))&&(fabs(dy) <= fabs(dz)))
    {
      e1x=0.0;
      e1y=1.0;
      e1z=0.0;
    }
  else
    {
      e1x=0.0;
      e1y=0.0;
      e1z=1.0;
    }
  e2x=dy*e1z-dz*e1y;
  e2y=dz*e1x-dx*e1z;
  e2z=dx*e1y-dy*e1x;
  e2=sqrt(e2x*e2x+e2y*e2y+e2z*e2z);
  e2x=e2x/e2;
  e2y=e2y/e2;
  e2z=e2z/e2;  
  e1x=e2y*dz-e2z*dy;
  e1y=e2z*dx-e2x*dz;
  e1z=e2x*dy-e2y*dx;
  e1=sqrt(e1x*e1x+e1y*e1y+e1z*e1z);
  e1x=e1x/e1;
  e1y=e1y/e1;
  e1z=e1z/e1;

  /////////////project and find crossings////////////////
  unsigned int nbpoints=polygon_x.size();
  //projected coordinates (u,v,w) in (e1,e2,d) basis i.e. r=u*e1+v*e2+w*d 
  double ui1,vi1,wi1;
  double ui2,vi2,wi2;
  double uj1,vj1,wj1;
  double uj2,vj2,wj2;
  double Det,Det1,Det2,t1,t2;
  double p1u,p1v,p1w;
  double p2u,p2v,p2w;
  /////////////////initialise///////////
  int nbarcs=0;
  map<pair<int,int>,int> map_ij_to_crossingorder;//map_ij_to_crossingorder[pair(i,j)]=n  with i<j => n-th crossing is between segments (i,i+1) and (j,j+1). 
  map<pair<int,int>,int>::iterator map_iterator;
  
  vector<double> crossing_temp_t1;
  vector<int> crossing_temp_j;
  vector<double> crossing_temp_p1u;
  vector<double> crossing_temp_p1v;
  vector<double> crossing_temp_p1w;
  vector<double> crossing_temp_p2u;
  vector<double> crossing_temp_p2v;
  vector<double> crossing_temp_p2w;
  vector<int> crossing_temp_type;
  vector<int> crossing_type;
  vector<int> crossing_order;

  vector<Crossing> crossings;
  vector<int> crossings_first_time_input_arc;//crossings_first_time_input_arc[n]=a => store the input arc number for the first time we reach this crossing;

  //following variables only used to build index_to_arc (when flag_planar=true)
  vector<int> crossings_first_time_input_index;//crossings_first_time_input_index[n]=i => store the index i of closest polygon point (i.e. (polygon_x[i],polygon_y[i],polygon_z[i]) before the crossing for the first time we reach this crossing) . NOTE: i=-1 if it is not the first crossing on segment (i,i+1)  
  vector<int> crossings_first_time_output_index;//crossings_first_time_output_index[n]=i => store the index i of closest polygon point (i.e. (polygon_x[i],polygon_y[i],polygon_z[i]) after the crossing for the first time we reach this crossing). NOTE: i=-1 if it is not the last crossing on segment (i,i+1)
  vector<int> index_to_arc(polygon_x.size()+1,-1);//index_to_arc[i]=a => point (polygon_x[i],polygon_y[i],polygon_z[i]) is in arc a. Note index_to_arc[N] should be interpreted as  index_to_arc[0] when cyclic

  for(int i=0;i<nbpoints;i++)
    {
      if(flag_cyclic==false&&i==nbpoints-1)//avoid crossing with segment (npoints-1,0)
	continue;
      crossing_temp_t1.clear();
      crossing_temp_j.clear();
      crossing_temp_p1u.clear();
      crossing_temp_p1v.clear();
      crossing_temp_p1w.clear();
      crossing_temp_p2u.clear();
      crossing_temp_p2v.clear();
      crossing_temp_p2w.clear();
      crossing_temp_type.clear();
      
      ui1=polygon_x[i]*e1x+polygon_y[i]*e1y+polygon_z[i]*e1z;
      vi1=polygon_x[i]*e2x+polygon_y[i]*e2y+polygon_z[i]*e2z; 
      wi1=polygon_x[i]*dx+polygon_y[i]*dy+polygon_z[i]*dz;
      ui2=polygon_x[(i+1)%nbpoints]*e1x+polygon_y[(i+1)%nbpoints]*e1y+polygon_z[(i+1)%nbpoints]*e1z;
      vi2=polygon_x[(i+1)%nbpoints]*e2x+polygon_y[(i+1)%nbpoints]*e2y+polygon_z[(i+1)%nbpoints]*e2z;
      wi2=polygon_x[(i+1)%nbpoints]*dx+polygon_y[(i+1)%nbpoints]*dy+polygon_z[(i+1)%nbpoints]*dz;
      for(int j=0;j<nbpoints;j++)
	{
	  if(flag_cyclic==false&&j==nbpoints-1)//avoid crossing with segement (npoints-1,0)
	    continue;
	  //check bounding boxes
	  if((j!=i)&&(j!=(i+1)%nbpoints)&&(j!=(i-1+nbpoints)%nbpoints))
	    {
	      uj1=polygon_x[j]*e1x+polygon_y[j]*e1y+polygon_z[j]*e1z;
	      vj1=polygon_x[j]*e2x+polygon_y[j]*e2y+polygon_z[j]*e2z;
	      wj1=polygon_x[j]*dx+polygon_y[j]*dy+polygon_z[j]*dz;
	      uj2=polygon_x[(j+1)%nbpoints]*e1x+polygon_y[(j+1)%nbpoints]*e1y+polygon_z[(j+1)%nbpoints]*e1z;
	      vj2=polygon_x[(j+1)%nbpoints]*e2x+polygon_y[(j+1)%nbpoints]*e2y+polygon_z[(j+1)%nbpoints]*e2z;
	      wj2=polygon_x[(j+1)%nbpoints]*dx+polygon_y[(j+1)%nbpoints]*dy+polygon_z[(j+1)%nbpoints]*dz;
	      if(min(ui1,ui2)>max(uj1,uj2)+ZERO)//no intersection
		continue;
	      if(min(uj1,uj2)>max(ui1,ui2)+ZERO)//no intersection
		continue;
	      if(min(vi1,vi2)>max(vj1,vj2)+ZERO)//no intersection
		continue;
	      if(min(vj1,vj2)>max(vi1,vi2)+ZERO)//no intersection
		continue;

	      ///////////////////////////// search for an intersection //////////////
	      Det=(uj2-uj1)*(vi2-vi1)-(ui2-ui1)*(vj2-vj1);
	      Det1=(uj2-uj1)*(vj1-vi1)-(uj1-ui1)*(vj2-vj1);
	      Det2=(ui2-ui1)*(vj1-vi1)-(uj1-ui1)*(vi2-vi1);
	      if((fabs(Det)<ZERO)&&(fabs(Det1)<ZERO)&&(fabs(Det2)<ZERO))//failed: two segments parallels (Det=0) and on top of each other (Det1=Det2=0). NOTE: if Det=0 and Det1,Det1!=0 => two parallel segment at finite distance => no intersection
		{
		  //Measure distance between point (ui1,vi1) and line passing through (uj1,vj1) and (uj2,vj2):
		  //d=|(rj1-ri1)x(rj2-rj1)|/|(rj2-rj1)|
		  // = |(uj1-ui1)*(vj2-vj1)-(vj1-vi1)*(uj2-uj1)|/|rj2-rj1|
		  // = |-Det1|/|rj2-rj1|
		  // = |Det1|/|rj2-rj1|
		  //i.e. if d=|Det1|/|rj2-rj1|>0 <=> |Det1|>0 => no crossing
		  //Measure distance between point (uj1,vj1) and line passing through (ui1,vi1) and (ui2,vi2):
		  //d=|(ri1-rj1)x(ri2-ri1)|/|(ri2-ri1)|
		  // = |(ui1-uj1)*(vi2-vi1)-(vi1-vj1)*(ui2-ui1)|/|ri2-ri1|
		  // = |-Det2|/|ri2-ri1|
		  // = |Det2|/|ri2-ri1|
		  //i.e. if d=|Det2|/|ri2-ri1|>0 <=> |Det2|>0 => no crossing
		  if(flag_debug)
		    {
		      cerr<<"**************************************************************************************************"<<endl;
		      cerr<<"Polygon::get_planar_diagram() failed, try another projection (parallel segments)"<<endl;
		      cerr<<"Det="<<Det<<" Det1="<<Det1<<" Det2="<<Det2<<endl;
		      cerr<<"Segment i:"<<endl;
		      cerr<<" (ui1,vi1)-(ui2,vi2)=("<<ui1<<","<<vi1<<")-("<<ui2<<","<<vi2<<") = ("<<ui1-ui2<<","<<vi1-vi2<<")"<<endl;
		      cerr<<"Segment j:"<<endl;
		      cerr<<" (uj1,vj1)-(uj2,vj2)=("<<uj1<<","<<vj1<<")-("<<uj2<<","<<vj2<<") = ("<<uj1-uj2<<","<<vj1-vj2<<")"<<endl;
		      cerr<<"**************************************************************************************************"<<endl;
		    }
		  throw runtime_error("Projection failed (parallel segments), try another projection.");
		}
	      else if(fabs(Det)>ZERO)
		{
		  t1=Det1/Det;t2=Det2/Det;
		  if((fabs(t1)<ZERO&&t2>-ZERO&&t2<1+ZERO)
		     ||(fabs(t1-1)<ZERO&&t2>-ZERO&&t2<1+ZERO)
		     ||(fabs(t2)<ZERO&&t1>-ZERO&&t1<1+ZERO)
		     ||(fabs(t2-1)<ZERO&&t1>-ZERO&&t1<1+ZERO))
		    {
		      if(flag_debug)
			{
			  cerr<<"**************************************************************************************************"<<endl;
			  cerr<<"Polygon::get_planar_diagram() failed, try another projection (intersection on end of segment)"<<endl;
			  cerr<<"**************************************************************************************************"<<endl;
			}
		      throw runtime_error("Projection failed (intersection on end of segment), try another projection.");
		    }
		  if((t1>=0)&&(t1<1)&&(t2>=0)&&(t2<1))//the intersection is in the segment
		    {
		      ////////////find the (3D) intersection points ////////////
		      p1u=t1*(ui2-ui1)+ui1; p1v=t1*(vi2-vi1)+vi1; p1w=t1*(wi2-wi1)+wi1;
		      p2u=t2*(uj2-uj1)+uj1; p2v=t2*(vj2-vj1)+vj1; p2w=t2*(wj2-wj1)+wj1;

		      //////// find the type of crossing
		      if(((uj2-uj1)*(vi2-vi1)-(vj2-vj1)*(ui2-ui1))*(p2w-p1w)>ZERO)
			crossing_temp_type.push_back(+1);
		      else if(((uj2-uj1)*(vi2-vi1)-(vj2-vj1)*(ui2-ui1))*(p2w-p1w)<-ZERO)
			crossing_temp_type.push_back(-1);
		      else
			{
			  if(flag_debug)
			    {
			      cerr<<"**************************************************************************************************"<<endl;
			      cerr<<"Polygon::get_planar_diagram() failed, try another projection (separation problem)"<<endl;
			      cerr<<"**************************************************************************************************"<<endl;
			    }
			  throw runtime_error("Projection failed (separation problem), try another projection.");
			}
		      crossing_temp_t1.push_back(t1);    
		      crossing_temp_j.push_back(j);  
		      crossing_temp_p1u.push_back(p1u);  
		      crossing_temp_p1v.push_back(p1v);
		      crossing_temp_p1w.push_back(p1w);
		      crossing_temp_p2u.push_back(p2u);
		      crossing_temp_p2v.push_back(p2v);
		      crossing_temp_p2w.push_back(p2w);
		    }
		}
	    }
	}
      
      ///////////////// sort the list of crossings with increasing t1//////////////
      double temp_t1;
      int    temp_j;   
      double temp_p1u; 
      double temp_p1v; 
      double temp_p1w; 
      double temp_p2u; 
      double temp_p2v; 
      double temp_p2w; 
      int    temp_type;
      int index;
      for (int j=1;j<crossing_temp_type.size();j++) 
	{
	  temp_t1=crossing_temp_t1[j];
	  temp_j= crossing_temp_j[j];   
	  temp_p1u= crossing_temp_p1u[j]; 
	  temp_p1v= crossing_temp_p1v[j]; 
	  temp_p1w= crossing_temp_p1w[j]; 
	  temp_p2u= crossing_temp_p2u[j]; 
	  temp_p2v= crossing_temp_p2v[j]; 
	  temp_p2w= crossing_temp_p2w[j]; 
	  temp_type=crossing_temp_type[j];

	  index=j-1;
	  
	  while (index > -1 && crossing_temp_t1[index] > temp_t1) 
	    {
	      crossing_temp_t1[index+1]=crossing_temp_t1[index];
	      crossing_temp_j[index+1]=crossing_temp_j[index];   
	      crossing_temp_p1u[index+1]=crossing_temp_p1u[index]; 
	      crossing_temp_p1v[index+1]=crossing_temp_p1v[index]; 
	      crossing_temp_p1w[index+1]=crossing_temp_p1w[index]; 
	      crossing_temp_p2u[index+1]=crossing_temp_p2u[index]; 
	      crossing_temp_p2v[index+1]=crossing_temp_p2v[index]; 
	      crossing_temp_p2w[index+1]=crossing_temp_p2w[index]; 
	      crossing_temp_type[index+1]=crossing_temp_type[index];
	      index--;
	    }
	  crossing_temp_t1[index+1]=temp_t1;
	  crossing_temp_j[index+1]=temp_j;   
	  crossing_temp_p1u[index+1]=temp_p1u; 
	  crossing_temp_p1v[index+1]=temp_p1v; 
	  crossing_temp_p1w[index+1]=temp_p1w; 
	  crossing_temp_p2u[index+1]=temp_p2u; 
	  crossing_temp_p2v[index+1]=temp_p2v; 
	  crossing_temp_p2w[index+1]=temp_p2w; 
	  crossing_temp_type[index+1]=temp_type;
	}
  
      /////////////////// pass through all crossings  /////////////////////////////
      for(int l=0;l<crossing_temp_type.size();l++)
	{
	  //////////// check if it is a new crossing ////////
	  map_iterator=map_ij_to_crossingorder.find(make_pair(min(i,crossing_temp_j[l]),max(i,crossing_temp_j[l])));
	  int crossingindex;
	  if(map_iterator==map_ij_to_crossingorder.end())//not found yet
	    {
	      crossingindex=crossings.size();
	      map_ij_to_crossingorder[make_pair(min(i,crossing_temp_j[l]),max(i,crossing_temp_j[l]))]=crossingindex;

	      /////////create crossing/////////
	      crossings.push_back(Crossing());
	      ////////store input/output arcs (for segment i-i+1)
	      crossings_first_time_input_arc.push_back(nbarcs);
	      if(flag_planar)
		{
		  //store input/output vertice index
		  if(l==0)//first crossing
		    crossings_first_time_input_index.push_back(i);
		  else
		    crossings_first_time_input_index.push_back(-1);
		  if(l==crossing_temp_type.size()-1)//last crossing
		    crossings_first_time_output_index.push_back(i+1);
		  else
		    crossings_first_time_output_index.push_back(-1);
		  if(flag_debug)
		    cerr<<" first time crossing: i="<<i<<" l="<<l<<" crossing="<<crossings.size()-1<<" crossings_first_time_input_index="<<crossings_first_time_input_index.back()<<" crossings_first_time_output_index="<<crossings_first_time_output_index.back()<<endl;
		}
	    }
	  else
	    {
	      crossingindex=map_iterator->second;
	      //second time we reach this crossing, we have the full information:

	      // check if segment i,i+1 is below segment j,j+1 
	      if((crossing_temp_p2w[l]-crossing_temp_p1w[l])>0)//i,i+1 below j,j+1
		{
		  crossings[crossingindex].arc0=nbarcs;//segment i,i+1
		  crossings[crossingindex].arc2=nbarcs+1;//segment i,i+1
		  if(crossing_temp_type[l]==1)//positive crossing, following usual definition
		    {
		      crossings[crossingindex].arc1=crossings_first_time_input_arc[crossingindex]+1;
		      crossings[crossingindex].arc3=crossings_first_time_input_arc[crossingindex];
		    }
		  else//negative crossing
		    {
		      crossings[crossingindex].arc1=crossings_first_time_input_arc[crossingindex];
		      crossings[crossingindex].arc3=crossings_first_time_input_arc[crossingindex]+1;
		    }
		}
	      else //i,i+1 above j,j+1
		{
		  crossings[crossingindex].arc0=crossings_first_time_input_arc[crossingindex];//segment j,j+1
		  crossings[crossingindex].arc2=crossings_first_time_input_arc[crossingindex]+1;//segment j,j+1
		  if(crossing_temp_type[l]==1)//positive crossing, following usual definition
		    {
		      crossings[crossingindex].arc1=nbarcs+1;
		      crossings[crossingindex].arc3=nbarcs;
		    }
		  else
		    {
		      crossings[crossingindex].arc1=nbarcs;
		      crossings[crossingindex].arc3=nbarcs+1;
		    }
		}
	      if(flag_planar)
		{
		  if(l==0)//first crossing along i,i+1
		    {
		      index_to_arc[i]=nbarcs;//crossings_first_time_input_arc[crossingindex];
		      if(flag_debug)
			cerr<<"  setting(1) index_to_arc["<<i<<"]="<<index_to_arc[i]<<endl;
		    }
		  if(l==crossing_temp_type.size()-1)//last crossing along i,i+1
		    {
		      index_to_arc[i+1]=nbarcs+1;//crossings_first_time_input_arc[crossingindex]+1;
		      if(flag_debug)
			cerr<<"  setting(2) index_to_arc["<<i<<"]="<<index_to_arc[i+1]<<endl;
		    }
		  if(crossings_first_time_input_index[crossingindex]>0)//first crossing along j,j+1
		    {
		      index_to_arc[crossings_first_time_input_index[crossingindex]]=crossings_first_time_input_arc[crossingindex];
		      if(flag_debug)
			cerr<<"  setting(3) index_to_arc["<<crossings_first_time_input_index[crossingindex]<<"]="<<index_to_arc[crossings_first_time_input_index[crossingindex]]<<endl;
		    }
		  if(crossings_first_time_output_index[crossingindex]>0)//last crossing along j,j+1
		    {
		      index_to_arc[crossings_first_time_output_index[crossingindex]]=crossings_first_time_input_arc[crossingindex]+1;
		      if(flag_debug)
			cerr<<"  setting(4) index_to_arc["<<crossings_first_time_output_index[crossingindex]<<"]="<<index_to_arc[crossings_first_time_output_index[crossingindex]]<<endl;
		    }
		  if(flag_debug)
		    cerr<<" second time crossing: i="<<i<<" l="<<l<<" crossing="<<crossingindex<<" crossings_first_time_input_index="<<crossings_first_time_input_index[crossingindex]<<" crossings_first_time_output_index="<<crossings_first_time_output_index[crossingindex]<<" nbarcs="<<nbarcs<<" nbarcs+1="<<nbarcs+1<<" crossings_first_time_input_arc="<<crossings_first_time_input_arc[crossingindex]<<" crossings_first_time_input_arc+1="<<crossings_first_time_input_arc[crossingindex]+1<<endl;
		}
	      /*
	      positive crossing:
	          ^   ^
	           \ /
	            /	 
	           / \
	      
	      negative crossing:
	          ^   ^
	           \ /
	            \	 
	           / \ 
	      */
	      
	    }

	  
	  nbarcs++;

	}
    }// i loop


  if(flag_planar)
    {
      //fill missing entries in index_to_arc
      int lastarc=0;
      for(int i=0;i<index_to_arc.size();i++)
	{
	  if(index_to_arc[i]<0)
	    index_to_arc[i]=lastarc;
	  else
	    lastarc=index_to_arc[i];
	}
    }  
  if(flag_cyclic)
    {
      //////////// replace arc=nbcrossings by arc=0
      for(int n=0;n<crossings.size();n++)
	{
	  if(crossings[n].arc0==nbarcs)crossings[n].arc0=0;
	  if(crossings[n].arc1==nbarcs)crossings[n].arc1=0;
	  if(crossings[n].arc2==nbarcs)crossings[n].arc2=0;
	  if(crossings[n].arc3==nbarcs)crossings[n].arc3=0;
	}
      if(flag_planar)
	{
	  for(int i=0;i<index_to_arc.size();i++)
	    if(index_to_arc[i]==nbarcs)index_to_arc[i]=0;
	}
    }
  else
    {
      //add endpoints
      crossings.insert(crossings.begin(),Crossing(0));   
      crossings.push_back(Crossing(nbarcs));
    }

  if(flag_debug&&flag_planar)
    {
      for(int i=0;i<index_to_arc.size();i++)
	{
	  if(i<polygon_x.size())
	    cerr<<"i="<<i<<" (x,y,z)="<<polygon_x[i]<<" "<<polygon_y[i]<<" "<<polygon_z[i]<<" arc="<<index_to_arc[i]<<endl;
	  else
	    cerr<<"i="<<i<<" (x,y,z)= - - - arc="<<index_to_arc[i]<<endl;
	}
    }

  PlanarDiagram planar_diagram(crossings,false /*flag_planar*/,flag_debug);//flag_planar=false because inside/outside not yet evaluated  
  if(flag_debug)
    {
      cerr<<"Polygon::get_planar_diagram() diagram before evaluating region:"<<endl;
      planar_diagram.save_to_file_list(cerr);
    }
  if(flag_planar&&crossings.size()>0)
    {
      crossings=planar_diagram.get_crossings();//now contain connected_crossing*
         
      ///////////find arcs inside/outside/////////
      //project
      vector<double> polygon_u(polygon_x.size());
      vector<double> polygon_v(polygon_x.size());
      vector<double> polygon_w(polygon_x.size());
      for(int i=0;i<polygon_x.size();i++)
	{
	  polygon_u[i]=polygon_x[i]*e1x+polygon_y[i]*e1y+polygon_z[i]*e1z;
	  polygon_v[i]=polygon_x[i]*e2x+polygon_y[i]*e2y+polygon_z[i]*e2z; 
	  polygon_w[i]=polygon_x[i]*dx+polygon_y[i]*dy+polygon_z[i]*dz;
	}
      //find polygon point with highest u:
      double max_u=polygon_u[0];
      int i0=0;
      for(int i=0;i<polygon_u.size();i++)
	{
	  if(polygon_u[i]>max_u)
	    {
	      max_u=polygon_u[i];
	      i0=i;
	    }
	}
      if(flag_debug)
	cerr<<"found initial point on exterior: i0="<<i0<<" arc="<<index_to_arc[i0]<<endl;

      //find direction along polymer corresponding to clockwise rotation
      //special case of endpoints (arci0=0 or i0=polygon_u.size()-1)
      int direction=1;
      int ineg=(i0-1+polygon_u.size())%polygon_u.size();//i.e. i0-1 modulo size 
      int ipos=(i0+1)%polygon_u.size();//i.e. i0-1 modulo size
      double cos_neg=(polygon_v[ineg]-polygon_v[i0])/sqrt((polygon_u[ineg]-polygon_u[i0])*(polygon_u[ineg]-polygon_u[i0])+(polygon_v[ineg]-polygon_v[i0])*(polygon_v[ineg]-polygon_v[i0]));
      double cos_pos=(polygon_v[ipos]-polygon_v[i0])/sqrt((polygon_u[ipos]-polygon_u[i0])*(polygon_u[ipos]-polygon_u[i0])+(polygon_v[ipos]-polygon_v[i0])*(polygon_v[ipos]-polygon_v[i0]));
      if(cos_pos>cos_neg)//=> localy oriented counterclockwise=> we need to go reverse direction
	direction=-1;
      if(flag_debug)
	cerr<<" direction="<<direction<<endl;

      //find next crossing in direction 
      int n0=-1;//first crossing
      int n1=-1;//next crossing
      int arc0=index_to_arc[i0];//first arc (incoming to crossing[n0])
      int arc0_initial;//first arc (incoming to crossing[n0])
      int arc1_initial;//second arc (incoming to crossing[n0])
      int arc1=-1;//arc outgoing from crossing[n0]
      int pos_arc0;//such that crossings[n0].get_arc(pos_arc0)=arc0;
      int pos_arc1;//such that crossings[n0].get_arc(pos_arc1)=arc1;
      int pos_arc0_initial;//such that crossings[n0].get_arc(pos_arc0)=arc0_initial;
      int pos_arc1_initial;//such that crossings[n0].get_arc(pos_arc1)=arc1_initial;
      //find crossings with incomming arc arc0
      for(int n=0;n<crossings.size()&&n0<0;n++)
	{
	  for(int i=0;i<4&&n0<0;i++)
	    {
	      if(crossings[n].get_arc(i)==arc0)
		{
		  //check if crossing is an endpoint
		  if(crossings[n].arc0>=0&&crossings[n].arc1<0&&crossings[n].arc2<0&&crossings[n].arc3<0)//endpoint
		    {
		      n0=n;
		      n1=crossings[n].connected_crossing0;
		      arc1=crossings[n].arc0;
		      pos_arc0=0;
		      pos_arc1=0;
		      crossings[n].region01=1;
		      break;
		    }
		  else //or if not endpoint but outgoing arc = (arc0+1) if not-cyclic %nbarcs
		    {
		      if(direction==1)
			{
			  if(flag_cyclic&&crossings[n].get_arc((i+2)%4)==(arc0+1)%nbarcs)//check we are in positive direction
			    {			    
			      n0=n;
			      n1=crossings[n].get_connected_crossing((i+3)%4);//next arc clockwise		    
			      arc1=crossings[n].get_arc((i+3)%4);//next arc clockwise		    
			      crossings[n].set_region(i,(i+3)%4,1);
			      pos_arc0=i;
			      pos_arc1=(i+3)%4;			      
			      break;
			    }
			  else if(!flag_cyclic&&crossings[n].get_arc((i+2)%4)==arc0+1)//check we are in positive direction
			    {
			      n0=n;
			      n1=crossings[n].get_connected_crossing((i+3)%4);//next arc clockwise			    
			      arc1=crossings[n].get_arc((i+3)%4);//next arc clockwise			    
			      crossings[n].set_region(i,(i+3)%4,1);
			      pos_arc0=i;
			      pos_arc1=(i+3)%4;
			      break;
			    }
			}
		      else//direction -1
			{
			  if(flag_cyclic&&crossings[n].get_arc((i+2)%4)==(arc0-1+nbarcs)%nbarcs)//check we are in negative direction
			    {			    
			      n0=n;
			      n1=crossings[n].get_connected_crossing((i+3)%4);//next arc clockwise		    
			      arc1=crossings[n].get_arc((i+3)%4);//next arc clockwise		    
			      crossings[n].set_region(i,(i+3)%4,1);
			      pos_arc0=i;
			      pos_arc1=(i+3)%4;
			      break;
			    }
			  else if(!flag_cyclic&&crossings[n].get_arc((i+2)%4)==arc0-1)//check we are in negative direction
			    {
			      n0=n;
			      n1=crossings[n].get_connected_crossing((i+3)%4);//next arc clockwise			    
			      arc1=crossings[n].get_arc((i+3)%4);//next arc clockwise			    
			      crossings[n].set_region(i,(i+3)%4,1);
			      pos_arc0=i;
			      pos_arc1=(i+3)%4;
			      break;
			    }
			}
		    }
		}
	    }
	}
      if(arc1<0)//should not happen
	{
	  if(flag_debug)
	    {
	      cerr<<"**************************************************************************************************"<<endl;
	      cerr<<"Polygon::get_planar_diagram() failed, try another projection"<<endl;
	      cerr<<"**************************************************************************************************"<<endl;
	    }
	  throw runtime_error("Projection failed, try another projection.");
	}
      arc0_initial=arc0;
      arc1_initial=arc1;
      pos_arc0_initial=pos_arc0;
      pos_arc1_initial=pos_arc1;      
      if(flag_debug)
	cerr<<"Found initial crossings: n0="<<n0<<" next crossing n1="<<n1<<" arc0="<<arc0<<" arc1="<<arc1<<" crossing["<<n0<<"].region: 0-1="<<crossings[n0].region01<<" 1-2="<<crossings[n0].region12<<" 2-3="<<crossings[n0].region23<<" 3-0="<<crossings[n0].region30<<" pos_arc0="<<pos_arc0<<" pos_arc1="<<pos_arc1<<endl;
      //find next crossings
      while(true)
	{
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
		      crossings[n1].region01=1;
		      arc1=crossings[n1].arc0;
		      pos_arc0=0;
		      pos_arc1=0;
		      n1=crossings[n1].connected_crossing0;
		      break;
		    }
		  else //or if not endpoint but outgoing arc = (arc1+1) if not-cyclic %nbarcs
		    {
		      n0=n1;
		      arc0=arc1;
		      crossings[n1].set_region(i,(i+3)%4,1);
		      arc1=crossings[n1].get_arc((i+3)%4);//next arc clockwise
		      pos_arc0=i;
		      pos_arc1=(i+3)%4;
		      n1=crossings[n1].get_connected_crossing((i+3)%4);//next arc clockwise		    
		      break;
		    }
		}
	    }
	  if(flag_debug)
	    cerr<<"Found additional crossings: n0="<<n0<<" next crossing n1="<<n1<<" arc0="<<arc0<<" arc1="<<arc1<<" pos_arc0="<<pos_arc0<<" pos_arc1="<<pos_arc1<<" crossing["<<n0<<"].region: 0-1="<<crossings[n0].region01<<" 1-2="<<crossings[n0].region12<<" 2-3="<<crossings[n0].region23<<" 3-0="<<crossings[n0].region30<<" arc0_initial="<<arc0_initial<<" arc1_initial="<<arc1_initial<<" pos_arc0_initial="<<pos_arc0_initial<<" pos_arc1_initial="<<pos_arc1_initial<<endl;
	  if(arc0==arc0_initial&&arc1==arc1_initial)
	    {
	      break;
	    }
	}
      //set remaining to 0
      for(int n=0;n<crossings.size();n++)
	for(int i=0;i<4;i++)
	  if(crossings[n].get_region(i,(i+1)%4)!=1)crossings[n].set_region(i,(i+1)%4,0);  
      planar_diagram=PlanarDiagram(crossings,flag_planar,flag_debug);
    }
		
  
  return planar_diagram;
}


//---------------------------------------------------------------------------
void Polygon::project(double dx,double dy,double dz)
{
  double debug_umin=1e100,debug_umax=-1e100;
  double debug_vmin=1e100,debug_vmax=-1e100;
  double debug_wmin=1e100,debug_wmax=-1e100;
  vector<double> u_projected;//only used if debug_filename!=""
  vector<double> v_projected;//only used if debug_filename!=""
  vector<double> w_projected;//only used if debug_filename!=""
  
  ////////create perpendicular basis (e1,e2,d)//////////////////////
  double d=sqrt(dx*dx+dy*dy+dz*dz);
  double e1x,e1y,e1z;
  double e2x,e2y,e2z;
  double e1,e2;
  dx=dx/d;
  dy=dy/d;
  dz=dz/d;
  if((fabs(dx) <= fabs(dy))&&(fabs(dx) <= fabs(dz)))
    {
      e1x=1.0;
      e1y=0.0;
      e1z=0.0;
    }
  else if((fabs(dy) <= fabs(dx))&&(fabs(dy) <= fabs(dz)))
    {
      e1x=0.0;
      e1y=1.0;
      e1z=0.0;
    }
  else
    {
      e1x=0.0;
      e1y=0.0;
      e1z=1.0;
    }
  e2x=dy*e1z-dz*e1y;
  e2y=dz*e1x-dx*e1z;
  e2z=dx*e1y-dy*e1x;
  e2=sqrt(e2x*e2x+e2y*e2y+e2z*e2z);
  e2x=e2x/e2;
  e2y=e2y/e2;
  e2z=e2z/e2;  
  e1x=e2y*dz-e2z*dy;
  e1y=e2z*dx-e2x*dz;
  e1z=e2x*dy-e2y*dx;
  e1=sqrt(e1x*e1x+e1y*e1y+e1z*e1z);
  e1x=e1x/e1;
  e1y=e1y/e1;
  e1z=e1z/e1;

  /////////////project////////////////
  unsigned int nbpoints=polygon_x.size();
  double ui1,vi1,wi1;
  /////////////////initialise///////////
  for(int i=0;i<nbpoints;i++)
    {
      ui1=polygon_x[i]*e1x+polygon_y[i]*e1y+polygon_z[i]*e1z;
      vi1=polygon_x[i]*e2x+polygon_y[i]*e2y+polygon_z[i]*e2z; 
      wi1=polygon_x[i]*dx+polygon_y[i]*dy+polygon_z[i]*dz;
      debug_umin=min(ui1,debug_umin);
      debug_umax=max(ui1,debug_umax);
      debug_vmin=min(vi1,debug_vmin);
      debug_vmax=max(vi1,debug_vmax);
      debug_wmin=min(wi1,debug_wmin);
      debug_wmax=max(wi1,debug_wmax);
      u_projected.push_back(ui1);
      v_projected.push_back(vi1);
      w_projected.push_back(wi1);
    }// i loop

  double uv_size=max(debug_umax-debug_umin,debug_vmax-debug_vmin);
  double w_size=debug_wmax-debug_wmin;
  for(int i=0;i< w_projected.size();i++)
    w_projected[i]=w_projected[i]*0.1*uv_size/w_size;
  polygon_x=u_projected;
  polygon_y=v_projected;
  polygon_z=w_projected;

}

//---------------------------------------------------------------------------

Polygon Polygon::get_polygon(int start_index,int end_index)
{
  return get_polygon(start_index,end_index,flag_cyclic);
}
//---------------------------------------------------------------------------

Polygon Polygon::get_polygon(int start_index,int end_index,bool flag_cyclic_new)
{
  if(flag_cyclic==false&&start_index>=end_index)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR: Polygon::get_polygon() start_index>end_index with flag_cyclic=false."<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }
  if(start_index<0||start_index>get_nb_points()||end_index<0||(flag_cyclic==false&&end_index>get_nb_points()))
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR: Polygon::get_polygon() invalid start_index or end_index."<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }
  if(end_index<start_index)
    end_index=end_index+get_nb_points();
  
  vector<double> x(end_index-start_index+1);
  vector<double> y(end_index-start_index+1);
  vector<double> z(end_index-start_index+1);
  for(int i=start_index;i<=end_index;i++)
    {
      x[i-start_index]=polygon_x[i%get_nb_points()];
      y[i-start_index]=polygon_y[i%get_nb_points()];
      z[i-start_index]=polygon_z[i%get_nb_points()];
    }
  return Polygon(x,y,z,flag_cyclic_new);
}

//---------------------------------------------------------------------------
//Private
//---------------------------------------------------------------------------

vector<string> Polygon::split_string(string str,string sep_list)
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

//---------------------------------------------------------------------------

bool Polygon::Intersection_segment_triangle(double x1,double y1,double z1,double x2,double y2,double z2,double X1,double Y1,double Z1,double X2,double Y2,double Z2,double X3,double Y3,double Z3,double tolerance)
{

  double v1x=(X2-X1), v1y=(Y2-Y1),v1z=(Z2-Z1);
  double v2x=(X3-X1), v2y=(Y3-Y1),v2z=(Z3-Z1);
  double sx=(x2-x1), sy=(y2-y1),sz=(z2-z1);//segment

  double nx=v1y*v2z-v1z*v2y, ny=v1z*v2x-v1x*v2z, nz=v1x*v2y-v1y*v2x;
  double norme_n=sqrt(nx*nx+ny*ny+nz*nz);
  if(norme_n<ZERO)return false;//the area of the triangle is 0 => no intersection
  if(sqrt(sx*sx+sy*sy+sz*sz)<ZERO) return false;// the length of the segment is 0 => no intersection
  double ns=nx*sx+ny*sy+nz*sz;
  if(fabs(ns)<ZERO) return false;// the triangle and the segment are parallel => no intersection

  double dx=(X1-x1), dy=(Y1-y1),dz=(Z1-z1);//segment
  double t=(nx*dx+ny*dy+nz*dz)/ns;
  if((t<-tolerance)||(t>1+tolerance)) return false; //the intersection is not in the segment => no intersection

  double rx=t*(x2-x1)+x1 -X1,ry=t*(y2-y1)+y1 -Y1,rz=t*(z2-z1)+z1 -Z1;//intersection point-(X1,Y1,Z1)
  double u=(nx*(ry*v2z-rz*v2y) + ny*(rz*v2x-rx*v2z) + nz*(rx*v2y-ry*v2x))/(norme_n*norme_n);
  double v=-(nx*(ry*v1z-rz*v1y) + ny*(rz*v1x-rx*v1z) + nz*(rx*v1y-ry*v1x))/(norme_n*norme_n);

  if((u>=-tolerance)&&(v>=-tolerance)&&(u+v<=1+tolerance))return true; // intersection in the triangle => intersection

  return false;// intersection is not in the triangle => no intersection


}

//---------------------------------------------------------------------------

bool Polygon::Intersection_line_triangle(double x1,double y1,double z1,double x2,double y2,double z2,double X1,double Y1,double Z1,double X2,double Y2,double Z2,double X3,double Y3,double Z3,double tolerance)
{

  double v1x=(X2-X1), v1y=(Y2-Y1),v1z=(Z2-Z1);
  double v2x=(X3-X1), v2y=(Y3-Y1),v2z=(Z3-Z1);
  double sx=(x2-x1), sy=(y2-y1),sz=(z2-z1);//segment

  double nx=v1y*v2z-v1z*v2y, ny=v1z*v2x-v1x*v2z, nz=v1x*v2y-v1y*v2x;
  double norme_n=sqrt(nx*nx+ny*ny+nz*nz);

  if(norme_n<ZERO)return false;//the area of the triangle is 0 => no intersection
  if(sqrt(sx*sx+sy*sy+sz*sz)<ZERO)
    {
      cerr<<"*********************************************************"<<endl;
      cerr<<"ERROR: Polygon::Intersection_line_triangle() invalid input"<<endl;
      cerr<<"*********************************************************"<<endl;
      exit(1);
    }
  double ns=nx*sx+ny*sy+nz*sz;
  if(fabs(ns)<ZERO) return false;// the triangle and the segment are parallel => no intersection

  double dx=(X1-x1), dy=(Y1-y1),dz=(Z1-z1);//segment
  double t=(nx*dx+ny*dy+nz*dz)/ns;

  double rx=t*(x2-x1)+x1 -X1,ry=t*(y2-y1)+y1 -Y1,rz=t*(z2-z1)+z1 -Z1;//intersection point-(X1,Y1,Z1)
  double u=(nx*(ry*v2z-rz*v2y) + ny*(rz*v2x-rx*v2z) + nz*(rx*v2y-ry*v2x))/(norme_n*norme_n);
  double v=-(nx*(ry*v1z-rz*v1y) + ny*(rz*v1x-rx*v1z) + nz*(rx*v1y-ry*v1x))/(norme_n*norme_n);
  // cerr<<"|n|="<<norme_n<<" u="<<u<<" v="<<v<<endl;

  if((u>=-tolerance)&&(v>=-tolerance)&&(u+v<=1+tolerance))return true; // intersection in the triangle => intersection

  return false;// intersection is not in the triangle => no intersection


}
