#ifndef TRANSFORM_H
#define TRANSFORM_H

//have imgui show the hierarchy?

class transform
{
public:

	transform()
	{
		//no parent
		iterEvent = nullptr;
		parent = worldRoot.get();
		//root = nullptr;
		hierarchyCount = 0;

		position = glm::vec4(0);
		rotation = glm::quat();
		scale = glm::vec4(1);
		eulerAngles = glm::vec4(0);

		localPosition = glm::vec4(0);
		localRotation = glm::quat();
		localScale = glm::vec4(0);
		localEulerAngles = glm::vec4(0);
		
		lossyScale = 1;

		right = glm::conjugate(rotation) * globalRight;
		forward = glm::conjugate(rotation) * globalForward;
		up = glm::conjugate(rotation) * globalUp;
		childIter = children.begin();

		//setup space matrices
		UpdateSpaceMatrices();
	}

	transform(transform* parent)
	{
		//no parent
		iterEvent = nullptr;
		this->parent = parent;
		//root.reset(this);
		hierarchyCount = 0;

		position = glm::vec4(0);
		rotation = glm::quat();
		scale = glm::vec4(1);
		eulerAngles = glm::vec4(0);

		localPosition = glm::vec4(0);
		localRotation = glm::quat();
		localScale = glm::vec4(0);
		localEulerAngles = glm::vec4(0);

		lossyScale = 1;

		right = glm::conjugate(rotation) * globalRight;
		forward = glm::conjugate(rotation) * globalForward;
		up = glm::conjugate(rotation) * globalUp;
		childIter = children.begin();

		//setup space matrices
		UpdateSpaceMatrices();
	}

	enum class space_t
	{
		local,
		world
	};

	//setters and getters

	//global
	glm::vec4 GetPosition() { return position; }	
	glm::quat GetRotation()	{ return rotation; }	
	glm::vec4 GetScale() { return scale; }	
	glm::vec4 GetEulerAngles() { return eulerAngles; }

	void SetPosition(glm::vec4 position) 
	{ 
		this->position = position; 
		//set local position

		//update transformation matrices when these are updated. 
		//only if this has a parent. or just do it anyway?
	}
	void SetRotation(glm::quat rotation) 
	{ 
		this->rotation = rotation; 
		//set local rotation
	}
	void SetScale(glm::vec4 scale) 
	{ 
		this->scale = scale; 
		this->localScale = scale * worldToLocalMatrix;
		//set local scale
	}
	void SetEulerAngles(glm::vec4 eulerAngles) 
	{ 
		this->eulerAngles = eulerAngles;
		//set local euler angles
	}

	//local 
	glm::vec3 GetLocalPosition() { return localPosition; }
	glm::quat GetLocalRotation() { return localRotation; }
	glm::vec3 GetLocalScale() { return scale; }
	glm::vec3 GetLocalEulerAngles() { return localEulerAngles; }

	void SetLocalPosition(glm::vec4 localPosition) 
	{ 
		this->localPosition = localPosition;
		//set global position
	}
	void SetLocalRotation(glm::quat localRotation)
	{
		this->localRotation = localRotation; 
		//set global rotation
	}
	void SetLocalScale(glm::vec4 localScale) 
	{
		this->localScale = localScale; 
		//set global scale
	}
	void SetLocalEulerAngles(glm::vec4 localEulerAngles) 
	{ 
		this->localEulerAngles = localEulerAngles;
		//set global euler scales
	}

//private:

	//use smart pointers later
	std::vector<std::unique_ptr<transform>> children; //should only children have unique pointers?
	std::vector<std::unique_ptr<transform>>::iterator childIter;

	static const std::unique_ptr<transform> worldRoot; //or just make these children or root

	transform* parent;
	//std::unique_ptr<transform> root;
	unsigned int hierarchyCount;

	//global
	glm::vec4 position;		//hmm. operator overloading to automatically change local position when global has been changed?
	glm::quat rotation;
	glm::vec4 scale;
	glm::vec4 eulerAngles;	

	//local members
	glm::vec4 localPosition;
	glm::quat localRotation;
	glm::vec4 localScale;
	glm::vec4 localEulerAngles;

	//conversion matrices
	glm::mat4 localToWorldMatrix;// change when P/S/R has changed? when to change? assuming this has a parent?
	glm::mat4 worldToLocalMatrix;

	//angles
	static const glm::vec4 globalRight;// = glm::vec3(1, 0, 0);
	static const glm::vec4 globalForward;// = glm::vec3(0, 0, 1);
	static const glm::vec4 globalUp;// = glm::vec3(0, 1, 0);

