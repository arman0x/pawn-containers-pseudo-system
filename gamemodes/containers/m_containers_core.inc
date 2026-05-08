stock Container:ResetPendingPrizeFetches()
{
    pendingPrizeFetches = 0;
}

stock Container:IncPendingPrizeFetches()
{
    pendingPrizeFetches++;
}

stock bool:Container:DecAndCheckPrizeFetches()
{
    return (--pendingPrizeFetches == 0);
}

stock Container:GetPlayerMenuContainerIDx(playerid)
{
    return playerOpenedMenuContainerIndex[playerid];
}

stock bool:Container:IsPlayerMenuOpened(playerid)
{
    return playerOpenedMenuContainerIndex[playerid] != INVALID_CONTAINER_ID;
}

stock Container:SetPlayerMenuContainerIDx(playerid, container_index = 0) // 0 = some id, just for conditions correct work
{
    playerOpenedMenuContainerIndex[playerid] = container_index;
}

stock Container:GetObjectModel(container_index)
{
    return ContainerData[container_index][CONTAINER_OBJ_MODEL];
}

stock Container:SetObjectModel(container_index, value)
{
    ContainerData[container_index][CONTAINER_OBJ_MODEL] = value;
}

stock Float:Container:GetObjectCoordX(container_index)
{
    return ContainerData[container_index][CONTAINER_OBJ_COORD_X];
}

stock Container:SetObjectCoordX(container_index, Float:value)
{
    ContainerData[container_index][CONTAINER_OBJ_COORD_X] = value;
}

stock Float:Container:GetObjectCoordY(container_index)
{
    return ContainerData[container_index][CONTAINER_OBJ_COORD_Y];
}

stock Container:SetObjectCoordY(container_index, Float:value)
{
    ContainerData[container_index][CONTAINER_OBJ_COORD_Y] = value;
}

stock Float:Container:GetObjectCoordZ(container_index)
{
    return ContainerData[container_index][CONTAINER_OBJ_COORD_Z];
}

stock Container:SetObjectCoordZ(container_index, Float:value)
{
    ContainerData[container_index][CONTAINER_OBJ_COORD_Z] = value;
}

stock Container:GetID(container_index)
{
    return ContainerData[container_index][CONTAINER_ID];
}

stock Container:SetID(container_index, value)
{
    ContainerData[container_index][CONTAINER_ID] = value;
}

stock Container:GetType(container_index)
{
    return ContainerData[container_index][CONTAINER_TYPE];
}

stock Container:SetType(container_index, value)
{
    ContainerData[container_index][CONTAINER_TYPE] = value;
}

stock Container:GetStartPrice(container_index)
{
    return ContainerData[container_index][CONTAINER_START_PRICE];
}

stock Container:SetStartPrice(container_index, value)
{
    ContainerData[container_index][CONTAINER_START_PRICE] = value;
}

stock Container:GetTotalBid(container_index)
{
    return ContainerData[container_index][CONTAINER_TOTAL_BID];
}

stock Container:SetTotalBid(container_index, bidAmount)
{
    ContainerData[container_index][CONTAINER_TOTAL_BID] = bidAmount;
}

stock Container:IncrementBidders(playerid, container_index)
{
    Iter_Add(ContainerBiddersIter[container_index], playerid);
}

stock Container:DecrementBidders(playerid, container_index)
{
    Iter_Remove(ContainerBiddersIter[container_index], playerid);
}

stock bool:Container:IsPlayerAlreadySetBid(container_index, playerid)
{
    return Iter_Contains(ContainerBiddersIter[container_index], playerid) ? true : false;
}

stock Container:GetPlayingPlayers(container_index, const players[], maxplayers = sizeof players)
{
    return Iter_ToArray(ContainerBiddersIter[container_index], players, maxplayers);
}

stock Container:ClearBiddersData(container_index)
{
    Iter_Clear(ContainerBiddersIter[container_index]);
}

stock Container:GetPlayerBid(container_index, playerid)
{
    return ContainerData[container_index][CONTAINER_PLACED_BID_PLAYERS][playerid];
}

stock Container:SetPlayerBid(container_index, playerid, bidAmount)
{
    ContainerData[container_index][CONTAINER_PLACED_BID_PLAYERS][playerid] = bidAmount;
}

