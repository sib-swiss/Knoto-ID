#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=-0.374704,0.374219,0.848267 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_124.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_124.txt "${sourcepath}"/output/polynomial_invariant_124.txt || exit $?
