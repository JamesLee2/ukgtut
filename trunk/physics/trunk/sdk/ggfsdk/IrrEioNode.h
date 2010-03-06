#pragma once

//��Ƽ�����챸���� WndFrame ��� �߰�
//��ġ������Ʈ ���׼��� 7.14
//����ü���� ���� �׶��̼� ȿ�� ���׼��� 2006.7.19
//����ü����½ù��׼��� (���� �÷��� ������ ��� ��½� �����Ͽ� ���)2006.8.7
//�ִϸ��̼ǽ� �����ϰ��� �ʱ�ȭ�Ǵ¹��׼��� 2006.8.17
//���� �����Ͱ��ù��׼��� 2006.8.18
//2008 1.5 : �ϸ���Ʈ 1.4���� ����

namespace irr
{
namespace scene
{
namespace scene_ggf
{
	
	class CEioSceneNode : public irr::scene::ISceneNode
	{
	protected:
		irr::video::S3DVertex Vertices[4];
		irr::video::SMaterial Material;
		//irr::core::dimension2d<irr::f32> m_ScreenSize;
		irr::core::aabbox3d<irr::f32> Box;

		irr::core::dimension2d<irr::f32> m_sizeTile;


		
	public:		

		CEioSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id)
			: irr::scene::ISceneNode(parent, mgr, id)
		{ 
			Reset();		

		}		

		~CEioSceneNode()
		{

		}

		void Reset()
		{
			Material.Wireframe = false;
			Material.Lighting = false;			

			irr::core::vector3df normal = irr::core::vector3df(0.0f,0.0f,-1.0f);			

			Vertices[0] = irr::video::S3DVertex( irr::core::vector3df(.0f,.0f,0.0f),normal,irr::video::SColor(0,255,255,255),irr::core::vector2d<irr::f32>(0.f,0.f));
			Vertices[1] = irr::video::S3DVertex( irr::core::vector3df(1.0f,.0f,0.0f),normal,irr::video::SColor(0,255,255,255),irr::core::vector2d<irr::f32>(1.f,0.f)); 
			Vertices[2] = irr::video::S3DVertex( irr::core::vector3df(1.0f,-1.0f,0.0f),normal,irr::video::SColor(0,255,255,255),irr::core::vector2d<irr::f32>(1.f,1.f));
			Vertices[3] = irr::video::S3DVertex( irr::core::vector3df(0.0f,-1.0f,0.0f),normal,irr::video::SColor(0,255,255,255),irr::core::vector2d<irr::f32>(0.f,1.f));

			Box.reset(Vertices[0].Pos);
			for (irr::s32 i=1; i<4; ++i)
				Box.addInternalPoint(Vertices[i].Pos);

		}


		/*
		���� �����ͼ���
		ã�������Ͷ�?: ��������Ʈ�� �����ϴµ� ������ �Ǵ��� ���,ȸ�� Ȯ����ҽ� �������� ��´�.
		*/
		void setReferancePoint(int rx,int ry)
		{
			irr::f32 fRx,fRy;

			fRx = ((irr::f32)rx) / m_sizeTile.Width;
			fRy = ((irr::f32)ry) / m_sizeTile.Height;

			Vertices[0].Pos = irr::core::vector3df(.0f,.0f,0.0f) + irr::core::vector3df(-fRx,fRy,0);
			Vertices[1].Pos = irr::core::vector3df(1.0f,.0f,0.0f) + irr::core::vector3df(-fRx,fRy,0);
			Vertices[2].Pos = irr::core::vector3df(1.0f,-1.0f,0.0f) + irr::core::vector3df(-fRx,fRy,0);
			Vertices[3].Pos = irr::core::vector3df(0.0f,-1.0f,0.0f) + irr::core::vector3df(-fRx,fRy,0);
			
			Box.reset(Vertices[0].Pos);
			for (irr::s32 i=1; i<4; ++i)
				Box.addInternalPoint(Vertices[i].Pos);
			
		}

