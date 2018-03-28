#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=0.160698,-0.980464,0.113429 --closure-method=direct --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input0.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=direct --output=convert_diagram_233.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_233.txt "${sourcepath}"/output/convert_diagram_233.txt || exit $?
