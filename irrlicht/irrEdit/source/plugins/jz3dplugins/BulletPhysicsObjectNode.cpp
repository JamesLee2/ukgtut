#include "jz3d.h"
#include "BulletPhysicsObjectNode.h"



namespace irr 
{
	namespace scene
	{
		extern const c8* const CBulletObjectAnimatorGeometryTypesNames[];

		const c8* const PhysicsObjectTypeNames[] =
		{			
			"bulletObject_static",
			"bulletObject_dynamic",
			"bulletObjectFpsCtrl",
			"bulletObjectCharacterCtrl",
			NULL
		};

		namespace jz3d 
		{	
			const char *CBulletPhysicsObjectNode::Name  = "CBulletPhysicsObjectNode";

			CBulletPhysicsObjectNode::CBulletPhysicsObjectNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
				: ISceneNode(parent,mgr,id),
				m_WorldID(0)
			{
#ifdef _DEBUG
				setDebugName(CBulletPhysicsObjectNode::Name);
#endif
				setAutomaticCulling(scene::EAC_OFF);

				m_strShape = irr::scene::CBulletObjectAnimator::getGeometryTypesNames()[0];
				m_strPhysicsObjectTypeName = PhysicsObjectTypeNames[0];
			}


			//! pre render event
			void CBulletPhysicsObjectNode::OnRegisterSceneNode()
			{
				if (IsVisible)
					SceneManager->registerNodeForRendering(this);

				ISceneNode::OnRegisterSceneNode();
			}


			//! render
			void CBulletPhysicsObjectNode::render()
			{
				// do nothing
				if ( DebugDataVisible & scene::EDS_BBOX )
				{			
					video::IVideoDriver* driver = SceneManager->getVideoDriver();

					//재질설정 
					{
						irr::video::SMaterial m;
						m.Lighting = false;
						m.ZWriteEnable = true;																	
						driver->setMaterial(m);
					}

					irr::video::SColor draw_color(255,255,0,0);						

					driver->setTransform(video::ETS_WORLD,getCollisionShapeTransform());

					{
						if(m_GeometryInfo.type == CBPAGT_BOX)
						{
							irr::core::vector3df box_size =
								irr::core::vector3df(m_GeometryInfo.box.X ,m_GeometryInfo.box.Y ,m_GeometryInfo.box.Z );

							irr::core::aabbox3df box(										
								-(box_size/2),
								(box_size/2)
								);									

							driver->draw3DBox(box,draw_color);
						}
						else if(m_GeometryInfo.type == CBPAGT_CAPSULE)
						{
							irr::core::quaternion qtY(0, 45*irr::core::DEGTORAD, 0);

							//btCapsuleShape* pCapsuleShape = (btCapsuleShape*)pShape;
							irr::video::IVideoDriver *pVideo = driver;

							btScalar radius =  m_GeometryInfo.Capsule.radius;//pCapsuleShape->getRadius();
							btScalar halfheight = m_GeometryInfo.Capsule.hight * .5f;//pCapsuleShape->getHalfHeight();

							irr::core::vector3df pointUP( irr::core::vector3df(radius, halfheight, 0) );
							irr::core::vector3df pointBottom( irr::core::vector3df(radius, -halfheight, 0) );
							irr::core::vector3df rotAxis(0,0,1);

							for (int i=0; i<8; i++)
							{
								irr::core::vector3df pointUP2 = qtY*pointUP;

								//캡슐의 몸통부분(실린더) 그리기
								pVideo->draw3DLine(pointUP, pointUP2, draw_color); //윗선그리기
								irr::core::vector3df pointBottom2 = qtY*pointBottom;
								pVideo->draw3DLine(pointBottom, pointBottom2, draw_color); //아랫선 그리기
								pVideo->draw3DLine(pointUP, pointBottom, draw_color); //옆선그리기

								{
									//캡슐의 양쪽 둥근부분 그리기
									irr::core::vector3df pointS_UP = pointUP-irr::core::vector3df(0, halfheight, 0);
									irr::core::vector3df pointS_Bottom = pointBottom+irr::core::vector3df(0, halfheight, 0);
									irr::core::quaternion qtZ, qtZ_I;
									qtZ.fromAngleAxis(30*irr::core::DEGTORAD, rotAxis);
									qtZ_I.fromAngleAxis(-30*irr::core::DEGTORAD, rotAxis);
									for (int j=0; j<5; j++)
									{
										irr::core::vector3df pointS2 = qtZ*pointS_UP; //상단반구
										pVideo->draw3DLine(
											pointS_UP+irr::core::vector3df(0, halfheight, 0), 
											pointS2+irr::core::vector3df(0, halfheight, 0), 
											draw_color);
										pointS_UP = pointS2;

										irr::core::vector3df pointS3 = qtZ_I*pointS_Bottom; //하단반구
										pVideo->draw3DLine(
											pointS_Bottom-irr::core::vector3df(0, halfheight, 0), 
											pointS3-irr::core::vector3df(0, halfheight, 0), 
											draw_color);
										pointS_Bottom = pointS3;
									}
								}

								rotAxis =qtY*rotAxis;
								pointUP = pointUP2;
								pointBottom = pointBottom2;
							}
						}
					}

				}				
			}


