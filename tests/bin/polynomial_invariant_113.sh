#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.286115,-0.807467,-0.515883 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_113.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_113.txt "${sourcepath}"/output/polynomial_invariant_113.txt || exit $?
