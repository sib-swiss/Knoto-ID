#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.3142,0.943728,0.103227 --closure-method=direct --output=convert_diagram_131.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_131.txt "${sourcepath}"/output/convert_diagram_131.txt || exit $?
