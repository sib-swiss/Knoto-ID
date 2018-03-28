#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projections-list="${sourcepath}"/input/input_projections.txt --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_31.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_31.txt "${sourcepath}"/output/polynomial_invariant_31.txt || exit $?
