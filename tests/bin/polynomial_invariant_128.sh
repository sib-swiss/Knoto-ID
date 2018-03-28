#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --projection=0.840433,0.338666,-0.423058 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_128.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_128.txt "${sourcepath}"/output/polynomial_invariant_128.txt || exit $?
