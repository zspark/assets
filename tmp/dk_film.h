namespace dkshader{
	class DKBuffer;
}

class DKFilm final:public DKBaseObject{
	M_DELETE_COPY_ASSIGN_CTOR(DKFilm)
	public:
		DKFilm();
		DKFilm(dkuint,dkuint);
		~DKFilm();
		void SetSize(dkuint,dkuint);//zero means keep old value;

	private:
		friend DKCamera;
		dkuint m_nWidth{0u};
		dkuint m_hHeight{0u};
		dkshader::DKBuffer* m_pBufferOutput{nullptr};
		//.. other buffers;
}

