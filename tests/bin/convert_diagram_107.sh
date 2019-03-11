#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.863539,-0.439379,-0.24748 --closure-method=open --output=convert_diagram_107.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_107.txt "${sourcepath}"/output/convert_diagram_107.txt || exit $?
