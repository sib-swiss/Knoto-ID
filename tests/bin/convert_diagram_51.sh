#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.304356,-0.934417,0.185021 --closure-method=direct --output=convert_diagram_51.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_51.txt "${sourcepath}"/output/convert_diagram_51.txt || exit $?
