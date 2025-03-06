#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

using namespace std;

// Variáveis globais
int num_threads = 4; // Número padrão de threads
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

int main(int argc, char* argv[]) {
    if (argc > 1) {
        num_threads = stoi(argv[1]); // Define o número de threads a partir do argumento
    }
    if (argc > 2) {
        expansion_factor = stoi(argv[2]); // Define o fator de expansão do texto
    }
    
    // Inicia a medição do tempo de preparação
    auto startPreparation = chrono::high_resolution_clock::now();
    
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

    // Divide o texto em partes para cada thread
    int textLength = text.size();
    int blockSize = textLength / num_threads;
    
    // Fim da medição do tempo de preparação
    auto endPreparation = chrono::high_resolution_clock::now();
    
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
    auto preparationTime = chrono::duration<double>(endPreparation - startPreparation).count();
    auto executionTime = chrono::duration<double>(endExecution - startExecution).count();
    auto totalTime = preparationTime + executionTime;
    
    // Exibir resultados
    cout << "Número de threads: " << num_threads << endl;
    cout << "Fator de expansão: " << expansion_factor << endl;
    cout << "Tamanho final do texto: " << textLength << " caracteres" << endl;
    cout << "Tamanho do bloco por thread: " << blockSize << " caracteres" << endl;
    cout << "Tempo de preparação: " << preparationTime << " segundos" << endl;
    cout << "Tempo de execução: " << executionTime << " segundos" << endl;
    cout << "Tempo total: " << totalTime << " segundos" << endl;
    cout << "Ocorrências de 'love': " << totalLove << endl;
    cout << "Ocorrências de 'hate': " << totalHate << endl;
    cout << "Palavra mais frequente: " << (totalLove > totalHate ? "love" : "hate") << endl;
    
    return 0;
}
