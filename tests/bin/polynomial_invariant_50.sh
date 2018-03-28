#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projections-list="${sourcepath}"/input/input_projections.txt --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_50.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_50.txt "${sourcepath}"/output/polynomial_invariant_50.txt || exit $?
