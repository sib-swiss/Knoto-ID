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

#include "Polynomial.hh"


using namespace std;

//************************************************************
//**********************   Polynomial   **********************
//************************************************************

Polynomial::Polynomial()
{
  var1_name="x";
  var2_name="y";  
}
//---------------------------------------------------------------------------
void Polynomial::add(const Polynomial & p)
{
  for(map<pair<long,long>,double>::const_iterator it=p.get_coefficients().begin();it!=p.get_coefficients().end();it++)
    {
      coefficients[it->first]+=it->second;
    }
  clean();
}
//---------------------------------------------------------------------------
void Polynomial::add_monomial(double coefficient,long n,long m)
{
  coefficients[pair<long,long>(n,m)]+=coefficient;
  clean();
}
//---------------------------------------------------------------------------
void Polynomial::multiply(const Polynomial & p)
{
  map<pair<long,long>,double> coefficients_tmp;//coefficient(n,m)=coefficient[pair(n,m)]
  for(map<pair<long,long>,double>::iterator it1=coefficients.begin();it1!=coefficients.end();it1++)
    {
      for(map<pair<long,long>,double>::const_iterator it2=p.get_coefficients().begin();it2!=p.get_coefficients().end();it2++)
	{
	  coefficients_tmp[pair<long,long>(it1->first.first+it2->first.first,it1->first.second+it2->first.second)]+=it1->second*it2->second;
	}
    }
  coefficients=coefficients_tmp;
  clean();
  
}
//---------------------------------------------------------------------------
void Polynomial::multiply_monomial(double coefficient,long n,long m)
{
  map<pair<long,long>,double> coefficients_tmp;//coefficient(n,m)=coefficient[pair(n,m)]
  for(map<pair<long,long>,double>::iterator it1=coefficients.begin();it1!=coefficients.end();it1++)
    {
      coefficients_tmp[pair<long,long>(it1->first.first+n,it1->first.second+m)]+=it1->second*coefficient;
    }
  coefficients=coefficients_tmp;
  clean();
}

//---------------------------------------------------------------------------
void Polynomial::clean()
{
  map<pair<long,long>,double>::iterator it=coefficients.begin();
  while(it!=coefficients.end())
    {
      if(fabs(it->second)<ZERO)
	{
	  coefficients.erase(it++);
	}
      else
	{
	  ++it;
	}
    }
}

