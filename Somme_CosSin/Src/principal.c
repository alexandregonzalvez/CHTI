#include "gassp72.h"

float somme(int index);

int main(void)
{
	
	float min = 10.0;
	float max = 0.0;
	
	for (int i = 0; i < 63; i++) {
		float s = somme(i);
		if (s > max) {
			max = s;
		} else if (s < min) {
			min = s;
		}
	}	
	
	while	(1)
		{
		}
}
