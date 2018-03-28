#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=-0.578572,-0.815556,0.0110619 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_12.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_12.txt "${sourcepath}"/output/polynomial_invariant_12.txt || exit $?
