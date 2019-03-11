#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --projection=-0.704645,-0.180052,0.686336 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_324.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_324.txt "${sourcepath}"/output/polynomial_invariant_324.txt || exit $?
