#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.404997,-0.853246,-0.328556 --closure-method=direct --output=convert_diagram_70.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_70.txt "${sourcepath}"/output/convert_diagram_70.txt || exit $?
