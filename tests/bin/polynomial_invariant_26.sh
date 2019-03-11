#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=0.373251,0.927725,-0.00320201 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_26.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_26.txt "${sourcepath}"/output/polynomial_invariant_26.txt || exit $?