stock Float:Container:GetMinimalBid(container_index)
{
    return Container:GetStartPrice(container_index) * CONTAINER_MINIMAL_BID_MULTIPLIER;
}

stock Float:Container:GetMaximalBid(container_index)
{
    return Container:GetStartPrice(container_index) * CONTAINER_MAXIMAL_BID_MULTIPLIER;
}

stock bool:Container:IsValidIndex(container_index)
{
    return Iter_Contains(CreatedContainerData, container_index) ? true : false;
}

stock Container:GetCreatedCount()
{
    return Iter_Count(CreatedContainerData);
}

stock Container:IncrementCreatedCount(container_index)
{
    return Iter_Add(CreatedContainerData, container_index);
}

stock bool:Container:GetBiddingStatus()
{
    return ContainersBiddingStatus;
}

stock Container:SetBiddingStatus(bool:status)
{
    ContainersBiddingStatus = status;
}

stock Text:Container:GetContainerMenuTDID(textdraw_index)
{
    return ContainerMenuTD[textdraw_index];
}

stock Text:Container:SetContainerMenuTDID(textdraw_index, Text:text_draw_id)
{
    ContainerMenuTD[textdraw_index] = text_draw_id;
} 

stock Text:Container:GetContainerMenuNicksTDID(container_index, textdraw_index)
{
    return ContainerMenuNickNamesTD[container_index][textdraw_index];
}

stock Text:Container:SetContainerMenuNicksTDID(container_index, textdraw_index, Text:text_draw_id)
{
    ContainerMenuNickNamesTD[container_index][textdraw_index] = text_draw_id;
}

stock Text:Container:GetContainerWonTDID(textdraw_index)
{
    return ContainerWonTD[textdraw_index];
}

stock Text:Container:SetContainerWonTDID(textdraw_index, Text:text_draw_id)
{
    ContainerWonTD[textdraw_index] = text_draw_id;
}

stock Container:GetMenuTDCount()
{
    return sizeof(ContainerMenuTD);
}

stock Container:GetWonMenuTDCount()
{
    return sizeof(ContainerWonTD);
}

stock Container:ResetPlayersBid(container_index)
{
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        ContainerData[container_index][CONTAINER_PLACED_BID_PLAYERS][i] = 0;
    }
}

stock Container:ResetBiddingState(container_index)
{
    Container:SetTotalBid(container_index, 0);
    Container:ResetPlayersBid(container_index);
    Container:ClearBiddersData(container_index);
}

stock E_CONTAINER_FLAGS:Container:TryRegisterPlayerBid(container_index, playerid, bidAmount)
{
    if(Container:IsPlayerAlreadySetBid(container_index, playerid) == true)
    {
        return CF_ALREADY_PLACED_BID;
    }

    Container:SetPlayerBid(container_index, playerid, bidAmount);

    Container:InsertPlayerBidDataToDB(container_index, playerid, bidAmount);

    Container:IncrementBidders(playerid, container_index);

    return CF_SUCCESS_BID;
}

stock Container:CreateObject(container_index)
{
    return CreateDynamicObject(
        Container:GetObjectModel(container_index), 
        Container:GetObjectCoordX(container_index), Container:GetObjectCoordY(container_index), Container:GetObjectCoordZ(container_index), 
        0.0, 0.0, 0.0
    );
}

stock Container:Create3DTextLabel(container_index)
{
    new contName[32];
    format(contName, sizeof(contName), "Container ID: %d, Type: %d", Container:GetID(container_index), Container:GetType(container_index));
    CreateDynamic3DTextLabel(
        contName, -1, 
        Container:GetObjectCoordX(container_index), Container:GetObjectCoordY(container_index) + CONTAINER_INTERACTION_Y_COORD_OFFSET, Container:GetObjectCoordZ(container_index),
        10.0
    );
}

stock Container:CreateInteractionArea(container_index)
{
    new STREAMER_TAG_AREA:area = CreateDynamicCircle(
        Container:GetObjectCoordX(container_index), Container:GetObjectCoordY(container_index) + CONTAINER_INTERACTION_Y_COORD_OFFSET,
        2.0
    );
    Streamer_SetIntData(STREAMER_TYPE_AREA, STREAMER_TAG_AREA:area, E_STREAMER_EXTRA_ID, CONTAINERS_INDEX_OFFSET + container_index);
}

