#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.228836,-0.9521,-0.202831 --closure-method=open --output=convert_diagram_95.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_95.txt "${sourcepath}"/output/convert_diagram_95.txt || exit $?
