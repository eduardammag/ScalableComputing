#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/wait.h>

std::mutex mtx; // Mutex para evitar conflitos de escrita ao usar threads

// Função para verificar se um número é primo
bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

// Função para encontrar primos em um intervalo (para threads)
void find_primes_thread(int start, int end, std::vector<int>& primes) {
    std::vector<int> local_primes;
    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
            local_primes.push_back(i);
        }
    }
    
    // Adiciona os primos ao vetor global com proteção de mutex
    std::lock_guard<std::mutex> lock(mtx);
    primes.insert(primes.end(), local_primes.begin(), local_primes.end());
}

// Função para encontrar primos em um intervalo (para processos)
void find_primes_process(int start, int end) {
    std::cout << "Processo " << getpid() << " calculando primos entre " << start << " e " << end << std::endl;
    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    int start = 1, end = 100, num_units = 4;
    std::vector<int> primes;

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " [thread/process]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    int range = (end - start + 1) / num_units;

    if (mode == "thread") {
        std::vector<std::thread> threads;

        // Criando threads
        for (int i = 0; i < num_units; ++i) {
            int s = start + i * range;
            int e = (i == num_units - 1) ? end : s + range - 1;
            threads.emplace_back(find_primes_thread, s, e, std::ref(primes));
        }

        // Esperando as threads terminarem
        for (auto& t : threads) {
            t.join();
        }

        // Exibe os primos encontrados
        std::cout << "Números primos encontrados: ";
        for (int p : primes) std::cout << p << " ";
        std::cout << std::endl;
    } 
    else if (mode == "process") {
        for (int i = 0; i < num_units; ++i) {
            pid_t pid = fork();
            if (pid == 0) { // Código do processo filho
                int s = start + i * range;
                int e = (i == num_units - 1) ? end : s + range - 1;
                find_primes_process(s, e);
                return 0; // Finaliza o processo filho
            }
        }

        // Processo pai espera os filhos terminarem
        for (int i = 0; i < num_units; ++i) {
            wait(NULL);
        }
    } 
    else {
        std::cerr << "Modo inválido! Use 'thread' ou 'process'." << std::endl;
        return 1;
    }

    return 0;
}
