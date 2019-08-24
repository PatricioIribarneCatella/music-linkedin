/* *****************************
 *            ITOA
 * ****************************/

// Convierte un entero a string.
// Fuente: http://newbiegeek.com/convertir-un-entero-a-cadena-de-caracteres-en-c/
char* itoa(int val, int base) {

	char* zero = "0";
	
	if (val == 0)
		return zero;

	static char buf[32] = {0};
	int i = 30;
	
	for (; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}


