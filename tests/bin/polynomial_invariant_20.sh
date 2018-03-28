#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.122267,-0.886747,0.445791 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_20.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_20.txt "${sourcepath}"/output/polynomial_invariant_20.txt || exit $?
