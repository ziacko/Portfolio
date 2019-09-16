#ifndef TRANSFORM_H
#define TRANSFORM_H

//have imgui show the hierarchy?

class transform
{
public:

	transform()
	{
		//no parent
		childIterEvent = nullptr;
		parent = nullptr;
		root = this;
		hierarchyCount = 0;

		position = glm::vec3(0);
		rotation = glm::quat();
		scale = glm::vec3(1);
		eulerAngles = glm::vec3(0);

		localPosition = glm::vec3(0);
		localRotation = glm::quat();
		localScale = glm::vec3(0);
		localEulerAngles = glm::vec3(0);
		
		lossyScale = 1;

		right = glm::conjugate(rotation) * globalRight;
		forward = glm::conjugate(rotation) * globalForward;
		up = glm::conjugate(rotation) * globalUp;
		childIter = children.begin();
	}

	enum class treeTrav_t
	{
		trav_continue,
		trav_quit
	};

	enum class space_t
	{
		self,
		world
	};

	//setters and getters

	//global
	glm::vec3 GetPosition() { return position; }	
	glm::quat GetRotation()	{ return rotation; }	
	glm::vec3 GetScale() { return scale; }	
	glm::vec3 GetEulerAngles() { return eulerAngles; }

	void SetPosition(glm::vec3 position) 
	{ 
		this->position = position; 
		//set local position
	}
	void SetRotation(glm::quat rotation) 
	{ 
		this->rotation = rotation; 
		//set local rotation
	}
	void SetScale(glm::vec3 scale) 
	{ 
		this->scale = scale; 
		//set local scale
	}
	void SetEulerAngles(glm::vec3 eulerAngles) 
	{ 
		this->eulerAngles = eulerAngles;
		//set local euler angles
	}

	//local 
	glm::vec3 GetLocalPosition() { return localPosition; }
	glm::quat GetLocalRotation() { return localRotation; }
	glm::vec3 GetLocalScale() { return scale; }
	glm::vec3 GetLocalEulerAngles() { return localEulerAngles; }

	void SetLocalPosition(glm::vec3 localPosition) 
	{ 
		this->localPosition = localPosition;
		//set local position
	}
	void SetLocalRotation(glm::quat localRotation)
	{
		this->localRotation = localRotation; 
		//set local rotation
	}
	void SetLocalScale(glm::vec3 localScale) 
	{
		this->localScale = localScale; 
		//set blobal scale
	}
	void SetLocalEulerAngles(glm::vec3 localEulerAngles) 
	{ 
		this->localEulerAngles = localEulerAngles;
	}

	using childIterEvent_t = std::function<treeTrav_t(transform* current, transform* child)>;
	childIterEvent_t childIterEvent;

private:

	//hierarchy

	//use smart pointers
	std::vector<std::unique_ptr<transform>> children;
	std::vector< std::unique_ptr<transform>>::iterator childIter;

	//a global list of transforms without parents?
	static std::vector<std::unique_ptr<transform>> parentless;

	transform* parent;
	transform* root;
	unsigned int hierarchyCount;

	//global
	glm::vec3 position;				//hmm. operator overloading to automatically change local position when global has been changed
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 eulerAngles;	

	//local members
	glm::vec3 localPosition;
	glm::quat localRotation;
	glm::vec3 localScale;
	glm::vec3 localEulerAngles;

	//conversion matrices
	//const glm::mat4 localToWorldMatrix; (PositionRotationScale)
	//const glm::mat4 worldToLocalMatrix; (lPositionlRotationlScale)

	//angles
	static const glm::vec3 globalRight;// = glm::vec3(1, 0, 0);
	static const glm::vec3 globalForward;// = glm::vec3(0, 0, 1);
	static const glm::vec3 globalUp;// = glm::vec3(0, 1, 0);

	glm::vec3 right;
	glm::vec3 forward;
	glm::vec3 up;

	float lossyScale;//global scale of object as a scalar?

