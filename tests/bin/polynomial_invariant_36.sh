#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.238974,0.16287,-0.957269 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_36.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_36.txt "${sourcepath}"/output/polynomial_invariant_36.txt || exit $?
