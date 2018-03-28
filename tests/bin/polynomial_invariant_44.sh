#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=-0.567343,0.785235,0.248049 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_44.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_44.txt "${sourcepath}"/output/polynomial_invariant_44.txt || exit $?
