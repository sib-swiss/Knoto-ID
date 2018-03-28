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


#ifndef RANDOM_HH 
#define RANDOM_HH

////boost
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp> 
#include <boost/random/variate_generator.hpp>


typedef boost::mt19937 random_generator_type;
typedef boost::uniform_real<> random_distribution_type;
typedef boost::variate_generator<random_generator_type&, random_distribution_type > random_variate_generator_type;
 
extern random_generator_type random_generator;
extern random_distribution_type random_distribution;
extern random_variate_generator_type random01;

#endif
