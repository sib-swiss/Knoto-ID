#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.178494,0.962484,-0.204365 --closure-method=direct --output=convert_diagram_49.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_49.txt "${sourcepath}"/output/convert_diagram_49.txt || exit $?
