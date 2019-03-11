#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projections-list="${sourcepath}"/input/input_projections.txt --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_167.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_167.txt "${sourcepath}"/output/polynomial_invariant_167.txt || exit $?
