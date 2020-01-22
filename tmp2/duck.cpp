
#ifndef M_IMPLEMENT
#define M_IMPLEMENT public
#endif

#ifndef M_INTERFACE
#define M_INTERFACE class
#endif

#ifndef M_DELETE_COPY_ASSIGN_CTOR(cls)
#define M_DELETE_COPY_ASSIGN_CTOR(cls) cls(const cls&)=delete; cls* operater=(const cls&)=delete;
#endif

template<typename T>
struct tpl_dkrange{
	T min;
	T max;
};

using tpl_dkrange<float> dkfrange01

enum EObjectType{
	OBJECT_TYPE_CAMERA,
	OBJECT_TYPE_LIGHT,
	OBJECT_TYPE_GEOMETRY,
	OBJECT_TYPE_MATERIAL,
	OBJECT_TYPE_SCENE,
};

M_INTERFACE DKUpdatable{
	virtual void Update()=0;
};

M_INTERFACE DKLinkable{
	/**
	 * check weather *this can accept node;
	 */
	virtual bool IsLinkNodeAcceptable()const=0;
	/**
	 * create a node which holds the instance of *this
	 */
	virtual HSNode* CreateLinkNode()=0;
	virtual void DestroyLinkNode(HSNode*)=0;
	/**
	 * link the given node to *this;
	 */
	virtual bool Link(HSNode*)=0;
	/**
	 * unlink the given node from *this;
	 */
	virtual bool Unlink(HSNode*)=0;
};

class DKLinkNode:public HSNode{
	public:
		DKLinkable* upstream{nullptr};
		DKLinkable* downstream{nullptr};
};

class DKBaseLight:public DKBaseObject{
	public:
		DKBaseLight();
		virtual ~DKBaseLight();
		void SetIntensity(float);
		void SetGammaCorrection(float);
		virtual ELightType GetType()const =0;
	protected:
		float m_fIntensity{0.f};
		float m_fGammaCorrection{0.45f};
}

class DKPointLight:public DKBaseLight, M_IMPLEMENT DKLinkable{
	public:
		DKPointLight();
		~DKPointLight();
		ELightType GetType()const override{return ELightType::POINT_LIGHT;}
		void SetColor(dkf4& c);
		void SetColor(float r,float g,float b,float a);
	private:
		dkf4 m_color{0.f,0.f,0.f,1.f};
}

class DKMaterial:public DKBaseObject{
	public:
		DKMaterial();
		~DKMaterial();
	private:
}

class DKScene final:public DKBaseObject,M_IMPLEMENT DKLinkable,M_IMPLEMENT DKUpdatable{
	M_DELETE_COPY_ASSIGN_CTOR(DKFilm)
	public:
		void Update()override;
		HSNode* CreateLinkNode()override{
			if(!m_pLinkNode)m_pLinkNode=new tpl_DKBaseNode<DKScene*>(this);
			return m_pLinkNode;
		}
		void DestroyLinkNode(HSNode*)override{
			M_SAFE_DELETE(m_pLinkNode);
		};
		bool Link(HSNode* n)override{
			EObjectType type;//TODO:How to get type??
			bool _r{true};
			switch(type){
				case EObjectType::OBJECT_TYPE_CAMERA:
					m_tree.Insert(n,&m_cameraNode,xxx);
					break;
				case EObjectType::OBJECT_TYPE_LIGHT:
					m_tree.Insert(n,&m_lightNode,xxx);
					break;
				case EObjectType::OBJECT_TYPE_GEOMETRY:
					m_tree.Insert(n,&m_geometryNode,xxx);
					break;
				default:
					_r=false;
					E_LOG_ERROR("WTF",0);
			}
			return _r;
		}
		bool Unlink(HSNode* n)override{
			return m_tree.Remove(n);
		}
		bool IsLinkNodeAcceptable()const override{return true;}

	public:
		void AddEntity(DKEntity*);
		void RemoveEntity(DKEntity*);
		void AddLight(DKBaseLight*);
		void RemoveLight(DKBaseLight*);
		void AddCamera(DKBaseCamera*);
		void RemoveCamera(DKBaseCamera*);

	private:
		void _Init(){
			m_tree.Insert(&m_lightNode,nullptr,xx);
			m_tree.Insert(&m_cameraNode,nullptr,xx);
			m_tree.Insert(&m_geometryNode,nullptr,xx);
		}
		void _Deinit(){
			m_tree.Remove(&m_lightNode);
			m_tree.Remove(&m_cameraNode);
			m_tree.Remove(&m_geometryNode);
		}

