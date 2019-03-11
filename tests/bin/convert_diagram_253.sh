#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=0.460988,0.835345,0.299482 --closure-method=direct --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input7.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=direct --output=convert_diagram_253.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_253.txt "${sourcepath}"/output/convert_diagram_253.txt || exit $?
