#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=-0.921169,0.215636,-0.323959 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_72.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_72.txt "${sourcepath}"/output/polynomial_invariant_72.txt || exit $?
