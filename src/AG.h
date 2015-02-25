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

/* Header que contem a definição de todos os tipos de dados utilizados */


#ifndef	AG_H
#define AG_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iostream>

using namespace std;

//struct correspondente ao gene
typedef struct Cidade{
	int x,y,ID;
} Cidade;

//namespace para armazenar as cidades lidas
namespace PCV{
	vector<Cidade> cidades;

    void addCidade(Cidade _cidade) {
		cidades.push_back(_cidade);
    }    
    
    Cidade getCidade(int _indice){
		return cidades.at(_indice);
    }
    
    int numCidades(){
		return cidades.size();
    }
};

//struct correspondente ao cromossomo
typedef struct Viagem{
	vector<Cidade> viagem;
	double distancia;
	double fitness;

	Viagem(){
		distancia = 0.0;
		fitness = 0.0;
		Cidade vazio;
		vazio.x = -1; vazio.y=-1; vazio.ID=-1;
		for(int i=0;i<PCV::numCidades();i++){
			viagem.push_back(vazio);
		}
	}

	int size(){
		return viagem.size();
	}

	void add(int _indice, Cidade _cidade){
		viagem[_indice] = _cidade;
	}

	Cidade getCidade(int _indice){
		return viagem[_indice];
	}

	bool contains(Cidade _cidade){
		bool retorno = false;
		for (vector<Cidade>::iterator it = viagem.begin() ; it != viagem.end(); ++it){
			if (it->ID == _cidade.ID)
				return true;
		}
		return retorno; 
	}

	double calculaDistancia(Cidade _origem, Cidade _destino){
		return sqrt(pow((_origem.x - _destino.x),2) + pow((_origem.y - _destino.y),2));
	}

	void geraIndividuo(){
		for (int i = 0; i < PCV::numCidades(); i++) {
			viagem[i] = PCV::getCidade(i);
		}
        // Reordena a viagem de modo aleatório
        random_shuffle(viagem.begin(), viagem.end());
	}

	double getFitness() {
        if (fitness == 0.0) {
            fitness = 1/getDistancia();
        }
        return fitness;
    }

	double getDistancia(){
        if (distancia == 0.0) {
            for (int i=0; i < viagem.size(); i++) {
				Cidade origem = viagem.at(i);
                Cidade destino;
                //Verifica se não é a última viagem
                if(i+1 < viagem.size()){
                    destino = viagem.at(i+1);
                }
                else{
                    destino = viagem.at(0);
                }
                //Adiciona distância da cidade de origem ao destino
                distancia += calculaDistancia(origem, destino);
            }
        }
        return distancia;
	}

	void print(){
		for(int i=0;i<size();i++){
			cout<<viagem[i].ID<<" ";
		}
		cout<<endl;
	}
} Viagem;

//struct correspondente a população do algoritmo genético
typedef struct Populacao{
	Viagem *viagens;
	int tamanho;

	Populacao(int _tamanho, bool _preencher){
		viagens = new Viagem[_tamanho];
		tamanho = _tamanho;

		if(_preencher){
			for (int i = 0; i < _tamanho; i++) {
				Viagem v;
				v.geraIndividuo();
				salvaViagem(i, v);
			}
		}
	}

	void salvaViagem(int _indice, Viagem _viagem){
		viagens[_indice] = _viagem;
	}

	Viagem getViagem(int _indice){
		return viagens[_indice];
	}

	Viagem getFittest() {
        Viagem fittest = viagens[0];
        
        for (int i = 1; i<tamanho; i++) {
            if (fittest.getFitness() <= viagens[i].getFitness()) {
                fittest = viagens[i];
            }
        }
        return fittest;
    }

} Populacao;

//struct que representa o algoritmo genético em si
typedef struct GA{
	double taxaMutacao;
	int tamPopTorneio;
	bool elitismo;

	GA(double _taxa, int _torneio, bool _elitismo){
		taxaMutacao = _taxa;
		tamPopTorneio = _torneio;
		elitismo = _elitismo;
		srand (time(NULL)); //inicializa random seed
	}

	Populacao evoluiPopulacao(Populacao pop){
		Populacao nova(pop.tamanho, 0);

		int offsetElitismo = 0;

		if(elitismo){
			nova.salvaViagem(0,pop.getFittest());
			offsetElitismo = 1;
		}

		//Cruza populacao
		for(int i = offsetElitismo;i<nova.tamanho;i++){
			Viagem pai = selecaoTorneio(pop);
			Viagem mae = selecaoTorneio(pop);

			Viagem filho = crossover(pai, mae);
			nova.salvaViagem(i,filho);
		}

		//Muta a nova população
		for (int i = offsetElitismo; i < nova.tamanho; i++) {
            mutacao(nova.getViagem(i));
        }

		return nova;
	}

	Viagem selecaoTorneio(Populacao pop){
		Populacao torneio(tamPopTorneio,0);
		
		for(int i=0;i<tamPopTorneio;i++){
			int randomID = rand() % pop.tamanho;
			torneio.salvaViagem(i,pop.getViagem(randomID));
		}

		return torneio.getFittest();
	}

	Viagem crossover(Viagem pai, Viagem mae){
		Viagem filho;

		int inicio = rand() % pai.size();
		int fim = rand() % pai.size();

		//preenche o filho com os valores do pai, de acordo com os intervalores inicio e fim
		for(int i = 0;i<filho.size();i++){
			if (inicio < fim && i > inicio && i < fim) {
                filho.add(i, pai.getCidade(i));
            }
            else if (inicio > fim) {
                if (!(i < inicio && i > fim)) {
                    filho.add(i, pai.getCidade(i));
                }
            }
		}

		//preenche o restante das cidades com os valores da mãe
        for (int i = 0; i < mae.size(); i++) {
            if (!filho.contains(mae.getCidade(i))) {
                for (int j = 0; j < filho.size(); j++) {
                    if (filho.getCidade(j).ID == -1) {
                        filho.add(j, mae.getCidade(i));
                        break;
                    }
                }
            }
        }

		return filho;
	}

	void mutacao(Viagem &_viagem) {
        for(int i=0; i < _viagem.size(); i++){
			double random = rand() * 0.00001;
			if(random < taxaMutacao){
                int j = rand() % _viagem.size();

                Cidade cidade1 = _viagem.getCidade(i);
                Cidade cidade2 = _viagem.getCidade(j);

                // Troca
                _viagem.add(j, cidade1);
                _viagem.add(i, cidade2);
            }
        }
    }
}GA;

#endif