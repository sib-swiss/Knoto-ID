#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.0501152,0.731469,0.68003 --closure-method=open --output=convert_diagram_137.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_137.txt "${sourcepath}"/output/convert_diagram_137.txt || exit $?
