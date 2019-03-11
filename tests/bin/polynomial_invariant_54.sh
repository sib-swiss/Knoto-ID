#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=-0.0561826,-0.945482,0.320791 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_54.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_54.txt "${sourcepath}"/output/polynomial_invariant_54.txt || exit $?
