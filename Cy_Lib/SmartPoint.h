
namespace cylib {
	
	// ���ü���������
	template<typename T> struct ReferenceCounterOperator
	{
		// ����һ�����ü�����
		static int* CreateCounter(T* reference)
		{
			return new int(0);

		}
		// ɾ�����ú���
		static void DeleteReference(int* counter, void* reference)
		{
			delete counter;// ɾ��������
			delete (T*)reference;// ɾ����Դ
		}
	};

	
	// ����ָ����
	template<typename T> class SmartPtr
	{
		
	private:
		template<typename X> friend class SmartPtr;
		// ɾ����
		typedef void (*Destructor)(int*, void*);
		// ���ü�����
		int* counter;
		// ������Դ���ڿ��������п��ܸı�����
		T* reference;
		// ԭʼ������Դ��������Դ��һ�δ���ʱ��ָ��
		void* originalReference;
		// ԭʼ��Դɾ�������������һ�����ñ�����ʱ���ã�ɾ����Դ
		Destructor originalDestructor;

		// �������ü���
		void Inc()
		{
			if (counter)
			{
				++(*counter);
			}
		}
		// �������ü����������Դ���ٱ�������ɾ����Դ
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

		// ���ص�ǰ������
		int* Counter() const
		{
			return counter;
		}

		// ˽�й�����
		SmartPtr(int* _counter, T* _reference, void* _originalReference, Destructor _originalDestructor)
			: counter(_counter)
			, reference(_reference)
			, originalReference(_originalReference)
			, originalDestructor(_originalDestructor)
		{
			Inc();
		}

	public:
		// ��ȡ��Դ��ֱ��ָ��
		T* get() const
		{
			return reference;
		}
		// ����->������
		T* operator->()const
		{
			return reference;
		}
		// ����*��������Σ�գ�
		T& operator*() const {
			return *reference;
		}

		// ����һ���յ�����ָ�룬��ָ���κ���Դ
		SmartPtr()
			: counter(0)
			, reference(0)
			, originalReference(0)
			, originalDestructor(0)
		{}
		// ��һ����ָͨ�빹������ָ�룬����������÷�
		SmartPtr(T* pointer)
			: counter(0)
			, reference(0)
			, originalReference(0)
			, originalDestructor(0)
		{
			if (pointer)
			{
				counter = ReferenceCounterOperator<T>::CreateCounter(pointer);// �����µļ�����
				reference = pointer;// ��ȡ��ǰ��Դ������
				originalReference = pointer;// ��ԭʼ��Դ��Ϊ��ǰ��Դ
				originalDestructor = ReferenceCounterOperator<T>::DeleteReference;// ����ɾ����
				Inc();// ���ü�������
			}
		};
		// ����һ��ͬ���͵�����ָ����п������죬����������Դ
		SmartPtr(const SmartPtr<T>& pointer)
			: counter(pointer.counter)
			, reference(pointer.reference)
			, originalReference(pointer.originalReference)
			, originalDestructor(pointer.originalDestructor)
		{
			Inc();// ���ü�������
		}
		// ���������͵�����ָ�����ת�Ϳ������죬����������Դ
		// ��ԭʼ����Uת��Ϊ��ǰ����ָ�������T������ԭʼ��Դ��ԭʼɾ��������
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

		// ������ǰ������ָ�룬�������ü���
		~SmartPtr()
		{
			Dec();
		}

		// ��һ����ָͨ���ֵ��������ָ��
		// ����ָ��֮ǰ���õ���Դȡ��������ָͨ�빹����µ�����ָ��
		// ����ʧ��������ָ����Ϊ��
		SmartPtr<T>& operator=(T* pointer)
		{
			Dec();// ԭ������Դ���ü���
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

		// ����һ������ָ���ֵ��������
		// ����ָ��֮ǰ���õ���Դȡ�����������µ�����ָ�����Դ
		SmartPtr<T>& operator=(const SmartPtr<T>& pointer)
		{
			if (this != &pointer)// �ж��Ƿ��Ը�ֵ
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

		// ��һ����ͬ���͵�����ָ�븳������
		// ����ָ��֮ǰ���õ���Դȡ�����������µ�����ָ�����Դ
		// ת��ʧ�ܵĻ����ؿ�����ָ��
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

		// ���رȽϲ����������ڱȽ�����ָ������ָͨ���Ƿ�ָ����ͬ��Դ
		bool operator==(const T* pointer)const { return reference == pointer; }
		bool operator!=(const T* pointer)const { return reference != pointer; }
		bool operator>(const T* pointer)const { return reference>pointer; }
		bool operator>=(const T* pointer)const { return reference >= pointer; }
		bool operator<(const T* pointer)const { return reference<pointer; }
		bool operator<=(const T* pointer)const { return reference <= pointer; }

		// ���رȽϲ����������ڱȽ���������ָ���Ƿ�ָ����ͬ��Դ
		bool operator==(const SmartPtr<T>& pointer)const { return reference == pointer.reference; }
		bool operator!=(const SmartPtr<T>& pointer)const { return reference != pointer.reference; }
		bool operator>(const SmartPtr<T>& pointer)const { return reference>pointer.reference; }
		bool operator>=(const SmartPtr<T>& pointer)const { return reference >= pointer.reference; }
		bool operator<(const SmartPtr<T>& pointer)const { return reference<pointer.reference; }
		bool operator<=(const SmartPtr<T>& pointer)const { return reference <= pointer.reference; }

		// ����ָ��ָ��ǿ�ʱ��true�Ĳ���ֵ
		operator bool()const{ return reference != 0; }

	};
	
}