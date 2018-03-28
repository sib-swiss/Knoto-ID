#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.168216,-0.839941,-0.515949 --closure-method=direct --output=convert_diagram_133.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_133.txt "${sourcepath}"/output/convert_diagram_133.txt || exit $?
