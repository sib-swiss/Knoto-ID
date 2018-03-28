#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.0334535,-0.556445,0.830211 --closure-method=open --output=convert_diagram_138.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_138.txt "${sourcepath}"/output/convert_diagram_138.txt || exit $?
