#pragma once
#ifndef MY_LIB_THREAD_CLASS
#define MY_LIB_THREAD_CLASS
#define t_Lock std::unique_lock<std::mutex>
#define t_Cond_ std::condition_variable
#define t_Flag_ std::atomic<Bool>
#define t_Thrd_ std::thread
#define t_Mutx_ std::mutex

namespace thrd{
	class GM_Thread {
private:
		t_Mutx_ mx_;	//std::mutex
		t_Cond_ cv_;	//condition_variable
		t_Thrd_ thrd_;	//std::thread
		t_Flag_ step_;	//atomic<Bool>
		t_Flag_ work_;	//atomic<Bool>
		void(*_the_main_proc)();
public:
		GM_Thread(void(*__procedure_pointer)()){

#ifdef THREAD_STDOUT_MODE
			std::cout << "STEP IN TO THREAD CONSTRUKTOR\n";
#endif //THREAD_STDOUT_MODE

			this->work_.store(true);//после первого прогона, отправл€ем поток спать
			this->step_.store(false);//после первого прогона, отправл€ем поток спать
			this->_the_main_proc = __procedure_pointer;
			this->thrd_ = t_Thrd_(
				[this]()
				{// thisthrd_ BEGIN
#ifdef THREAD_STDOUT_MODE
					std::cout << "START_WORK_THREAD: " << std::thisthrd_::get_id() << '\n';
#endif //THREAD_STDOUT_MODE
					t_Lock locker(this->mx_);
					locker.unlock();
					 do {
						this->_the_main_proc();
						locker.lock();
						this->cv_.wait(locker, [this] {return this->step_.load(); });
						this->step_.store(false);
						locker.unlock();
					 } while (work_);
#ifdef EMG_ALLOC_STDOUT
					 _CrtDumpMemoryLeaks();
#endif // EMG_ALLOC_STDOUT
#ifdef THREAD_STDOUT_MODE
					std::cout << "END_WORK_THREAD: " << std::thisthrd_::get_id() << '\n';
#endif //THREAD_STDOUT_MODE
				}// thisthrd_ END
			);
		}
		// after __runstep_ = false only!!!
		~GM_Thread() {
#ifdef THREAD_STDOUT_MODE
			std::cout << "STEP IN TO THREAD DESTRUKTOR ";
#endif //THREAD_STDOUT_MODE
			if (!this->thrd_.joinable()) return;
			this->step_.store(true);
			this->cv_.notify_one();
			this->thrd_.join();
		}
		__forceinline void stop() {
			work_.store(false);
		}
		__forceinline void step() {
			t_Lock lock(this->mx_);
			this->step_.store(true);
			this->cv_.notify_one();
		}

	};
};

#undef t_Lock
#undef t_Cond_
#undef t_Flag_
#undef t_Thrd_
#undef t_Mutx_
#endif