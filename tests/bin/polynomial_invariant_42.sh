#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=-0.365784,-0.866976,0.33846 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_42.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_42.txt "${sourcepath}"/output/polynomial_invariant_42.txt || exit $?
