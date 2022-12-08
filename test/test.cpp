/*
 * @Author       : yunbai
 * @Date         : 2022-04-22
 * @copyleft Apache 2.0
 */ 
#include "../code/log/log.h"
#include "../code/pool/threadpool.h"

#include "../code/timer/heaptimer.h"

#include <features.h>
#include <iomanip>

#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 30
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#endif

void TestLog() {
    int cnt = 0, level = 0;
    Log::Instance()->init(level, "./testlog1", ".log", 0);
    for(level = 3; level >= 0; level--) {
        Log::Instance()->SetLevel(level);
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(i,"%s 111111111 %d ============= ", "Test", cnt++);
            }
        }
    }
    cnt = 0;
    Log::Instance()->init(level, "./testlog2", ".log", 5000);
    for(level = 0; level < 4; level++) {
        Log::Instance()->SetLevel(level);
        for(int j = 0; j < 10000; j++ ){
            for(int i = 0; i < 4; i++) {
                LOG_BASE(i,"%s 222222222 %d ============= ", "Test", cnt++);
            }
        }
    }
}

void ThreadLogTask(int i, int cnt) {
    for(int j = 0; j < 10000; j++ ){
        LOG_BASE(i,"PID:[%04d]======= %05d ========= ", gettid(), cnt++);
    }
}

void TestThreadPool() {
    Log::Instance()->init(0, "./testThreadpool", ".log", 5000);
    ThreadPool threadpool(6);
    for(int i = 0; i < 18; i++) {
        threadpool.AddTask(std::bind(ThreadLogTask, i % 4, i * 10000));
    }
    getchar();
}

auto now(){
    time_t tt = std::chrono::system_clock::to_time_t(Clock::now());
    auto res = std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S");
    return res;
}

void test_heaptimer(){
    //clock_t start,finish;
    //start = clock();

    std::shared_ptr<HeapTimer> timer_p(new HeapTimer());

    //std::cout <<  now() << std::endl;
    for(int i =1; i< 10000; i++) {
        timer_p->add(i, i* 2000,
                     [=]() { std::cout << "Clock::now :" << now() << "  " << timer_p->getMS() << std::endl; });
    }
    while(!timer_p->get().empty()){
        timer_p->tick();
    }

    //finish = clock();
    //std::cout<<finish-start<<std::endl;
}

int main() {
    TestLog();
    TestThreadPool();
    //test_heaptimer();
}