		//������ ���۷��������ͷμ����Ѵ�
		void setReferancePoint_center()
		{
			setReferancePoint((irr::s32)m_sizeTile.Width/2,(irr::s32)m_sizeTile.Height/2);
		}

		virtual void setTextureCoord(int tx,int ty,int tw,int th,irr::video::ITexture *pTex = NULL)
		{
			float _tx,_ty,_th,_tw;

			if(pTex == NULL)
			{
				//pTex = Material.Textures[0];			
				pTex = getMaterial(0).getTexture(0);

			}

			if(pTex)
			{
				_tx = (irr::f32)((irr::f32)tx / (irr::f32)pTex->getSize().Width);
				_ty = (irr::f32)((irr::f32)ty / (irr::f32)pTex->getSize().Height);
				_tw = (irr::f32)((irr::f32)tw / (irr::f32)pTex->getSize().Width);
				_th = (irr::f32)((irr::f32)th / (irr::f32)pTex->getSize().Height);

				Vertices[0].TCoords = irr::core::vector2d<irr::f32>(_tx,_ty);
				Vertices[1].TCoords = irr::core::vector2d<irr::f32>(_tx + _tw,_ty);
				Vertices[2].TCoords = irr::core::vector2d<irr::f32>(_tx + _tw,_ty + _th);
				Vertices[3].TCoords = irr::core::vector2d<irr::f32>(_tx,_ty + _th);

				m_sizeTile = irr::core::dimension2d<irr::f32>((irr::f32)tw,(irr::f32)th);
				//set2DScale(1.f,1.f);
			}
		}

		void setTextureCoord(int tx,int ty,int tw,int th,irr::c8 *szTexName)
		{
			setTextureCoord(tx,ty,tw,th,SceneManager->getVideoDriver()->getTexture(szTexName));
		}		

		void Init(int tx,int ty,int tw,int th,irr::c8 *szTexName,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			irr::video::ITexture *pTex = SceneManager->getVideoDriver()->getTexture(szTexName);
			Init(tx,ty,tw,th,pTex,mtype);
		}

		virtual void Init(int tx,int ty,int tw,int th,irr::video::ITexture *pTex,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			Reset();
			setMaterialTexture(0,pTex);
			setMaterialType(mtype);		

			getMaterial(0).MaterialTypeParam = 0.01f; //����ü�ο� �������°��� ���� ���������μ���

			setTextureCoord(tx,ty,tw,th,pTex);		

			setScale(irr::core::vector3df((irr::f32)tw,(irr::f32)th,0.f));
			
		}

		
		void Init(ggf::layer::S_TileLayer *pLayer,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			Init(
				pLayer->m_CutRect[0],pLayer->m_CutRect[1],pLayer->m_CutRect[2],pLayer->m_CutRect[3],
				(irr::c8 *)pLayer->m_strTexName.c_str(),
				mtype
				);
		}
		

		/*
		3�� ��ǥ�踦 2�� ��ũ����ǥ��κ�ȯ	
		*/
		irr::core::vector3df ScreenToWorld(irr::f32 x,irr::f32 y,irr::f32 z)
		{			
			irr::core::dimension2d<irr::s32> dmScreen = SceneManager->getVideoDriver()->getScreenSize();
			return irr::core::vector3df((irr::f32)(x - (dmScreen.Width/2)),(irr::f32)((dmScreen.Height/2) - y ),z);
		}

		void set2DScreenPosition(irr::core::position2d<irr::s32> pos,irr::f32 ZDepth)
		{
			irr::core::vector3df vecScreen = ScreenToWorld((irr::f32)pos.X,(irr::f32)pos.Y,(irr::f32)ZDepth);
			setPosition(vecScreen);
			updateAbsolutePosition();		
		}			

		void set2DScreenPosition(irr::s32 x,irr::s32 y,irr::f32 zDep = 0.f)
		{
			set2DScreenPosition(irr::core::position2d<irr::s32>(x,y),zDep);			
		}

