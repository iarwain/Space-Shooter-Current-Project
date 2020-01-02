/**
 * @file firstGame.cpp
 * @date 22-Dec-2019
 */

#include "orx.h"

orxOBJECT* player;
orxOBJECT* spider;
orxOBJECT* pistol;
orxOBJECT* rifle;
orxOBJECT* playersPistol;
orxOBJECT* playersRifle;
orxCAMERA* camera;
bool canJump = true;
bool hasPistol = false;
bool hasRifle = false;
bool pistolPickUp = false;
bool riflePickUp = false;

void Movement() {
    orxConfig_PushSection(orxObject_GetName(player));
    orxFLOAT horizontalSpeed = orxConfig_GetFloat("HorizontalSpeed");

    orxVECTOR speed;
    orxObject_GetSpeed(player, &speed);

    if(orxInput_IsActive("GoLeft"))
    {
        speed.fX = -horizontalSpeed;
        if(hasPistol)
        {
            orxObject_SetTargetAnim(player, "playerWalkLeftPistol");
        }
        else if (hasRifle)
        {
            orxObject_SetTargetAnim(player, "playerWalkLeftRifle");
        }
        else
        {
            orxObject_SetTargetAnim(player, "playerWalkLeft");
        }
    }
    else if (orxInput_IsActive("GoRight"))
    {
        speed.fX = horizontalSpeed;
        if(hasPistol)
        {
            orxObject_SetTargetAnim(player, "playerWalkRightPistol");
        }
        else if (hasRifle)
        {
            orxObject_SetTargetAnim(player, "playerWalkRightRifle");
        }
        else
        {
            orxObject_SetTargetAnim(player, "playerWalkRight");
        }
    }
    else
    {
        speed.fX = 0.0f;
        orxObject_SetTargetAnim(player, orxNULL);
    }
    orxObject_SetSpeed(player, &speed);

    if (orxInput_HasBeenActivated("Jump") && canJump == true)
    {
        orxVECTOR jumpSpeed;
        orxConfig_GetVector("JumpSpeed", &jumpSpeed);
        orxObject_ApplyImpulse(player, &jumpSpeed, orxNULL);
        canJump = false;
    }
    orxConfig_PopSection();
}

void EnemyMovement() {
    for (orxOBJECT* enemy = orxOBJECT(orxStructure_GetFirst(orxSTRUCTURE_ID_OBJECT));
        enemy;
        enemy = orxOBJECT(orxStructure_GetNext(enemy)))
    {
        orxConfig_PushSection(orxObject_GetName(enemy));

        if(orxConfig_GetBool("IsEnemy"))
        {
            orxVECTOR speed;
            orxObject_GetSpeed(enemy, &speed);

            orxFLOAT horizontalSpeed = orxConfig_GetFloat("HorizontalSpeed");

            // Just going left for now
            speed.fX = -horizontalSpeed;
            orxObject_SetSpeed(enemy, &speed);
        }
        orxConfig_PopSection();
    }
}

