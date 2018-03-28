#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=0.414681,0.904323,0.101191 --closure-method=direct --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input1.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=direct --output=convert_diagram_237.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_237.txt "${sourcepath}"/output/convert_diagram_237.txt || exit $?
