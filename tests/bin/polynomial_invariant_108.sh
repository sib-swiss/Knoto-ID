#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.770709,0.637152,0.00673467 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_108.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_108.txt "${sourcepath}"/output/polynomial_invariant_108.txt || exit $?
