#include "chifPhysics_BindLua.h"
#include "chifPhysics.h"
#include "chifScene_BindLua.h"
#include "chifMath_BindLua.h"
#include "chifPrimitive_BindLua.h"

namespace chif::lua
{
	Luna<Physics_BindLua>::FunctionType Physics_BindLua::methods[] = {
		lunamethod(Physics_BindLua, SetEnabled),
		lunamethod(Physics_BindLua, IsEnabled),
		lunamethod(Physics_BindLua, SetSimulationEnabled),
		lunamethod(Physics_BindLua, IsSimulationEnabled),
		lunamethod(Physics_BindLua, SetInterpolationEnabled),
		lunamethod(Physics_BindLua, IsInterpolationEnabled),
		lunamethod(Physics_BindLua, SetDebugDrawEnabled),
		lunamethod(Physics_BindLua, IsDebugDrawEnabled),
		lunamethod(Physics_BindLua, SetAccuracy),
		lunamethod(Physics_BindLua, GetAccuracy),
		lunamethod(Physics_BindLua, SetFrameRate),
		lunamethod(Physics_BindLua, GetFrameRate),
		lunamethod(Physics_BindLua, SetLinearVelocity),
		lunamethod(Physics_BindLua, SetAngularVelocity),
		lunamethod(Physics_BindLua, ApplyForceAt),
		lunamethod(Physics_BindLua, ApplyForce),
		lunamethod(Physics_BindLua, ApplyForceAt),
		lunamethod(Physics_BindLua, ApplyImpulse),
		lunamethod(Physics_BindLua, ApplyImpulseAt),
		lunamethod(Physics_BindLua, ApplyTorque),
		lunamethod(Physics_BindLua, SetActivationState),
		lunamethod(Physics_BindLua, ActivateAllRigidBodies),
		lunamethod(Physics_BindLua, Intersects),
		lunamethod(Physics_BindLua, PickDrag),
		{ NULL, NULL }
	};
	Luna<Physics_BindLua>::PropertyType Physics_BindLua::properties[] = {
		{ NULL, NULL }
	};


