#pragma once

/***********************************************************************
���� ��ó : http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=19725
���ռ����� ����� �Լ�����
- 2007 7.4. �ϸ���Ʈ 1.3.1�������� ���� �۵�Ȯ��
- 2008 6.26. �ϸ���Ʈ 1.4.1�������� ���� �۵�Ȯ��(����)
- 2008 8.20. DrawDebugBox �߰�
- 2008 8.26. ReverseMeshSurface �߰�
- 2009 9.10  ������� ���� �߰�
************************************************************************/

/* �ʼ� ���� ���ϵ�
#include <windows.h>
#include <stdlib.h>
#include <stdarg.h>
*/


namespace ggf
{
namespace irr_util
{

	//�־��� ��忡 �ﰢ������ ��Ͻ�Ű��
inline void ISceneNode_assignTriangleSelector(irr::scene::ISceneNode* node, irr::scene::ISceneManager* smgr, irr::io::IFileSystem* ifs) 
{ 
  if (!node || !smgr || !ifs) 
    return; 

  if (node->getType() == irr::scene::ESNT_OCT_TREE) 
  { 
    irr::io::IAttributes* attribs = ifs->createEmptyAttributes(); 
    if (attribs) 
    { 
      node->serializeAttributes(attribs); 

      // get the mesh name out 
      irr::core::stringc name = attribs->getAttributeAsString("Mesh"); 

      attribs->drop(); 

      // get the animated mesh for the object 
      irr::scene::IAnimatedMesh* mesh = smgr->getMesh(name.c_str()); 
      if (mesh) 
      { 
        irr::scene::ITriangleSelector* selector = 
          smgr->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128); 
        node->setTriangleSelector(selector); 
        selector->drop(); 
      } 
    } 
  } 
  else if (node->getType() == irr::scene::ESNT_MESH) 
  { 
    irr::scene::IMeshSceneNode* msn = (irr::scene::IMeshSceneNode*)node; 

    if (msn->getMesh()) 
    { 
      irr::scene::ITriangleSelector* selector = 
        smgr->createTriangleSelector(msn->getMesh(), msn); 
      msn->setTriangleSelector(selector); 
      selector->drop(); 
    } 
  } 
  else if (node->getType() == irr::scene::ESNT_ANIMATED_MESH) 
  { 
    irr::scene::IAnimatedMeshSceneNode* msn = (irr::scene::IAnimatedMeshSceneNode*)node; 

    irr::scene::IAnimatedMesh* am = msn->getMesh(); 
    if (am) 
    { 
      irr::scene::ITriangleSelector* selector = 
        smgr->createTriangleSelector(am->getMesh(0), msn); 
      msn->setTriangleSelector(selector); 
      selector->drop(); 
    } 
  } 
  else if (node->getType() == irr::scene::ESNT_TERRAIN) 
  { 
    irr::scene::ITerrainSceneNode* tsn = (irr::scene::ITerrainSceneNode*)node; 

    irr::scene::ITriangleSelector* selector = 
      smgr->createTerrainTriangleSelector(tsn, 0); // probably don't want lod 0 all the time... 
    tsn->setTriangleSelector(selector); 
    selector->drop(); 
  } 
  else if (node->getType() == irr::scene::ESNT_CUBE) 
  { 
    irr::scene::ITriangleSelector* selector = 
      smgr->createTriangleSelectorFromBoundingBox(node); 
    node->setTriangleSelector(selector); 
    selector->drop();    
  } 
  else 
  { 
    // not something we want to collide with 
  } 
} 

//�ڽĳ����� ��� ��ȸ�ؼ� �ﰢ������ ��Ͻ�Ű��
inline void ISceneNode_assignTriangleSelectors(irr::scene::ISceneNode* node, irr::scene::ISceneManager* smgr, irr::io::IFileSystem* ifs) 
{ 
  // assign a selector for this node 
  ISceneNode_assignTriangleSelector(node, smgr, ifs); 

  // now recurse on children... 
  irr::core::list<irr::scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin(); 
  irr::core::list<irr::scene::ISceneNode*>::ConstIterator end   = node->getChildren().end(); 

  for (/**/; begin != end; ++begin) 
    ISceneNode_assignTriangleSelectors(*begin, smgr, ifs); 
} 

//�������ڳ��� ��� ��忡 �ﰢ������ ����ϱ�
inline void ISceneManager_assignTriangleSelectors(irr::scene::ISceneManager* smgr, irr::io::IFileSystem* ifs) 
{ 
  ISceneNode_assignTriangleSelectors(smgr->getRootSceneNode(), smgr, ifs); 
} 

//�־��� ����� �ڽĳ�带 ��� ��ȸ�ϸ� �ﰢ�����ڸ� �ѵ� ������
inline void ISceneNode_gatherTriangleSelectors(irr::scene::ISceneNode* node, irr::scene::IMetaTriangleSelector* meta) 
{ 
  irr::scene::ITriangleSelector* selector = node->getTriangleSelector(); 
  if (selector) 
    meta->addTriangleSelector(selector); 

  // now recurse on children... 
  irr::core::list<irr::scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin(); 
  irr::core::list<irr::scene::ISceneNode*>::ConstIterator end   = node->getChildren().end(); 

  for (/**/; begin != end; ++begin) 
    ISceneNode_gatherTriangleSelectors(*begin, meta); 
} 

//���Ŵ������� ��� �ﰢ�����ڸ� �ѵ� ������
inline void ISceneManager_gatherTriangleSelectors(irr::scene::ISceneManager* smgr, irr::scene::IMetaTriangleSelector* meta) 
{ 
  ISceneNode_gatherTriangleSelectors(smgr->getRootSceneNode(), meta); 
} 



inline void ISceneNode_RecursiveUpdateAbsoutePosition(irr::scene::ISceneNode* pNode) 
{ 
	if (!pNode ) 
		return; 

	// Do something with the node here. 
	pNode->updateAbsolutePosition();

	// recursive call to children 
	const irr::core::list<irr::scene::ISceneNode*> & children = pNode->getChildren(); 
	irr::core::list<irr::scene::ISceneNode*>::ConstIterator it = children.begin(); 
	for (; it != children.end(); ++it) 
	{ 
		ISceneNode_RecursiveUpdateAbsoutePosition(*it); 
	} 
} 

inline void IAnimatedMeshSceneNode_RecursiveSetFrame(irr::scene::ISceneNode* pNode,irr::s32 nFrame) 
{ 
	if (!pNode ) 
		return; 

	if(pNode->getType() == irr::scene::ESNT_ANIMATED_MESH)
	{
		((irr::scene::IAnimatedMeshSceneNode*)pNode)->setCurrentFrame((irr::f32)nFrame);
	}

	
	// recursive call to children 
	const irr::core::list<irr::scene::ISceneNode*> & children = pNode->getChildren(); 
	irr::core::list<irr::scene::ISceneNode*>::ConstIterator it = children.begin(); 
	for (; it != children.end(); ++it) 
	{ 
		IAnimatedMeshSceneNode_RecursiveSetFrame(*it,nFrame); 
	} 
} 


/***********************************************************************
etc ���������ȵ�...
************************************************************************/



