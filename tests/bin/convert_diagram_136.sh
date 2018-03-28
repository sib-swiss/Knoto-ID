#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.508983,-0.730936,0.454608 --closure-method=open --output=convert_diagram_136.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_136.txt "${sourcepath}"/output/convert_diagram_136.txt || exit $?
