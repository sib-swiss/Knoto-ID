#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=-0.363142,-0.924722,0.114096 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_236.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_236.txt "${sourcepath}"/output/polynomial_invariant_236.txt || exit $?
