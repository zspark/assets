
/**
 * API demo
 */
bool Init(){
	dkengine::Init(config);/// some basic and global config settings;
	DKContent* C=dkengine::CreateContent(config);// basic config for this content;
	DKScene* s=new DKScene(); C->dkAddScene(s);
	DKCamera* c=new DKOthroCamera();
	c->dkSetViewDistance(0.1f,1000000.f);
	c->dkSetFocusLength(0.0f);
	DKInstanceCamera* i1=s->dkAddCamera(c,100,100);
	DKInstanceCamera* i2=s->dkAddCamera(c,100,100);
	DKInstanceCamera* i3=s->dkAddCamera(c,100,100);
	i1->SetFilmSize(w,h);
	i1->SetPresentPattern(EPresentPattern::NORMAL);// renders it's normals to film;
	i1->dkTranslate(0.f,1.f,0.f);
	i2->dkRotate(0.f,10.f,0.f);
	i3->dkScale(1.f,1.3f,1.f);
	i3->dkSetLookAt(xx);

	DKEntity* e=new DKEntity();
	DKGeometry* g1=new DKMesh();
	DKGeometry* g2=new DKMesh();
	DKMaterial* m=new DKMaterial();
	m->dkSetDiffuseColor(1.f,1.f,0.9f);
	m->dkSetFrenial(min,max,exponent);
	DKInstance* i4=e->dkAddGeometry(g1);
	DKInstance* i5=e->dkAddGeometry(g2);
	DKInstance* i6=s->dkAddEntity(e);
	i6->dkSetMaterial(m);


	DKGeometryGroup* ggrp=new DKGeometryGroup();
	DKInstance* i100=ggrp->dkAddSubGeometry(g1);
	DKInstance* i101=ggrp->dkAddSubGeometry(g2);
	DKEntity* e2=new DKEntity();
	DKInstance* i102=e2->dkAddGeometry(ggrp);
	i102->dkTranslate(0.1f,0.0f,0.f);
	s->dkAddEntity(e2);/// you can ignore returned DKInstance,just to use it's default settings;

	DKModel* m=nullptr;
	//called back;
	for(int i=0;i<N;++i){
		DKGeometry* g1=new DKMesh();
		g1->SetVertices(xx);
	}

	DKLight* l=new DKPointLight();
	l->dkSetColor(1.f,0.f,0.f,1.f);
	l->dkSetIntensity(10.f);

	i4->dkSetDisplayName("head.mesh");
	i4->dkSetVisibility(false);
	i5->dkSetDisplayName("body.mesh");
	i5->dkSetDisplayDescription("This is xxxxx");
	i6->dkSetMaterial(xxxxxx);
	DKInstance* i7=s->dkAddLight(l);
	s->dkRemoveLight(i7);
	return true;
}

bool Deinit(){
	delete s,c,e,g1,g2,m,l;
	dkengine::DestroyContent(C);
	dkengine::Deinit();
}

void Loop(){
	dkengine::Loop(C,time);
}

