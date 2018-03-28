#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.686761,-0.519446,0.508463 --closure-method=open --output=convert_diagram_139.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_139.txt "${sourcepath}"/output/convert_diagram_139.txt || exit $?
