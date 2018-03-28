#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.173653,0.912558,0.370246 --closure-method=open --output=convert_diagram_57.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_57.txt "${sourcepath}"/output/convert_diagram_57.txt || exit $?
