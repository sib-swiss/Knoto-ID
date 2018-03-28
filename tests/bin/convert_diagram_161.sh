#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=pd --output=convert_diagram_161.txt "${sourcepath}"/input/input_diagram_open_sphere_0.txt || exit $? 
diff convert_diagram_161.txt "${sourcepath}"/output/convert_diagram_161.txt || exit $?