	private:
		tpl_DKBaseNode<DKScene*>* m_pLinkNode{nullptr};
		HSNode m_lightNode;
		HSNode m_cameraNode;
		HSNode m_geometryNode;
		HSHierarchicalTree m_tree;
};

template<typename T>
class tpl_DKBaseNode:public HSNode{
		tpl_DKBaseNode(const tpl_DKBaseNode<T>& ptr)=delete;
		tpl_DKBaseNode<T>* operator=(const tpl_DKBaseNode<T>&)=delete;
	public:
		tpl_DKBaseNode(T i): m_ref_pBaseObject(i){}
		virtual ~tpl_DKBaseNode(){m_ref_pBaseObject=nullptr;};
		T GetBaseObject(){return m_ref_pBaseObject;};
		const T& GetBaseObject()const{return m_ref_pBaseObject;};
	private:
		T m_ref_pBaseObject{nullptr};
};

class DKUpdatableNode final:public HSNode{
	M_DELETE_COPY_ASSIGN_CTOR(DKUpdatableNode)
	public:
		DKUpdatableNode(DKUpdatable* o):m_ref_pUpdatableObject(o){}
		~DKUpdatableNode(){m_ref_pUpdatableObject=nullptr;}
		inline void Update(){ m_ref_pUpdatableObject->Update(); }
	private:
		DKUpdatable* m_ref_pUpdatableObject{nullptr};
}

template<typename T>
class tpl_DKInstance:public HSNode{
	tpl_DKInstance(const tpl_DKInstance<T>& ptr)=delete;
	tpl_DKInstance<T>* operator=(const tpl_DKInstance<T>&)=delete;
	public:
		T GetAPIObject(){return m_ref_pAPIObject;};
		const T& GetAPIObject()const{return m_ref_pAPIObject;};
		const glm::Matrix4x4& GetMatrixToParent()const{return m_matrixToParent;}
		void Translate(dkfloat x,dkfloat y,dkfloat z);
		void Translate(const dkf3& t);
		void Scale(dkfloat s);
		void Scale(dkfloat x,dkfloat y,dkfloat z);
		void Scale(const dkf3& s);
		void Rotate(dkfloat r);
		void Rotate(dkfloat x,dkfloat y,dkfloat z);
		void Rotate(const dkf3& r);
		void ResetSRT();

	protected:
		glm::Matrix4x4 m_matrixToParent;// init to uniform matrix;

	private:
		tpl_DKInstance();
		virtual ~tpl_DKInstance();
		T m_ref_pAPIObject{nullptr};
};

class DKInstanceCamera:public tpl_DKInstance<DKBaseCamera*>{
	public:

	private:
		DKFilm* m_ref_pFilm{nullptr};
};

typedef tpl_DKInstance<DKBaseCamera*> DKInstanceCamera;

/*
template<typename T>
class DKReferenceNode:public HSNode{
	public:
		M_DELETE_COPY_ASSIGN_CTOR(DKReferenceNode)
		DKReferenceNode(T);
		~DKReferenceNode(){
			m_ref_pReferancedNode=nullptr;
		}
		T GetReferencedNode(){return m_ref_pReferencedNode;}
		const T GetReferencedNode()const{return m_ref_pReferencedNode;}
	private:
		T m_ref_pReferencedNode{nullptr};
};
*/


class DKCameraNode:public tpl_DKInstance<DKCamera*>{
	public:
		DKCameraNode()=delete;
		DKCameraNode(DKCameraNode*);
		~DKCameraNode();
	public:
		void SetPosition(float x,float y,float z);
		void SetLookAt(float x,float y,float z);
	private:
		void _CalculateMatrix();// after calculation, then send matrix to render;
};

class DKEngineKernel final{
	public:
		DKEngineKernel();
		~DKEngineKernel();
		void Init(){
			/*
			 * -update-once-xxxx(s)
			 * -update-permanent-xxxx(s)
			 * -scenes-sA 
			 *		  -sB-lights-(s)
			 *			 -geometry-(s)
			 *			 -cameras-(s)
			 */
			//TODO:Jerry:
			m_tree.Insert(nullptr,m_nodeUpdateOnce,xxx);
			m_tree.Insert(nullptr,m_nodeUpdatePermanent,xxx);
			m_tree.Insert(nullptr,m_nodeScenes,xxx);
		}
		void Loop(){
			HSNode* _tmp{nullptr};
			/// 1. update-once nodes;
			_tmp=m_tree.Travers(&m_nodeUpdateOnce,true);
			while(_tmp){
				DKUpdatableNode* _n=static_cast<DKUpdatableNode*>(_tmp);
				_n->Update();
				_tmp=m_tree.Travers(&m_nodeUpdateOnce,false);
			}

			/// 2. delete once nodes;
			m_tree.RemoveChildren(m_nodeUpdateOnce);

			/// 1. update-per... nodes;
			/// 3. render scenes
		}
		void AppendToUpdateOnce(tpl_DKBaseNode*);
	private:
		HSHierarchicalTree m_tree;
		HSNode m_nodeUpdateOnce;
		HSNode m_nodeUpdatePermanent;
		HSNode m_nodeScenes;
};

