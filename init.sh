#!/bin/bash

find . -mindepth 1 \
! -name 'exam.sh' \
! -name 'init.sh' \
! -name 'Makefile' \
! -path './.git*' \
! -path './.gitignore' \
-exec rm -rf {} +

cat >hello.c <<'EOF'
#include <stdio.h>
int main() {
	printf("this is hello\n");
	return 0;
}
EOF


mkdir code

echo "#include <stdio.h>" >> code/0.c

for ((i=1; i<=15; i++))
do
  echo "void output$i();" >> code/0.c
done

echo "int main() {" >> code/0.c

random_numbers=($(seq 1 15 | shuf))

for random_number in "${random_numbers[@]}"
do
  echo "  output$random_number();" >> code/0.c
done

echo "  return 0;" >> code/0.c
echo "}" >> code/0.c

for ((i=1; i<=15; i++))
do
  cat >code/$i.c <<EOF
#include <stdio.h>

void output$i() {
  fprintf(stderr, "Hello World%d\\n", $i);
}
EOF
done
