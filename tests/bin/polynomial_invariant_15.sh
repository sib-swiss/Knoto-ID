#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=0.153371,-0.771495,0.617474 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_15.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_15.txt "${sourcepath}"/output/polynomial_invariant_15.txt || exit $?
