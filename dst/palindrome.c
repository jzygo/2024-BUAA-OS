#include <stdio.h>
int main() {
	int n;
	scanf("%d", &n);
	//判断输入的数字是否是回文数
	int m = n;
	int r = 0;
	while (m > 0) {
		r = r * 10 + m % 10;
		m = m / 10;
	}
	if (r==n) {
		printf("Y\n");
	} else {
		printf("N\n");
	}
	return 0;
}
