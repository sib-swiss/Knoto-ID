#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.0642592,-0.162492,0.984615 --closure-method=direct --output=convert_diagram_20.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_20.txt "${sourcepath}"/output/convert_diagram_20.txt || exit $?
