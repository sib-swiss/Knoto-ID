#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.888451,0.389826,-0.242262 --closure-method=direct --output=convert_diagram_48.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_48.txt "${sourcepath}"/output/convert_diagram_48.txt || exit $?
