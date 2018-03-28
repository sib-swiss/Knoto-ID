#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.291787,0.139322,0.946282 --closure-method=open --output=convert_diagram_140.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_140.txt "${sourcepath}"/output/convert_diagram_140.txt || exit $?
