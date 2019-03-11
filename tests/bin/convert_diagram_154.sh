#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.551166,-0.42353,0.718915 --closure-method=open --output=convert_diagram_154.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff convert_diagram_154.txt "${sourcepath}"/output/convert_diagram_154.txt || exit $?
