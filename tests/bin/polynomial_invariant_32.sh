#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=0.34903,-0.764653,-0.541742 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_32.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_32.txt "${sourcepath}"/output/polynomial_invariant_32.txt || exit $?
