#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.449628,-0.450605,0.771227 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_2.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_2.txt "${sourcepath}"/output/polynomial_invariant_2.txt || exit $?
