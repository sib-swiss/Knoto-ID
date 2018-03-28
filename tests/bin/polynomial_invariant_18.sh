#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.543369,0.634381,-0.549828 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_18.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_18.txt "${sourcepath}"/output/polynomial_invariant_18.txt || exit $?
