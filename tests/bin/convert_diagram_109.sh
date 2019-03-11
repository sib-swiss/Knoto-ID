#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.0207494,0.682111,-0.730954 --closure-method=open --output=convert_diagram_109.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_109.txt "${sourcepath}"/output/convert_diagram_109.txt || exit $?
