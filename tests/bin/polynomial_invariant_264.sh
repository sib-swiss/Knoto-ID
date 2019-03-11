#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --projection=0.280506,0.662832,0.69424 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_264.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_264.txt "${sourcepath}"/output/polynomial_invariant_264.txt || exit $?
