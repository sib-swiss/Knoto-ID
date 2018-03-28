#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=0.792697,-0.159323,0.588427 --closure-method=open --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input6.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=open --output=convert_diagram_251.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_251.txt "${sourcepath}"/output/convert_diagram_251.txt || exit $?