		void set2DScreenPosition(irr::core::vector2di v2Pos,irr::f32 zDep = 0.f)
		{
			set2DScreenPosition(irr::core::position2d<irr::s32>(v2Pos.X,v2Pos.Y),zDep);	

		}

		void set2DScale(irr::f32 sclx,irr::f32 scly)
		{
			//irr::core::vector3df oriScale = m_sizeTile;
			setScale(irr::core::vector3df(m_sizeTile.Width*sclx,m_sizeTile.Height*scly,0.f));
		}

		irr::core::position2d<irr::s32> get2DScreenPosition()
		{
			irr::core::dimension2d<irr::s32> dmScreen = SceneManager->getVideoDriver()->getScreenSize();
			irr::core::vector3df pos = getPosition();
			
			return irr::core::position2d<irr::s32>((irr::s32)(pos.X + (dmScreen.Width/2)),(irr::s32)((dmScreen.Height/2) - pos.Y ));
		}

		void get2DScreenPosition(irr::core::vector3df &v3pos)
		{
			irr::core::vector3df pos = getPosition();
			v3pos = irr::core::vector3df((irr::f32)get2DScreenPosition().X,(irr::f32)get2DScreenPosition().Y,pos.Z);
		}

		void get2DScreenPosition(irr::core::vector3di &v3pos)
		{
			irr::core::vector3df pos = getPosition();
			v3pos = irr::core::vector3di(get2DScreenPosition().X,get2DScreenPosition().Y,(irr::s32)pos.Z);
		}

		void get2DScreenPosition(irr::core::vector2di &v2pos)
		{
			//irr::core::vector3df pos = getPosition();
			v2pos = irr::core::vector2di(get2DScreenPosition().X,get2DScreenPosition().Y);
		}

		void get2DScreenPosition(irr::core::vector2df &v2pos)
		{
			//irr::core::vector3df pos = getPosition();
			v2pos = irr::core::vector2df((irr::f32)get2DScreenPosition().X,(irr::f32)get2DScreenPosition().Y);
		}

		
		/*
		irr::core::rect<irr::f32> getBoundRect()
		{
			irr::core::rect<irr::f32> boundRect = irr::core::rect<irr::f32>(Box.MinEdge.X,Box.MinEdge.Y,Box.MaxEdge.X,Box.MaxEdge.Y);

			boundRect.repair();
			return boundRect;
			//return irr::core::rect<irr::f32>(Box.MinEdge,Box.MaxEdge);
		}
		*/


		//virtual void OnPreRender()
		virtual void OnRegisterSceneNode()
		{  
			if (IsVisible)
			{
				//�� �̷��� �ϳ��ϸ�..���������ִ� ��� �׷�����������������
				//�̷������� ���н�Ű�������� ���İ��� �̻��ϰԳ��ü�����
				// because this node supports rendering of mixed mode meshes consisting of 
				// transparent and solid material at the same time, we need to go through all 
				// materials, check of what type they are and register this node for the right
				// render pass according to that.

				irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

				irr::video::IMaterialRenderer* rnd = 
						driver->getMaterialRenderer(Material.MaterialType);
				if(rnd && rnd->isTransparent())
				{
					SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
				}
				else
				{
					SceneManager->registerNodeForRendering(this, scene::ESNRP_SOLID);
				}
			}

			//ISceneNode::OnPreRender();
			ISceneNode::OnRegisterSceneNode();
	/*
			if (IsVisible)    
			{
				SceneManager->registerNodeForRendering(this);
			}

			//printf("prerender\n");

			ISceneNode::OnPreRender();
			*/
		}

		

