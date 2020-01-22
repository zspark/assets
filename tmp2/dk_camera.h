
//==================================================================================================
//
//API
//

M_ABSTRACT class DKBaseCamera:public DKBaseObject,M_IMPLEMENT DKUpdatable{
	public:
		void SetFOV(float);
		void SetVisualDistance(dkfloat n,dkfloat f);
		void SetVisualDistance(const dkfloat2&);
		void SetScreenSize(dkuint w,dkuint h);
		void SetScreenSize(const dkint2&);//<=0 means to keep old value;
		void SetPresentPattern(EPresentPattern);

	public:
		void Update()override;

	protected:
		virtual void _CalcMatrix()=0;

	private:
		float m_fFOV{60.0f};
		dkfloat2 m_fDistance{0.01f,100000000.0f};
		dkuint2 m_nSize{0u,0u};
		glm::Matrix4x4 m_matrixProjection;
};

class DKOthroCamera:public DKBaseCamera,M_IMPLEMENT DKLinkable{
		void _CalcMatrix()override;

	public:
		HSNode* CreateLinkNode()override{
			return new tpl_DKBaseNode<DKOthroCamera*>(this);
		}
		void DestroyLinkNode(HSNode* n)override{
			//TODO:Jerry: check n's concrete type, then delete;
			M_SAFE_DELETE(n);
		};
		bool Link(HSNode*)override{}
		bool Unlink(HSNode* _n)override{return false;}
		bool IsLinkNodeAcceptable()const override{return false;}
};

class DKOthroCameraInstance:public tpl_DKInstance<DKOthroCamera*>{
	M_DELETE_COPY_ASSIGN_CTOR(DKOthroCameraInstance)
	public:
		void SetFilmSize(w,h);
	private:
		DKOthroCameraInstance();
		~DKOthroCameraInstance();
	private:
		DKFilm* m_pFilm{nullptr};
};

//==================================================================================================
//
//implementation;
//

DKOthroCameraInstance::DKOthroCameraInstance(){
	m_pFilm=new DKFilm();
}
DKOthroCameraInstance::~DKOthroCameraInstance(){
	M_SAFE_DELETE(m_pFilm);
}

