#include "../code/pool/threadpool.h"
#include <iostream>
#include <features.h>
#include <unistd.h> 
#include <mutex>
#include <stdio.h>

std::mutex mtx;

void TestTask(int i, int cnt){
    for(int j = 0; j <100; j++){
            mtx.lock();
            cnt++;
            //std::cout << gettid() << " " << i << " "<< cnt * cnt  << std::endl;
            if(cnt == 99) printf("%d %d   %d \n", gettid(), i, cnt);
            mtx.unlock();
    }
}

void TestThreadPool(){
    ThreadPool threadpool (6);
    for(int i=0; i < 12; ++i){
        threadpool.AddTask(std::bind(TestTask,  i , i * 0));
    }
}

int main(){
    //TestThreadPool();
}