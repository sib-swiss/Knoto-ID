#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=0.34903,-0.764653,-0.541742 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_31.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_31.txt "${sourcepath}"/output/polynomial_invariant_31.txt || exit $?
