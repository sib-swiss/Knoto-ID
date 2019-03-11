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
}

//---------------------------------------------------------------------------

Polynomial::Polynomial(vector<string> var_names)
{

  //order var_names
  for(int i=0;i<var_names.size();i++)
    var_names_to_index[var_names[i]]=0;
  int index=0;
  for(map<string,int>::iterator it=var_names_to_index.begin();it!=var_names_to_index.end();it++)
    {
      it->second=index;
      index++;
    }
}

//---------------------------------------------------------------------------

Polynomial::Polynomial(string string_polynomial,vector<string> var_names)
{  
  //order var_names
  for(int i=0;i<var_names.size();i++)
    var_names_to_index[var_names[i]]=0;
  int index=0;
  for(map<string,int>::iterator it=var_names_to_index.begin();it!=var_names_to_index.end();it++)
    {
      it->second=index;
      index++;
    }
  bool flag_keep_var_names=false;
  if(var_names.size()>0)
    flag_keep_var_names=true;
    
  if(!load_from_string(string_polynomial,flag_keep_var_names))
    {
      throw runtime_error("invalid polynomial \""+string_polynomial+"\"");
    }
}

//---------------------------------------------------------------------------
void Polynomial::add(const Polynomial & p,bool flag_force)
{
  if((!flag_force)&&var_names_to_index!=p.var_names_to_index)
    {
      cerr<<"ERROR Polynomial::add(): polynomial do not have the same variables."<<endl;
      exit(1);
    }
  for(map<vector<long>,double>::const_iterator it=p.coefficients.begin();it!=p.coefficients.end();it++)
    {
      coefficients[it->first]+=it->second;
    }
  clean();
}
//---------------------------------------------------------------------------
void Polynomial::add(double coefficient,string var,long exponent)
{
  if(var=="")
    {
      vector<long> exps(var_names_to_index.size(),0);
      coefficients[exps]+=coefficient;
      clean();
    }
  else
    {
      map<string,int>::iterator it=var_names_to_index.find(var);
      int index;
      if(it==var_names_to_index.end())//does not exist, create variable
	{
	  cerr<<"ERROR: Polynomial::add() variable does not exist."<<endl;  //to catch errors with PolynomialInvariant. Could be removed
	  exit(1); //to catch errors with PolynomialInvariant. Could be removed
	  index=add_variable(var);
	}
      else
	{
	  index=it->second;
	}
      vector<long> exps(var_names_to_index.size(),0);
      exps[index]=exponent;
      coefficients[exps]+=coefficient;
      clean();
    }
}
//---------------------------------------------------------------------------
void Polynomial::multiply(const Polynomial & p,bool flag_force)
{
  if((!flag_force)&&var_names_to_index!=p.var_names_to_index)
    {
      cerr<<"ERROR Polynomial::multiply(): polynomial do not have the same variables."<<endl;
      exit(1);
    }
  
  map<vector<long>,double> coefficients_tmp;//coefficient(n,m)=coefficient[pair(n,m)]
  vector<long> exponents(var_names_to_index.size(),0);
  for(map<vector<long>,double>::iterator it1=coefficients.begin();it1!=coefficients.end();it1++)
    {
      for(map<vector<long>,double>::const_iterator it2=p.coefficients.begin();it2!=p.coefficients.end();it2++)
	{
	  for(int i=0;i<it1->first.size();i++)	    
	    exponents[i]=it1->first[i]+it2->first[i];
	  coefficients_tmp[exponents]+=it1->second*it2->second;
	}
    }
  coefficients=coefficients_tmp;
  clean();
  
}

