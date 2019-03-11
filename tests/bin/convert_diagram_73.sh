#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.139171,-0.989965,0.0245207 --closure-method=open --output=convert_diagram_73.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_73.txt "${sourcepath}"/output/convert_diagram_73.txt || exit $?
