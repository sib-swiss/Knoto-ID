#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projections-list="${sourcepath}"/input/input_projections.txt --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_26.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_26.txt "${sourcepath}"/output/polynomial_invariant_26.txt || exit $?
