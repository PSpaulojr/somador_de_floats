/* Somador de Floats
 *
 * Este programa (baseado nos exemplos de maquinas de estados dados em aula) recebera 
 * uma serie de caracteres em sua entrada. Ao receber um caractere fim de linha ('\n'),
 * deve imprimir na tela a soma dos numeros inteiros e em ponto flutuante que recebeu e,
 * apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>

int pointer_alg = 0;    //Contador indice para mexer em string que guarda os algarismos dos numeros rastreados na entrada
float soma = 0;         //Contem a soma dos numeros rastreados na entrada

int eh_digito(char c){
    if( (c >='0') && (c<='9') ) return 1;
    else return 0;
}

int eh_espaco(char c){
    if( (c == ' ') || (c == '\t') ) return 1;
    else return 0;
}

int eh_ponto(char c){
    if( c == '.' ) return 1;
    else return 0;
}

int eh_eol(char c){
    if( (c == '\n') || (c == '\r') ) return 1;
    else return 0;
}

typedef struct{
  int estado_atual;
} mef, *Mef;  //Maquina de estados finitos

int opera_mef (void *essa_maquina, char entrada, char* algarismos){
 Mef maquina = (Mef) essa_maquina;
 
 switch (maquina->estado_atual){
   case 0:
     if (eh_digito(entrada)){
       maquina->estado_atual = 3;  //comecou numero, guarda numero
       algarismos[pointer_alg] = entrada;
       pointer_alg ++;
     }
     else if (eh_espaco(entrada)){
       maquina->estado_atual = 2;  //espaco, aguarda comecar numero
     }
     else {
       maquina->estado_atual = 1;  //nao comecou numero
     }
     break;

    case 1:
      if(eh_espaco(entrada)){
        maquina->estado_atual = 2; //espaco, aguarda comecar numero
      }
      else {
        maquina->estado_atual = 1; //nao comecou numero
      }
      break;

    case 2:
      if(eh_espaco(entrada)){
        maquina->estado_atual = 2; //espaco, aguarda comecar numero
      }
      else if (eh_digito(entrada)){
        maquina->estado_atual = 3; //era espaco agora comecou um numero, guarda numero
        algarismos[pointer_alg] = entrada;
        pointer_alg ++;
      }
      else maquina->estado_atual = 1; //Nao comecou nenhum numero
      break;

    case 3: 
      if(eh_digito(entrada)){
        maquina->estado_atual = 3; //guarda numero
        algarismos[pointer_alg] = entrada;
        pointer_alg ++;
      }
      else if(eh_eol(entrada) || eh_espaco(entrada)){ //Eh numero, guarda e comeca outro
        maquina->estado_atual = 4;
        algarismos[pointer_alg] = entrada;
        pointer_alg = 0;
        soma = soma + atof(algarismos);
      }
      else if(eh_ponto(entrada)){ //pode ser float, guarda numero
        maquina->estado_atual = 5;
        algarismos[pointer_alg] = entrada;
        pointer_alg ++;
      }
      else{
        maquina->estado_atual = 1; //nao Eh numero, joga fora
        for(int j=0; j<5; j++) algarismos[j] = '\0';
        pointer_alg = 0;
      }
      break;

    case 4:                    //Jah foi numero! Esperando comecar outro...
      if(eh_digito(entrada)){        
        maquina->estado_atual = 3;  //comeca numero, guarda numero
        algarismos[pointer_alg] = entrada;
        pointer_alg ++;
      }
      else if (eh_espaco(entrada)){ //nao Eh numero, volta espera por digito em 2
        maquina->estado_atual = 2;
      }
      else{
        maquina->estado_atual = 1; //nao Eh numero
      }
      break;

    case 5:
      if(eh_digito(entrada)){
        maquina->estado_atual = 6;  //Pode ser float (ex: 4.5*), espera em 6 para confirmar
        algarismos[pointer_alg] = entrada;
        pointer_alg ++;
      }
      else if( (eh_espaco(entrada)) ||  (eh_eol(entrada)) ){ //Nao Eh numero, joga fora e comeca outro (ex: 1.' ')
        maquina->estado_atual = 2;
        for(int j=0; j<5; j++) algarismos[j] = '\0';
        pointer_alg = 0;
      }
      else {
        maquina->estado_atual = 1; //nao Eh numero, joga fora
        for(int j=0; j<5; j++) algarismos[j] = '\0';
        pointer_alg = 0;
      }
      break;

    case 6:
      if(eh_digito(entrada)){
        maquina->estado_atual = 6; //pode ser float, aguarda em 6 (ex: 3.44*)
        algarismos[pointer_alg] = entrada;
        pointer_alg ++;
      }
      else if( (eh_espaco(entrada)) || (eh_eol(entrada)) ) {
        maquina->estado_atual = 4;  //Eh numero! Guarda e comeca outro
        algarismos[pointer_alg] = entrada; 
        soma = soma + atof(algarismos);
        pointer_alg = 0;
      }
      else{
        maquina->estado_atual = 1;  //Nao Eh numero! Joga fora e comeca outro (ex: 4.5A)
        for(int j=0; j<5; j++) algarismos[j] = '\0';
        pointer_alg = 0;
      }
  }
  return maquina->estado_atual;
}

int main() {
  char buffer_in[500];
  char c;
  int pointer;
  char numero[5];

  for (int i=0; i<100; i++) buffer_in[i] = '\0';
  for (int j=0; j<5; j++) numero[j] = '\0';

  pointer = 0;
  do {
    c=getchar();
    buffer_in[pointer++] = c;
  } while (c != '\n');

  mef maquina;
  maquina.estado_atual = 0;

  pointer = 0;
  
  while (buffer_in[pointer] != '\0') {
    opera_mef((void*) (&maquina), buffer_in[pointer], numero);
    if(maquina.estado_atual == 4){
      for(int j=0; j<5; j++) numero[j] = '\0';
    }
    pointer++;  
  }
    printf("%g\n", soma);
}