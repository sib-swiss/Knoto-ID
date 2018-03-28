#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.237509,-0.939987,0.244976 --closure-method=direct --output=convert_diagram_38.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_38.txt "${sourcepath}"/output/convert_diagram_38.txt || exit $?
