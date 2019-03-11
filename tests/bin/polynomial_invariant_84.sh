#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=0.311159,0.925078,-0.217741 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_84.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_84.txt "${sourcepath}"/output/polynomial_invariant_84.txt || exit $?