//---------------------------------------------------------------------------
vector<string> Polynomial::get_variable_names()const
{
  vector<string> var_names(var_names_to_index.size());
  for(map<string,int>::const_iterator it=var_names_to_index.begin();it!=var_names_to_index.end();it++)
    {
      if(it->second<0||it->second>=var_names_to_index.size())
	{
	  cerr<<"ERROR with variables in Polynomial."<<endl;
	  exit(1);	  
	}
      var_names[it->second]=it->first;
    }
  return var_names;
}
////---------------------------------------------------------------------------
bool Polynomial::load_from_string(string str,bool flag_keep_var_names)
{
  //NOTE:
  //format: monom1 monom2 monom3 ...
  //with monomi: coef*var_names[0]^exp1*var_names[1]^exp2*var_names[3]^exp3*... OR coef*var^exp OR coef
  //and var: must start with alphabetic character, followed by alphanumeric characters
  //and coef: sign integer OR integer OR sign
  //and exp: sign integer OR integer OR  (sign integer) OR (integer)
  //Examples (valid):
  //"1"
  //"0"
  //"2+x^2+1*x^ 3*y+2*x^(4)*y^(-5)-3*x*y^4 -3*x^(2)+ x^(-2)"
  //"x^ 3*y^(-4)-y^(-6)*x^2 + x^(-2)"
  //Examples (errors):
  //"x^(3+4)*y^(-4)  => error: (3+4)
  //"x^(3)*3y^(-4)  => error: invalid variable name 3y



  coefficients.clear();
  map<string,int> var_names_to_index_tmp;
  const boost::regex e1("[ \r]");
  str= regex_replace(str, e1, "", boost::match_default);
  //replace "^(...)" by "^..."
  const boost::regex e2("\\^\\(([+-]*[0-9]+)\\)([+-]|$|\\*)");
  str= regex_replace(str, e2, "\\^\\1\\2", boost::match_default);
  //add a + in first position if not already a + or -
  const boost::regex e3("^([^+-])");
  str= regex_replace(str, e3, "+\\1", boost::match_default);

  //check validity
  std::string::const_iterator start, end;
  std::string::const_iterator start2, end2;
  start = str.begin();
  end = str.end();
  boost::match_results<std::string::const_iterator> m;
  boost::match_flag_type flags = boost::match_default;
  //search for invalid variable names (starting with numeric char)
  const boost::regex invalidvar1("(?:^|[^a-zA-Z])([0-9]+[a-zA-Z]+[0-9a-zA-Z]*)");
  if(regex_search(start,end, m,invalidvar1, flags))
    {
      return false;
    }
  //search for two consecutive -,+ or *
  const boost::regex invalidvar2("([-\\+\\*][-\\+\\*])");
  if(regex_search(start,end, m,invalidvar2, flags))
    {
      return false;
    }

  const boost::regex monomial("([+-])(?:([0-9]+)\\*?)?((?:[a-zA-Z]+[0-9a-zA-Z]*(?:\\^[+-]*[0-9]+)?)?(?:\\*[a-zA-Z]+[0-9a-zA-Z]*(?:\\^[+-]*[0-9]+)?)*)");
  const boost::regex var("(?:^|\\*)(?:([a-zA-Z]+[0-9a-zA-Z]*)(?:\\^([+-]*)([0-9]+))?)");
  boost::match_results<std::string::const_iterator> m2;
  vector<vector<pair<string,long> > > monomials_products;//monomials_products[n]=vector((variable1,exponent1),(variable2,exponent2),...) 
  vector<double> monomials_coefficients;//monomials_coeff[n]=signed coefficient for n-th monomial
  string str_matched="";
  //searc for monomials
  while(true)
    {
      if(regex_search(start, end, m,monomial, flags)&&m[0].first==start)
	{
	  //m[1]=sign
	  //m[2]=coef
	  //m[3]=product
	  if(m.size()!=4)
	    {
	      return false;
	    }
	  if(m[2]==""&&m[3]=="")
	    {
	      return false;
	    }
	  double coef=1;
	  vector<pair<string,long> > product;
	  if(m[2]!="")
	    coef=atol(string(m[2].first,m[2].second).c_str());
	  if(m[1]=="-")
	    coef=-coef;
	  //search with monomial
	  start2=m[3].first;
	  end2=m[3].second;
	  string str_matched2="";
	  while(true)
	    {
	      if(regex_search(start2,end2, m2,var, flags)&&m2[0].first==start2)
		{
		  //m2[1]=variable
		  //m2[2]=exponent_sign
		  //m2[3]=exponent
		  string var=m2[1];
		  long exp=1;
		  if(m2[3]!="")
		    exp=atol(string(m2[3].first,m2[3].second).c_str());
		  if(m2[2]=="-")
		    exp=-exp;
		  product.push_back(pair<string,int>(var,exp));
		  var_names_to_index_tmp[var]=-1;
		  start2 = m2[0].second;		  
		}
	      else
		{
		  break;
		}
	      str_matched2=str_matched2+string(m2[0].first,m2[0].second);
	    }
	  if(str_matched2!=string(m[3].first,m[3].second))
	    {
	      return false;
	    }
	  start = m[0].second;
	  monomials_coefficients.push_back(coef);
	  monomials_products.push_back(product);
	}
      else
	{
	  break;
	}
      str_matched=str_matched+string(m[0].first,m[0].second);      
    }
  if(str_matched!=str)
    {
      return false;
    }
  //create var_names_to_index
  if(flag_keep_var_names==false)//i.e. replace var_names_to_index
    {
      int index=0;
      var_names_to_index=var_names_to_index_tmp;
      for(map<string,int>::iterator it=var_names_to_index.begin();it!=var_names_to_index.end();it++)
	{
	  it->second=index;
	  index++;
	}
    }
  else//
    {
      //add all variable in var_names_to_index to var_names_to_index_tmp
      int index=0;
      for(map<string,int>::iterator it=var_names_to_index.begin();it!=var_names_to_index.end();it++)
	{
	  var_names_to_index_tmp[it->first]=0;
	}
      if(var_names_to_index_tmp.size()>var_names_to_index.size())// => var_names_to_index_tmp (i.e. input string) contain variables that are not in var_names_to_index
	{	  
	  return false;
	}
    }

  //create coefficients
  vector<long> exponents(var_names_to_index.size());
  for(int n=0;n<monomials_coefficients.size();n++)
    {
      for(int i=0;i<exponents.size();i++)
	exponents[i]=0;
      for(int i=0;i<monomials_products[n].size();i++)
	{
	  int var_index=var_names_to_index[monomials_products[n][i].first];
	  exponents[var_index]+=monomials_products[n][i].second;
	}
      coefficients[exponents]=monomials_coefficients[n];
    }

  return true;
}

