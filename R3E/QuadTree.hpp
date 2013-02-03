#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "SafeDelete.hpp"
#include "BoundingBox.hpp"
#include "Ray.hpp"

class QuadTree {
public:
	class Node {
	public:
		Node() : mIndex(-1), mVisible(true) {
			memset(mChildren, 0, sizeof(Node*) * 4);
		}

		~Node(){
			SAFE_DELETE(mChildren[0]);
			SAFE_DELETE(mChildren[1]);
			SAFE_DELETE(mChildren[2]);
			SAFE_DELETE(mChildren[3]);
		}

		void Create(int layers, int idx){
			mIndex = -1;
			--layers;
			if(!layers){
				mIndex = idx;
				return;
			}

			int offset = 1;
			for(int i = 1; i < layers; ++i) offset *= 4;
			for(int i = 0; i < 4; ++i){
				mChildren[i] = new Node();
				mChildren[i]->Create(layers, idx + (i * offset));
			}
		}

		bool SetIdxBox(int idx, const BoundingBox& box){
			if(!mChildren[0]){
				if(mIndex != idx) return false;
				mBoundingBox = box;
				return true;
			}

			if(mChildren[0]->SetIdxBox(idx, box)) return true;
			if(mChildren[1]->SetIdxBox(idx, box)) return true;
			if(mChildren[2]->SetIdxBox(idx, box)) return true;
			if(mChildren[3]->SetIdxBox(idx, box)) return true;

			return false;
		}

		void Calculate(){
			if(!mChildren[0]) return;

			mBoundingBox.mMin = Vector3(+99999.0f);
			mBoundingBox.mMax = Vector3(-99999.0f);
			for(int i = 0; i < 4; ++i){
				mChildren[i]->Calculate();
				mBoundingBox.AddPoint(mChildren[i]->mBoundingBox.mMin);
				mBoundingBox.AddPoint(mChildren[i]->mBoundingBox.mMax);
			}
		}

		void UpdateCulling(const Frustum& camf, bool* vlist){
			int intersect = mBoundingBox.CheckIntersectFrustum(camf);
			if(!mChildren[0]){
				mVisible = (intersect != 0);
				vlist[mIndex] = mVisible;
				return;
			}

			if(!intersect) return SetCulled(vlist);
			if(intersect == 2) return SetVisible(vlist);

			mVisible = true;

			mChildren[0]->UpdateCulling(camf, vlist);
			mChildren[1]->UpdateCulling(camf, vlist);
			mChildren[2]->UpdateCulling(camf, vlist);
			mChildren[3]->UpdateCulling(camf, vlist);
		}

		bool IsVisible() const {
			return mVisible;
		}

		const BoundingBox& GetBoundingBox() const {
			return mBoundingBox;
		}

		const Node* GetNodeContaining(const BoundingBox& bb) const {
			if(!mBoundingBox.Contains(bb)) return NULL;
			if(!mChildren[0]) return this;

			for(unsigned int i = 0; i < 4; ++i){
				const Node* ret = mChildren[i]->GetNodeContaining(bb);
				if(ret) return ret;
			}

			return this;
		}

	private:
		void SetCulled(bool* vlist){
			mVisible = false;
			if(!mChildren[0]){
				vlist[mIndex] = false;
				return;
			}

			mChildren[0]->SetCulled(vlist);
			mChildren[1]->SetCulled(vlist);
			mChildren[2]->SetCulled(vlist);
			mChildren[3]->SetCulled(vlist);
		}

		void SetVisible(bool* vlist){
			mVisible = true;
			if(!mChildren[0]){
				vlist[mIndex] = true;
				return;
			}

			mChildren[0]->SetVisible(vlist);
			mChildren[1]->SetVisible(vlist);
			mChildren[2]->SetVisible(vlist);
			mChildren[3]->SetVisible(vlist);
		}

	public:
		int mIndex;
		bool mVisible;
		BoundingBox mBoundingBox;
		Node* mChildren[4];
	};

public:
	QuadTree(int layers){
		mLayers = layers;
		mRootNode.Create(layers+1, 0);
	}

	~QuadTree(){}

	void UpdateCulling(const Frustum& camf, bool* vlist){
		mRootNode.UpdateCulling(camf, vlist);
	}

	void SetIdxBox(int idx, const BoundingBox& box){
		mRootNode.SetIdxBox(idx, box);
	}

	void Calculate(){
		mRootNode.Calculate();
	}

	bool RootVisible() const {
		return mRootNode.IsVisible();
	}

	const BoundingBox& RootBoundingBox() const {
		return mRootNode.GetBoundingBox();
	}

	const Node* GetNodeContaining(const BoundingBox& bb) const {
		return mRootNode.GetNodeContaining(bb);
	}

	int mLayers;
	Node mRootNode;
};

#endif