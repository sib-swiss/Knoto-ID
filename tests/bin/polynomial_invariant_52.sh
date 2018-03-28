#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=-0.37891,0.842926,0.381972 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_52.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_52.txt "${sourcepath}"/output/polynomial_invariant_52.txt || exit $?
