#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=-0.811944,-0.456815,-0.363409 --closure-method=open --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input7.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=open --output=convert_diagram_255.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_255.txt "${sourcepath}"/output/convert_diagram_255.txt || exit $?