	/*
	�޽� ���� ��� ������ 
	�뵵��...
	ī���� �޸�..
	���Ǿյڸ� ���� �ٸ� �ؽ���� ǥ���Ҷ�..
	
	��뿹..
	
	    irr::scene::ISceneNode* pNode = 0;
		irr::scene::IAnimatedMesh* pMesh;			

		pMesh = pSmgr->addHillPlaneMesh("myHill",
			irr::core::dimension2d<irr::f32>(32,32),
			irr::core::dimension2d<irr::u32>(8,8), 
			0,0	
			);


		pMesh = pSmgr->addHillPlaneMesh("myHill_back",
			irr::core::dimension2d<irr::f32>(32,32),
			irr::core::dimension2d<irr::u32>(8,8), 
			0,0	
			);
		ReverseMeshSurface(pMesh->getMeshBuffer(0));

	*/
	inline void ReverseMeshSurface(irr::scene::IMeshBuffer *pmb)
	{
		irr::u16 *pIb = pmb->getIndices();
		irr::video::S3DVertex *pVb = (irr::video::S3DVertex *)pmb->getVertices();
		{
			irr::u32 i;
			for(i=0;i<pmb->getIndexCount();i)
			{		
				int nTemp =	pIb[i+1];
				pIb[i+1] =	pIb[i+2];
				pIb[i+2] =	nTemp;
				i+=3;
			}
		}			
	}

inline irr::core::vector3df GetWorldPosition(irr::scene::ISceneNode* pNode)
{
	irr::scene::ISceneNode *par = pNode->getParent();

	if(par)
	{
		irr::core::vector3df pos;
		irr::core::matrix4 mat = par->getRelativeTransformation();

		par = par->getParent();
		while(par)
		{
			mat = par->getRelativeTransformation() * mat;
			par = par->getParent();

		}

		mat.transformVect(pos,pNode->getPosition());			

		return pos;
	}
	else
	
		return pNode->getPosition();
	
}


//���������ǥ�ÿ� ���� �ʹ� �Ⱥ����� ��ġ�ľ��Ҷ� �ڽ��� ����
inline void DrawDebugBox(irr::video::IVideoDriver *pVideo,irr::core::vector3df pos,irr::f32 scale,irr::video::SColor color)
{
	irr::core::vector3df ptscale(scale,scale,scale);
	pVideo->draw3DBox(irr::core::aabbox3df(pos - ptscale,pos + ptscale),color);							
}

//��ġ �Ӹ��ƴ϶� ���⵵ �˰������
inline void DrawDebugAxies(irr::video::IVideoDriver *pVideo,irr::core::vector3df pos,irr::f32 scale,irr::core::quaternion Rot)
{
	irr::core::vector3df AxiesX(1,0,0);
	irr::core::vector3df AxiesY(0,1,0);
	irr::core::vector3df AxiesZ(0,0,1);

	AxiesX = Rot * AxiesX;
	AxiesY = Rot * AxiesY;
	AxiesZ = Rot * AxiesZ;

	pVideo->draw3DLine(pos,pos+AxiesX*scale,irr::video::SColor(255,255,0,0));
	pVideo->draw3DLine(pos,pos+AxiesY*scale,irr::video::SColor(255,0,255,0));
	pVideo->draw3DLine(pos,pos+AxiesZ*scale,irr::video::SColor(255,0,0,255));	
}

//�����Ϸ� �ε��� ��Ʈ������ �޽� ����
inline irr::scene::IMesh* ExtractMeshFromOctree(irr::IrrlichtDevice *pDevice,												
												irr::scene::ISceneNode *pOctNode,
												irr::core::stringc &meshName)
{
	//irr::scene::ISceneNode *pRoadRoot = CBycleApp::Get().m_System.m_pSmgr->getSceneNodeFromName(stageRes.m_strRoad.c_str());

	irr::scene::ISceneManager *pSmgr = pDevice->getSceneManager();
	irr::io::IFileSystem* ifs = pDevice->getFileSystem();

	//irr::core::list<irr::scene::ISceneNode*>::ConstIterator it = pRoadRoot->getChildren().begin(); 
	//for (; it != pRoadRoot->getChildren().end(); ++it) 
	{ 
		//if((*it)->getType() == irr::scene::ESNT_OCT_TREE)
		{
			irr::scene::ISceneNode *pNode = pOctNode;
			{ 
				irr::io::IAttributes* attribs = ifs->createEmptyAttributes(); 
				if (attribs) 
				{ 
					pNode->serializeAttributes(attribs); 

					// get the mesh name out 
					meshName = attribs->getAttributeAsString("Mesh"); 

					attribs->drop(); 

					// get the animated mesh for the object 
					irr::scene::IAnimatedMesh* mesh = pSmgr->getMesh(meshName.c_str()); 

					return mesh->getMesh(0);

					//irr::scene::CBulletObjectAnimatorGeometry geom;
					//irr::scene::CBulletObjectAnimatorParams physicsParams;


					////pNode->setMaterialFlag(irr::video::EMF_WIREFRAME,true);	
					//// add level static mesh
					//geom.type = scene::CBPAGT_CONCAVE_MESH;					
					//geom.mesh.irrMesh = NULL;//pNode->getMesh();////pMesh->getMesh(0);					
					//geom.meshFile = name;
					//physicsParams.mass = 0.0f;

					//scene::CBulletObjectAnimator* levelAnim = 
					//	m_pBulletPhysicsFactory->createBulletObjectAnimator(
					//	pSmgr,
					//	pNode,
					//	m_pWorldAnimator->getID(),
					//	&geom,
					//	&physicsParams
					//	);
					//pNode->addAnimator(levelAnim);
					//levelAnim->drop();

				} 
			}				
		}				
	} 	
	meshName = "";
	return NULL;
}


/*
�����;��� ��ŷ�� �ﰢ��,��ġ,���ε��� ���
*/
inline bool Mesh_RayHitNodeTriangles (irr::scene::IAnimatedMeshSceneNode* Node,
							   irr::core::line3df Ray,
							   irr::core::vector3df &intersection,
							   irr::core::triangle3df &triangle,
							   irr::u32 &FaceIndex)
{ 
	irr::u32 nMeshBuffer = 0; 
	int v_index[3] = {0,0,0}; 
	//irr::core::triangle3df triangle; 
	irr::scene::IMeshBuffer *mesh_buffer = NULL; 
	irr::scene::IAnimatedMesh* mesh = Node->getMesh();
	if (Node->getTransformedBoundingBox().intersectsWithLine(Ray)) 
	{ 
		int frame=(int)Node->getFrameNr(); 
		irr::scene::IMesh* irr_mesh=mesh->getMesh(frame); 
		for( nMeshBuffer=0 ; nMeshBuffer < irr_mesh->getMeshBufferCount() ; nMeshBuffer++ ) 
		{ 
			mesh_buffer = irr_mesh->getMeshBuffer(nMeshBuffer); 
			irr::video::S3DVertex *vertices = (irr::video::S3DVertex*)mesh_buffer->getVertices(); 
			irr::u16 *indices = mesh_buffer->getIndices(); 
			
			irr::u32 i;
			for(i=0; i<mesh_buffer->getIndexCount()/3; i++) 
			{ 
				v_index[0] = indices[i*3 ]; 
				v_index[1] = indices[i*3+1]; 
				v_index[2] = indices[i*3+2]; 
				triangle.pointA=vertices[ v_index[0] ].Pos; 
				triangle.pointB=vertices[ v_index[1] ].Pos; 
				triangle.pointC=vertices[ v_index[2] ].Pos; 
				Node->getAbsoluteTransformation().transformVect(triangle.pointA); 
				Node->getAbsoluteTransformation().transformVect(triangle.pointB); 
				Node->getAbsoluteTransformation().transformVect(triangle.pointC); 
				if (triangle.getIntersectionWithLine(Ray.start,Ray.end,intersection)) 
				{ 
					/*driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4()); 
					material.Lighting=false; 
					material.setTexture(0,0); 
					driver->setMaterial(material); 
					driver->draw3DTriangle(triangle, irr::video::SColor(255,255,0,0)); */
					FaceIndex = i;
					return true; 
				} 
			} 
		} 
	} 
	return false; 
} 

/*
����� �α� �Լ�
pirrDevice :  �ΰ��ϰ�쿡�� ������ ����Լ� �ٷ����
������ printf�� ����

*/
inline void DebugOutputFmt(irr::IrrlichtDevice *pirrDevice, const char *format,...)
{
#ifdef _DEBUG
	va_list ap;

	va_start(ap,format);

	static char szBuf[256];		
	//sprintf_s(szBuf,format,ap);
	_vsnprintf_s(szBuf, 256, format, ap );

	if(pirrDevice)
	{
		pirrDevice->getLogger()->log(szBuf);

	}
	else		
		OutputDebugStringA(szBuf);

	va_end(ap);
#endif

}

//inline void Octree2Bullet(irr::IrrlichtDevice *pDevice
//					irr::scene::CBulletAnimatorManager *pBamgr,
//					irr::scene::CBulletWorldAnimator*pWorldAnimator,
//					irr::scene::ISceneNode *pRootNode)						
//{
//
//}
//
//
	
}
}


