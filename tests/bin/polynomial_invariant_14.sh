#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=0.730792,0.679009,0.0699316 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_14.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_14.txt "${sourcepath}"/output/polynomial_invariant_14.txt || exit $?
