#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.958886,0.159265,0.234887 --closure-method=direct --output=convert_diagram_133.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_133.txt "${sourcepath}"/output/convert_diagram_133.txt || exit $?
