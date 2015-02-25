/**
* ----------------------------------------------------------------
* Pontificia Universidade Catolica de Minas Gerais 
* Curso de Ciencia da Computacao 
* Inteligência Artificial
* 
* Trabalho Pratico - PCV_AG
* 
*
* Objetivo:
* Resolução do Problema do Caixeiro Viajante utilizando Algoritmo Genético
*   
* @author Alyson Deives Pereira		Matricula: 416589
* @version 1.0 23/11/2013            
*
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <time.h>
#include <limits.h>
#include "AG.h"

using namespace std;

void main(int argc, char ** argv){
	ifstream leitor ("entrada.txt");    	//Stream do arquivo de leitura
	//ofstream escritor ("saida.txt");		//Stream do arquivo de escrita
	int cidades, tamPopInicial, tamPopTorneio, numIter;
	double txMutacao;
	bool elitismo;

	//Seta parametros
	if(argc < 3){
		txMutacao = 0.015;
		tamPopTorneio = 5;
		tamPopInicial = 300;
		elitismo = true;
		numIter = 100;
	}
	else{
		txMutacao = atof(argv[1]);
		tamPopInicial = atoi(argv[2]);
		tamPopTorneio = atoi(argv[3]);
		numIter = atoi(argv[4]);
		elitismo = (bool) atoi(argv[5]);
	}

	leitor>>cidades;

	//Realiza a leitura do arquivo
    for(int i = 0;i<cidades;i++){
        Cidade cidade;
		leitor >> cidade.x;
        leitor >> cidade.y;
		cidade.ID = i;
        PCV::addCidade(cidade);
    }
    leitor.close();//Fecha stream de leitura

	Populacao pop(tamPopInicial,1); //Gera população inicial
	GA ga(txMutacao, tamPopTorneio, elitismo); //inicializa algoritmo genético 

	cout<<"Distancia Inicial: "<<pop.getFittest().getDistancia() <<endl;
	pop.getFittest().print(); //imprime melhor cromossomo da população

	pop = ga.evoluiPopulacao(pop);
    for (int i = 0; i < numIter; i++) {
        pop = ga.evoluiPopulacao(pop);
    }

	//Imprime resultado
    cout<<"Completado"<<endl;
    cout<<"Distancia Final: "<< pop.getFittest().getDistancia() << endl;
	pop.getFittest().print();

	cout<<endl<<"Pressione qualquer tecla para sair."<<endl;
	getchar();
}