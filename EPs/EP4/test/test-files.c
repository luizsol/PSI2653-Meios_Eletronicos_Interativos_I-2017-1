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
#include "../htmllib.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	if(argc < 3){
		return 1;
	}
	char newpath[400];
	newpath[0] = '\0';
	composepath((char *) argv[1],(char *) argv[2], newpath);
	//printf("%s\n", newpath);
	char output[4000];
	output[0] = '\0';
	generatedirhtml(newpath, output);
	printf("%s\n", output);
	return 0;
}