stock Container:CreateVisuals(container_index)
{
    Container:CreateObject(container_index);
    Container:Create3DTextLabel(container_index);
    Container:CreateInteractionArea(container_index);
}

#define CONTAINER_START_BIDDING_CALLBACK_NAME "Cont_ProcessBiddingStart" // naming reference for timer
forward Container:ProcessBiddingStart();
public Container:ProcessBiddingStart()
{
    foreach(new container_index : CreatedContainerData)
    {
        Container:ResetBiddingState(container_index);
    }

    Container:SetBiddingStatus(true);
    Container:SendGlobalMessage(E_CONTAINER_CLIENT_MESSAGE:CC_CONTAINERS_BIDDING_STARTED);

    Container:StartShutdownTimer(CONTAINER_START_BIDDING_SHUTDOWN_TIMER_INTERVAL_IN_MS);
}

stock Container:StartBiddingTimer(interval)
{
    SetTimer(CONTAINER_START_BIDDING_CALLBACK_NAME, interval, false);
}

#define CONTAINER_START_BIDDING_SHUTDOWN_CALLBACK_NAME "Cont_ProcessBiddingShutdown" // naming reference for timer
forward Container:ProcessBiddingShutdown();
public Container:ProcessBiddingShutdown()
{
    Container:SetBiddingStatus(false);
    Container:SendGlobalMessage(E_CONTAINER_CLIENT_MESSAGE:CC_CONTAINERS_BIDDING_ENDED);

    Container:ClearAllTopBiddersTD();
    Container:FetchWinnersDataFromDB();

    Container:StartBiddingTimer(CONTAINER_START_BIDDING_TIMER_INTERVAL_IN_MS);
}

stock Container:StartShutdownTimer(interval)
{
    SetTimer(CONTAINER_START_BIDDING_SHUTDOWN_CALLBACK_NAME, interval, false);
}

stock Container:GetIndexFromAreaID(areaid)
{
    new container_index = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);
    return container_index - CONTAINERS_INDEX_OFFSET;
}

stock Container:GetPlayerContainerIndex(playerid)
{
    new area[1];
    GetPlayerDynamicAreas(playerid, area);

    #if defined CONTAINERS_DEBUG_MODE
        printf("[Containers]: player %d, area %d", playerid, area[0]);
    #endif

    if(area[0] == 0) 
    {
        return INVALID_CONTAINER_ID;
    }

    return Container:GetIndexFromAreaID(area[0]);
}

stock bool:Container:CanPlayerInteract(playerid)
{
    new container_index = Container:GetPlayerContainerIndex(playerid);

    return Container:IsValidIndex(container_index)
        && Container:GetBiddingStatus() == true;
}

stock E_CONTAINER_FLAGS:Container:TryPlaceBid(playerid, container_index, bidAmount)
{
    if(GetPlayerMoney(playerid) < bidAmount)
    {
        return CF_INSUFFICIENT_FUNDS;
    }

    if(bidAmount >= Container:GetMinimalBid(container_index) && bidAmount <= Container:GetMaximalBid(container_index))
    {
        return CF_SUCCESS_BID;
    }
    else
    {
        return CF_INCORRECT_BID_SUM;
    }
}

