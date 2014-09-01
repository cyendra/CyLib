
namespace cylib {
	
	// 引用计数操作器
	template<typename T> struct ReferenceCounterOperator
	{
		// 返回一个引用计数器
		static int* CreateCounter(T* reference)
		{
			return new int(0);

		}
		// 删除引用函数
		static void DeleteReference(int* counter, void* reference)
		{
			delete counter;// 删除计数器
			delete (T*)reference;// 删除资源
		}
	};

	
	// 智能指针类
	template<typename T> class SmartPtr
	{
		
	private:
		template<typename X> friend class SmartPtr;
		// 删除器
		typedef void (*Destructor)(int*, void*);
		// 引用计数器
		int* counter;
		// 引用资源，在拷贝过程中可能改变类型
		T* reference;
		// 原始引用资源，保持资源第一次创建时的指针
		void* originalReference;
		// 原始资源删除函数，在最后一个引用被析构时调用，删除资源
		Destructor originalDestructor;

		// 增加引用计数
		void Inc()
		{
			if (counter)
			{
				++(*counter);
			}
		}
		// 减少引用计数，如果资源不再被引用则删除资源
		void Dec()
		{
			if (counter)
			{
				if (--(*counter) == 0)
				{
					originalDestructor(counter, originalReference);
					counter = 0;
					reference = 0;
					originalReference = 0;
					originalDestructor = 0;
				}
			}
		}

		// 返回当前计数器
		int* Counter() const
		{
			return counter;
		}

		// 私有构造器
		SmartPtr(int* _counter, T* _reference, void* _originalReference, Destructor _originalDestructor)
			: counter(_counter)
			, reference(_reference)
			, originalReference(_originalReference)
			, originalDestructor(_originalDestructor)
		{
			Inc();
		}

	public:
		// 获取资源的直接指针
		T* get() const
		{
			return reference;
		}
		// 重载->操作符
		T* operator->()const
		{
			return reference;
		}
		// 重载*操作符，危险！
		T& operator*() const {
			return *reference;
		}

		// 构造一个空的智能指针，不指向任何资源
		SmartPtr()
			: counter(0)
			, reference(0)
			, originalReference(0)
			, originalDestructor(0)
		{}
		// 用一个普通指针构造智能指针，是最基本的用法
		SmartPtr(T* pointer)
			: counter(0)
			, reference(0)
			, originalReference(0)
			, originalDestructor(0)
		{
			if (pointer)
			{
				counter = ReferenceCounterOperator<T>::CreateCounter(pointer);// 创建新的计数器
				reference = pointer;// 获取当前资源的引用
				originalReference = pointer;// 将原始资源置为当前资源
				originalDestructor = ReferenceCounterOperator<T>::DeleteReference;// 连接删除器
				Inc();// 引用计数增加
			}
		};
		// 用另一个同类型的智能指针进行拷贝构造，不创建新资源
		SmartPtr(const SmartPtr<T>& pointer)
			: counter(pointer.counter)
			, reference(pointer.reference)
			, originalReference(pointer.originalReference)
			, originalDestructor(pointer.originalDestructor)
		{
			Inc();// 引用计数增加
		}
		// 用其他类型的智能指针进行转型拷贝构造，不创建新资源
		// 将原始类型U转换为当前智能指针的类型T，但是原始资源与原始删除器不变
		template<typename U> SmartPtr(const SmartPtr<U>& pointer)
			: counter(0)
			, reference(0)
			, originalReference(0)
			, originalDestructor(0)
		{
			T* converted = pointer.get();
			if (converted)
			{
				counter = pointer.Counter();
				reference = converted;
				originalReference = pointer.originalReference;
				originalDestructor = pointer.originalDestructor;
				Inc();
			}
		}

		// 析构当前的智能指针，减少引用计数
		~SmartPtr()
		{
			Dec();
		}

		// 将一个普通指针的值赋给智能指针
		// 智能指针之前引用的资源取消，由普通指针构造出新的智能指针
		// 构造失败则将智能指针置为空
		SmartPtr<T>& operator=(T* pointer)
		{
			Dec();// 原本的资源引用减少
			if (pointer)
			{
				counter = ReferenceCounterOperator<T>::CreateCounter(pointer);
				reference = pointer;
				originalReference = pointer;
				originalDestructor = &ReferenceCounterOperator<T>::DeleteReference;
				Inc();
			}
			else
			{
				counter = 0;
				reference = 0;
				originalReference = 0;
				originalDestructor = 0;
			}
			return *this;
		}

		// 将另一个智能指针的值赋给自身
		// 智能指针之前引用的资源取消，并引用新的智能指针的资源
		SmartPtr<T>& operator=(const SmartPtr<T>& pointer)
		{
			if (this != &pointer)// 判断是否自赋值
			{
				Dec();
				counter = pointer.counter;
				reference = pointer.reference;
				originalReference = pointer.originalReference;
				originalDestructor = pointer.originalDestructor;
				Inc();
			}
			return *this;
		}

		// 将一个不同类型的智能指针赋给自身
		// 智能指针之前引用的资源取消，并引用新的智能指针的资源
		// 转型失败的话返回空智能指针
		template<typename U> SmartPtr<T>& operator=(const SmartPtr<U>& pointer)
		{
			T* converted = pointer.get();
			Dec();
			if (converted)
			{
				counter = pointer.counter;
				reference = converted;
				originalReference = pointer.originalReference;
				originalDestructor = pointer.originalDestructor;
				Inc();
			}
			else
			{
				counter = 0;
				reference = 0;
				originalReference = 0;
				originalDestructor = 0;
			}
			return *this;
		}

		// 重载比较操作符，用于比较智能指针与普通指针是否指向相同资源
		bool operator==(const T* pointer)const { return reference == pointer; }
		bool operator!=(const T* pointer)const { return reference != pointer; }
		bool operator>(const T* pointer)const { return reference>pointer; }
		bool operator>=(const T* pointer)const { return reference >= pointer; }
		bool operator<(const T* pointer)const { return reference<pointer; }
		bool operator<=(const T* pointer)const { return reference <= pointer; }

		// 重载比较操作符，用于比较两个智能指针是否指向相同资源
		bool operator==(const SmartPtr<T>& pointer)const { return reference == pointer.reference; }
		bool operator!=(const SmartPtr<T>& pointer)const { return reference != pointer.reference; }
		bool operator>(const SmartPtr<T>& pointer)const { return reference>pointer.reference; }
		bool operator>=(const SmartPtr<T>& pointer)const { return reference >= pointer.reference; }
		bool operator<(const SmartPtr<T>& pointer)const { return reference<pointer.reference; }
		bool operator<=(const SmartPtr<T>& pointer)const { return reference <= pointer.reference; }

		// 智能指针指向非空时有true的布尔值
		operator bool()const{ return reference != 0; }

	};
	
}