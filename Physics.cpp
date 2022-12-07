#include "Physics.h"

Physics* instance = nullptr;

Physics::Physics()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

}

Physics::~Physics()
{
	//delete physics setup 
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
}

btRigidBody* Physics::CreateRigidBody(float _mass, glm::vec3 worldStartPos, btCollisionShape* collisionShape)
{
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(_mass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(Vec3(worldStartPos));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	
	return body;
}

void Physics::AddRigidbody(btRigidBody* body, btCollisionShape* collider, btDefaultMotionState* myMotionState)
{
	colliders.push_back(collider);

	dynamicsWorld->addRigidBody(body);
}

const btVector3 Physics::Vec3(glm::vec3 vec3)
{
	return btVector3(vec3.x, vec3.y, vec3.z);
}


