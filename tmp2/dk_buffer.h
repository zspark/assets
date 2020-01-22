
//==================================================================================================
//
//API
//

//M_ABSTRACT class DKBaseCamera:public DKBaseObject,M_IMPLEMENT DKUpdatable{
class DKRTBuffer{
	public:
		static DKRTBuffer* CreateFromOpenGlBuffer();
	public:
		DKRTBuffer();
		~DKRTBuffer();
		dkchar* Map();
		const dkchar* Map()const;
		void Unmap();
		dkuint GetLengthInBytes()const;
	private:

}

class DKRTTexture{
	public:
		DKRTTexture();
		~DKRTTexture();
		dkchar* Map();
		const dkchar* Map()const;
		void Unmap();
		dkuint GetLengthInBytes()const;
	private:

}

/**
 * wrapper of cudaContent
 */
class DKRTContent{
}

template<typename T>
class tpl_DKRTRecord{
	public:
		T data;
}

class DKRTSBT{ }

class DKRTProgram{ }
class DKRTProgramGroup{ }
class DKRTPipeline{ }

//==================================================================================================
//
//implementation;
//

