#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.0568189,0.811658,-0.581364 --closure-method=open --output=convert_diagram_108.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_108.txt "${sourcepath}"/output/convert_diagram_108.txt || exit $?
