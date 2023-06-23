#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "hash.h"

/* nesse arquivo ira resultar em outroas arquivos, o de dados(so gerei esse), primario e secundario */

using namespace std;



vector<string> split(string s, string delimiter) {
    size_t pos_inicio = 0, pos_final, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_final = s.find(delimiter, pos_inicio)) != string::npos) {
        token = s.substr(pos_inicio, pos_final - pos_inicio);
        pos_inicio = pos_final + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_inicio));
    return res;
}


string remove_caractere(const string& s, char c) {
    string result;
    for (char currentChar : s) {
        if (currentChar != c) {
            result += currentChar;
        }
    }
    return result;
}


string substitui_ocorrencias(string s, const string& de, const string& para) {
    size_t pos_inicial = 0;
    while ((pos_inicial = s.find(de, pos_inicial)) != string::npos) {
        s.replace(pos_inicial, de.length(), para);
        pos_inicial += para.length();
    }
    return s;
}

string trunca_string(const string& s, size_t largura) {
    if (s.length() > largura) {
        return s.substr(0, largura - 3) + " ..";
    }
    return s;
}


// Funcão que gera arquivo de dados

void upload() {
    Hash myhash(1043937);
    ifstream infile("artigo.csv");
    string line;

    if (!infile) {
        cout << " - Arquivo não encontrado!" << endl;
        exit(1);
    }

    while (getline(infile, line)) {
        if (line.find(";;") != string::npos) {
            line = substitui_ocorrencias(line, ";;", ";\"\";");
        }

        if (line.find("NULL") != string::npos) {
            line = substitui_ocorrencias(line, "NULL", "\"NULL\"");
        }

        vector<string> lineS = split(line, "\";\"");

        for (string& s : lineS) {
            s = remove_caractere(s, '"');
        }

        int id = stoi(lineS[0]);
        string titulo = lineS[1];
        titulo = trunca_string(titulo, 300);
        int ano = stoi(lineS[2]);
        string autores = lineS[3];
        autores = trunca_string(autores, 150);
        int citacoes = stoi(lineS[4]);
        string atualizacao = lineS[5];
        string snipet = lineS[6];
        snipet = trunca_string(snipet, 100);

        Elemento* NovoElemento = new Elemento(id,titulo,ano,autores,citacoes,atualizacao,snipet);
        myhash.insere(NovoElemento->ID,NovoElemento); 
    }

    myhash.gera_arquivos();
    myhash.estatisticas();
}

int main() {
    cout << "Realizando a leitura do arquivo e inserindo registros na hash.." << endl;
    upload();
    cout << "Rodar o arquivo <findrec>! :)" << endl;
}