	void DetachChild(transform* child)
	{		
		//make each child its own root,
		//and set each parent to null
		child->parent = nullptr;
		child->root = child;

		//iterate though all of its children and set their root			
		child->childIterEvent = std::bind(&transform::SetNewRoot, this, _1, _2);
		child->IterateChildren();
		childIterEvent = nullptr; //reset to nullptr
	}

	void DetachChildren()
	{
		//just step through the top most layer,
		//detach each child,
		//move pointer ownership to parentless
		for (auto& iter : children)
		{
			//make a temp unique pointer and transfer ownership to parentless
			transform* child = iter.release();
			DetachChild(child);
			std::unique_ptr<transform> uniqueChild;
			uniqueChild.reset(child);
			parentless.push_back(std::move(uniqueChild));
		}

		children.clear(); //this should remove all the empty unique pointers
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
			//parent->
			return;
		}


		parent = newParent;
		//share the same root
		root = newParent->root;		

		//have parent push back this object into it's children
		//parent->children.push_back(std::move(this));
		//we need to shift pointer ownership. from global transform list?

	}

	transform* IterateChildren()
	{
		for (auto& iter : children)
		{
			//the current node in tree
			if (childIterEvent != nullptr)
			{
				if (childIterEvent(this, iter.get()) == treeTrav_t::trav_quit)
				{
					return this;
				}
			}
			iter->IterateChildren();
		}
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

	treeTrav_t SetNewRoot(transform* currentTransform, transform* child)
	{
		child->root = currentTransform;
		return treeTrav_t::trav_continue;
	}

	//Sets the world space position and rotation of the Transform component.
	void SetPositionAndRotation(glm::vec3 position, glm::quat rotation)
	{
		this->position = position;
		this->rotation = rotation;
	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(float x, float y, float z)
	{

	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(float x, float y, float z, space_t relativeTo = space_t::self)
	{

	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(float x, float y, float z, transform* relativeTo)
	{

	}

	//Moves the transform in the directionand distance of translation. --
	void Translate(glm::vec3 translation)
	{
		//move in global space?


	}

	//Moves the transform in the direction and distance of translation. -- 
	void Translate(glm::vec3 translation, space_t relativeTo = space_t::self)
	{
		switch (relativeTo)
		{
		case space_t::self:
		{
			break;
		}

		case space_t::world:
		{
			break;
		}
		}
	}
	//Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as a Euler angle and not a Quaternion. --
	void Rotate(glm::vec3 eulerAngles, space_t relativeTo = space_t::self)
	{

	}
	//Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as a Euler angle and not a Quaternion. --
	void Rotate(float xAngle, float yAngle, float zAngle, space_t relativeTo = space_t::self)
	{

	}

	//Use Transform.Rotate to rotate GameObjects in a variety of ways. The rotation is often provided as a Euler angle and not a Quaternion. --
	void Rotate(glm::vec3 axis, float angle, space_t relativeTo = space_t::self)
	{

	}

	//Rotates the transform about axis passing through point in world coordinates by angle degrees. --
	void RotateAround(glm::vec3 point, glm::vec3 axis, float angle)
	{

	}

	//Rotates the transform so the forward vector points at /target/'s current position.
	void LookAt(transform* target)
	{

	}

	void LookAt(transform* target, glm::vec3 worldUp = globalUp)
	{

	}

	void LookAt(glm::vec3 worldPosition)
	{

	}

	void LookAt(glm::vec3 worldPosition, glm::vec3 worldUp = globalUp)
	{

	}

	/*
Find	Finds a child by n and returns it. -- need to add a string to each transform? string name?

TransformDirection	Transforms direction from local space to world space.
TransformPoint	Transforms position from local space to world space.
TransformVector	Transforms vector from local space to world space.

InverseTransformDirection	Transforms a direction from world space to local space. The opposite of Transform.TransformDirection.
InverseTransformPoint	Transforms position from world space to local space.
InverseTransformVector	Transforms a vector from world space to local space. The opposite of Transform.TransformVector.
	*/
};

const glm::vec3 transform::globalRight = glm::vec3(1, 0, 0);
const glm::vec3 transform::globalForward = glm::vec3(0, 0, 1);
const glm::vec3 transform::globalUp = glm::vec3(0, 1, 0);


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