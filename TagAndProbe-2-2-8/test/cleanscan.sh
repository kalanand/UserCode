sed 's/\*//g' $1 | egrep -v R | egrep -v n | sed '/^$/d' | awk '{print $2" "$3" "$4" "$5}' > $1.sed
