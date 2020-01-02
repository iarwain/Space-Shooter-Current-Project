/**
 * @file firstGame.cpp
 * @date 22-Dec-2019
 */

#include "orx.h"

orxOBJECT* player = orxNULL;
orxOBJECT* playersGun = orxNULL;
bool canJump = true;

void PickUp(orxOBJECT *gun)
{
    // Has gun?
    if(playersGun)
    {
        // Delete currently carried gun
        orxObject_SetLifeTime(playersGun, orxFLOAT_0);
    }

    // Get new gun from the pickup's property named GunModel
    playersGun = orxObject_CreateFromConfig(orxConfig_GetString("GunModel"));
    if(playersGun)
    {
        // Delete pickup
        orxObject_SetLifeTime(gun, orxFLOAT_0);

        // Set the correct animation set from the pickup's property named GunAnimSet
        orxObject_SetAnimSet(player, orxAnimSet_CreateFromConfig(orxConfig_GetString("GunAnimSet")));
    }
}

orxSTATUS orxFASTCALL PhysicsEventHandler(const orxEVENT* _pstEvent)
{
    if (_pstEvent->eID == orxPHYSICS_EVENT_CONTACT_ADD) {
        // We store them as two pairs (sender/recipient then recipient/sender), so as to not duplicate the logic code
        struct
        {
            orxOBJECT *Self, *Other;
        } Colliders[2] = {
            {orxOBJECT(_pstEvent->hSender), orxOBJECT(_pstEvent->hRecipient)},
            {orxOBJECT(_pstEvent->hRecipient), orxOBJECT(_pstEvent->hSender)}
        };

        // Iterating over the two pairs
        for(orxU32 i = 0; i < orxARRAY_GET_ITEM_COUNT(Colliders); i++)
        {
            orxConfig_PushSection(orxObject_GetName(Colliders[i].Other));

            // Platform?
            if(orxConfig_GetBool("IsPlatform"))
            {
                canJump = true;
            }
            // Gun pickup?
            else if(orxConfig_GetBool("IsGun"))
            {
                PickUp(Colliders[i].Other);
            }
            // Bullet?
            else if(orxConfig_GetBool("IsBullet"))
            {
                // Die
                orxObject_SetTargetAnim(Colliders[i].Self, "Dead");
                // Remove bullet
                orxObject_SetLifeTime(Colliders[i].Other, orxFLOAT_0);
            }

            orxConfig_PopSection();
        }
    }
    return orxSTATUS_SUCCESS;
}

void Movement() {
    orxConfig_PushSection(orxObject_GetName(player));
    orxFLOAT horizontalSpeed = orxConfig_GetFloat("HorizontalSpeed");

    orxVECTOR speed;

    // Get current speed
    orxObject_GetSpeed(player, &speed);

    // Update horizontal speed based on inputs
    if(orxInput_IsActive("GoLeft"))
    {
        speed.fX = -horizontalSpeed;
        orxObject_SetTargetAnim(player, "PlayerWalkLeft");
    }
    else if (orxInput_IsActive("GoRight"))
    {
        speed.fX = horizontalSpeed;
        orxObject_SetTargetAnim(player, "PlayerWalkRight");
    }
    else
    {
        speed.fX = orxFLOAT_0;
        orxObject_SetTargetAnim(player, orxNULL);
    }

    // Apply new speed
    orxObject_SetSpeed(player, &speed);

    // Jump?
    if (orxInput_HasBeenActivated("Jump") && canJump == true)
    {
        orxVECTOR jumpSpeed;
        orxConfig_GetVector("JumpSpeed", &jumpSpeed);

        // Apply impulse, would be better handled as a speed as well
        orxObject_ApplyImpulse(player, &jumpSpeed, orxNULL);
        canJump = false;
    }
    orxConfig_PopSection();
}

void EnemyMovement() {
    // In its current form, this could be entirely done in config with KeyEvents
    for (orxOBJECT* enemy = orxObject_GetNext(orxNULL, orxU32_UNDEFINED);
        enemy;
        enemy = orxObject_GetNext(enemy, orxU32_UNDEFINED))
    {
        orxConfig_PushSection(orxObject_GetName(enemy));
        if(orxConfig_GetBool("IsEnemy"))
        {
            orxFLOAT horizontalSpeed = orxConfig_GetFloat("HorizontalSpeed");

            // Match the speed with the current anim
            orxVECTOR speed;
            orxObject_GetSpeed(enemy, &speed);

            const orxSTRING anim = orxObject_GetCurrentAnim(enemy);
            if(orxString_Compare(anim, "WalkLeft") == 0)
            {
                speed.fX = -horizontalSpeed;
            }
            else if(orxString_Compare(anim, "WalkRight") == 0)
            {
                speed.fX = horizontalSpeed;
            }
            else
            {
                speed.fX = 0;
            }
            orxObject_SetSpeed(enemy, &speed);
        }
        orxConfig_PopSection();
    }
}

void Shoot()
{
    // Has gun?
    if(playersGun)
    {
        // Enable it based on input
        orxObject_Enable(playersGun, orxInput_IsActive("Shoot"));
    }
}

void orxFASTCALL Update(const orxCLOCK_INFO *info, void *context)
{
    Movement();
    Shoot();
    EnemyMovement();
}

orxSTATUS orxFASTCALL Init()
{
    orxViewport_CreateFromConfig("Viewport");
    orxObject_CreateFromConfig("Scene");

    // Alternatively, the player could be spawned by the scene and we'd retrieve it here
    player = orxObject_CreateFromConfig("playerObject");

    orxEvent_AddHandler(orxEVENT_TYPE_PHYSICS, PhysicsEventHandler);

    // Use a clock callback for the game's logic
    orxClock_Register(orxClock_FindFirst(-1.0f, orxCLOCK_TYPE_CORE), Update, orxNULL, orxMODULE_ID_MAIN, orxCLOCK_PRIORITY_NORMAL);

    return orxSTATUS_SUCCESS;
}

orxSTATUS orxFASTCALL Run()
{
    orxSTATUS eResult = orxSTATUS_SUCCESS;

    if(orxInput_IsActive("Quit"))
    {
        eResult = orxSTATUS_FAILURE;
    }

    return eResult;
}

void orxFASTCALL Exit()
{
}

orxSTATUS orxFASTCALL Bootstrap()
{
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);
    return orxSTATUS_SUCCESS;
}

int main(int argc, char **argv)
{
    orxConfig_SetBootstrap(Bootstrap);
    orx_Execute(argc, argv, Init, Run, Exit);

    return EXIT_SUCCESS;
}