			//! returns the axis aligned bounding box of this node
			const core::aabbox3d<f32>& CBulletPhysicsObjectNode::getBoundingBox() const
			{
				return Box;
			}

			//! Creates a clone of this scene node and its children.
			ISceneNode* CBulletPhysicsObjectNode::clone(ISceneNode* newParent, ISceneManager* newManager)
			{
				if (!newParent) newParent = Parent;
				if (!newManager) newManager = SceneManager;

				CBulletPhysicsObjectNode* nb = new CBulletPhysicsObjectNode(newParent, 
					newManager, ID);

				nb->cloneMembers(this, newManager);
				nb->Box = Box;				

				nb->m_strShape = m_strShape;
				nb->m_GeometryInfo = m_GeometryInfo;
				nb->m_PhysicsParam = m_PhysicsParam;

				nb->drop();
				return nb;
			}		


			void CBulletPhysicsObjectNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
			{
				ISceneNode::serializeAttributes(out, options);		

				

				//데이터 쓰기 코드추가
				//...

				out->addEnum("PhysicsType",
					irr::core::stringc(m_strPhysicsObjectTypeName.c_str()).c_str(),
					PhysicsObjectTypeNames
					);

				if(m_strPhysicsObjectTypeName == L"bulletObject_dynamic")
				{

					out->addInt("worldID",m_WorldID);

					out->addFloat("mass",m_PhysicsParam.mass);
					out->addFloat("ccdThreshold",m_PhysicsParam.ccdThreshold);
					out->addFloat("linearDamping",m_PhysicsParam.linearDamping);
					out->addFloat("angularDamping",m_PhysicsParam.angularDamping);
					out->addFloat("friction",m_PhysicsParam.friction);
					out->addFloat("restitution",m_PhysicsParam.restitution);

					out->addVector3d("gravity",m_PhysicsParam.gravity);
					out->addVector3d("centerOfMassOffset",m_PhysicsParam.centerOfMassOffset);

					out->addEnum("Shape",
						irr::core::stringc(m_strShape.c_str()).c_str(),
						irr::scene::CBulletObjectAnimator::getGeometryTypesNames()
						);


					if(m_strShape == L"none")
					{
					}
					else if(m_strShape == L"sphere")
					{
						out->addFloat("radius",m_GeometryInfo.sphere.radius);
					}
					if(m_strShape == L"box")
					{
						out->addVector3d("box_size",irr::core::vector3df(m_GeometryInfo.box.X ,m_GeometryInfo.box.Y ,m_GeometryInfo.box.Z ));
					}
					else if(m_strShape == L"cylinder")
					{

					}
					else if(m_strShape == L"capsule")
					{
						out->addFloat("height",m_GeometryInfo.Capsule.hight);
						out->addFloat("radius",m_GeometryInfo.Capsule.radius);
					}
					else if(m_strShape == L"cone")
					{
					}
					else if(m_strShape == L"plane")
					{
					}
					else if(m_strShape == L"convex")
					{
					}
					else if(m_strShape == L"concave")
					{
					}
					else if(m_strShape == L"concave_gimpact")
					{					
						out->addString("meshFile", m_GeometryInfo.meshFile.c_str());
					}
					else if(m_strShape == L"concave_Terrain")
					{
						out->addInt("meshLod", m_GeometryInfo.terrain_mesh.meshLod);
					}		
				}
				else if(m_strPhysicsObjectTypeName == L"bulletObject_static")
				{
					out->addInt("worldID",m_WorldID);

					//out->addFloat("mass",m_PhysicsParam.mass);
					//out->addFloat("ccdThreshold",m_PhysicsParam.ccdThreshold);
					//out->addFloat("linearDamping",m_PhysicsParam.linearDamping);
					//out->addFloat("angularDamping",m_PhysicsParam.angularDamping);
					out->addFloat("friction",m_PhysicsParam.friction);
					out->addFloat("restitution",m_PhysicsParam.restitution);
					//out->addVector3d("gravity",m_PhysicsParam.gravity);
					out->addVector3d("centerOfMassOffset",m_PhysicsParam.centerOfMassOffset);

					out->addEnum("Shape",
						irr::core::stringc(m_strShape.c_str()).c_str(),
						irr::scene::CBulletObjectAnimator::getGeometryTypesNames()
						);


					if(m_strShape == L"none")
					{
					}
					else if(m_strShape == L"sphere")
					{
						out->addFloat("radius",m_GeometryInfo.sphere.radius);
					}
					if(m_strShape == L"box")
					{
						out->addVector3d("box_size",irr::core::vector3df(m_GeometryInfo.box.X ,m_GeometryInfo.box.Y ,m_GeometryInfo.box.Z ));
					}
					else if(m_strShape == L"cylinder")
					{

					}
					else if(m_strShape == L"capsule")
					{
						out->addFloat("height",m_GeometryInfo.Capsule.hight);
						out->addFloat("radius",m_GeometryInfo.Capsule.radius);
					}
					else if(m_strShape == L"cone")
					{
					}
					else if(m_strShape == L"plane")
					{
					}
					else if(m_strShape == L"convex")
					{
					}
					else if(m_strShape == L"concave")
					{
					}
					else if(m_strShape == L"concave_gimpact")
					{					
						out->addString("meshFile", m_GeometryInfo.meshFile.c_str());
					}
					else if(m_strShape == L"concave_Terrain")
					{
						out->addInt("meshLod", m_GeometryInfo.terrain_mesh.meshLod);
					}	
				}
				else if(m_strPhysicsObjectTypeName == L"bulletObjectFpsCtrl")
				{
				}
				else if(m_strPhysicsObjectTypeName == L"bulletObjectCharacterCtrl")
				{
				}
				

			}

