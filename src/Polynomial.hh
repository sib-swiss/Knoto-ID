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


#ifndef POLYNOMIAL_HH
#define POLYNOMIAL_HH

#include <iostream>
#include <map>
#include <cmath>
#include <sstream>
#include <boost/regex.hpp>
#include <stdexcept>


#ifndef ZERO
#define ZERO 1e-10
#endif

//************************************************************
//**********************   Polynomial   **********************
//************************************************************
//simple (and not efficient) class for multivariate Laurent polynomials with integer exponent
class Polynomial
{
public:
  Polynomial();//Create polynomial 0
  Polynomial(std::vector<std::string> var_names);//Create polynomial 0 with variables var_names
  Polynomial(std::string string_polynomial,std::vector<std::string> var_names= std::vector<std::string>());//Create polynomial 0 and optionally specify variables var_names. Load polynomial from string. If var_names is specified and does not contain all variable appearing in string_polynomial => error

  //the following function require p to have exaclty the same variable names (with same order), i.e. same var_names_to_index.
  //if flag_force==false => don't check that variable names are the same (faster, but dangerous)
  void add(const Polynomial & p,bool flag_force=false);
  void add(double coefficient,std::string var="",long exponent=1);//add coefficient*var^exponent. If var=="" add only coefficient and ignore exponent
  void multiply(const Polynomial & p,bool flag_force=false);
  //not used// void multiply_monomial(double coefficient,long n,long m);//multiply by coefficient*x^n*y^m


  std::vector<std::string> get_variable_names()const;

  //load polygon from string (and replace current coefficients). return true if success, false if failed.
  //if flag_keep_var_names=false => variable names (var_names_to_index) are erased and created from input (arbitrary order)
  //if flag_keep_var_names=true => keep existing variable names (var_names_to_index) and error if it does not contain all variable appearing in input
  //WARNING: if flag_keep_var_names=false => variable names (var_names_to_index) can change.
  bool load_from_string(std::string input,bool flag_keep_var_names=false);
  std::string to_string()const;

  friend std::ostream& operator<<(std::ostream& out, const Polynomial& p);

  bool operator==(const Polynomial &p)const;
  
private:
  void clean();//remove coefficients=0
  int add_variable(std::string var_name);//add variable var_name and return its index. WARNING: computationally heavy. This will reorder all variables (change var_name_to_index), the coefficients map will have to be recreated.


  
  //stored as sum_n0,n1,n2,... coefficients(n0,n1,n2,...) x0^n0 x1^n1 x2^n2 x3^n3... 
  std::map<std::vector<long>,double> coefficients;//coefficient(n,m)=coefficient[vector(n0,n1,n2,...)] with n0 the exponent for n0-th variable, n1 exponent for n1-th variable, ... with n-th variable var_name such that var_name_to_index[var_name]=n
  std::map<std::string,int> var_names_to_index; //var_names_to_index[name]=n such that n-th variable is "name"
  
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif
