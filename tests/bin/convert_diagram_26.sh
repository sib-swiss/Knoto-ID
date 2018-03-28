#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.125061,0.567846,-0.813579 --closure-method=open --output=convert_diagram_26.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_26.txt "${sourcepath}"/output/convert_diagram_26.txt || exit $?