	glm::vec4 right;
	glm::vec4 forward;
	glm::vec4 up;

	float lossyScale;//global scale of object as a scalar?

	using iterEvent_t = std::function<void(transform* current)>;
	iterEvent_t iterEvent;

	using IterFunction = void (transform::*)(transform* current);

	//also keep a local vector of transform pointers so we don't have to iterate back down from root
	std::vector<transform*> trimmedTree;

	void DetachSelf(transform* newParent)
	{
		if(newParent == nullptr)
		{
			newParent = worldRoot.get();
		}
		//change parent to root
		parent = worldRoot.get();
		//this->root.reset(newRoot); //root should never change?
		//remove self from parent vector
		int selfID = GetSiblingIndex();

		transform* newChild = parent->children[selfID].release();

		//should be free to delete the child now
		parent->children.erase(parent->children.begin() + selfID);

		//now move the new child into newparent's children
		newParent->children.push_back(std::make_unique<transform>(newChild));

		//iterate though all of its children and update matrices?
		//or only make a ~worldToLocal matrix when needed?
		//IterateThroughChildren(this, &transform::SetRootProc);
	}

	//detach all immediate children
	void DetachChildren()
	{
		//just step through the top most layer of children
		for (auto& iter : children)
		{
			//make parent worldRoot
			iter->DetachSelf(worldRoot.get());
		}

		children.clear(); //this should remove all the empty/useless pointers
	}

	transform* GetChild(size_t childIndex)
	{
		//we just need to return a reference
		return children[childIndex].get();
	}

	int GetSiblingIndex()
	{
		if (parent == nullptr)
		{
			return -1;
		}
		int index = 0;
		//iterate through the parent and check if one of the parent children is this object
		for (auto& iter : parent->children)
		{
			if (iter.get() == this) //if the iter is also pointing to this object then return the index
			{
				return index;
			}
			index++;
		}
	}

	//set sibling index
	void SetSiblingIndex(size_t newIndex)
	{
		//swap where I currently am in the parent list and where I want to be
		std::iter_swap(parent->children.begin() + GetSiblingIndex(), parent->children.begin() + newIndex);
	}

	//I don't remember why these were here tbh
	void SetAsFirstSibling()
	{
		//just swap for now. figure out how to re-order later
		std::iter_swap(parent->children.begin(), parent->children.begin() + GetSiblingIndex());
	}

	void SetAsLastSibling()
	{
		//just swap for now
		std::iter_swap(parent->children.end(), parent->children.begin() + GetSiblingIndex());
	}

	void SetParent(transform* newParent)
	{
		//if this already has a parent then we need to decouple it from previous parent
		if (parent != nullptr)
		{
			//transfer pointer ownership
			DetachSelf(newParent);
		}

		parent = newParent;

		//ok set the local PRS to be relative to the new parent
		localPosition = parent->position - position;
		localScale = parent->scale - scale; //what if they're both vec3(1)?


		//have parent push back this object into it's children
		//parent->children.push_back(std::move(this));
		//we need to shift pointer ownership. from global transform list?

	}

	bool IsChildOf(transform* parent)
	{
		for (auto& iter : parent->children)
		{
			//if the iter is also pointing to this object then return the index
			if (iter.get() == this) 
			{
				return true;
			}
		}

		return false;
	}

	void IterateThroughChildren(transform* newRoot, IterFunction pFun)
	{
		//for every child, set their callback to whatever is passed in and call it
		for (size_t iter = 0; iter < children.size(); iter++)
		{
			//bind the function pointer to the current child iter
			//iterEvent = std::bind(pFun, children[iter], _1);
			if (iterEvent != nullptr)
			{
				//call the event and then use recursion
				iterEvent(newRoot); //there is a chance that it only works for the original node, resetting root for this object over and over
				children[iter]->iterEvent = this->iterEvent;
			}
			IterateThroughChildren(children[iter].get(), pFun);
			//reset the callback to nullptr
			children[iter]->iterEvent = nullptr;
		}
	}

	void IterateToRoot(transform* original, IterFunction pFun)
	{
		//if parent is root then don't bother
		while(parent != worldRoot.get())
		{
			iterEvent = std::bind(pFun, this, _1);
			if(iterEvent != nullptr)
			{
				iterEvent(parent);
				parent->iterEvent = this->iterEvent;
			}

			IterateToRoot(original, pFun);
			parent->iterEvent = nullptr;
		}
	}

	void RootToNode(IterFunction pFun)
	{
		//go backwards?
		for(size_t iter = trimmedTree.size() - 1; iter > 0; iter--)
		{
			trimmedTree[iter]->UpdateWorldPRS(this);
		}
	}

