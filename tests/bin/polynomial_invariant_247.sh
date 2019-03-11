#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.885947,0.404946,-0.22609 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_247.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_247.txt "${sourcepath}"/output/polynomial_invariant_247.txt || exit $?