		virtual void render()
		{  	
			if (IsVisible == false) return;
			
			//printf("render %d \n",getID());

			irr::u16 indices[] = { 0,1,2,3,0,2 };
			irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

			//SceneManager->setActiveCamera((irr::scene::ICameraSceneNode *) SceneManager->getSceneNodeFromId(0));
			//driver->draw2DLine(irr::core::position2d<irr::s32>(0,0),irr::core::position2d<irr::s32>(100,0));			
			//driver->draw3DLine(irr::core::vector3df(0.,0.,0.),irr::core::vector3df(100.,100.,0.));			
			
			

			driver->setMaterial(Material);			
			
			irr::core::matrix4 _TempTrans;
			_TempTrans = driver->getTransform(irr::video::ETS_WORLD);
			
			driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);			
			driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 2);

			//driver->draw3DLine(Vertices[0].Pos,Vertices[1].Pos);
			
			if(DebugDataVisible)
			{//������������
				irr::video::SMaterial _Material;


				_Material.Lighting = false;
				_Material.Wireframe = false;				

				driver->setMaterial(_Material);			
				irr::core::aabbox3df box1 = getBoundingBox();
				//irr::core::aabbox3df box2 = getTransformedBoundingBox();			
				driver->draw3DBox(box1);				
				//driver->draw3DBox(box2);
			}
			driver->setTransform(irr::video::ETS_WORLD,_TempTrans);		//���庯ȯ����	
			//SceneManager->setActiveCamera((irr::scene::ICameraSceneNode *) SceneManager->getSceneNodeFromId(1));

		}

		/*
		virtual void OnPostRender(irr::u32 timeMS)
		{
			//printf("post render %d\n",getID());
			//printf("%d \n",timeMS);
			//
			ISceneNode::OnPostRender(timeMS);
		}
		*/

		virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const  
		{    
			return Box;  
		}
		virtual irr::u32 getMaterialCount() const		
		//virtual irr::u32 getMaterialCount()
		{
			return 1;
		}
		virtual irr::video::SMaterial& getMaterial(irr::u32 i)
		{
			return Material;
		}
		



