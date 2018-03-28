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



#ifndef ZERO
#define ZERO 1e-10
#endif

//************************************************************
//**********************   Polynomial   **********************
//************************************************************
//simple (and not efficient) class for bivariate Laurent polynomials with integer exponent
class Polynomial
{
public:
  Polynomial();//construct polynomial 0
  void add(const Polynomial & p);
  void add_monomial(double coefficient,long n,long m);//add coefficient*x^n*y^m
  void multiply(const Polynomial & p);
  void multiply_monomial(double coefficient,long n,long m);//multiply by coefficient*x^n*y^m

  void set_variable_names(std::string var1,std::string var2){var1_name=var1;var2_name=var2;}
  std::string get_variable1_name(){return var1_name;}
  std::string get_variable2_name(){return var2_name;}
  void clean();//remove coefficients=0

  const std::map<std::pair<long,long>,double> & get_coefficients()const {return coefficients;}

  //load polygon from string. return true if success, false if failed.
  //if var1="" and var2="" => variable names taken from input (arbitrary order)
  bool load_from_string(std::string input,std::string var1="",std::string var2="");
  std::string to_string();

  friend std::ostream& operator<<(std::ostream& out, const Polynomial& p);

private:
  //stored as sum_n,m coefficients(n,m) x^n y^m
  std::map<std::pair<long,long>,double> coefficients;//coefficient(n,m)=coefficient[pair(n,m)]
  std::string var1_name;//only for output
  std::string var2_name;//only for output
  
};


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif
