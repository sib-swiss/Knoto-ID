#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.566206,-0.0935279,0.818941 --closure-method=direct --output=convert_diagram_23.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_23.txt "${sourcepath}"/output/convert_diagram_23.txt || exit $?