stock Container:SendClientMessage(playerid, E_CONTAINER_CLIENT_MESSAGE:messageType)
{
    switch(E_CONTAINER_CLIENT_MESSAGE:messageType)
    {
        case CC_SYSTEM_ERROR:
        {
            return SendClientMessage(playerid, -1, !"Произошла системная ошибка");
        }

        case CC_ER_BID_NT_STRTD_OR_INV_IDX:
        {
            return SendClientMessage(playerid, -1, !"Торги еще не начались");
        }
        case CC_BID_CONFIRMED:
        {
            return SendClientMessage(playerid, -1, !"Ставка принята");
        }
        case CC_BID_INCORRECT_SUM:
        {
            return SendClientMessage(playerid, -1, !"Указана неправильная сумма");
        }
        case CC_BID_INSUFFICIENT_FUNDS:
        {
            return SendClientMessage(playerid, -1, !"Недостаточно средств для этой ставки");
        }
        case CC_BID_ALREADY_PLACED:
        {
            return SendClientMessage(playerid, -1, !"Вы уже сделали ставку на этот контейнер");
        }
        case CC_BID_NO_FREE_SLOT:
        {
            return SendClientMessage(playerid, -1, !"На этот контейнер уже поставили слишком многие");
        }
        case CC_BID_WON:
        {
            return SendClientMessage(playerid, -1, !"Поздравляем! Вы выиграли ставку");
        }

        default:
        {
            return SendClientMessage(playerid, -1, !"Произошла неизвестная ошибка");
        }
    }

    return 0;
}

stock Container:SendGlobalMessage(E_CONTAINER_CLIENT_MESSAGE:messageType)
{
    switch(E_CONTAINER_CLIENT_MESSAGE:messageType)
    {
        case CC_SYSTEM_ERROR:
        {
            return print("[Containers]: System error in global message sending");
        }
        case CC_CONTAINERS_BIDDING_STARTED:
        {
            return SendClientMessageToAll(-1, !"Торги начались, 1 минута на ставки");
        }
        case CC_CONTAINERS_BIDDING_ENDED:
        {
            return SendClientMessageToAll(-1, !"Торги окончены");
        }
        default:
        {
            return print("[Containers]: Unknown error in global message sending");
        }
    }

    return 0;
}

stock Container:ShowBidDialogForPlayer(playerid, container_index)
{
    new currentBid[128];
    format(currentBid, 128, "\
        Текущая ставка: $%d\n\
        Минимальная ставка: $%d\n\
        Максимальная ставка: $%d", 
        Container:GetTotalBid(container_index), 
        floatround(Container:GetMinimalBid(container_index)),
        floatround(Container:GetMaximalBid(container_index))
    );

    ShowPlayerDialog(playerid, DIALOG_CONTAINER_BID, DIALOG_STYLE_INPUT, !"Укажите ставку:", currentBid, !"Подтвердить", !"Отмена");
}

forward Text:Container:GetContainerMenuNicksTDID(container_index, textdraw_index);

stock Container:RefreshTopBiddersTD(container_index)
{
    new topIds[MAX_VISIBLE_CONTAINER_PARTICIPANTS] = {INVALID_PLAYER_ID, ...};
    new topBids[MAX_VISIBLE_CONTAINER_PARTICIPANTS];

    foreach(new pid : ContainerBiddersIter[container_index])
    {
        new playerBidAmount = Container:GetPlayerBid(container_index, pid);

        for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
        {
            if(playerBidAmount > topBids[i])
            {
                for(new j = MAX_VISIBLE_CONTAINER_PARTICIPANTS - 1; j > i; j--)
                {
                    new prev_index = j - 1;
                    topIds[j] = topIds[prev_index];
                    topBids[j] = topBids[prev_index];
                }

                topIds[i] = pid;
                topBids[i] = playerBidAmount;

                break;
            }
        }
    }

    new containerTotalBid = Container:GetTotalBid(container_index);

    for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
    {
        if(topIds[i] != INVALID_PLAYER_ID)
        {
            new string[64], 
                playerID = topIds[i], 
                percent = topBids[i] * 100 / containerTotalBid,
                _playerName[MAX_PLAYER_NAME];
    
            Player:GetPlayerNameByID(playerID, _playerName);

            format(string, sizeof(string), "%s~y~%d%%", _playerName, percent);
            TextDrawSetString(Container:GetContainerMenuNicksTDID(container_index, i), string);
        }
        else
        {
            TextDrawSetString(Container:GetContainerMenuNicksTDID(container_index, i), "None");
        }
    }
}

stock Container:ClearTopBiddersTD(container_index)
{
    for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
    {
        TextDrawSetString(Container:GetContainerMenuNicksTDID(container_index, i), "None");
    }
}

stock Container:ClearAllTopBiddersTD()
{
    foreach(new container_index : CreatedContainerData)
    {
        for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
        {
            TextDrawSetString(Container:GetContainerMenuNicksTDID(container_index, i), "None");
        }
    }
}

