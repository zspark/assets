
//==================================================================================================
//
//API
//

M_ABSTRACT class DKBaseGeometry:public DKBaseObject{
	public:
		DKBaseGeometry();
		virtual ~DKBaseGeometry();
	private:
}

/*
class DKBaseGeometryGroup:public DKBaseGeometry{
	public:
		void AddSubGeometry(DKBaseGeometry*);
		void RemoveSubGeometry(DKBaseGeometry*);
		void Clear(bool deleteall);
		const int GetCount()const;
	private:
		HSNode* m_rootNode;
};
*/

class DKSphere:public DKBaseGeometry{}
class DKPointSet:public DKBaseGeometry{
	public:
		DKPointSet();
		~DKPointSet();
		void AppendPoint(f3&&);
		void AppendPoints(const std::vector<f3>& vecPoint);
		void AppendPoints(initialize_list<float>&&);
		void Clear();
		void operater<<(float f);
	private:
		dkfloat* m_points;
}
class DKMesh:public DKBaseGeometry{
	public:
		void SetIndices(dkuint* pos,dkuint len);
		void SetVertices(float* pos,dkuint len);
		void SetDrawMode(EDrawMode);/// line line_strip triangle fan etc.
	private:
}

class DKEntity final:public DKBaseObject{
	public:
		DKEntity();
		~DKEntity();
		void SetMaterial(DKMaterial&);
		void AddGeometry(DKBaseGeometry&);
	private:
		HSNode m_root;
}

//==================================================================================================
//
//implementation;
//

void DKPointSet::operater<<(float f){
	m_points[i]=f;
}

