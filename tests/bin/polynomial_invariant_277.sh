#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.615481,0.0974826,-0.7821 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_277.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_277.txt "${sourcepath}"/output/polynomial_invariant_277.txt || exit $?
