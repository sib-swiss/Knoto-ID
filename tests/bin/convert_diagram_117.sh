#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.508222,0.158195,-0.846572 --closure-method=direct --output=convert_diagram_117.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_117.txt "${sourcepath}"/output/convert_diagram_117.txt || exit $?
