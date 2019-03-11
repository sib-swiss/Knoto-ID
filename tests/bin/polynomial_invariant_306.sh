#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=0.599125,0.128785,-0.79023 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_306.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_306.txt "${sourcepath}"/output/polynomial_invariant_306.txt || exit $?
