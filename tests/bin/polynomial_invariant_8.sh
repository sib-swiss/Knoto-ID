#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.171266,-0.954249,-0.245107 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_8.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_8.txt "${sourcepath}"/output/polynomial_invariant_8.txt || exit $?
