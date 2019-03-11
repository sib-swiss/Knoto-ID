#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=0.730792,0.679009,0.0699316 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_13.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_13.txt "${sourcepath}"/output/polynomial_invariant_13.txt || exit $?