orxSTATUS orxFASTCALL PhysicsEventHandler(const orxEVENT* _pstEvent)
{
    if (_pstEvent->eID == orxPHYSICS_EVENT_CONTACT_ADD) {
        orxOBJECT* pstRecipientObject, * pstSenderObject;

        pstSenderObject = orxOBJECT(_pstEvent->hSender);
        pstRecipientObject = orxOBJECT(_pstEvent->hRecipient);

        orxSTRING senderObjectName = (orxSTRING)orxObject_GetName(pstSenderObject);
        orxSTRING recipientObjectName = (orxSTRING)orxObject_GetName(pstRecipientObject);

        if (orxString_Compare(senderObjectName, "spaceShipMiddlePlatformObject") == 0)
        {
            canJump = true;
        }

        if (orxString_Compare(recipientObjectName, "spaceShipMiddlePlatformObject") == 0)
        {
            canJump = true;
        }

        if (orxString_Compare(senderObjectName, "pistolObject") == 0)
        {
            pistolPickUp = true;
        }
        else {
            pistolPickUp = false;
        }

        if (orxString_Compare(recipientObjectName, "pistolObject") == 0)
        {
            pistolPickUp = true;
        }
        else {
            pistolPickUp = false;
        }

        if (orxString_Compare(senderObjectName, "rifleObject") == 0)
        {
            riflePickUp = true;
        }
        else {
            riflePickUp = false;
        }

        if (orxString_Compare(recipientObjectName, "rifleObject") == 0)
        {
            riflePickUp = true;
        }
        else {
            riflePickUp = false;
        }

        for (orxOBJECT* enemy = orxOBJECT(orxStructure_GetFirst(orxSTRUCTURE_ID_OBJECT));
            enemy;
            enemy = orxOBJECT(orxStructure_GetNext(enemy)))
        {
            orxConfig_PushSection(orxObject_GetName(enemy));
            if (orxString_Compare(recipientObjectName, "rifleBulletObject") == 0 && orxString_Compare(senderObjectName, "spiderObject") == 0)
            {
                orxObject_SetTargetAnim(pstSenderObject, "DeadLeft");
                orxObject_SetLifeTime(pstRecipientObject, 0);
            }
            if (orxString_Compare(senderObjectName, "rifleBulletObject") == 0 && orxString_Compare(recipientObjectName, "spiderObject") == 0)
            {
                orxObject_SetTargetAnim(pstRecipientObject, "DeadLeft");
                orxObject_SetLifeTime(pstSenderObject, 0);
            }
            if (orxString_Compare(recipientObjectName, "pistolBulletObject") == 0 && orxString_Compare(senderObjectName, "spiderObject") == 0)
            {
                orxObject_SetTargetAnim(pstSenderObject, "DeadLeft");
                orxObject_SetLifeTime(pstRecipientObject, 0);
            }
            if (orxString_Compare(senderObjectName, "pistolBulletObject") == 0 && orxString_Compare(recipientObjectName, "spiderObject") == 0)
            {
                orxObject_SetTargetAnim(pstRecipientObject, "DeadLeft");
                orxObject_SetLifeTime(pstSenderObject, 0);
            }
            orxConfig_PopSection();
        }
    }
    return orxSTATUS_SUCCESS;
}

orxSTATUS orxFASTCALL Init()
{
    orxViewport_CreateFromConfig("Viewport");

    orxObject_CreateFromConfig("Scene");

    player = orxObject_CreateFromConfig("playerObject");

    playersRifle = orxObject_CreateFromConfig("playersRifle");

    playersPistol = orxObject_CreateFromConfig("playersPistol");

    orxObject_CreateFromConfig("spaceShipMiddlePlatformObject");

    orxEvent_AddHandler(orxEVENT_TYPE_PHYSICS, PhysicsEventHandler);

    pistol = orxObject_CreateFromConfig("pistolObject");

    rifle = orxObject_CreateFromConfig("rifleObject");

    return orxSTATUS_SUCCESS;
}

void PickUp()
{
    if (orxInput_IsActive("PickUp") && pistolPickUp == true && hasPistol == false && hasRifle == false || hasPistol == false && hasRifle == false && pistolPickUp == true)
    {
        orxObject_SetLifeTime(pistol, 0);
        hasPistol = true;
    }

    if (orxInput_IsActive("PickUp") && riflePickUp == true && hasPistol == false && hasRifle == false || hasPistol == false && hasRifle == false && riflePickUp == true)
    {
        orxObject_SetLifeTime(rifle, 0);
        hasRifle = true;
    }
}

void Shoot()
{
    orxObject_Enable(playersPistol, orxInput_IsActive("Shoot") && hasPistol);
    orxObject_Enable(playersRifle, orxInput_IsActive("Shoot") && hasRifle);
}

orxSTATUS orxFASTCALL Run()
{
    orxSTATUS eResult = orxSTATUS_SUCCESS;

    if(orxInput_IsActive("Quit"))
    {

        eResult = orxSTATUS_FAILURE;
    }

    PickUp();

    Movement();

    Shoot();

    EnemyMovement();

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
