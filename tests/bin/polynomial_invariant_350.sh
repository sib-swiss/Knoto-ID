#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db=internal --input-format=pd --nb-moves-III=100000 --output=polynomial_invariant_350.txt "${sourcepath}"/input/input_diagram_open_plane_0.txt || exit $? 
diff polynomial_invariant_350.txt "${sourcepath}"/output/polynomial_invariant_350.txt || exit $?
