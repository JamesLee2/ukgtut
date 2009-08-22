#pragma once

#include <irrlicht.h>

namespace irr 
{
	namespace scene
	{
		namespace jz3d 
		{

			class CCollusionMngNode : public ISceneNode
			{
			public:

				core::aabbox3d<f32> Box;
				irr::core::stringc m_strBone;
				irr::core::array<char *> m_BoneList;

				static const int TypeID = MAKE_IRR_ID('@','C','L','M');
				static const char *Name;

				CCollusionMngNode(ISceneNode* parent, ISceneManager* mgr, s32 id);				

				//! returns the axis aligned bounding box of this node
				virtual const core::aabbox3d<f32>& getBoundingBox() const;

				//! This method is called just before the rendering process of the whole scene.
				virtual void OnRegisterSceneNode();
				
				virtual void render();				

				//! Returns type of the scene node
				virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE)TypeID; }

				//! Creates a clone of this scene node and its children.
				virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

				//! Writes attributes of the scene node.
				virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

				//! Reads attributes of the scene node.
				virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);
			};
		}
	}
}
