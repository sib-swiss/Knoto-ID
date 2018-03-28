#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.376925,0.893359,0.244616 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_69.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_69.txt "${sourcepath}"/output/polynomial_invariant_69.txt || exit $?
