#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=0.19724,0.970233,-0.140514 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_66.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_66.txt "${sourcepath}"/output/polynomial_invariant_66.txt || exit $?
