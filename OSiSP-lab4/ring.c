#include "ring.h"

// Функция для добавления нового сообщения в очередь.
void push(Queue* queue) {
    if (queue->currentPlaceToWrite > END_PLACE_TO_WRITE) // Если текущее место для записи превышает конечное место для записи в кольцевом буфере.
        queue->currentPlaceToWrite = START_PLACE_TO_WRITE; // Устанавливаем текущее место для записи в начало кольцевого буфера.
    Node *temp = (Node *)((uintptr_t)queue + queue->currentPlaceToWrite); // Создание указателя на узел в кольцевом буфере.
    temp->message = ((uintptr_t)queue + queue->currentPlaceToWrite + sizeof(Node)); // Установка указателя на сообщение внутри узла.
    initMessage((Message*)temp->message); // Инициализация сообщения.
    if (queue->ringHead != 0) { // Если очередь не пуста.
        temp->next = queue->ringHead; // Устанавливаем указатель на следующий узел.
        temp->prev = queue->ringTail; // Устанавливаем указатель на предыдущий узел.
        ((Node*)(queue->ringHead+(uintptr_t)queue))->prev = (uintptr_t)temp - (uintptr_t)queue; // Обновляем указатель на предыдущий узел в текущем головном узле.
        ((Node*)(queue->ringTail+(uintptr_t)queue))->next = (uintptr_t)temp - (uintptr_t)queue; // Обновляем указатель на следующий узел в текущем хвостовом узле.
        queue->ringTail = (uintptr_t)temp - (uintptr_t)queue; // Устанавливаем указатель на хвостовой узел.
    } else { // Если очередь пуста.
        queue->ringHead = (uintptr_t)temp - (uintptr_t)queue; // Устанавливаем указатель на головной и хвостовой узлы на созданный узел.
        queue->ringTail = (uintptr_t)temp - (uintptr_t)queue;
        temp->next = (uintptr_t)temp - (uintptr_t)queue; // Устанавливаем указатели на самого себя.
        temp->prev = (uintptr_t)temp - (uintptr_t)queue;
    }
    queue->currentPlaceToWrite += STEP_TO_WRITE; // Перемещаем текущее место для записи на следующий шаг.
}

// Функция для удаления сообщения из очереди.
void pop(Queue* queue) {
    if (queue->ringHead != 0) { // Если очередь не пуста.
        if (queue->ringHead != queue->ringTail) { // Если в очереди больше одного элемента.
            Node *temp = (Node *) (queue->ringHead + (uintptr_t) queue); // Создание указателя на головной узел.
            ((Node *) (queue->ringTail + (uintptr_t) queue))->next = temp->next; // Обновление указателя на следующий узел в хвостовом узле.
            ((Node *) (temp->next + (uintptr_t) queue))->prev = queue->ringTail; // Обновление указателя на предыдущий узел в следующем узле.
            queue->ringHead = temp->next; // Установка нового головного узла.
        } else { // Если в очереди остался только один элемент.
            queue->ringHead = 0; // Установка указателей на головной и хвостовой узлы в ноль.
            queue->ringTail = 0;
        }
    }
}

// Функция для инициализации сообщения.
void initMessage(Message* message) {
    message->type = 0; // Устанавливаем тип сообщения.
    message->hash = 0; // Устанавливаем начальное значение хэша.
    message->size = rand() % 257; // Устанавливаем случайный размер сообщения от 0 до 256.
    for (size_t i = 0; i < message->size; i++) { // Цикл по всем элементам данных сообщения.
        message->data[i] = rand() % 256; // Генерация случайного значения данных.
        message->hash += message->data[i]; // Обновление хэша.
    }
    message->hash = ~message->hash; // Инвертирование хэша.
}

// Функция для печати сообщения.
void printMessage(Message* mes) {
    printf("Message type: %d, hash: %d, size: %d, data: ", mes->type, mes->hash, mes->size); // Вывод информации о сообщении.
    for(size_t i = 0; i<mes->size; i++) // Цикл по всем элементам данных сообщения.
        printf("%d", mes->data[i]); // Вывод элементов данных.
    printf("\n"); // Переход на новую строку.
}
