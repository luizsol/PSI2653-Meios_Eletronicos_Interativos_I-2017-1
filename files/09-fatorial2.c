#include <stdio.h>

char versao[] = "2.1";
int n;
int resultado;

int fatorial (int x)
  {
  int y;
  if (x <= 1)
    return(1);
  else
    {
    y = x * fatorial(x-1);
    return (y);
    }
  }

int main()
  {
  printf("Programa fatorial, versao %s \n", versao);
  printf("Entre com o valor: " );
  scanf("%d",&n);
  resultado = fatorial(n);
  printf("Resultado: %d \n",resultado);
  }	