stock Container:ShowMenuForPlayer(playerid, container_index)
{
    for(new i = 0; i < Container:GetMenuTDCount(); i++)
    {
        TextDrawShowForPlayer(playerid, Container:GetContainerMenuTDID(i));
    }

    for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
    {
        TextDrawShowForPlayer(playerid, Container:GetContainerMenuNicksTDID(container_index, i));
    }

    Container:ShowPlayerCursor(playerid);
    Container:SetPlayerMenuContainerIDx(playerid, container_index);
}

stock Container:HideMenuForPlayer(playerid, container_index)
{
    for(new i = 0; i < Container:GetMenuTDCount(); i++)
    {
        TextDrawHideForPlayer(playerid, Container:GetContainerMenuTDID(i));
    }

    for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
    {
        TextDrawHideForPlayer(playerid, Container:GetContainerMenuNicksTDID(container_index, i));
    }

    CancelSelectTextDraw(playerid);
    Container:SetPlayerMenuContainerIDx(playerid, INVALID_CONTAINER_ID);
}

stock Container:ShowWonTDForPlayer(playerid, won_prize_model)
{
    if(Container:IsPlayerMenuOpened(playerid) == true)
    {
        Container:HideMenuForPlayer(playerid, Container:GetPlayerMenuContainerIDx(playerid));
    }

    TextDrawSetPreviewModel(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), won_prize_model);

    for(new i = 0; i < Container:GetWonMenuTDCount(); i++)
    {
        TextDrawShowForPlayer(playerid, Container:GetContainerWonTDID(i));
    }

    Container:ShowPlayerCursor(playerid);
    Container:SetPlayerMenuContainerIDx(playerid);
}

stock Container:HideWonTDForPlayer(playerid)
{
    for(new i = 0; i < Container:GetWonMenuTDCount(); i++)
    {
        TextDrawHideForPlayer(playerid, Container:GetContainerWonTDID(i));
    }

    CancelSelectTextDraw(playerid);
    Container:SetPlayerMenuContainerIDx(playerid, INVALID_CONTAINER_ID);
}

stock Container:ShowPlayerCursor(playerid)
{
    SelectTextDraw(playerid, CONTAINER_SELECT_TEXTDRAW_HEX_COLOR);
}

