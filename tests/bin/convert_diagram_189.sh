#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=pd --output=convert_diagram_189.txt "${sourcepath}"/input/input_diagram_closed_1.txt || exit $? 
diff convert_diagram_189.txt "${sourcepath}"/output/convert_diagram_189.txt || exit $?
