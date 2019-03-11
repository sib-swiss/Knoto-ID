#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.950262,-0.0468181,0.307913 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_289.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_289.txt "${sourcepath}"/output/polynomial_invariant_289.txt || exit $?
