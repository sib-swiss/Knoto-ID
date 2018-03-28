#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.625681,0.405735,0.666259 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_40.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_40.txt "${sourcepath}"/output/polynomial_invariant_40.txt || exit $?
