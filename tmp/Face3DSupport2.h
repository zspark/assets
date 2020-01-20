
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
			m_tree.Insert(nullptr,&m_nodeUpdateOnce,xxx);
			m_tree.Insert(nullptr,&m_nodeUpdatePermanent,xxx);
			m_tree.Insert(nullptr,&m_nodeScenes,xxx);
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
		HSNode m_nodeEngineRoot;
		HSNode m_nodeUpdateOnce;
		HSNode m_nodeUpdatePermanent;
		HSNode m_nodeScenes;
};

