#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=-0.180715,0.734251,0.654384 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_22.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_22.txt "${sourcepath}"/output/polynomial_invariant_22.txt || exit $?
