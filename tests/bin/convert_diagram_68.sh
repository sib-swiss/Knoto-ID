#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.022856,0.999725,-0.00533058 --closure-method=direct --output=convert_diagram_68.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_68.txt "${sourcepath}"/output/convert_diagram_68.txt || exit $?
