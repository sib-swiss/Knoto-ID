#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.613638,0.338913,-0.713152 --closure-method=direct --output=convert_diagram_128.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_128.txt "${sourcepath}"/output/convert_diagram_128.txt || exit $?
