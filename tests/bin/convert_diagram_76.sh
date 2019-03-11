#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.502272,-0.388962,-0.77229 --closure-method=open --output=convert_diagram_76.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_76.txt "${sourcepath}"/output/convert_diagram_76.txt || exit $?