			//! Reads attributes of the scene node.
			void CBulletPhysicsObjectNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
			{	
				//데이터 읽기 코드추가
				//...

				//out->addInt("worldID",m_WorldID);

				m_strPhysicsObjectTypeName = irr::core::stringw(in->getAttributeAsEnumeration("PhysicsType"));

				if(m_strPhysicsObjectTypeName == L"bulletObject_dynamic")
				{

					m_WorldID = in->getAttributeAsInt("worldID");
					m_PhysicsParam.mass = in->getAttributeAsFloat("mass");
					m_PhysicsParam.ccdThreshold = in->getAttributeAsFloat("ccdThreshold");
					m_PhysicsParam.linearDamping = in->getAttributeAsFloat("linearDamping");
					m_PhysicsParam.angularDamping = in->getAttributeAsFloat("angularDamping");
					m_PhysicsParam.friction = in->getAttributeAsFloat("friction");
					m_PhysicsParam.restitution = in->getAttributeAsFloat("restitution");
					m_PhysicsParam.gravity = in->getAttributeAsVector3d("gravity");
					m_PhysicsParam.centerOfMassOffset = in->getAttributeAsVector3d("centerOfMassOffset");				

					m_strShape = irr::core::stringw(in->getAttributeAsEnumeration("Shape"));

					if(m_strShape == L"sphere")
					{
						m_GeometryInfo.sphere.radius = in->getAttributeAsFloat("radius");
						m_GeometryInfo.type = CBPAGT_SPHERE;
						m_spColShape = std::tr1::shared_ptr<btCollisionShape>(new btSphereShape(m_GeometryInfo.sphere.radius));
					}
					else if(m_strShape == L"box")
					{
						irr::core::vector3df box_size = in->getAttributeAsVector3d("box_size");
						m_GeometryInfo.box.X = box_size.X;
						m_GeometryInfo.box.Y = box_size.Y;
						m_GeometryInfo.box.Z = box_size.Z;
						m_GeometryInfo.type = CBPAGT_BOX;

						btVector3 btbox;

						Irrlicht2Bullet(box_size,btbox);

						m_spColShape = std::tr1::shared_ptr<btCollisionShape>(new btBoxShape(btbox));
					}
					else if(m_strShape == L"capsule")
					{
						m_GeometryInfo.Capsule.hight = in->getAttributeAsFloat("height");
						m_GeometryInfo.Capsule.radius = in->getAttributeAsFloat("radius");
						m_GeometryInfo.type = CBPAGT_CAPSULE;

						m_spColShape = std::tr1::shared_ptr<btCollisionShape>(new btCapsuleShape(m_GeometryInfo.Capsule.radius,
							m_GeometryInfo.Capsule.hight)
							);

					}
				}
				else if(m_strPhysicsObjectTypeName == L"bulletObject_static")
				{
					m_WorldID = in->getAttributeAsInt("worldID");
					m_PhysicsParam.mass = 0;//in->getAttributeAsFloat("mass");
					m_PhysicsParam.ccdThreshold = 0;//in->getAttributeAsFloat("ccdThreshold");
					m_PhysicsParam.linearDamping = 0;//in->getAttributeAsFloat("linearDamping");
					m_PhysicsParam.angularDamping = 0;//in->getAttributeAsFloat("angularDamping");
					m_PhysicsParam.friction = in->getAttributeAsFloat("friction");
					m_PhysicsParam.restitution = in->getAttributeAsFloat("restitution");
					//m_PhysicsParam.gravity = irr::core::vector3df//in->getAttributeAsVector3d("gravity");
					//m_PhysicsParam.centerOfMassOffset = in->getAttributeAsVector3d("centerOfMassOffset");				

					m_strShape = irr::core::stringw(in->getAttributeAsEnumeration("Shape"));

					if(m_strShape == L"sphere")
					{
						m_GeometryInfo.sphere.radius = in->getAttributeAsFloat("radius");
						m_GeometryInfo.type = CBPAGT_SPHERE;
						m_spColShape = std::tr1::shared_ptr<btCollisionShape>(new btSphereShape(m_GeometryInfo.sphere.radius));
					}
					else if(m_strShape == L"box")
					{
						irr::core::vector3df box_size = in->getAttributeAsVector3d("box_size");
						m_GeometryInfo.box.X = box_size.X;
						m_GeometryInfo.box.Y = box_size.Y;
						m_GeometryInfo.box.Z = box_size.Z;
						m_GeometryInfo.type = CBPAGT_BOX;

						btVector3 btbox;

						Irrlicht2Bullet(box_size,btbox);

						m_spColShape = std::tr1::shared_ptr<btCollisionShape>(new btBoxShape(btbox));
					}
					else if(m_strShape == L"capsule")
					{
						m_GeometryInfo.Capsule.hight = in->getAttributeAsFloat("height");
						m_GeometryInfo.Capsule.radius = in->getAttributeAsFloat("radius");
						m_GeometryInfo.type = CBPAGT_CAPSULE;

						m_spColShape = std::tr1::shared_ptr<btCollisionShape>(new btCapsuleShape(m_GeometryInfo.Capsule.radius,
							m_GeometryInfo.Capsule.hight)
							);

					}

				}

				ISceneNode::deserializeAttributes(in, options);				
			}
			bool CBulletPhysicsObjectNode::register2BulletPhysicsWorld(btCollisionWorld *pWorld)
			{
				if(pWorld == NULL)
				{
					irr::s32 TypeID = ESNAT_BULLET_OBJECT;
					//씬노드 펙토리찾기
					irr::scene::CBulletAnimatorManager *ani_factory =
						static_cast<irr::scene::CBulletAnimatorManager *>(ggf::util::findAnimatorFactory(SceneManager,TypeID));
					if(ani_factory)
					{
						pWorld = ani_factory->getBulletWorldByID(m_WorldID)->getWorld();
						if(!pWorld)
						{
							return false;
						}
					}
				}

				if(m_strPhysicsObjectTypeName == L"bulletObject_static")
				{

				}
				return true;
			}
		}
	}
}
