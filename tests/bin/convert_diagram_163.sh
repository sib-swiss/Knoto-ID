#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=gauss --output=convert_diagram_163.txt "${sourcepath}"/input/input_diagram_open_sphere_0.txt || exit $? 
diff convert_diagram_163.txt "${sourcepath}"/output/convert_diagram_163.txt || exit $?
