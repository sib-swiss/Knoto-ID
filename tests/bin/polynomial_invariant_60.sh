#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=0.10632,-0.844417,0.52503 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_60.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_60.txt "${sourcepath}"/output/polynomial_invariant_60.txt || exit $?
