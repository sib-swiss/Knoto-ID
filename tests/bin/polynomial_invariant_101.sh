#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.248605,-0.571143,-0.782299 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_101.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_101.txt "${sourcepath}"/output/polynomial_invariant_101.txt || exit $?
