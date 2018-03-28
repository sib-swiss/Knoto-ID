#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.857434,0.172836,0.4847 --closure-method=open --output=convert_diagram_59.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_59.txt "${sourcepath}"/output/convert_diagram_59.txt || exit $?
