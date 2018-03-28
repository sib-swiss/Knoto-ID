#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.344991,0.938606,0.00042066 --closure-method=open --output=convert_diagram_89.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_89.txt "${sourcepath}"/output/convert_diagram_89.txt || exit $?
