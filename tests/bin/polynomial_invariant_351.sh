#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --input-format=pd --nb-moves-III=100000 --output=polynomial_invariant_351.txt "${sourcepath}"/input/input_diagram_open_plane_0.txt || exit $? 
diff polynomial_invariant_351.txt "${sourcepath}"/output/polynomial_invariant_351.txt || exit $?
