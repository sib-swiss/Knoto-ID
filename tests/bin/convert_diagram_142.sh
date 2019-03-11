#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.817897,-0.169763,0.54975 --closure-method=open --output=convert_diagram_142.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_142.txt "${sourcepath}"/output/convert_diagram_142.txt || exit $?
