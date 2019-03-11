#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.553872,-0.795435,-0.245985 --closure-method=direct --output=convert_diagram_3.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_3.txt "${sourcepath}"/output/convert_diagram_3.txt || exit $?
