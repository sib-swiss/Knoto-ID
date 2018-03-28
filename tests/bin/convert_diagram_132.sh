#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.520352,-0.578979,-0.627708 --closure-method=direct --output=convert_diagram_132.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_132.txt "${sourcepath}"/output/convert_diagram_132.txt || exit $?
