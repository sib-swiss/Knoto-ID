Knoto-ID
========
The backbone of most proteins forms an open curve.  To study their
entanglement, a common strategy consists in searching for the presence
of knots in their backbones using topological invariants.  However,
this approach requires to close the curve into a loop, which alters
the geometry of curve.  Knoto-ID allows evaluating the entanglement of
open curves without the need to close them, using the recent concept
of knotoids which is a generalization of classical knot theory to open
curves.  Knoto-ID can analyse the global topology of the full chain as
well as the local topology by exhaustively studying all subchains or
only determining the knotted core.  The use of Knoto-ID is not limited
to proteins, it can be used to analyse any open curve in 3D space such
as chromosomes, synthetic polymers, random walks, etc.

If you use this software for a publication, please cite:
J. Dorier, D. Goundaroulis, F. Benedetti and A. Stasiak, "Knoto-ID: a
tool to study the entanglement of open protein chains using the
concept of knotoids", submitted (2018).

Source code is available at <https://github.com/sib-swiss/Knoto-ID>.

## Features

Knoto-ID is a collection of command line tools to study knot and
knotoid diagrams on the sphere and on the plane. Its main features
are:

* Accept protein structures in Protein Data Bank (PDB) format,
  3D curves in xyz format, knot and knotoid diagrams in extended Gauss
  code format or PD code format.
* Draw knot(oid) diagrams.
* Evaluate the following polynomial invariant: classical Jones
  polynomial for knots, Jones polynomial for knotoids and Turaev loop
  bracket for planar knotoids.
* Evaluate polynomial invariant for multiple projection direction and
  produce projection maps on the plane of spherical coordinates or
  directly on a 3D globe (in interactive webGL format).
  with the 3D curve used as input.
* Find knotted cores of 3D curves. 
* Output a 3D curve (in interactive webGL format) and highlight its
  knotted core.
* Generate fingerprint matrices (for open curves) and disk matrices
  (for closed curves) to summarize entanglement of all subchains of a
  3D curve.

Please read the user guide distributed with Knoto-ID for more information.
  


## Installation

Decompress Knoto-ID:
 tar -xf Knoto-ID-<version>.tar.gz 

Go in the Knoto-ID directory
 cd Knoto-ID-<version>/ 

Knoto-ID is organized into four directories:

* bin/: executables polynomial_invariant and knotted_core.
* scripts/: scripts pdb_to_xyz.R, plot_3D_curve.R, plot_projection_map.R, plot_knotted_core.R and plot_diagram.R.
* doc/: user guide. 
* examples/: input files used in the user guide.

Knoto-ID includes five scripts pdb_to_xyz.R, plot_3D_curve.R, plot_projection_map.R,
plot_knotted_core.R and plot_diagram.R. To use these scripts, R
(www.R-project.org) must be installed along with the following
packages:

* optparse
* ggplot2
* RColorBrewer
* reshape2
* geometry
* geosphere
* rgl
* rmarkdown
* Rpdb

These package can installed by entering the following command in R:

install.packages(c("optparse","ggplot2","RColorBrewer","reshape2","geometry","geosphere","rgl","rmarkdown","Rpdb"))

In addition, to output in webGL format (plot_projection_map.R and
plot_3D_curve.R), pandoc (http://pandoc.org/) must also be installed.


Tested with:

* Mac OS X 10.12.6: R 3.3.2, optparse 1.4.4, ggplot2 2.2.1,
  RColorBrewer 1.1-2, reshape2 1.4.2, geometry 0.3-6, geosphere 1.5-7,
  rgl 0.98.1, rmarkdown 1.9, Rpdb 2.3, pandoc 2.1.1.
* Mac OS X 10.13.3: R 3.4.3, optparse 1.4.4, ggplot2 2.2.1,
  RColorBrewer 1.1-2, reshape2 1.4.3, geometry 0.3-6, geosphere 1.5-7,
  rgl 0.99.9, rmarkdown 1.8, Rpdb 2.3, pandoc 1.12.4.2.
* Ubuntu 16.04.3 LTS: R 3.2.3, optparse 1.4.4, ggplot2 2.2.1,
  RColorBrewer 1.1-2, reshape2 1.4.3, geometry 0.3-6, geosphere 1.5-7,
  rgl 0.99.9, Rpdb 2.3, pandoc 1.16.0.2.
* Fedora 25: R 3.4.2, optparse 1.4.4, ggplot2 2.2.1, RColorBrewer 1.1-2,
  reshape2 1.4.3, geometry 0.3-6, geosphere 1.5-7, rgl 0.99.9,
  rmarkdown 1.8, Rpdb 2.3, pandoc 1.17.0.3.
* Windows 10: R 3.4.3, optparse 1.4.4, ggplot2 2.2.1, RColorBrewer 1.1-2,
  reshape2 1.4.3, geometry 0.3-6, geosphere 1.5-8, rgl 0.99.9,
  rmarkdown 1.9, Rpdb 2.3, pandoc 2.1.2.


## Usage

Please read the user guide: doc/user_guide.pdf.


## Credits

Contributors

* Julien Dorier
* Dimos Goundaroulis

Institutions

* Vital-IT, [SIB Swiss Institute of Bioinformatics](http://www.sib.swiss)
* Center for Integrative Genomics, [University of Lausanne](https://www.unil.ch)

Circle packing

The implementation of the circle packing algorithm used to generate
xyz coordinates for a knot(oid) diagram is based on the python version
by David Eppstein:
http://www.ics.uci.edu/~eppstein/PADS/CirclePack.py, licensed under
the MIT License (http://opensource.org/licenses/MIT).

## Copyright

Copyright (C) 2017 by SIB Swiss Institute of Bioinformatics, Julien Dorier and Dimos Goundaroulis.

## Licensing
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.




