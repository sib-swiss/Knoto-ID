#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.398636,0.463122,0.791585 --closure-method=direct --output=convert_diagram_64.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_64.txt "${sourcepath}"/output/convert_diagram_64.txt || exit $?