	/*void SetRootProc(transform* root)
	{
		//this->root = root;
	}*/

	void RecursiveUpdate(transform* original)
	{
		trimmedTree.push_back(parent); //just need the critical path back to root?
		IterateToRoot(original, &transform::UpdateLocalPRS);
		RootToNode(&transform::UpdateWorldPRS);
	}

	//maths section

	//Sets the world space position and rotation of the Transform component.
	void SetWorldPositionAndRotation(glm::vec4 position, glm::quat rotation)
	{
		this->position = position;
		this->rotation = rotation;

		//TODO: also update the localToWorld matrices of the children.
		//also update the locals if parent != worldRoot;
	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(float x, float y, float z)
	{
		//position += arguments in wold space
		position += glm::vec4(x, y, z, 1);

		//need to update local position
		//and the local/world positions of all children
	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(float x, float y, float z, space_t relativeTo = space_t::local)
	{
		if (relativeTo == space_t::local)
		{
			localPosition += glm::vec4(x, y, z, 0);
			//TODO: update world position, etc
		}

		else
		{
			position += glm::vec4(x, y, z, 0);
			//TODO: update local position, etc.
		}

	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(float x, float y, float z, transform* relativeTo)
	{
		//position += relativeTo.position - glm::vec3(x, y, z)

		//...ok first get our position in relation to the argument?
		//or create a vec3 for translation values relative to argument?
		//make an augmented localToWorld?
		//or multiply the vec3 by arg's world-to-local and apply it to me. then update matrices?
	}

	//Moves the transform in the direction and distance of translation. -- in world space
	void Translate(glm::vec3 translation)
	{
		//move in global space?
		position += glm::vec4(translation.x, translation.y, translation.z, 0);
		//TODO: update local position, etc.

	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(glm::vec3 translation, space_t relativeTo = space_t::local)
	{
		switch (relativeTo)
		{
			case space_t::local:
			{
				localPosition += glm::vec4(translation.x, translation.y, translation.z, 0);
				break;
			}

			case space_t::world:
			{
				position += glm::vec4(translation.x, translation.y, translation.z, 0);
				break;
			}
		}
	}

	//Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as a Euler angle and not a Quaternion. --
	void Rotate(glm::vec3 eulerAngles, space_t relativeTo = space_t::local)
	{
		rotation = glm::rotate(rotation, eulerAngles);
	}

	//Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as a Euler angle and not a Quaternion. --
	void Rotate(float xAngle, float yAngle, float zAngle, space_t relativeTo = space_t::local)
	{
		rotation = glm::rotate(rotation, glm::vec3(xAngle, yAngle, zAngle));
	}

	//Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as a Euler angle and not a Quaternion. --
	void Rotate(glm::vec3 axis, float angle, space_t relativeTo = space_t::local)
	{
		rotation = glm::rotate(rotation, angle, axis);
	}

	//Rotates the transform about axis passing through point in world coordinates by angle degrees. --
	void RotateAround(const glm::vec3& point, glm::vec3 axis, float angle)
	{
		
	}

	//Rotates the transform so the forward vector points at /target/'s current position.
	void LookAt(const transform* target)
	{
		rotation = glm::lookAt(glm::vec3(position.x, position.y, position.z), glm::vec3(target->position.x, target->position.y, target->position.z), glm::vec3(up.x, up.y, up.z));
		//will also need to change local rotation for all of these, children, etc.
	}

	void LookAt(const transform* target, glm::vec3 worldUp = globalUp)
	{
		rotation = glm::lookAt(glm::vec3(position.x, position.y, position.z), glm::vec3(target->position.x, target->position.y, target->position.z), glm::vec3(worldUp.x, worldUp.y, worldUp.z));
	}

	void LookAt(const glm::vec3& worldPosition)
	{
		rotation = glm::quatLookAt(glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z), glm::vec3(up.x, up.y, up.z));
	}

	void LookAt(const glm::vec3& worldPosition, glm::vec3 worldUp = globalUp)
	{
		rotation = glm::toQuat(glm::lookAt(glm::vec3(position.x, position.y, position.z), worldPosition, worldUp));
	}

	void UpdateLocalPRS(transform* leaf)
	{
		//assuming this has a parent
		glm::mat4 localISR = glm::mat4(1.0f); //Identity, Scale and Rotation
		localISR = glm::scale(localISR, glm::vec3(localScale.x, localScale.y, localScale.z)) * glm::toMat4(localRotation);
		localISR[3] = localPosition;
		if(parent != nullptr)
		{
			leaf->localToWorldMatrix = leaf->localToWorldMatrix * localISR;//i hope this is right
		}

		else
		{
			leaf->localToWorldMatrix = leaf->localToWorldMatrix * localISR;
		}
	}

	void UpdateWorldPRS(transform* leaf)
	{
		glm::mat4 worldISR = glm::mat4(1.0f);
		worldISR = glm::scale(worldISR, glm::vec3(scale.x, scale.y, scale.z)) * glm::toMat4(rotation);
		worldISR[3] = position;
		if (parent != nullptr)
		{
			leaf->worldToLocalMatrix = leaf->worldToLocalMatrix * worldISR;
		}
		else
		{
			leaf->worldToLocalMatrix = leaf->worldToLocalMatrix * worldISR;
		}
	}

	//depends on what was updated first?
	void UpdateSpaceMatrices(space_t changeSpace = space_t::world)
	{
		//assuming the transform has no parent.
		if (parent == worldRoot.get())
		{
			if (changeSpace == space_t::world)
			{
				//assuming this has a parent
				glm::mat4 localISR = glm::mat4(1.0f); //Identity, Scale and Rotation
				localISR = glm::scale(localISR, glm::vec3(localScale.x, localScale.y, localScale.z)) * glm::toMat4(localRotation);
				localISR[3] = localPosition;
				localToWorldMatrix = localISR;
			}

			else
			{
				glm::mat4 worldISR = glm::mat4(1.0f);
				worldISR = glm::scale(worldISR, glm::vec3(scale.x, scale.y, scale.z)) * glm::toMat4(rotation);
				worldISR[3] = position;
				worldToLocalMatrix = worldISR;
			}
		}

		//if there is a parent, this gets way more complicated
		else
		{
			//ok let's start from me and work all the way up to root
			RecursiveUpdate(this);
			trimmedTree.clear();
		}
	}

	//ok we need a function to update local and global variables
	void UpdateLocalAndGlobal(space_t changeSpace = space_t::world)
	{		
		//so in what space have the stats been changed?
		if (changeSpace == space_t::world)
		{
			//so the transform has been changed in world space
			//meaning that we have to update the local space values
			//first create a fresh world-to-local matrix with the new world space PRS
			//then use that new matrix to update the local space PRS

			UpdateSpaceMatrices(space_t::world); //assuming a world space setting has been changed

			localPosition = worldToLocalMatrix * position;
			localRotation = glm::toQuat(glm::toMat4(rotation) * worldToLocalMatrix);
			localScale = worldToLocalMatrix * scale;
			localEulerAngles = glm::vec4(glm::eulerAngles(localRotation), 1.0f);
			UpdateSpaceMatrices(space_t::local); //create a new localToWorld matrix
		}

		else
		{
			//invert the above

			UpdateSpaceMatrices(space_t::local); //assuming a local space setting has been changed

			position = localToWorldMatrix * localPosition;
			rotation = glm::toQuat(glm::toMat4(localRotation) * localToWorldMatrix);
			scale = localToWorldMatrix * localScale;
			eulerAngles = glm::vec4(glm::eulerAngles(rotation), 1.0f);
			UpdateSpaceMatrices(space_t::world); //create a new worldToLocal matrix
		}
	}

	/* yes i stole these from unity. eat my ass
Find	Finds a child by n and returns it. -- need to add a string to each transform? string name? hashtable?

TransformDirection	Transforms direction from local space to world space.
TransformPoint	Transforms position from local space to world space.
TransformVector	Transforms vector from local space to world space.

InverseTransformDirection	Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
InverseTransformPoint	Transforms position from world space to local space.
InverseTransformVector	Transforms a vector from world space to local space. The opposite of Transform.TransformVector.
	*/
};

const glm::vec4 transform::globalRight = glm::vec4(1, 0, 0, 1);
const glm::vec4 transform::globalForward = glm::vec4(0, 0, 1, 1);
const glm::vec4 transform::globalUp = glm::vec4(0, 1, 0, 1);

const std::unique_ptr<transform> transform::worldRoot = std::make_unique<transform>();

/*

namespace glm
{
	class vec4
	{
		using addEvent_t = std::function<void()>;
		addEvent_t addEvent = nullptr;


		vec4& operator=(const vec4& other)
		{
			xyzw = other;
			if(addEvent != nullptr)
			{
				addEvent();
			}
		}
	};
}

void updateLocal()
{
	//adjust localPosition when position is changed
}

vec4 position = glm::vec4(0);
position.addEvent = updateLocal;


*/

#endif