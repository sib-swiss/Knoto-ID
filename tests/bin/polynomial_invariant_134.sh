#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --names-db=internal --projection=-0.577462,-0.814371,-0.0577736 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_134.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_134.txt "${sourcepath}"/output/polynomial_invariant_134.txt || exit $?
