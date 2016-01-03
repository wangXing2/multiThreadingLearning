#include <pthread.h>

class Thread 
{
 public:
    Thread() : m_tid(0), m_running(0), m_detached(0) {};
    virtual ~Thread();
    
    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void* run() = 0;

 private:
    pthread_t m_tid;
    int m_running;
    int m_detached;
};

Thread::~Thread() {
    if (m_running == 1 && m_detached == 0) {
        pthread_detach(m_tid);
    }
    if (m_running == 1) {
        pthread_cancel(m_tid);
    }
}

static void* runThread(void *arg);

int Thread::start() {
    int result = pthread_create(&m_tid, NULL, runThread, this);
    if (result == 0) 
        m_running = 1;
    return result;
}

static void* runThread(void* arg) {
    return ((Thread*)arg)->run();
}

int Thread::join() {
    int result = -1;
    if (m_running == 1) {
        result = pthread_join(m_tid, NULL);
        if (result == 0)
            m_detached = 1;
    }
    return result;
}

int Thread::detach() {
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) {
            m_detached = 1;
        }
    }
    return result;
}

pthread_t Thread::self() {
    return m_tid;
}
