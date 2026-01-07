#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <ctime>
using namespace std;

/**
 * 1. ВИКОРИСТАННЯ ENUM
 * Визначаємо стани нашої системи.
 * Це набагато безпечніше, ніж використовувати #define STATE_IDLE 0
 */
enum class SystemState {
    EROR,
    IDLE,
    START,
    READ,
    DONE
};


const int MAX_SAMPLES = 5;
const int STEP_DELAY_MS = 800;


#define STATUS_OK      0x01
#define STATUS_BUSY    0x02

 // Функція-заглушка для імітації роботи заліза
static void process_delay(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

int main() {
    srand(std::time(nullptr));
    SetConsoleOutputCP(1251);
    SystemState currentState = SystemState::IDLE;
    bool isRunning = true;
    int samplesCollected = 0;

    cout << "--- Запуск системного контролера ---" << endl;
    int data;
    while (isRunning) {
        switch (currentState) {
        case SystemState::IDLE:
            cout << "[IDLE] Система в очікуванні. Готовність: " << (STATUS_OK ? "OK" : "NO") << endl;
            process_delay(STEP_DELAY_MS);
            currentState = SystemState::START;
            break;

        case SystemState::START:
            cout << "[START] Ініціалізація периферії..." << endl;
            process_delay(STEP_DELAY_MS);
            currentState = SystemState::READ;
            break;

        case SystemState::READ:
            if (samplesCollected < MAX_SAMPLES) {
                samplesCollected++;
                cout << "[READ] Збір даних... Семпл " << samplesCollected << "/" << MAX_SAMPLES << endl;

                // Генерація випадкового значення (84 - 93)
                data = 84 + (rand() % 10);
                cout << "   >>> Отримане значення: " << data << endl;

                process_delay(STEP_DELAY_MS / 2);

                if (data > 90) {
                    currentState = SystemState::EROR;
                    // Зменшуємо лічильник, щоб перезібрати цей семпл після скидання
                    samplesCollected--;
                }
            }
            else {
                currentState = SystemState::DONE;
            }
            break;

        case SystemState::DONE:
            cout << "[DONE] Обробка завершена успішно." << endl;
            isRunning = false; // Вихід з циклу для завершення симуляції
            break;

        case SystemState::EROR:
            cout << "[EROR] Скидання системи..." << endl;
            currentState = SystemState::IDLE;
            break;
        default:
            cout << "[CRITICAL] Невідомий стан!" << endl;
            isRunning = false;
            break;
        }
    }

    cout << "--- Моніторинг завершено ---" << endl;
    return 0;
}


