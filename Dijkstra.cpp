
//Dijkstra.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INFINITO 999


//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ESTRUTURAS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================
typedef struct BAIRRO
{
    char nome[50];
    int referencia;
    char status; //V - encontrado, F - não econtrado
}BAIRRO;

typedef struct CAMINHO
{
    int origem;
    int destino;
    float distancia;
}CAMINHO;



//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ASSINATURA DAS FUNÇÕES <<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================
void lerArquivoMatriz();
void lerArquivoBairro();
void imprimirBairro();
void imprimirMatriz();
void verificarDistancia();
int round(float number);
void dijkstra(float **matriz, int origem, int destino);
float valorTotal(float distancia);


//VAR GLOBAIS
BAIRRO *bairros;
CAMINHO *caminhos;
float  **matriz;
int dimensaoMatriz = 0;
int bandeira = 0;

//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FUNÇÃO PRINCIPAL <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

int _tmain(int argc, _TCHAR* argv[])
{	
	int origem = 0, destino = 0;
	char ch;

	lerArquivoBairro();
    lerArquivoMatriz();
	//imprimirMatriz();

	do{
		system("CLS");
		imprimirBairro();
		
		printf("\n\nInforme os dados:\n");
		printf(">Ponto de origem: "); scanf("%d", &origem);
		printf(">Destino: "); scanf("%d", &destino);
		printf(">Bandeira (1/2): "); scanf("%d", &bandeira);

		dijkstra(matriz, origem, destino);

		fflush(stdin);
		printf("\nRefazer (s/n)\n>");scanf("%c", &ch);
		fflush(stdin);

	}while(ch != 'n');


	return 0;
}

//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DIJKSTRA <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

void dijkstra(float **matriz, int origem, int destino)
{
    float *distancia = NULL; 
	int *caminhoEncontrado = NULL;
	int	*caminho = NULL;
    int verticeAtual;
	int i, verticeAtualHelper;
	float distanciaHelper, menorDistancia, novaDistancia;
	BAIRRO *bairrosHelper;//novo(ajuda a imprimir os elementos em ordem)

	bairrosHelper = (BAIRRO*)malloc(sizeof(BAIRRO)*dimensaoMatriz);
	distancia = (float*)malloc((dimensaoMatriz)*sizeof(int));
	caminhoEncontrado = (int*)malloc((dimensaoMatriz)*sizeof(int));
	caminho = (int*)malloc((dimensaoMatriz)*sizeof(int));

	if((distancia == NULL)||(caminhoEncontrado == NULL)||(caminho == NULL))
	{
	   printf("\n ERRO!! (alocacao de vetor) \n"); 
	   exit(1);                                                                                                                                                                 
	}

	//INICIALIZAÇÃO DOS VETORES
    for (i = 0; i < (dimensaoMatriz); i++)
	{
        caminhoEncontrado[i] = 0; 
        distancia[i] = INFINITO; //a distância não conhecida
    }

    //INICIALIZAÇÃO PELA ORIGEM
    caminhoEncontrado[origem] = 1;
    distancia[origem] = 0;

    verticeAtual = origem;
    verticeAtualHelper = verticeAtual;

	//MOSTRANDO O PROCESSO(MATRIZ DO CAMIINHO MINIMO)
	printf("\n");
	for(int t = 0; t < dimensaoMatriz; t++){
		printf("| %.2f ", distancia[t]);
	}

    while (verticeAtual != destino)
	{
        menorDistancia = INFINITO; //inicia com infinito ou seja sem caminho definido
        distanciaHelper = distancia[verticeAtual];//guardandmatro a distancia do vertice atual

        for (i = 0; i < (dimensaoMatriz); i++)
		{
          
            if (caminhoEncontrado[i] == 0)
			{ 
                
                novaDistancia =  distanciaHelper + matriz[verticeAtual][i];//distancia presente na matriz.

                //SE A DISTANCIA AVALIADA FOR MENOR ATUALIZA O CAMINHO
				//caso a distancia seja 999 ou superior não entra por que ja indica que não a caminho ou seja marcado como infinito
                if (novaDistancia < distancia[i])
				{
                    distancia[i] = novaDistancia;
                    caminho[i] = verticeAtual;
                }

                //determina o vértice (entre todos que não pertentecem ao caminho encontrado) com menor distância
                if (distancia[i] < menorDistancia)
				{
                    menorDistancia = distancia[i];
                    verticeAtualHelper = i;//serve tambem pra dizer mais a frente no código que o menor caminho para tal vertice foi encontrado
                }
            }
        }

		//MOSTRANDO O PROCESSO(MATRIZ DO CAMIINHO MINIMO)
		printf("\n");
		for(int t = 0; t < dimensaoMatriz; t++){
			printf("| %.2f ", distancia[t]);
		}


		//CASO NÃO HAJA UM CAMINHO
        if (verticeAtual == verticeAtualHelper)
		{
            printf("\n\nCAMINHO NAO EXISTE\n\n");
            return;
        }
        verticeAtual = verticeAtualHelper;
        caminhoEncontrado[verticeAtual] = 1;
    } 

    //RESULTADO DO PROCESSO
	int br = 0;//br(bairros helper indice)
    printf("\n\n\nRESULTADO: ");
    int percurso = destino;
	
	strcpy(bairrosHelper[br].nome, bairros[destino].nome); 
	bairrosHelper[br].referencia = bairros[destino].referencia;

	//printf(" %s  <- ", bairros[destino].nome);
    
	//RETOMANDO O PERCURSO
    while (percurso != origem)
	{
		br++;
		strcpy(bairrosHelper[br].nome, bairros[caminho[percurso]].nome);
		bairrosHelper[br].referencia = bairros[caminho[percurso]].referencia;
        percurso = caminho[percurso];
    } 


	int h = 0;
	float soma = 0.00;
	for(h = br; h >= 0; h--){
		printf("=> %s",bairrosHelper[h].nome);

		//somando o total do menor caminho
		if(h != 0) soma += matriz[bairrosHelper[h].referencia][bairrosHelper[h-1].referencia];

	}

	printf("\nValor da distancia = %.2f", soma);
	printf("\nValor a ser pago = %.2f", valorTotal(soma));
    
	
	free(distancia);free(caminhoEncontrado);free(caminho);
}



