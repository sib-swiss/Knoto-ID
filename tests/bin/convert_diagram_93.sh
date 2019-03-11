#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.419683,0.875313,0.240196 --closure-method=open --output=convert_diagram_93.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_93.txt "${sourcepath}"/output/convert_diagram_93.txt || exit $?
