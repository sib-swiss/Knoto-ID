#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.483603,-0.609685,0.628023 --closure-method=open --output=convert_diagram_15.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_15.txt "${sourcepath}"/output/convert_diagram_15.txt || exit $?