//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LER ARQUIVO MATRIZ <<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

void lerArquivoMatriz()
{
    float distancia;
    FILE *fp;
	char caractere;
	char linha[70];
	int i = 0, j = 0;

	matriz = (float**)malloc( (dimensaoMatriz)*sizeof(float*) );
	for(i = 0 ;i < dimensaoMatriz ; i++)
	{
        matriz[i]=(float*)malloc((dimensaoMatriz)*sizeof(float));
	}


    fp = fopen("testeDistancias.txt", "r");

	for(i = 0 ;i < dimensaoMatriz; i++)
	{
		for(j = 0 ;j < dimensaoMatriz ; j++)
		{
			fscanf(fp,"%f", &distancia);
			if (distancia == 0)
                distancia = 999.00;
			matriz[i][j] = distancia;
			fgetc(fp);
		}
	}

	fclose (fp);
}

//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> IMPRIMIR MATRIZ  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

void imprimirMatriz(){
    int i = 0, j = 0;

    for(i; i < dimensaoMatriz; i++){
        for(j; j < dimensaoMatriz; j++){
            printf("%.2f ", matriz[i][j]);
        }
        j = 0;
        printf("\n");
    }
}

//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LER ARQUIVO BAIRRO <<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

void lerArquivoBairro(){

    FILE *fp;
    char nome[50];
    int indice = 0;
    int quantLinha = 0;

    fp = fopen("testeBairros.txt", "r");

    while(!feof(fp))
	{
		fgets(nome, 50, fp);
		quantLinha++;
	}

	fclose (fp);

	dimensaoMatriz = quantLinha;

    bairros = (BAIRRO*) malloc(sizeof(BAIRRO)*(dimensaoMatriz));

    fp = fopen("testeBairros.txt", "r");

	while(!feof(fp))
	{
		fgets(nome, 50, fp);

		//retirando o '\n'
		for(int c = 0; c < strlen(nome); c++){
            if(nome[c] == '\n')
                nome[c] = '\0';
		}

		strcpy(bairros[indice].nome, nome);
		bairros[indice].referencia = indice;
		bairros[indice].status = 'F';
		indice++;
	}

	fclose (fp);

}

//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> IMPRIMIR BAIRRO <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

void imprimirBairro(){
    int i = 0;

    for(i; i < dimensaoMatriz; i++)
    {
        if(i % 3 == 0){
            printf("\n");
            printf("%d - %s ", i, bairros[i].nome);
        }
        else
            printf("                       %d - %s ", i, bairros[i].nome);

    }
}

//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> VERIFICAR DISTANCIA <<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================

void verificarDistancia(){
    int origem = 0, destino = 0;

    imprimirBairro();
    printf("\n\nOrigem: ");scanf("%d", &origem);
    printf("Destino: ");scanf("%d", &destino);

    printf("\n\nresp: %.2f", matriz[origem][destino]);
}


int round(float number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}


//===================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> VALOR TOTAL  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//===================================================================================
float valorTotal(float distancia){

	if(bandeira == 1)
		return ((distancia*1.25)+4.5);

	return ((distancia*2.75)+6.5);
	
}




