	int Physics_BindLua::SetEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::physics::SetEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetEnabled(bool value) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::IsEnabled(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::physics::IsEnabled());
		return 1;
	}
	int Physics_BindLua::SetSimulationEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::physics::SetSimulationEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetSimulationEnabled(bool value) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::IsSimulationEnabled(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::physics::IsSimulationEnabled());
		return 1;
	}
	int Physics_BindLua::SetInterpolationEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::physics::SetInterpolationEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetInterpolationEnabled(bool value) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::IsInterpolationEnabled(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::physics::IsInterpolationEnabled());
		return 1;
	}
	int Physics_BindLua::SetDebugDrawEnabled(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::physics::SetDebugDrawEnabled(chif::lua::SGetBool(L, 1));
		}
		else
			chif::lua::SError(L, "SetDebugDrawEnabled(bool value) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::IsDebugDrawEnabled(lua_State* L)
	{
		chif::lua::SSetBool(L, chif::physics::IsDebugDrawEnabled());
		return 1;
	}
	int Physics_BindLua::SetAccuracy(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::physics::SetAccuracy(chif::lua::SGetInt(L, 1));
		}
		else
			chif::lua::SError(L, "SetAccuracy(int value) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::GetAccuracy(lua_State* L)
	{
		chif::lua::SSetInt(L, chif::physics::GetAccuracy());
		return 1;
	}
	int Physics_BindLua::SetFrameRate(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			chif::physics::SetFrameRate(chif::lua::SGetFloat(L, 1));
		}
		else
			chif::lua::SError(L, "SetFrameRate(float value) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::GetFrameRate(lua_State* L)
	{
		chif::lua::SSetFloat(L, chif::physics::GetFrameRate());
		return 1;
	}

	int Physics_BindLua::SetLinearVelocity(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				chif::lua::SError(L, "SetLinearVelocity(RigidBodyPhysicsComponent component, Vector velocity) first argument is not a RigidBodyPhysicsComponent!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "SetLinearVelocity(RigidBodyPhysicsComponent component, Vector velocity) second argument is not a Vector!");
				return 0;
			}
			chif::physics::SetLinearVelocity(
				*component->component,
				*(XMFLOAT3*)vec
			);
		}
		else
			chif::lua::SError(L, "SetLinearVelocity(RigidBodyPhysicsComponent component, Vector velocity) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::SetAngularVelocity(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				chif::lua::SError(L, "SetAngularVelocity(RigidBodyPhysicsComponent component, Vector velocity) first argument is not a RigidBodyPhysicsComponent!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "SetAngularVelocity(RigidBodyPhysicsComponent component, Vector velocity) second argument is not a Vector!");
				return 0;
			}
			chif::physics::SetAngularVelocity(
				*component->component,
				*(XMFLOAT3*)vec
			);
		}
		else
			chif::lua::SError(L, "SetAngularVelocity(RigidBodyPhysicsComponent component, Vector velocity) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::ApplyForce(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				chif::lua::SError(L, "ApplyForce(RigidBodyPhysicsComponent component, Vector force) first argument is not a RigidBodyPhysicsComponent!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyForce(RigidBodyPhysicsComponent component, Vector force) second argument is not a Vector!");
				return 0;
			}
			chif::physics::ApplyForce(
				*component->component,
				*(XMFLOAT3*)vec
			);
		}
		else
			chif::lua::SError(L, "ApplyForce(RigidBodyPhysicsComponent component, Vector force) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::ApplyForceAt(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 2)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				chif::lua::SError(L, "ApplyForceAt(RigidBodyPhysicsComponent component, Vector force, Vector at) first argument is not a RigidBodyPhysicsComponent!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyForceAt(RigidBodyPhysicsComponent component, Vector force, Vector at) second argument is not a Vector!");
				return 0;
			}
			Vector_BindLua* vec2 = Luna<Vector_BindLua>::lightcheck(L, 3);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyForceAt(RigidBodyPhysicsComponent component, Vector force, Vector at) third argument is not a Vector!");
				return 0;
			}
			bool at_local = true;
			if (argc > 3)
				at_local = chif::lua::SGetBool(L, 4);

			chif::physics::ApplyForceAt(
				*component->component,
				*(XMFLOAT3*)vec,
				*(XMFLOAT3*)vec2,
				at_local
			);
		}
		else
			chif::lua::SError(L, "ApplyForceAt(RigidBodyPhysicsComponent component, Vector force, Vector at) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::ApplyImpulse(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				scene::HumanoidComponent_BindLua* humanoid = Luna<scene::HumanoidComponent_BindLua>::lightcheck(L, 1);
				if (humanoid == nullptr)
				{
					chif::lua::SError(L, "ApplyImpulse(RigidBodyPhysicsComponent component, Vector impulse) first argument is not a RigidBodyPhysicsComponent!");
					chif::lua::SError(L, "ApplyImpulse(HumanoidComponent component, HumanoidBone bone, Vector impulse) first argument is not a HumanoidComponent!");
					return 0;
				}
				chif::scene::HumanoidComponent::HumanoidBone bone = (chif::scene::HumanoidComponent::HumanoidBone)chif::lua::SGetInt(L, 2);
				Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 3);
				if (vec == nullptr)
				{
					chif::lua::SError(L, "ApplyImpulse(HumanoidComponent component, HumanoidBone bone, Vector impulse) third argument is not a Vector!");
					return 0;
				}
				chif::physics::ApplyImpulse(
					*humanoid->component,
					bone,
					*(XMFLOAT3*)vec
				);
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyImpulse(RigidBodyPhysicsComponent component, Vector impulse) second argument is not a Vector!");
				return 0;
			}
			chif::physics::ApplyImpulse(
				*component->component,
				*(XMFLOAT3*)vec
			);
		}
		else
			chif::lua::SError(L, "ApplyImpulse(RigidBodyPhysicsComponent component, Vector impulse) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::ApplyImpulseAt(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 2)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				scene::HumanoidComponent_BindLua* humanoid = Luna<scene::HumanoidComponent_BindLua>::lightcheck(L, 1);
				if (humanoid == nullptr)
				{
					chif::lua::SError(L, "ApplyImpulseAt(RigidBodyPhysicsComponent component, Vector impulse, Vector at) first argument is not a RigidBodyPhysicsComponent!");
					chif::lua::SError(L, "ApplyImpulseAt(HumanoidComponent component, HumanoidBone bone, Vector impulse, Vector at) first argument is not a HumanoidComponent!");
					return 0;
				}
				chif::scene::HumanoidComponent::HumanoidBone bone = (chif::scene::HumanoidComponent::HumanoidBone)chif::lua::SGetInt(L, 2);
				Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 3);
				if (vec == nullptr)
				{
					chif::lua::SError(L, "ApplyImpulseAt(HumanoidComponent component, HumanoidBone bone, Vector impulse, Vector at) third argument is not a Vector!");
					return 0;
				}
				Vector_BindLua* vec2 = Luna<Vector_BindLua>::lightcheck(L, 4);
				if (vec2 == nullptr)
				{
					chif::lua::SError(L, "ApplyImpulseAt(HumanoidComponent component, HumanoidBone bone, Vector impulse, Vector at) fourth argument is not a Vector!");
					return 0;
				}
				bool at_local = true;
				if (argc > 4)
					at_local = chif::lua::SGetBool(L, 5);

				chif::physics::ApplyImpulseAt(
					*humanoid->component,
					bone,
					*(XMFLOAT3*)vec,
					*(XMFLOAT3*)vec2,
					at_local
				);
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyImpulseAt(RigidBodyPhysicsComponent component, Vector impulse, Vector at) second argument is not a Vector!");
				return 0;
			}
			Vector_BindLua* vec2 = Luna<Vector_BindLua>::lightcheck(L, 3);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyImpulseAt(RigidBodyPhysicsComponent component, Vector impulse, Vector at) third argument is not a Vector!");
				return 0;
			}
			bool at_local = true;
			if (argc > 3)
				at_local = chif::lua::SGetBool(L, 4);

			chif::physics::ApplyImpulseAt(
				*component->component,
				*(XMFLOAT3*)vec,
				*(XMFLOAT3*)vec2,
				at_local
			);
		}
		else
			chif::lua::SError(L, "ApplyImpulseAt(RigidBodyPhysicsComponent component, Vector impulse, Vector at) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::ApplyTorque(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::RigidBodyPhysicsComponent_BindLua* component = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (component == nullptr)
			{
				chif::lua::SError(L, "ApplyTorque(RigidBodyPhysicsComponent component, Vector torque) first argument is not a RigidBodyPhysicsComponent!");
				return 0;
			}
			Vector_BindLua* vec = Luna<Vector_BindLua>::lightcheck(L, 2);
			if (vec == nullptr)
			{
				chif::lua::SError(L, "ApplyTorque(RigidBodyPhysicsComponent component, Vector torque) second argument is not a Vector!");
				return 0;
			}
			chif::physics::ApplyTorque(
				*component->component,
				*(XMFLOAT3*)vec
			);
		}
		else
			chif::lua::SError(L, "ApplyTorque(RigidBodyPhysicsComponent component, Vector torque) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::SetActivationState(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::RigidBodyPhysicsComponent_BindLua* rigid = Luna<scene::RigidBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
			if (rigid == nullptr)
			{
				scene::SoftBodyPhysicsComponent_BindLua* soft = Luna<scene::SoftBodyPhysicsComponent_BindLua>::lightcheck(L, 1);
				if (soft == nullptr)
				{
					chif::lua::SError(L, "SetActivationState(RigidBodyPhysicsComponent | SoftBodyPhysicsComponent component, int state) first argument is not a RigidBodyPhysicsComponent or SoftBodyPhysicsComponent!");
					return 0;
				}
				chif::physics::SetActivationState(
					*soft->component,
					(chif::physics::ActivationState)chif::lua::SGetInt(L, 2)
				);
				return 0;
			}
			chif::physics::SetActivationState(
				*rigid->component,
				(chif::physics::ActivationState)chif::lua::SGetInt(L, 2)
			);
		}
		else
			chif::lua::SError(L, "SetActivationState(RigidBodyPhysicsComponent | SoftBodyPhysicsComponent component, int state) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::ActivateAllRigidBodies(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 0)
		{
			scene::Scene_BindLua* scene = Luna<scene::Scene_BindLua>::lightcheck(L, 1);
			if (scene == nullptr)
			{
				chif::lua::SError(L, "ActivateAllRigidBodies(Scene scene) first argument is not a Scene!");
				return 0;
			}
			chif::physics::ActivateAllRigidBodies(*scene->scene);
		}
		else
			chif::lua::SError(L, "ActivateAllRigidBodies(Scene scene) not enough arguments!");
		return 0;
	}

	int Physics_BindLua::Intersects(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 1)
		{
			scene::Scene_BindLua* scene = Luna<scene::Scene_BindLua>::lightcheck(L, 1);
			if (scene == nullptr)
			{
				chif::lua::SError(L, "Intersects(Scene, Ray) first argument is not a Scene!");
				return 0;
			}
			primitive::Ray_BindLua* ray = Luna<primitive::Ray_BindLua>::lightcheck(L, 2);
			if (ray == nullptr)
			{
				chif::lua::SError(L, "Intersects(Scene, Ray) second argument is not a Ray!");
				return 0;
			}

			chif::physics::RayIntersectionResult result = chif::physics::Intersects(*scene->scene, ray->ray);
			chif::lua::SSetLongLong(L, result.entity);
			Luna<Vector_BindLua>::push(L, result.position);
			Luna<Vector_BindLua>::push(L, result.normal);
			chif::lua::SSetLongLong(L, result.humanoid_ragdoll_entity);
			chif::lua::SSetInt(L, (int)result.humanoid_bone);
			Luna<Vector_BindLua>::push(L, result.position_local);
			return 6;
		}
		chif::lua::SError(L, "Intersects(Scene, Ray) not enough arguments!");
		return 0;
	}
	int Physics_BindLua::PickDrag(lua_State* L)
	{
		int argc = chif::lua::SGetArgCount(L);
		if (argc > 2)
		{
			scene::Scene_BindLua* scene = Luna<scene::Scene_BindLua>::lightcheck(L, 1);
			if (scene == nullptr)
			{
				chif::lua::SError(L, "Intersects(Scene, Ray, PickDragOperation) first argument is not a Scene!");
				return 0;
			}
			primitive::Ray_BindLua* ray = Luna<primitive::Ray_BindLua>::lightcheck(L, 2);
			if (ray == nullptr)
			{
				chif::lua::SError(L, "Intersects(Scene, Ray, PickDragOperation) second argument is not a Ray!");
				return 0;
			}
			PickDragOperation_BindLua* op = Luna<PickDragOperation_BindLua>::lightcheck(L, 3);
			if (op == nullptr)
			{
				chif::lua::SError(L, "Intersects(Scene, Ray, PickDragOperation) third argument is not a PickDragOperation!");
				return 0;
			}

			chif::physics::PickDrag(*scene->scene, ray->ray, op->op);
			return 0;
		}
		chif::lua::SError(L, "Intersects(Scene, Ray, PickDragOperation) not enough arguments!");
		return 0;
	}

	void Physics_BindLua::Bind()
	{
		static bool initialized = false;
		if (!initialized)
		{
			initialized = true;
			Luna<PickDragOperation_BindLua>::Register(chif::lua::GetLuaState());
			Luna<Physics_BindLua>::Register(chif::lua::GetLuaState());
			Luna<Physics_BindLua>::push_global(chif::lua::GetLuaState(), "physics");

			chif::lua::RunText(R"(
ACTIVATION_STATE_ACTIVE = 0
ACTIVATION_STATE_INACTIVE = 1
)");
		}
	}




	Luna<PickDragOperation_BindLua>::FunctionType PickDragOperation_BindLua::methods[] = {
		lunamethod(PickDragOperation_BindLua, Finish),
		{ NULL, NULL }
	};
	Luna<PickDragOperation_BindLua>::PropertyType PickDragOperation_BindLua::properties[] = {
		{ NULL, NULL }
	};

	int PickDragOperation_BindLua::Finish(lua_State* L)
	{
		op = {};
		return 0;
	}
}