stock Container:CreateMenuTDs()
{
    Container:SetContainerMenuTDID(0, TextDrawCreate(269.0000, 152.9777, "Box")); // global background
    TextDrawLetterSize(Container:GetContainerMenuTDID(0), 0.0000, 10.2999);
    TextDrawTextSize(Container:GetContainerMenuTDID(0), 356.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(0), 1);
    TextDrawColor(Container:GetContainerMenuTDID(0), -1);
    TextDrawUseBox(Container:GetContainerMenuTDID(0), 1);
    TextDrawBoxColor(Container:GetContainerMenuTDID(0), 255);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(0), 255);
    TextDrawFont(Container:GetContainerMenuTDID(0), 1);
    TextDrawSetProportional(Container:GetContainerMenuTDID(0), 0);
    TextDrawSetShadow(Container:GetContainerMenuTDID(0), 0);

    Container:SetContainerMenuTDID(1, TextDrawCreate(270.5000, 155.1555, "Box")); // global background
    TextDrawLetterSize(Container:GetContainerMenuTDID(1), 0.0000, 9.8000);
    TextDrawTextSize(Container:GetContainerMenuTDID(1), 354.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(1), 1);
    TextDrawColor(Container:GetContainerMenuTDID(1), -1);
    TextDrawUseBox(Container:GetContainerMenuTDID(1), 1);
    TextDrawBoxColor(Container:GetContainerMenuTDID(1), -226);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(1), 255);
    TextDrawFont(Container:GetContainerMenuTDID(1), 1);
    TextDrawSetProportional(Container:GetContainerMenuTDID(1), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(1), 0);

    Container:SetContainerMenuTDID(2, TextDrawCreate(310.7500, 224.5332, "Box")); // exit backgroud
    TextDrawLetterSize(Container:GetContainerMenuTDID(2), 0.0000, 1.1248);
    TextDrawTextSize(Container:GetContainerMenuTDID(2), 342.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(2), 1);
    TextDrawColor(Container:GetContainerMenuTDID(2), -1);
    TextDrawUseBox(Container:GetContainerMenuTDID(2), 1);
    TextDrawBoxColor(Container:GetContainerMenuTDID(2), 118);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(2), 255);
    TextDrawFont(Container:GetContainerMenuTDID(2), 1);
    TextDrawSetProportional(Container:GetContainerMenuTDID(2), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(2), 0);

    Container:SetContainerMenuTDID(3, TextDrawCreate(310.5000, 224.2221, "Box")); // exit backgroud
    TextDrawLetterSize(Container:GetContainerMenuTDID(3), 0.0000, 0.9749);
    TextDrawTextSize(Container:GetContainerMenuTDID(3), 342.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(3), 1);
    TextDrawColor(Container:GetContainerMenuTDID(3), -1);
    TextDrawUseBox(Container:GetContainerMenuTDID(3), 1);
    TextDrawBoxColor(Container:GetContainerMenuTDID(3), 255);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(3), 255);
    TextDrawFont(Container:GetContainerMenuTDID(3), 1);
    TextDrawSetProportional(Container:GetContainerMenuTDID(3), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(3), 0);

    Container:SetContainerMenuTDID(4, TextDrawCreate(273.7500, 224.5332, "Box")); // set bid background
    TextDrawLetterSize(Container:GetContainerMenuTDID(4), 0.0000, 1.1498);
    TextDrawTextSize(Container:GetContainerMenuTDID(4), 305.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(4), 1);
    TextDrawColor(Container:GetContainerMenuTDID(4), -1);
    TextDrawUseBox(Container:GetContainerMenuTDID(4), 1);
    TextDrawBoxColor(Container:GetContainerMenuTDID(4), 118);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(4), 255);
    TextDrawFont(Container:GetContainerMenuTDID(4), 1);
    TextDrawSetProportional(Container:GetContainerMenuTDID(4), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(4), 0);

    Container:SetContainerMenuTDID(5, TextDrawCreate(273.5000, 224.2221, "Box")); // set bid background
    TextDrawLetterSize(Container:GetContainerMenuTDID(5), 0.0000, 0.9998);
    TextDrawTextSize(Container:GetContainerMenuTDID(5), 305.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(5), 1);
    TextDrawColor(Container:GetContainerMenuTDID(5), -1);
    TextDrawUseBox(Container:GetContainerMenuTDID(5), 1);
    TextDrawBoxColor(Container:GetContainerMenuTDID(5), 255);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(5), 255);
    TextDrawFont(Container:GetContainerMenuTDID(5), 1);
    TextDrawSetProportional(Container:GetContainerMenuTDID(5), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(5), 0);

    Container:SetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID, TextDrawCreate(280.5000, 225.1555, "SET_BID"));
    TextDrawLetterSize(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 0.1041, 0.8377);
    TextDrawTextSize(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 297.0000, 10.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 1);
    TextDrawColor(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), -1);
    TextDrawSetOutline(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 255);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 255);
    TextDrawFont(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 2);
    TextDrawSetProportional(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), 0);
    TextDrawSetSelectable(Container:GetContainerMenuTDID(CONTAINER_SET_BID_TEXTDRAW_ID), true);

    Container:SetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID, TextDrawCreate(319.7500, 225.1555, "CLOSE"));
    TextDrawLetterSize(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 0.1041, 0.8377);
    TextDrawTextSize(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 333.0000, 10.0000);
    TextDrawAlignment(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 1);
    TextDrawColor(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), -1);
    TextDrawSetOutline(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 255);
    TextDrawBackgroundColor(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 255);
    TextDrawFont(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 2);
    TextDrawSetProportional(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 1);
    TextDrawSetShadow(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), 0);
    TextDrawSetSelectable(Container:GetContainerMenuTDID(CONTAINER_CLOSE_MENU_TEXTDRAW_ID), true);
}

