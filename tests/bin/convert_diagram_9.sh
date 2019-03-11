#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.654068,-0.331286,-0.680033 --closure-method=open --output=convert_diagram_9.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_9.txt "${sourcepath}"/output/convert_diagram_9.txt || exit $?
