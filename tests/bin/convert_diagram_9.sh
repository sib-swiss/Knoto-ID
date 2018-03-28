#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.604676,-0.555898,-0.57039 --closure-method=open --output=convert_diagram_9.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_9.txt "${sourcepath}"/output/convert_diagram_9.txt || exit $?