class DKObjectsLinkageController final:public tpl_Singleton<DKObjectsLinkageController>{
	public:
		/**
		 * link `from` as a child of `to`
		 // * if `to` is nullptr, menas link to root
		 */
		bool Link(DKLinkable* from,DKLinkable* to){
			if(to->IsLinkNodeAcceptable()){
				HSNode* _n=from->CreateLinkNode();
				to->Link(_n);
				m_mapLinkage[to].push_back(_n);
				return true;
			}else{
				M_LOG_WARN("you should pass an link-node acceptable DKLinkable instance to second parametar!",0);
				return false;
			}
		}

		bool Unlink(DKLinkable* from,DKLinkable* to){
			std::vector<HSNode*>& _vec=m_mapLinkage[to];
		}

	private:
		void Init()override;
		void Deinit()override{
			///TODO:Jerry:delete all nodes;
			m_mapLinkage.clear();
		}

	private:
		std::map<DKLinkable*,std::vector<HSNode*>> m_mapLinkage;
		HSHierarchicalTree* m_ref_pEngineTree;
};







/**
 * API usage
 */

// user case #1
DKCamera* c=new DKOthroCamera();
DKScene* s1=new DKScene();
s1->dkAddCamera(c);
DKScene* s2=new DKScene();
s2->dkAddCamera(c);/// share one camera `c`;
delete s2;/// delete `s2` but not `c`, `c` will still been used in `s1`;
delete s1;/// delete `s1` but not `c`, because `c` is down-stream user created object, should be deleted there.
delete c;/// delete `c`;

// user case #2
// same above;
delete c;/// delete `c`, `s1` and `s2` will have no camera obtained.
delete s2;/// delete `s2` ;
delete s1;/// delete `s1` ;

// user case #3
DKCamera* c=new DKOthroCamera();
DKScene* s1=new DKScene();
DKScene* s2=new DKScene();
DKInstance* i1=s1->dkAddCamera(c);
DKInstance* i2=s2->dkAddCamera(c);/// share one camera `c`;
s1->dkRemoveInstance(i1);/// remove one instance of `c` from `s1`, `c` is not deleted;
s1->dkRemoveInstance(i111);/// remove one instance of some other type from `s1`;

// user case #4
DKScene* s=new DKScene();
DKEntity* e=new DKEntity();
DKGeometry* g1=new DKMesh();
DKGeometry* g2=new DKMesh();
DKMaterial* m=new DKMaterial();
e->dkSetMaterial(m);
e->dkAddGeometry(g1);
e->dkAddGeometry(g2);// `g1` and `g2` share same material `m`;
s->dkAddEntity(e);

// user case #5
DKEntity* e=new DKEntity();
DKMaterial* m1=new DKMaterial();
DKMaterial* m2=new DKMaterial();
e->dkSetMaterial(m1);/// set `m1` to `e`;
e->dkSetMaterial(m2);/// replace `m1` with `m2`, one entity use one material at a time;

// user case #6
DKGroup=new DKGroup();
DKCamera* c=new DKOthroCamera();
g->dkAddMember(c);
g->dkSetMatrix(m);
DKScene* s=new DKScene();
s->dkAddGroup(g);

// user case #7
DKScene* s=new DKScene();
s->dkAddCamera(new DKOthroCamera());
s->dkAddCamera(new DKOthroCamera());
s->dkAddCamera(new DKOthroCamera());
delete s;/// the above three DKOthroCameras will not deleted, memory leak, be carful.!!

// user case #8
DKCamera* c=new DKOthroCamera();
DKScene* s=new DKScene();
DKInstance* i1=s->dkAddCamera(c);
DKInstance* i2=s->dkAddCamera(c);
DKInstance* i3=s->dkAddCamera(c);/// add the same camera but returns a totally different instance;
s->dkRemoveInstance(i1);

