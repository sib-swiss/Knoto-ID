#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.341081,-0.461682,-0.818849 --closure-method=open --output=convert_diagram_14.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_14.txt "${sourcepath}"/output/convert_diagram_14.txt || exit $?
