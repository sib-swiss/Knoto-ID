#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.197985,-0.855446,-0.478553 --closure-method=direct --output=convert_diagram_99.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_99.txt "${sourcepath}"/output/convert_diagram_99.txt || exit $?
