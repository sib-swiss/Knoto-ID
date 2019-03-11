#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=0.746561,-0.450928,0.489194 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_164.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_164.txt "${sourcepath}"/output/polynomial_invariant_164.txt || exit $?
