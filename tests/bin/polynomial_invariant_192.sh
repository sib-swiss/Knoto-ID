#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --projection=0.0291004,-0.967722,-0.250334 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_192.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_192.txt "${sourcepath}"/output/polynomial_invariant_192.txt || exit $?
