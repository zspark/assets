

#ifndef M_B_2_KiB
#define M_B_2_KiB(b) ((float)(b)>>10)
#endif

#ifndef M_B_2_MiB
#define M_B_2_MiB(b) ((float)(b)>>20)
#endif

#ifndef M_B_2_GiB
#define M_B_2_GiB(b) ((float)(b)>>30)
#endif

class DKMemoryBlock final{
	M_DELETE_COPY_ASSIGN_CTOR(DKMemoryBlock)
	public:
		DKMemoryBlock()=delete;
		DKMemoryBlock(size_t);
		~DKMemoryBlock();
		size_t GetPreservingSize()const{return m_nCapability;}

	private:
		template <typename T> friend class DKMemoryAllocator;
		size_t m_nCapability{1024u};//1KiB
		dkbyte* m_pBuffer{nullptr};
};

template<typename T>
struct DKSRange {
	T begin;
	T length;
	inline T end(){return begin+length;}
}

class DKMemoryAllocator{
	M_DELETE_COPY_ASSIGN_CTOR(DKMemoryAllocator)
	public:
		DKMemoryAllocator()=default;
		~DKMemoryAllocator()=default;
		/**
		 * takes ownership of pb
		 */
		void SetMemoryBlock(DKMemoryBlock* pb);
		void SetMemoryBlock(shared_ptr<DKMemoryBlock> b,const DKSRange&& r);
		
		/// allocate `n` T instances;
		template<typename T>
		T* Allocate(dkuint n){
			if(n<=0){
				M_LOG_WARN("allocating parameter MUST bigger than 0, argument is:{0}",1,n);
				return nullptr;
			}

			const size_t _l{sizeof(T)*n};
			const size_t _e{m_pivot+_l};
			if(_e>m_range.length){
				M_LOG_ERROR("going to allocate {0} bytes ({1} kilobytes) memory with {2} begin(s), is out of range:({3},{4})",5,M_N_2_S(_l),M_N_2_S(M_B_2_KiB(_l)),M_N_2_S(m_pivot),M_N_2_S(m_range.begin),M_N_2_S(m_range.length));
				return nullptr;
			}

			T* r=new(m_ptrBlock->m_pBuffer+m_range.begin+m_pivot) T();
			m_pivot=_e;
			return r;
		}
		void FreeAll(){ m_pivot=0u; }
	private:
		shared_ptr<DKMemoryBlock> m_ptrBlock;
		DKSRange m_range;
		size_t m_pivot{0u};
};

DKMemoryBlock::DKMemoryBlock(size_t n){
	m_nCapability=n;
	m_pBuffer=(dkbyte*)malloc(n);

DKMemoryBlock::~DKMemoryBlock(){
	free(m_pBuffer);
	m_pBuffer=nullptr;
}
void DKMemoryBlock::SetMemoryBlock(DKMemoryBlock* pb){
	m_ptrBlock.reset(pb);
	m_pivot=0u;
	m_range.begin=0;
	m_range.length=pb->GetPreservingSize();
}
void DKMemoryAllocator::SetMemoryBlock(shared_ptr<DKMemoryBlock> b,const DKSRange&& r){
	m_ptrBlock=b;
	m_pivot=0u;
	m_range=r;
}


//============================================================================================
// example
struct A:protected DKMemoryAllocator{
	A(){
		SetMemoryBlock(new DKMemoryBlock(1<<10));//1KiB
	}
	~A(){
		FreeAll();
	}

	struct B{int b;};
	void Some_frequently_called_func(){
		const int N{200};
		B* pb=Allocate(N);
		for(int i=0;i<N;++i){
			pb->b=i;
			++pb;
		}
		/// some staffs working with 200 Bs;
		FreeAll();
	}
	
}
