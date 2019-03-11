#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.142963,0.035301,0.989098 --closure-method=direct --output=convert_diagram_80.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_80.txt "${sourcepath}"/output/convert_diagram_80.txt || exit $?
