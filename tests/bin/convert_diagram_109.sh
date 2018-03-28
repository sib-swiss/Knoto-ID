#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.103231,0.0173861,0.994505 --closure-method=open --output=convert_diagram_109.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_109.txt "${sourcepath}"/output/convert_diagram_109.txt || exit $?
