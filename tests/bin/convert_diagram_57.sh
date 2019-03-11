#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.938164,0.296437,-0.17881 --closure-method=open --output=convert_diagram_57.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_57.txt "${sourcepath}"/output/convert_diagram_57.txt || exit $?
