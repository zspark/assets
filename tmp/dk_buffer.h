
//==================================================================================================
//
//API
//

//M_ABSTRACT class DKBaseCamera:public DKBaseObject,M_IMPLEMENT DKUpdatable{
class DKBuffer{
	public:
		DKBuffer();
		~DKBuffer();
		dkchar* Map();
		const dkchar* Map()const;
		void Unmap();
		dkuint GetLengthInBytes()const;
	private:

}

class DK{
	public:
		DKBuffer();
		~DKBuffer();
		dkchar* Map();
		const dkchar* Map()const;
		void Unmap();
		dkuint GetLengthInBytes()const;
	private:

}


//==================================================================================================
//
//implementation;
//

