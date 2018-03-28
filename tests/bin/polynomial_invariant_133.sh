#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.0337623,-0.701158,0.712206 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_133.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_133.txt "${sourcepath}"/output/polynomial_invariant_133.txt || exit $?
