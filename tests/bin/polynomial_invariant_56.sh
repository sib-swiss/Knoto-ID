#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.299263,-0.864901,-0.402974 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_56.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_56.txt "${sourcepath}"/output/polynomial_invariant_56.txt || exit $?
