#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --projection=-0.0302461,-0.390422,0.920139 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_136.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_136.txt "${sourcepath}"/output/polynomial_invariant_136.txt || exit $?