//---------------------------------------------------------------------------
string Polynomial::to_string()const
{
  stringstream ss;
  ss<<*this;
  return ss.str();
}

//---------------------------------------------------------------------------
ostream& operator<<(ostream& out, const Polynomial& p)
{
  vector<string> var_names=p.get_variable_names();
  if(p.coefficients.size()==0)
    {
      out<<"0";
      return out;
    }

  for(map<vector<long>,double>::const_iterator it=p.coefficients.begin();it!=p.coefficients.end();it++)
    {
      //coefficient
      long coef=it->second;
      string s="*";
      if(fabs(coef-1)<ZERO)
	{
	  out<<" + ";
	  s="";
	}
      else if(fabs(coef+1)<ZERO)
	{
	  out<<" - ";
	  s="";
	}
      else
	{
	  if(coef>0)
	    out<<" + "<<coef;
	  else
	    out<<" - "<<fabs(coef);
	  s="*";
	}
      //product
      long exponent;
      string var;
      bool flag_all_zero=true;
      for(int i=0;i<it->first.size();i++)
	{
	  exponent=it->first[i];
	  var=var_names[i];
	  if(exponent==1)
	    {
	      out<<s<<var;
	      s="*";
	      flag_all_zero=false;
	    }
	  else if(exponent!=0)
	    {
	      out<<s<<var<<"^("<<exponent<<")";
	      s="*";
	      flag_all_zero=false;
	    }
	}
      if(flag_all_zero)
	{
	  if(s=="")//i.e. coefficient not yet written
	    out<<s<<1;
	  s="*";
	}
    }
  return out;
}

//---------------------------------------------------------------------------

bool Polynomial::operator==(const Polynomial &p)const
{
  
  if(var_names_to_index==p.var_names_to_index)
    {
      return coefficients==p.coefficients;
    }
  else
    {
      return to_string()==p.to_string();
    }

}


//---------------------------------------------------------------------------
void Polynomial::clean()
{
  map<vector<long>,double>::iterator it=coefficients.begin();
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
//---------------------------------------------------------------------------
int Polynomial::add_variable(string var_name)
{
  vector<int> old_to_new_index(var_names_to_index.size());//old_to_new_index[m]=n <=> var_name_to_index[var]=n after insertion correspond to var_names_to_index[var]=m before insertion

  int new_index=-1;
  //insert new variable  
  var_names_to_index[var_name]=-1;  
  //recreate var_names and adapt index
  int index=0;
  for(map<string,int>::iterator it=var_names_to_index.begin();it!=var_names_to_index.end();it++)
    {
      if(it->first==var_name)
	{
	  new_index=index;
	}
      else
	{
	  old_to_new_index[it->second]=index;
	}
      it->second=index;
      index++;
    }
  
  //recreate coefficient
  map<vector<long>,double> coefficients_new;
  vector<long> expnew(var_names_to_index.size(),0);
  for(map<vector<long>,double>::iterator it=coefficients.begin();it!=coefficients.end();it++)
    {
      for(int i=0;i<it->first.size();i++)
	{
	  expnew[old_to_new_index[i]]=it->first[i];
	  coefficients_new[expnew]=it->second;
	}
    }
  
  coefficients=coefficients_new;

  return new_index;
}

