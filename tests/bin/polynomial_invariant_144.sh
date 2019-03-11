#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --projection=-0.0508897,-0.951747,0.302636 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_144.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_144.txt "${sourcepath}"/output/polynomial_invariant_144.txt || exit $?
