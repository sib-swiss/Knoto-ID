#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=pd --planar --output-format=pd --output=convert_diagram_176.txt "${sourcepath}"/input/input_diagram_open_plane_1.txt || exit $? 
diff convert_diagram_176.txt "${sourcepath}"/output/convert_diagram_176.txt || exit $?