stock Container:CreateMenuNicknamesTDs(container_index)
{
    for(new i = 0; i < MAX_VISIBLE_CONTAINER_PARTICIPANTS; i++)
    {
        Container:SetContainerMenuNicksTDID(container_index, i, TextDrawCreate(308.2500, 158.2600 + (i * 11.8), "None"));
        TextDrawLetterSize(Container:GetContainerMenuNicksTDID(container_index, i), 0.2222, 1.2733);
        TextDrawAlignment(Container:GetContainerMenuNicksTDID(container_index, i), 2);
        TextDrawColor(Container:GetContainerMenuNicksTDID(container_index, i), -1);
        TextDrawSetOutline(Container:GetContainerMenuNicksTDID(container_index, i), 255);
        TextDrawBackgroundColor(Container:GetContainerMenuNicksTDID(container_index, i), 255);
        TextDrawFont(Container:GetContainerMenuNicksTDID(container_index, i), 1);
        TextDrawSetProportional(Container:GetContainerMenuNicksTDID(container_index, i), 1);
        TextDrawSetShadow(Container:GetContainerMenuNicksTDID(container_index, i), 0);
    }
}
stock Container:CreateWonTDs()
{
    Container:SetContainerWonTDID(0, TextDrawCreate(285.0000, 175.9999, "Box"));
    TextDrawLetterSize(Container:GetContainerWonTDID(0), 0.0000, 6.1749);
    TextDrawTextSize(Container:GetContainerWonTDID(0), 346.0000, 0.0000);
    TextDrawAlignment(Container:GetContainerWonTDID(0), 1);
    TextDrawColor(Container:GetContainerWonTDID(0), -1);
    TextDrawUseBox(Container:GetContainerWonTDID(0), 1);
    TextDrawBoxColor(Container:GetContainerWonTDID(0), 137);
    TextDrawBackgroundColor(Container:GetContainerWonTDID(0), 255);
    TextDrawFont(Container:GetContainerWonTDID(0), 1);
    TextDrawSetProportional(Container:GetContainerWonTDID(0), 1);
    TextDrawSetShadow(Container:GetContainerWonTDID(0), 0);

    Container:SetContainerWonTDID(1, TextDrawCreate(292.2500, 180.9777, "YOU_WON!"));
    TextDrawLetterSize(Container:GetContainerWonTDID(1), 0.2575, 1.2048);
    TextDrawAlignment(Container:GetContainerWonTDID(1), 1);
    TextDrawColor(Container:GetContainerWonTDID(1), -1);
    TextDrawBackgroundColor(Container:GetContainerWonTDID(1), 255);
    TextDrawFont(Container:GetContainerWonTDID(1), 1);
    TextDrawSetProportional(Container:GetContainerWonTDID(1), 1);
    TextDrawSetShadow(Container:GetContainerWonTDID(1), 0);

    Container:SetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID, TextDrawCreate(270.5000, 159.0444, ""));
    TextDrawTextSize(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 90.0000, 90.0000);
    TextDrawAlignment(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 1);
    TextDrawColor(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), -1);
    TextDrawBackgroundColor(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 0);
    TextDrawFont(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 5);
    TextDrawSetProportional(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 0);
    TextDrawSetShadow(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 0);
    TextDrawSetPreviewModel(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 451);
    TextDrawSetPreviewRot(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 0.0000, 0.0000, 0.0000, 1.0000);
    TextDrawSetPreviewVehCol(Container:GetContainerWonTDID(CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID), 1, 1);

    Container:SetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID, TextDrawCreate(304.2500, 220.1777, "CLOSE"));
    TextDrawLetterSize(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 0.2164, 0.9622);
    TextDrawTextSize(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 327.0000, 10.0000);
    TextDrawAlignment(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 1);
    TextDrawColor(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), -1);
    TextDrawBackgroundColor(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 255);
    TextDrawFont(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 1);
    TextDrawSetProportional(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 1);
    TextDrawSetShadow(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), 0);
    TextDrawSetSelectable(Container:GetContainerWonTDID(CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID), true);
}

stock Container:GetPlayerIDByName(const player_name[])
{
    foreach(Player, i)
    {
        new _playerName[MAX_PLAYER_NAME];
        Player:GetPlayerNameByID(i, _playerName);

        if(!strcmp(_playerName, player_name, true))
        {
            return i;
        }
    }

    return INVALID_PLAYER_ID;
}