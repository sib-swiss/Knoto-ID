#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.432292,-0.495955,0.753095 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_73.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_73.txt "${sourcepath}"/output/polynomial_invariant_73.txt || exit $?
