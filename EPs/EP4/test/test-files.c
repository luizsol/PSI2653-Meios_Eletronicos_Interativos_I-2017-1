/* -----------------------------------------------------------------------------
 * Title:       test-files
 * File:        test-files.c
 * Author:      Luiz Sol
 * Version:     0.0 (2017-05-27)
 * Date:        2017-05-27
 * Description: Testa as funções relacionada a arquivos
 * -----------------------------------------------------------------------------
 */

#include "../serverlib.h"

int main(int argc, char const *argv[])
{
	if(argc < 3){
		return 1;
	}
	char newpath[400];
	newpath[0] = '\0';
	composepath(argv[1], argv[2], newpath);
	printf("%s\n", newpath);
	return 0;
}