		irr::video::ITexture *getMatrialTexture()
		{
			//return Material.Textures[0];
			return getMaterial(0).getTexture(0);
		}

	};

	/*
	 �ؽ�����ü������ �־���ũ��� �����ϰ� ����� �������������ϴ� �ִϸ��̼�Ŭ����
	 ù��°�������� ��ġ �ؽ��� �󿡼� 0,0���� ������ũ�⸸ŭ�߶� �̹����̴�.
	*/
	class CEioSceneNodeA1 : public CEioSceneNode
	{
		int m_nFrame;
		int m_nTotalFrame;
		irr::u32 m_uTotalAniTick;
		irr::u32 m_uTickSave;
		irr::u32 m_uDeltaTick;

		irr::core::dimension2d<irr::s32> m_sizeCut;
		irr::core::rect<irr::s32> m_rtCut; //�ִϸ��̼��̹������߶󳾿�������
		int m_nPlayMode;		
		int m_nWork;

		std::vector<int>	m_vtAniTable;

	public:	

		enum {
			PLAY_STOP = 0,
			PLAY_ONESTOP,
			PLAY_ONESTOPHIDE,
			PLAY_LOOP
		};


		CEioSceneNodeA1(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id) :CEioSceneNode(parent,mgr,id)
		{
		}
		~CEioSceneNodeA1()
		{
		}

		/*
		rtCut �ִ� �۾� ��ȿ�̹��� ����
		sizeCut ������ �̹��� ũ��
		*/

		void Init(int nTotalFrame,irr::core::rect<irr::s32> rtCut,irr::core::dimension2d<irr::s32> sizeCut,irr::c8 *szTexName,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			m_sizeCut	= sizeCut;
			m_nFrame	= 0;
			m_nTotalFrame = nTotalFrame;			
			m_rtCut		= rtCut;

			CEioSceneNode::Init(
				rtCut.UpperLeftCorner.X,
				rtCut.UpperLeftCorner.Y,
				sizeCut.Width,sizeCut.Height,szTexName,mtype);//ù��°�����ӿ���ġ��Ŵ
			setAniMode(PLAY_STOP);
			setAniTotalTick(1000);

		}

		void Init(int nTotalFrame,irr::core::dimension2d<irr::s32> sizeCut,irr::c8 *szTexName,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			m_sizeCut = sizeCut;
			m_nFrame = 0;
			m_nTotalFrame = nTotalFrame;			

			CEioSceneNode::Init(0,0,sizeCut.Width,sizeCut.Height,szTexName,mtype);//ù��°�����ӿ���ġ��Ŵ			

			irr::video::ITexture *pTex = getMatrialTexture();
			m_rtCut = irr::core::rect<irr::s32>(0,0,pTex->getSize().Width-1,pTex->getSize().Height-1);
			setAniMode(PLAY_STOP);
			setAniTotalTick(1000);
		}		

		void Init(ggf::layer::S_AniTileLayer *pAniTile,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{

			Init(
				pAniTile->m_nTotalFrame,
				pAniTile->m_rectCut,
				irr::core::dimension2d<irr::s32>(pAniTile->m_rectFrame.getWidth(),pAniTile->m_rectFrame.getHeight()),
				(irr::c8 *)pAniTile->m_strTexName.c_str(),
				mtype
				);
		}

		irr::s32 getTotalFrame()
		{
			if(m_vtAniTable.size())
			{
				return (int)m_vtAniTable.size();
			}
			return m_nTotalFrame;
		}

		irr::s32 getCurrentFrame()
		{
			return m_nFrame;
		}

		//�ִϸ��̼��̳����°�?
		bool isEndAnimation()
		{
			if((m_nTotalFrame-1) == m_nFrame)
				return true;
			else
				return false;
		}

		

		void setAniFramePos(int nFrame,irr::core::dimension2d<irr::s32> sizeCut,irr::core::rect<irr::s32> rtCut)
		{
			m_nFrame = nFrame;
			//irr::video::ITexture *pTex;
			//pTex = getMatrialTexture();
			
			int fx;// = 
			int fy;// = pTile->m_rectCut.UpperLeftCorner.Y;
			int a1;//���γ�ħ ����

			a1 = sizeCut.Width * nFrame;

			//if( a1 >= pTile->m_rectCut.getWidth()) //���� ��ġ�����ι��������
			//if(a1 >= pTex->getSize().Width ) //���� ��ħ�� �߻��ϸ�
			if(a1 >= rtCut.getWidth() ) //���� ��ħ�� �߻��ϸ�
			{
				int nTemp;
				int nGridY;
				//nTemp	= a1 % pTex->getSize().Width;
				//nGridY	= a1 / pTex->getSize().Width;
				nTemp	= a1 % rtCut.getWidth();
				nGridY	= a1 / rtCut.getWidth();

				fx = nTemp + rtCut.UpperLeftCorner.X;
				fy = (nGridY * sizeCut.Height) + rtCut.UpperLeftCorner.Y;
			}
			else
			{
				fx = a1 + rtCut.UpperLeftCorner.X;
				fy = 0  + rtCut.UpperLeftCorner.Y;		
			}

			setTextureCoord(fx,fy,sizeCut.Width,sizeCut.Height);

			//irr::core::rect<s32> rect = pTile->m_rectFrame + irr::core::position2d<s32>(fx,fy);
		}

		void setAniFrame(int nFrame)
		{			
			irr::s32 nSize = (irr::s32)m_vtAniTable.size();//�ִ� ���̺������

			if(nSize)
			{
				if(nFrame > nSize )
				{
					nFrame = nSize-1;
				}			

				nFrame = m_vtAniTable[nFrame]; //�ε��̺�ȯ
				//printf("%d \n",nFrame);
			}
			
			setAniFramePos(nFrame,m_sizeCut,m_rtCut);
		}

		void setAniFrame(irr::u32 uDeltaTick)
		{
			irr::f32 fDiv;

			if(m_uTotalAniTick > 0)
				fDiv = (irr::f32)((irr::f32)uDeltaTick / (irr::f32)m_uTotalAniTick);
			else
				fDiv = 0;


			if( fDiv > 1.0)
			{
				fDiv = 1.0;
			}			
			
			setAniFrame((int)(fDiv*(getTotalFrame() - 1)));
			//setAniFrame((int)(fDiv*(m_nTotalFrame-1)));			
		}

		void setAniMode(int nPlayMode)
		{
			setVisible(true);
			m_nWork = 0;
			m_nPlayMode = nPlayMode;
		}

		void setAniTotalTick(irr::u32 uTick)
		{
			m_uTotalAniTick = uTick;
		}		

		virtual void OnAnimate(irr::u32 timeMS)		
		//virtual void OnPostRender(irr::u32 timeMS)
		{
			//printf("post render %d\n",getID());

			if(IsVisible )
			{
				switch(m_nPlayMode)
				{
				case PLAY_STOP:
					break;
				case PLAY_ONESTOP:
					switch(m_nWork)
					{
					case 0:
						m_uTickSave = timeMS;					
						m_nWork = 10;
						break;
					case 10:
						m_uDeltaTick = timeMS - m_uTickSave;					
						setAniFrame(m_uDeltaTick);
						if(m_uDeltaTick >= m_uTotalAniTick)
							m_nWork = 100;
						break;
					case 100:
						break;
					}
					break;
				case PLAY_ONESTOPHIDE:
					//printf("%d,%d \n",m_nFrame,m_nWork);
					switch(m_nWork)
					{
					case 0:
						m_uTickSave = timeMS;					
						m_nWork = 10;
						break;
					case 10:
						//printf("%d\n",getID());
						m_uDeltaTick = timeMS - m_uTickSave;
						setAniFrame(m_uDeltaTick);
						if(m_uDeltaTick >= m_uTotalAniTick)
							m_nWork = 100;
						break;
					case 100:
						setVisible(false);
						break;
					}			
					break;
				case PLAY_LOOP:
					switch(m_nWork)
					{
					case 0:
						m_uTickSave = timeMS;					
						m_nWork = 10;
						break;
					case 10:
						m_uDeltaTick = timeMS - m_uTickSave;
						//m_uTickSave = timeMS;		
						setAniFrame(m_uDeltaTick);
						if(m_uDeltaTick >= m_uTotalAniTick)
						{
							m_uTickSave = timeMS;								
							m_nWork = 20;

						}
						break;
					case 20:
						m_uDeltaTick = timeMS - m_uTickSave;
						if(m_uDeltaTick > (m_uTotalAniTick/m_nTotalFrame) )
						{
							m_nWork = 0;
						}					
						break;

					}
					break;				
				}
			}



			{
				//printf("post render %d\n",getID());

				if(IsVisible )
				{
					switch(m_nPlayMode)
					{
					case PLAY_STOP:
						break;
					case PLAY_ONESTOP:
						switch(m_nWork)
						{
						case 0:
							m_uTickSave = timeMS;					
							m_nWork = 10;
							break;
						case 10:
							m_uDeltaTick = timeMS - m_uTickSave;					
							setAniFrame(m_uDeltaTick);
							if(m_uDeltaTick >= m_uTotalAniTick)
								m_nWork = 100;
							break;
						case 100:
							break;
						}
						break;
					case PLAY_ONESTOPHIDE:
						//printf("%d,%d \n",m_nFrame,m_nWork);
						switch(m_nWork)
						{
						case 0:
							m_uTickSave = timeMS;					
							m_nWork = 10;
							break;
						case 10:
							//printf("%d\n",getID());
							m_uDeltaTick = timeMS - m_uTickSave;
							setAniFrame(m_uDeltaTick);
							if(m_uDeltaTick >= m_uTotalAniTick)
								m_nWork = 100;
							break;
						case 100:
							setVisible(false);
							break;
						}			
						break;
					case PLAY_LOOP:
						switch(m_nWork)
						{
						case 0:
							m_uTickSave = timeMS;					
							m_nWork = 10;
							break;
						case 10:
							m_uDeltaTick = timeMS - m_uTickSave;
							//m_uTickSave = timeMS;		
							setAniFrame(m_uDeltaTick);
							if(m_uDeltaTick >= m_uTotalAniTick)
							{
								m_uTickSave = timeMS;								
								m_nWork = 20;

							}
							break;
						case 20:
							m_uDeltaTick = timeMS - m_uTickSave;
							if(m_uDeltaTick > (m_uTotalAniTick/m_nTotalFrame) )
							{
								m_nWork = 0;
							}					
							break;

						}
						break;				
					}
				}

				//ISceneNode::OnPostRender(timeMS);
			}


			irr::scene::ISceneNode::OnAnimate(timeMS);
			//ISceneNode::OnPostRender(timeMS);
		}

		void playOneStopAnimation(irr::u32 uTotalTick)
		{
			setAniMode(PLAY_ONESTOP);
			setAniTotalTick(uTotalTick);
		}

		void playOneStopHideAnimation(irr::u32 uTotalTick)
		{
			setAniMode(PLAY_ONESTOPHIDE);
			setAniTotalTick(uTotalTick);
		}

		void playLoopAnimation(irr::u32 uTotalTick)
		{
			setAniMode(PLAY_LOOP);
			setAniTotalTick(uTotalTick);
		}

		void stopAnimation()
		{
			setAniMode(PLAY_STOP);
		}

		//! �ε��̸�� �ִϸ� �߰�
		void addFrameIndex(int nIndex)
		{
			m_vtAniTable.push_back(nIndex);
		}

		void resetFrameIndex()
		{
			m_vtAniTable.clear();
		}


	};

	/*
	�ؽ��翡 �������Ͽ� ȭ�鿡�����ٰ�� ����ϴ� 2.5����
	*/
	class CEioNodeWndFrame : public CEioSceneNode
	{
	public:
		CEioNodeWndFrame(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id) :CEioSceneNode(parent,mgr,id)
		{
		}
		~CEioNodeWndFrame()
		{
		}

		void setTextureCoord(int tx,int ty,int tw,int th,irr::video::ITexture *pTex = NULL)
		{
			float _tx,_ty,_th,_tw;

			if(pTex == NULL)
			{
				pTex = getMaterial(0).getTexture(0);
				//pTex = Material.Textures[0];
			}

			if(pTex)
			{
				_tx = (irr::f32)((irr::f32)tx / (irr::f32)pTex->getSize().Width);
				_ty = (irr::f32)((irr::f32)ty / (irr::f32)pTex->getSize().Height);
				_tw = (irr::f32)((irr::f32)tw / (irr::f32)pTex->getSize().Width);
				_th = (irr::f32)((irr::f32)th / (irr::f32)pTex->getSize().Height);
				
				
				
				Vertices[3].TCoords = irr::core::vector2d<irr::f32>(_tx,_ty);
				Vertices[2].TCoords = irr::core::vector2d<irr::f32>(_tx + _tw,_ty);
				Vertices[1].TCoords = irr::core::vector2d<irr::f32>(_tx + _tw,_ty + _th);
				Vertices[0].TCoords = irr::core::vector2d<irr::f32>(_tx,_ty + _th);

				m_sizeTile = irr::core::dimension2d<irr::f32>((irr::f32)tw,(irr::f32)th);
				set2DScale(1.f,1.f);
			}
		}

		void Init(int tx,int ty,int tw,int th,irr::video::ITexture *pTex,irr::video::E_MATERIAL_TYPE mtype = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL)
		{
			setMaterialTexture(0,pTex);
			setMaterialType(mtype);			

			setTextureCoord(tx,ty,tw,th,pTex);		

			setScale(irr::core::vector3df((irr::f32)tw,(irr::f32)th,0.f));
			
		}


	};

}
}
}
