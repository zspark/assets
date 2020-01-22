

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
		DKMemoryBlock();
		DKMemoryBlock(dkuint);
		~DKMemoryBlock();
		dkbool SetPreservingSizeInBytes(dkuint);
		size_t GetPreservingSizeInBytes()const{return m_nPreversingSizeInBytes;}

	private:
		template <typename T> friend class DKMemoryAllocator;
		dkuint m_nPreversingSizeInBytes{1024u};//1KiB
		dkbyte* m_pBuffer{nullptr};
};

struct DKSMemoryRange {
	size_t offsetInBytesInBlock;
	size_t length;
}

template<typename T>
class DKMemoryAllocator final {
	M_DELETE_COPY_ASSIGN_CTOR(DKMemoryAllocator)
		using Tref=T&;
		using pT=T*;
	public:
		DKMemoryAllocator();
		~DKMemoryAllocator();
		void SetMemoryBlock(shared_ptr<DKMemoryBlock> b,const DKSMemoryRange&& r){
			m_ptrBlock=b;
			m_range=r;
			m_posInBytesInRange=0;
		}
		void SetRange(const DKSMemoryRange&& r){
			m_range=r;
			m_posInBytesInRange=0u;
		}

		/// allocate `n` T instances;
		pT Allocate(dkuint n){
			if(n<=0){
				M_LOG_WARN("allocating parameter MUST bigger than 0, argument is:{0}",1,n);
				return nullptr;
			}

			const size_t _l{sizeof(T)*n};
			const size_t _e{m_posInBytesInRange+_l};
			if(_e>m_range.length){
				M_LOG_ERROR("allocating size:(in bytes:{0},in kilobytes:{1}) with byte offset(in range):{2}, is out of range:({3},{4})",5,M_N_2_S(_l),M_N_2_S(M_B_2_KiB(_l)),M_N_2_S(m_posInBytesInRange),M_N_2_S(m_range.offsetInBytesInBlock),M_N_2_S(m_range.length));
				return nullptr;
			}

			pT r=new(m_ptrBlock->m_pBuffer+m_range.offsetInBytesInBlock+m_posInBytesInRange) T();
			m_posInBytesInRange=_e;
			return r;
		}
		void FreeAll(){
			m_posInBytesInRange=0u;
		}
	private:
		shared_ptr<DKMemoryBlock> m_ptrBlock;
		DKSMemoryRange m_range;
		size_t m_posInBytesInRange{0u};
};

