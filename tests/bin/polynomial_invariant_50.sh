#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=0.792203,0.172976,-0.585229 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_50.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_50.txt "${sourcepath}"/output/polynomial_invariant_50.txt || exit $?
