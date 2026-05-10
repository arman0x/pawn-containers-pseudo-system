public OnGameModeInit()
{
    Iter_Init(ContainerBiddersIter);

    Container:CreateMenuTDs();
    Container:CreateWonTDs();

    //Container:FetchDataFromDatabase(); - calls in main callback (init order fix)

    #if defined Cont_OnGameModeInit
        return Cont_OnGameModeInit();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else 
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Cont_OnGameModeInit

forward Cont_OnGameModeInit();

public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid)
{
    new container_index = Container:GetIndexFromAreaID(areaid);

    #if defined CONTAINERS_DEBUG_MODE
        printf("[Containers]: player %d, container_index %d, is valid %d", 
        playerid, container_index, Container:IsValidIndex(container_index));
    #endif

    if(Container:IsValidIndex(container_index))
    {
        if(Container:GetBiddingStatus() == false)
        {
            Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_ER_BID_NT_STRTD_OR_INV_IDX);
        }
        else
        {
            Container:ShowMenuForPlayer(playerid, container_index);
        }
    }

    #if defined Cont_OnPlayerEnterDynamicArea
        return Cont_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerEnterDynamicArea
    #undef OnPlayerEnterDynamicArea
#else
    #define _ALS_OnPlayerEnterDynamicArea
#endif

#define OnPlayerEnterDynamicArea Cont_OnPlayerEnterDynamicArea

public OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid)
{
    if(Container:IsPlayerMenuOpened(playerid) == true)
    {
        Container:HideMenuForPlayer(playerid, Container:GetPlayerMenuContainerIDx(playerid));
    }

    #if defined Cont_OnPlayerLeaveDynamicArea
        return Cont_OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerLeaveDynamicArea
    #undef OnPlayerLeaveDynamicArea
#else
    #define _ALS_OnPlayerLeaveDynamicArea
#endif

#define OnPlayerLeaveDynamicArea Cont_OnPlayerLeaveDynamicArea

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(_:clickedid == INVALID_TEXT_DRAW && Container:IsPlayerMenuOpened(playerid) == true) // ESC
    {
        Container:ShowPlayerCursor(playerid);
    }

    if(clickedid == Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID))
    {
        if(Container:CanPlayerInteract(playerid) == false)
        {
            return Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_ER_BID_NT_STRTD_OR_INV_IDX);
        }
    
        Container:ShowBidDialogForPlayer(playerid, Container:GetPlayerContainerIndex(playerid));
    }
    else if(clickedid == Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID))
    {
        Container:HideMenuForPlayer(playerid, Container:GetPlayerMenuContainerIDx(playerid));
    }
    else if(clickedid == Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID))
    {
        Container:HideWonTDForPlayer(playerid);
    }

    #if defined Cont_OnPlayerClickTextDraw
        return Cont_OnPlayerClickTextDraw(playerid, clickedid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif

#define OnPlayerClickTextDraw Cont_OnPlayerClickTextDraw

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_CONTAINER_BID:
        {
            if(!response)
            {
                Container:ShowPlayerCursor(playerid);
                return 1;
            }

            if(!Container:CanPlayerInteract(playerid))
            {
                return Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_ER_BID_NT_STRTD_OR_INV_IDX);
            }
 
            new betAmount = strval(inputtext);
            new container_index = Container:GetPlayerContainerIndex(playerid);

            new E_CONTAINER_FLAGS:bid_place_flag = Container:TryPlaceBid(playerid, container_index, betAmount);

            switch(_:bid_place_flag)
            {
                case CF_SUCCESS_BID:
                {
                    new E_CONTAINER_FLAGS:bid_register_flag = Container:TryRegisterPlayerBid(container_index, playerid, betAmount);

                    switch(_:bid_register_flag)
                    {
                        case CF_ALREADY_PLACED_BID:
                        {
                            Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_BID_ALREADY_PLACED);
                        }
                        case CF_NO_FREE_BID_SLOT:
                        {
                            Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_BID_NO_FREE_SLOT);
                        }
                        case CF_SUCCESS_BID:
                        {
                            Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_BID_CONFIRMED);

                            GivePlayerMoney(playerid, -betAmount);

                            Container:SetTotalBid(container_index, Container:GetTotalBid(container_index) + betAmount);

                            Container:RefreshTopBiddersTD(container_index);
                        }
                        default:
                        {
                            Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_SYSTEM_ERROR);
                        }
                    }
                }
                case CF_INCORRECT_BID_SUM:
                {
                    Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_BID_INCORRECT_SUM);
                    Container:ShowBidDialogForPlayer(playerid, container_index);
                }
                case CF_INSUFFICIENT_FUNDS:
                {
                    Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_BID_INSUFFICIENT_FUNDS);
                    Container:ShowBidDialogForPlayer(playerid, container_index);
                }
                default:
                {
                    Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:CC_SYSTEM_ERROR);
                    Container:ShowBidDialogForPlayer(playerid, container_index);
                }
            }
        }
    }
    #if defined Cont_OnDialogResponse
        return Cont_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif

#define OnDialogResponse Cont_OnDialogResponse

public OnPlayerDisconnect(playerid, reason)
{
    Container:SetPlayerMenuContainerIDx(playerid, INVALID_CONTAINER_ID);
    
    foreach(new container_index : CreatedContainerData)
    {
        if(Container:IsPlayerAlreadySetBid(container_index, playerid))
        {
            Container:DecrementBidders(playerid, container_index);
            Container:SetPlayerBid(container_index, playerid, 0);
        }
    }

    #if defined Cont_OnPlayerDisconnect
        return Cont_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Cont_OnPlayerDisconnect

public OnPlayerConnect(playerid)
{
    Container:SetPlayerMenuContainerIDx(playerid, INVALID_CONTAINER_ID);

    Container:FetchPlayerBidDataFromDB(playerid);

    #if defined Cont_OnPlayerConnect
        return Cont_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect Cont_OnPlayerConnect

forward Cont_OnPlayerConnect(playerid);
