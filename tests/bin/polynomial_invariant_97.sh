#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.550663,-0.381652,0.742369 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_97.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_97.txt "${sourcepath}"/output/polynomial_invariant_97.txt || exit $?
