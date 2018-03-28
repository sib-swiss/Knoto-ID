#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=-0.567343,0.785235,0.248049 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_45.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_45.txt "${sourcepath}"/output/polynomial_invariant_45.txt || exit $?
