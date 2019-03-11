#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.359299,-0.896315,-0.259853 --closure-method=open --output=convert_diagram_44.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_44.txt "${sourcepath}"/output/convert_diagram_44.txt || exit $?
