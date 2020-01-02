/**
 * @file firstGame.cpp
 * @date 22-Dec-2019
 */

#include "orx.h"

orxOBJECT* player;
orxOBJECT* dummy;
orxOBJECT* spider;
orxOBJECT* pistol;
orxOBJECT* rifle;
orxOBJECT* playersPistol;
orxOBJECT* playersRifle;
orxCAMERA* camera;
bool canJump = true;
bool isRight = true;
bool hasPistol = false;
bool hasRifle = false;
bool pistolPickUp = false;
bool riflePickUp = false;
int moveRight = 10;

void enemyMovement()
{
    for (orxOBJECT* enemy = orxOBJECT(orxStructure_GetFirst(orxSTRUCTURE_ID_OBJECT));
        enemy;
        enemy = orxOBJECT(orxStructure_GetNext(enemy)))
    {
        orxConfig_PushSection(orxObject_GetName(enemy));
        if (orxConfig_GetBool("IsEnemy"))
        {
            orxVECTOR speed;
            orxConfig_GetVector("LeftSpeed", &speed);
            orxObject_SetTargetAnim(enemy, "WalkLeft");
            orxObject_SetSpeed(enemy, &speed);
        }
        orxConfig_PopSection();
    }
}

void Movement() {

    orxVECTOR leftSpeed = { -19.5, 0, 0 };
    orxVECTOR rightSpeed = { 19.5, 0, 0 };

    orxVECTOR flipLeft = { -1, 1, 1 };
    orxVECTOR flipRight = { 1, 1, 1 };

    if (orxInput_IsActive("GoLeft") && hasPistol == false && hasRifle == false)
    {
        orxObject_ApplyImpulse(player, &leftSpeed, orxNULL);
        orxObject_SetTargetAnim(player, "playerWalkLeft");
        isRight = false;
    }
    if (orxInput_IsActive("GoRight") && hasPistol == false && hasRifle == false)
    {
        orxObject_ApplyImpulse(player, &rightSpeed, orxNULL);
        orxObject_SetTargetAnim(player, "playerWalkRight");
        isRight = true;
    }
    if (orxInput_IsActive("GoLeft") && hasPistol == true && hasRifle == false)
    {
        orxObject_ApplyImpulse(player, &leftSpeed, orxNULL);
        orxObject_SetTargetAnim(player, "playerWalkLeftPistol");
        isRight = false;

        orxObject_SetScale(playersPistol, &flipLeft);
    }
    if (orxInput_IsActive("GoRight") && hasPistol == true && hasRifle == false)
    {
        orxObject_ApplyImpulse(player, &rightSpeed, orxNULL);
        orxObject_SetTargetAnim(player, "playerWalkRightPistol");
        isRight = true;

        orxObject_SetScale(playersPistol, &flipRight);
    }
    if (orxInput_IsActive("GoLeft") && hasPistol == false && hasRifle == true)
    {
        orxObject_ApplyImpulse(player, &leftSpeed, orxNULL);
        orxObject_SetTargetAnim(player, "playerWalkLeftRifle");
        isRight = false;

        orxObject_SetScale(playersRifle, &flipLeft);
    }
    if (orxInput_IsActive("GoRight") && hasPistol == false && hasRifle == true)
    {
        orxObject_ApplyImpulse(player, &rightSpeed, orxNULL);
        orxObject_SetTargetAnim(player, "playerWalkRightRifle");
        isRight = true;

        orxObject_SetScale(playersRifle, &flipRight);
    }
    else if (isRight == true && hasPistol == false && hasRifle == false)
    {
        orxObject_SetTargetAnim(player, "playerIdleRight");
    }
    else if (isRight == false && hasPistol == false && hasRifle == false)
    {
        orxObject_SetTargetAnim(player, "playerIdleLeft");
    }
    else if (isRight == true && hasPistol == true && hasRifle == false)
    {
        orxObject_SetTargetAnim(player, "playerIdleRightPistol");
        orxObject_SetScale(playersPistol, &flipRight);
    }
    else if (isRight == false && hasPistol == true && hasRifle == false)
    {
        orxObject_SetTargetAnim(player, "playerIdleLeftPistol");
        orxObject_SetScale(playersPistol, &flipLeft);
    }
    else if (isRight == true && hasPistol == false && hasRifle == true)
    {
        orxObject_SetTargetAnim(player, "playerIdleRightRifle");
        orxObject_SetScale(playersRifle, &flipRight);
    }
    else if (isRight == false && hasPistol == false && hasRifle == true)
    {
        orxObject_SetTargetAnim(player, "playerIdleLeftRifle");
        orxObject_SetScale(playersRifle, &flipLeft);
    }

    orxVECTOR jumpSpeed = { 0, -1340 /* -2680 with Jump Pack */, 0 };

    if (orxInput_IsActive("Jump") && orxInput_HasNewStatus("Jump") && canJump == true)
    {
        orxObject_ApplyImpulse(player, &jumpSpeed, orxNULL);
        canJump = false;
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

    enemyMovement();

    player = orxObject_CreateFromConfig("playerObject");

    playersRifle = (orxOBJECT*)orxObject_GetChild(player);

    playersPistol = (orxOBJECT*)orxObject_GetSibling(playersRifle);

    dummy = orxObject_CreateFromConfig("dummyObject");

    orxObject_CreateFromConfig("spaceShipMiddlePlatformObject");

    camera = orxCamera_CreateFromConfig("Camera");

    orxEvent_AddHandler(orxEVENT_TYPE_PHYSICS, PhysicsEventHandler);

    orxCamera_SetParent(camera, dummy);

    orxObject_SetParent(dummy, player);

    pistol = orxObject_CreateFromConfig("pistolObject");

    rifle = orxObject_CreateFromConfig("rifleObject");

    orxObject_Enable(playersPistol, orxFALSE);

    orxObject_Enable(playersRifle, orxFALSE);
   
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
    if (orxInput_IsActive("Shoot") && hasPistol == true && hasRifle == false)
    {
        orxObject_Enable(playersPistol, orxTRUE);
    }
    else {
        orxObject_Enable(playersPistol, orxFALSE);
    }
    if (orxInput_IsActive("Shoot") && hasRifle == true && hasPistol == false)
    {
        orxObject_Enable(playersRifle, orxTRUE);
    }
    else {
        orxObject_Enable(playersRifle, orxFALSE);
    }
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
