#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <fstream>

using namespace std;

// Variáveis globais
int expansion_factor = 1; // Fator de multiplicação do texto
mutex mtx; // Mutex para sincronização das threads

// Função para contar ocorrências de uma palavra em um trecho do texto
int countOccurrences(const string& text, const string& word) {
    int count = 0;
    size_t pos = text.find(word, 0);
    while (pos != string::npos) {
        count++;
        pos = text.find(word, pos + word.length()); // Avança para a próxima ocorrência
    }
    return count;
}

// Função que cada thread irá executar
void threadTask(const string& text, int start, int end, int& countLove, int& countHate) {
    string subText = text.substr(start, end - start); // Extrai o trecho do texto
    int localLove = countOccurrences(subText, "love");
    int localHate = countOccurrences(subText, "hate");
    
    // Bloqueia o acesso às variáveis globais para evitar condições de corrida
    lock_guard<mutex> lock(mtx);
    countLove += localLove;
    countHate += localHate;
}

int main() {
    // Carregar o arquivo de texto
    ifstream file("shakespeare.txt");
    if (!file) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }
    
    stringstream buffer;
    buffer << file.rdbuf(); // Lê todo o conteúdo do arquivo
    string original_text = buffer.str();
    file.close();

    // Aumentar o volume de dados multiplicando o texto
    string text;
    for (int i = 0; i < expansion_factor; i++) {
        text += original_text;
    }

    // Variáveis para armazenar os resultados
    ofstream resultFile("execution_times.csv");
    resultFile << "NumThreads,TotalTime,SearchTime\n"; // Cabeçalho do arquivo CSV

    // Executa o programa com diferentes números de threads
    for (int num_threads = 1; num_threads <= 20; num_threads++) {
        // Divide o texto em partes para cada thread
        int textLength = text.size();
        int blockSize = textLength / num_threads;

        // Medição do tempo de preparação
        auto startPreparation = chrono::high_resolution_clock::now();
        
        // Inicia a medição do tempo de execução
        auto startExecution = chrono::high_resolution_clock::now();

        // Contadores globais para as palavras
        int totalLove = 0, totalHate = 0;
        vector<thread> threads;
        
        // Criar e iniciar as threads
        for (int i = 0; i < num_threads; i++) {
            int start = i * blockSize;
            int end = (i == num_threads - 1) ? textLength : (i + 1) * blockSize;
            threads.emplace_back(threadTask, cref(text), start, end, ref(totalLove), ref(totalHate));
        }
        
        // Aguarda todas as threads terminarem
        for (auto& t : threads) {
            t.join();
        }

        // Fim da medição do tempo de execução
        auto endExecution = chrono::high_resolution_clock::now();

        // Cálculo dos tempos
        auto preparationTime = chrono::duration<double>(startExecution - startPreparation).count();
        auto executionTime = chrono::duration<double>(endExecution - startExecution).count();
        auto totalTime = preparationTime + executionTime;

        // Escrever os tempos no arquivo CSV
        resultFile << num_threads << "," << totalTime << "," << executionTime << "\n";
        
        // Exibir resultados
        cout << "Número de threads: " << num_threads << endl;
        cout << "Tempo total: " << totalTime << " segundos" << endl;
        cout << "Tempo de execução (pesquisa): " << executionTime << " segundos" << endl;
    }

    resultFile.close();
    return 0;
}