////---------------------------------------------------------------------------
bool Polynomial::load_from_string(string str,string var1_name_t,string var2_name_t)
{
  //NOTE:
  //format: monom1 monom2 monom3 ...
  //with monomi: coef*var1^exp1*var2^exp2 OR coef*var*exp OR coef
  //and var: only alphabetic chars
  //and coef: sign integer OR integer
  //and exp: sign integer OR integer OR  (sign integer) OR (integer)
  //Examples (valid):
  //"1"
  //"0"
  //"2+x^2+1*x^ 3*y+2*x^(4)*y^(-5)-3*x*y^4 -3*x^(2)+ x^(-2)"
  //"x^ 3*y^(-4)-y^(-6)*x^2 + x^(-2)"
  //Exapmles (errors):
  //"x^(3+4)*y^(-4)  => error: (3+4)
  //"A+1-y^(-6)*x^2 + V^(-2)"  => error: 4 variable A,x,y,V

  coefficients.clear();
  var1_name=var1_name_t;
  var2_name=var2_name_t;
  //remove " ", "\r"
  const boost::regex e1("[ \r]");
  str= regex_replace(str, e1, "", boost::match_default);
  //replace "^(...)" by "^..."
  const boost::regex e2("\\^\\(([+-]*[0-9]+)\\)([+-]|$|\\*)");
  str= regex_replace(str, e2, "\\^\\1\\2", boost::match_default);
  //add a + in first position if not already a + or -
  const boost::regex e3("^([^+-])");
  str= regex_replace(str, e3, "+\\1", boost::match_default);
  if(str=="")
    {
      //cerr<<"ERROR Polynomial::load_from_string(): empty string"<<endl;
      coefficients.clear();
      var1_name=var1_name_t;
      var2_name=var2_name_t;
      return false;
    }
	
  
  //find monomial
  const boost::regex monomial1("([+-])(?:([0-9]+)\\*)?([a-zA-Z]+)(?:\\^([+-]*)([0-9]+))?\\*([a-zA-Z]+)(?:\\^([+-]*)([0-9]+))?");// +1*x^2*y^3 or -1*x*y
  const boost::regex monomial2("([+-])(?:([0-9]+)\\*)?([a-zA-Z]+)(?:\\^([+-]*)([0-9]+))?");// +1*x^2 or -1*y
  const boost::regex monomial3("([+-])([0-9]+)");// +1
  std::string::const_iterator start, end;
  start = str.begin();
  end = str.end();
  boost::match_results<std::string::const_iterator> m;
  boost::match_flag_type flags = boost::match_default;
  long coef,exp1,exp2;
  string var1,var2;
  string str_matched="";
  while(true)
    {
      if(regex_search(start, end, m,monomial1, flags)&&m[0].first==start)
	{
	  if(m.size()!=9)
	    {
	      coefficients.clear();
	      var1_name=var1_name_t;
	      var2_name=var2_name_t;
	      return false;
	    }
	  coef=1;
	  if(m[2]!="")
	    coef=atol(string(m[2].first,m[2].second).c_str());
	  if(m[1]=="-")
	    coef=-coef;
	  exp1=1;
	  if(m[5]!="")
	    exp1=atol(string(m[5].first,m[5].second).c_str());
	  if(m[4]=="-")
	    exp1=-exp1;
	  exp2=1;
	  if(m[8]!="")
	    exp2=atol(string(m[8].first,m[8].second).c_str());
	  if(m[7]=="-")
	    exp2=-exp2;
	  var1=m[3];
	  var2=m[6];
	  start = m[0].second;

	  if((var1==var1_name||var1_name=="")&&(var2==var2_name||var2_name==""))
	    {
	      var1_name=var1;
	      var2_name=var2;
	      add_monomial(coef,exp1,exp2);
	    }
	  else if((var2==var1_name||var1_name=="")&&(var1==var2_name||var2_name==""))
	    {
	      var1_name=var2;
	      var2_name=var1;
	      add_monomial(coef,exp2,exp1);
	    }
	  else
	    {
	      coefficients.clear();
	      var1_name=var1_name_t;
	      var2_name=var2_name_t;
	      return false;
	    }
	    
	}
      else if(regex_search(start, end, m,monomial2, flags)&&m[0].first==start)
	{
	  coef=1;
	  if(m[2]!="")
	    coef=atol(string(m[2].first,m[2].second).c_str());
	  if(m[1]=="-")
	    coef=-coef;
	  exp1=1;
	  if(m[5]!="")
	    exp1=atol(string(m[5].first,m[5].second).c_str());
	  if(m[4]=="-")
	    exp1=-exp1;
	  var1=m[3];
	  exp2=0;
	  var2="";
	  start = m[0].second;

	  if(var1==var1_name||var1_name=="")
	    {
	      var1_name=var1;
	      add_monomial(coef,exp1,exp2);
	    }
	  else if(var1==var2_name||var2_name=="")
	    {
	      var2_name=var1;
	      add_monomial(coef,exp2,exp1);
	    }
	  else
	    {
	      coefficients.clear();
	      var1_name=var1_name_t;
	      var2_name=var2_name_t;
	      return false;
	    }	   
	}
      else if(regex_search(start, end, m,monomial3, flags)&&m[0].first==start)
	{
	  coef=1;
	  if(m[2]!="")
	    coef=atol(string(m[2].first,m[2].second).c_str());
	  if(m[1]=="-")
	    coef=-coef;
	  exp1=0;
	  var1="";
	  exp2=0;
	  var2="";
	  start = m[0].second;
	  
	  add_monomial(coef,exp2,exp1);
	}
      else
	{
	  break;
	}
      str_matched=str_matched+string(m[0].first,m[0].second);
      
    }
  if(str_matched!=str)
    {
      coefficients.clear();
      var1_name=var1_name_t;
      var2_name=var2_name_t;
      return false;
    }
	
  return true;
}

//---------------------------------------------------------------------------
string Polynomial::to_string()
{
  stringstream ss;
  ss<<*this;
  return ss.str();
}

//---------------------------------------------------------------------------
ostream& operator<<(ostream& out, const Polynomial& p)
{
  if(p.coefficients.size()==0)
    out<<"0";
  else
    {
      for(map<pair<long,long>,double>::const_iterator it=p.coefficients.begin();it!=p.coefficients.end();it++)
	{
	  string s="*";
	  if(fabs(it->second-1)<ZERO)
	    {
		out<<" + ";
		s="";
	    }
	  else if(fabs(it->second+1)<ZERO)
	    {
		out<<" - ";
		s="";
	    }
	  else
	    {
	      if(it->second>0)
		out<<" + "<<it->second;
	      else
		out<<" - "<<fabs(it->second);
	      s="*";
	    }
	  if(it->first.first==0&&it->first.second==0)
	    {
	      if(s=="")//i.e. coefficient not yet written
		out<<s<<1;
	      s="*";
	    }	  
	  else
	    {
	      if(it->first.first==1)
		{
		  out<<s<<p.var1_name;
		  s="*";
		}
	      else if(it->first.first!=0)
		{
		  out<<s<<p.var1_name<<"^("<<it->first.first<<")";
		  s="*";
		}
	      if(it->first.second==1)
		{
		  out<<s<<p.var2_name;
		  s="*";
		}
	      else if(it->first.second!=0)
		{
		  out<<s<<p.var2_name<<"^("<<it->first.second<<")";
		  s="*";
		}
	    }
	}
    } 

  return out;
}

