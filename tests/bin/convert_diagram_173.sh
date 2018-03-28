#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --planar --output-format=pd --output=convert_diagram_173.txt "${sourcepath}"/input/input_diagram_open_plane_0.txt || exit $? 
diff convert_diagram_173.txt "${sourcepath}"/output/convert_diagram_173.txt || exit $?
