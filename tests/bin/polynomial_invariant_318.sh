#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --projection=-0.0918478,0.882564,0.461134 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_318.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_318.txt "${sourcepath}"/output/polynomial_invariant_318.txt || exit $?
