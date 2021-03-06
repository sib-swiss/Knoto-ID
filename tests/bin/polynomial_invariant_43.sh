#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=-0.365784,-0.866976,0.33846 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_43.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_43.txt "${sourcepath}"/output/polynomial_invariant_43.txt || exit $?
