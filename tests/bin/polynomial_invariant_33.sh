#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=0.126794,0.97177,-0.198962 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_33.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_33.txt "${sourcepath}"/output/polynomial_invariant_33.txt || exit $?
