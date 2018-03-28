#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=-0.640926,0.76529,-0.0595355 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_29.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_29.txt "${sourcepath}"/output/polynomial_invariant_29.txt || exit $?
