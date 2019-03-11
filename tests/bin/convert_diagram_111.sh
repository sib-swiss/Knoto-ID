#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.821829,0.419683,-0.385309 --closure-method=open --output=convert_diagram_111.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_111.txt "${sourcepath}"/output/convert_diagram_111.txt || exit $?
