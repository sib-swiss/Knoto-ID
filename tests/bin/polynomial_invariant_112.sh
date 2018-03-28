#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --projection=-0.286115,-0.807467,-0.515883 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_112.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_112.txt "${sourcepath}"/output/polynomial_invariant_112.txt || exit $?
