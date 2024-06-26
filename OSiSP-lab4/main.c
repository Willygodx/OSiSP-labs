#include "func.h"

int continuing = 1;

StackNode* stackFiller = NULL;
StackNode* stackExtractor = NULL;

int main() {
    srand(time(NULL));

    // Удаление, если созданы, семафоров и совместной памяти.
    shm_unlink(SHARED_MEMORY_NAME);
    sem_unlink(FILL_SEM_NAME);
    sem_unlink(EXTRACT_SEM_NAME);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    // Сигнал отмены работы.
    signal(SIGUSR1, changeContinuingStatus);

    // Открыть совместо используемую память для чтения и записи всем.
    int shmFd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if(shmFd == -1) {
        printf("Error while creating shared memory.\n");
        exit(EXIT_FAILURE);
    }
    // Выделить место под заданное количество элементов очереди.
    ftruncate(shmFd, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message));
    // Получение образа памяти.
    Queue* queue = mmap(NULL, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if(queue == MAP_FAILED) {
        printf("Error while mapping shared data.\n");
        exit(EXIT_FAILURE);
    }
    // Значение указателей по умолчанию.
    queue->ringHead = 0;
    queue->ringTail = 0;
    queue->countDeleted = 0;
    queue->countAdded = 0;
    queue->currentPlaceToWrite = (uintptr_t)queue + sizeof(Queue);

    // Инициализация семафоров.
    sem_t* fillSem;
    if((fillSem = sem_open(FILL_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open filling semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* extractSem;
    if((extractSem = sem_open(EXTRACT_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open extracting semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* queueAccess;
    if((queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open queue semaphore, code %d.\n", errno);
        exit(errno);
    }

    while(continuing) {
        char ch = getchar();
        switch (ch) {
            case 'w': {
                // Создание нового процесса-заполнителя.
                pid_t pid = fork();
                if(pid == -1) {
                    printf("Error occurred while creating new filler, error code %d.\n", errno);
                    exit(errno);
                } else if(pid == 0) {
                    fillMessages();
                    exit(0);
                } else
                    pushStack(&stackFiller, pid);
                break;
            }
            case 's':
                // Отправка сигнала SIGUSR1 процессу-заполнителю для прекращения его работы.
                if(stackFiller != NULL) {
                    kill(stackFiller->pid, SIGUSR1);
                    waitpid(stackFiller->pid, NULL, 0);
                    popStack(&stackFiller);
                } else
                    printf("There are no fillers.\n");
                break;
            case 'e': {
                // Создание нового процесса-извлекателя.
                pid_t pid = fork();
                if(pid == -1) {
                    printf("Error occurred while creating new extractor, error code %d.\n", errno);
                    exit(errno);
                } else if(pid == 0) {
                    extractMessages();
                    exit(0);
                } else
                    pushStack(&stackExtractor, pid);
                break;
            }
            case 'd':
                // Отправка сигнала SIGUSR1 процессу-извлекателю для прекращения его работы.
                if(stackExtractor != NULL) {
                    kill(stackExtractor->pid, SIGUSR1);
                    waitpid(stackExtractor->pid, NULL, 0);
                    popStack(&stackExtractor);
                } else
                    printf("There are no extractors.\n");
                break;
            case 'q':
                // Прекращение работы всех заполнителей и извлекателей.
                while(stackFiller != NULL) {
                    kill(stackFiller->pid, SIGUSR1);
                    waitpid(stackFiller->pid, NULL, 0);
                    popStack(&stackFiller);
                }
                while(stackExtractor != NULL) {
                    kill(stackExtractor->pid, SIGUSR1);
                    waitpid(stackExtractor->pid, NULL, 0);
                    popStack(&stackExtractor);
                }
                printf("All fillers and extractors are killed. End of program.\n");
                continuing = 0;
                break;
        }
    }
    // Освобождение памяти.
    munmap(queue, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message));
    close(shmFd);
    shm_unlink(SHARED_MEMORY_NAME);

    // Освобождение семафоров.
    sem_close(fillSem);
    sem_unlink(FILL_SEM_NAME);
    sem_close(extractSem);
    sem_unlink(EXTRACT_SEM_NAME);
    sem_close(queueAccess);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    return 0;
}
