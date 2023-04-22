#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include"btBulletDynamicsCommon.h"


class Physics
{
public:
	Physics();
	~Physics();

	btRigidBody* CreateRigidBody(float _mass, glm::vec3 worldStartPos, btCollisionShape* collisionShape);
	void AddRigidbody(btRigidBody* body, btCollisionShape* collider, btDefaultMotionState* myMotionState);

	static const btVector3 Vec3(glm::vec3 vec3);

	btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector< btCollisionShape*> colliders;


private:

	//physics
	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* overlappingPairCache;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;


};

