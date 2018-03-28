#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.176934,-0.974973,0.134621 --closure-method=open --output=convert_diagram_62.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_62.txt "${sourcepath}"/output/convert_diagram_62.txt || exit $?
