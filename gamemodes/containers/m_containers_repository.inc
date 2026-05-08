#define CONTAINER_LOAD_DATA_CALLBACK_NAME "Cont_LoadDataFromDatabase"
forward Container:LoadDataFromDatabase();
public Container:LoadDataFromDatabase() 
{
    new rowCount = cache_num_rows();

    if(rowCount == 0)
    {
        print(!"No container data found in database");
        return false;
    }

    if(rowCount > MAX_CONTAINERS)
    {
        printf(!"Too many containers in database. Max allowed is: %d", MAX_CONTAINERS);
        return false;
    }

    for(new row = 0; row < rowCount; row++)
    {
        new id, type, startPrice, objModel;
        new Float:coordX, Float:coordY, Float:coordZ;

        cache_get_value_name_int(row, CONTAINERS_COLUMN_ID, id);
        cache_get_value_name_int(row, CONTAINERS_COLUMN_TYPE, type);
        cache_get_value_name_int(row, CONTAINERS_COLUMN_START_PRICE, startPrice);
        cache_get_value_name_int(row, CONTAINERS_COLUMN_OBJ_MODEL, objModel);
        cache_get_value_name_float(row, CONTAINERS_COLUMN_OBJ_COORD_X, coordX);
        cache_get_value_name_float(row, CONTAINERS_COLUMN_OBJ_COORD_Y, coordY);
        cache_get_value_name_float(row, CONTAINERS_COLUMN_OBJ_COORD_Z, coordZ);

        Container:SetID(row, id);
        Container:SetType(row, type);
        Container:SetStartPrice(row, startPrice);
        Container:SetObjectModel(row, objModel);
        Container:SetObjectCoordX(row, coordX);
        Container:SetObjectCoordY(row, coordY);
        Container:SetObjectCoordZ(row, coordZ);

        Container:ResetPlayersBid(row);
        Container:CreateVisuals(row);
        Container:FetchTotalBidDataFromDB(row);
        Container:CreateMenuNicknamesTDs(row);

        Container:IncrementCreatedCount(row);
    }

    Container:StartBiddingTimer(CONTAINER_START_BIDDING_TIMER_INTERVAL_IN_MS);

    printf("[Containers]: Fetched and loaded %d containers from DB", rowCount);
    return true;
}

stock Container:FetchDataFromDatabase() 
{
    mysql_tquery(handle, "\
        SELECT * FROM "CONTAINERS_TABLE_NAME"", ""CONTAINER_LOAD_DATA_CALLBACK_NAME""
    );
}

#define CONTAINER_LOAD_TOTAL_BID_DATA_CALLBACK_NAME "Cont_LoadTotalBidDataFromDB"
forward Container:LoadTotalBidDataFromDB(container_index);
public Container:LoadTotalBidDataFromDB(container_index) 
{
    new bool:isNull;
    cache_is_value_index_null(0, 0, isNull);

    if(isNull == false)
    {
        new totalBid;
        cache_get_value_index_int(0, 0, totalBid);
        Container:SetTotalBid(container_index, totalBid);
    }

    return true;
}

stock Container:FetchTotalBidDataFromDB(container_index)
{
    new string[256], containerID = container_index + 1;
    mysql_format(handle, string, 256, "\
        SELECT SUM("CONTAINERS_AUCTION_PLAYERS_COLUMN_TOTAL_BID") \
        FROM `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
        WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION"` = '1' \
        AND `"CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID"` = '%d'",
        containerID
    );

    mysql_tquery(handle, string, ""CONTAINER_LOAD_TOTAL_BID_DATA_CALLBACK_NAME"", "i", container_index);
}

stock Container:InsertPlayerBidDataToDB(container_index, playerid, betAmount)
{
    new _playerName[MAX_PLAYER_NAME];
    Player:GetPlayerNameByID(playerid, _playerName);

    new string[256];
    mysql_format(handle, string, 256, "\
        INSERT INTO "CONTAINERS_AUCTION_PLAYERS_TABLE_NAME" \
        ("CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME", "CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID", \
        "CONTAINERS_AUCTION_PLAYERS_COLUMN_TOTAL_BID", "CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION") \
        VALUES ('%e', %d, %d, 1)", 
        _playerName, Container:GetID(container_index), betAmount
    );

    mysql_tquery(handle, string);
}

#define CONTAINER_LOAD_PLAYER_BID_DATA_CALLBACK_NAME "Cont_LoadPlrBidDataFromDatabase"
forward bool:Container:LoadPlrBidDataFromDatabase(playerid);
public bool:Container:LoadPlrBidDataFromDatabase(playerid) 
{
    new rowCount = cache_num_rows(); 

    if(rowCount == 0)
    {
        return false;
    }

    for(new row = 0; row < rowCount; row++)
    {
        new containerID;
        cache_get_value_name_int(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID, containerID);

        new totalBid;
        cache_get_value_name_int(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_TOTAL_BID, totalBid);

        new isActualAuction;
        cache_get_value_name_int(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION, isActualAuction);

        if(isActualAuction == 0)
        {
            new bool:isWinnerNull;
            cache_is_value_name_null(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_WON_PRIZE_ID, isWinnerNull);

            if(!isWinnerNull)
            {
                new wonPrizeID;
                cache_get_value_name_int(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_WON_PRIZE_ID, wonPrizeID);

                Container:ProcessPlayerWinning(playerid, wonPrizeID);
            }

            continue;
        }

        new containerIndex = containerID - 1;

        if(containerIndex < 0)
        {
            printf("LoadPlrBidDataFromDatabase error, containerIndex: %d",containerIndex);
        }
        Container:SetPlayerBid(containerIndex, playerid, totalBid);
        Container:IncrementBidders(playerid, containerIndex);
    }

    Container:DeletePlayerDataFromDB(playerid);

    return true;
}

stock Container:ProcessPlayerWinning(playerid, won_prize_id, won_prize_model = 0)
{
    Container:SendClientMessage(playerid, CC_BID_WON);

    if(won_prize_model != 0) 
    {
        Container:ShowWonTDForPlayer(playerid, won_prize_model);
    }
    else
    {
        Container:FetchPrizeObjectModelByID(playerid, won_prize_id);
    }
}

stock Container:FetchPlayerBidDataFromDB(playerid)
{
    new _playerName[MAX_PLAYER_NAME];
    Player:GetPlayerNameByID(playerid, _playerName);

    new string[256];
    mysql_format(handle, string, 256, "\
        SELECT * FROM "CONTAINERS_AUCTION_PLAYERS_TABLE_NAME" \
        WHERE "CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME" = '%e'", 
        _playerName
    );

    mysql_tquery(handle, string, ""CONTAINER_LOAD_PLAYER_BID_DATA_CALLBACK_NAME"", "i", playerid);
}

#define CONTAINER_LOAD_WINNERS_DATA_CALLBACK_NAME "Cont_LoadWinnersDataFromDB"
forward bool:Container:LoadWinnersDataFromDB();
public bool:Container:LoadWinnersDataFromDB()
{
    new rowCount = cache_num_rows(); 

    if(rowCount == 0)
    {
        return false;
    }

    Container:ResetPendingPrizeFetches();

    new createdContainersCount = Container:GetCreatedCount();

    new winningNumber[MAX_CONTAINERS];
    new currentBorder[MAX_CONTAINERS];
    new bool:isContainerWinnerFound[MAX_CONTAINERS];
    
    for(new i = 0; i < createdContainersCount; i++)
    {
        winningNumber[i] = random(Container:GetTotalBid(i));
    }

    for(new row = 0; row < rowCount; row++)
    {
        new containerID;
        cache_get_value_name_int(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID, containerID);

        if(containerID < 1 || containerID > createdContainersCount)
        {
            print("Invalid container ID found in winners fetching");
            continue;
        }

        new containerIndex = containerID - 1;

        if(Container:GetTotalBid(containerIndex) == 0)
        {
            continue;
        }

        if(isContainerWinnerFound[containerIndex] == true)
        {
            continue;
        }

        new totalBid;
        cache_get_value_name_int(row, "total", totalBid);

        currentBorder[containerIndex] += totalBid;

        if(currentBorder[containerIndex] > winningNumber[containerIndex])
        {
            isContainerWinnerFound[containerIndex] = true;

            new cachedPlayerName[MAX_PLAYER_NAME];
            cache_get_value_name(row, CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME, cachedPlayerName, MAX_PLAYER_NAME);

            Container:IncPendingPrizeFetches();
            Container:FetchRandomPrizeFromDB(cachedPlayerName, .container_id = containerIndex + 1);

            #if defined CONTAINERS_DEBUG_MODE
                printf("[Containers]: player: %s, total Bid: %d", cachedPlayerName, totalBid);
            #endif
        }
    }

    Container:ClearAllActualAuctionInDB();

    return true;
}

stock Container:FetchWinnersDataFromDB()
{
    mysql_tquery(handle, "\
        SELECT `"CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID"`, `"CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME"`, SUM(`"CONTAINERS_AUCTION_PLAYERS_COLUMN_TOTAL_BID"`) AS total \
        FROM `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
        WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION"` = '1' \
        GROUP BY `"CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID"`, `"CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME"` \
        ", ""CONTAINER_LOAD_WINNERS_DATA_CALLBACK_NAME""
    );
}

forward bool:Container:DecAndCheckPrizeFetches();

#define CONTAINER_PRIZE_FETCHED_CALLBACK_NAME "Cont_OnPrizeFetched"
forward bool:Container:OnPrizeFetched(const player_name[], container_id);
public bool:Container:OnPrizeFetched(const player_name[], container_id)
{
    if(Container:DecAndCheckPrizeFetches())
    {
        Container:DelAllStaleAuctDataFromDB();
    }

    if(cache_num_rows() == 0)
    {
        return false;
    }

    new wonPrizeID;
    cache_get_value_name_int(0, CONTAINERS_ITEMS_COLUMN_ID, wonPrizeID);

    new wonPrizeModel;
    cache_get_value_name_int(0, CONTAINERS_ITEMS_COLUMN_OBJECT_MODEL, wonPrizeModel);

    new targetPlayerid = Container:GetPlayerIDByName(player_name);

    new string[300];

    if(targetPlayerid != INVALID_PLAYER_ID)
    {
        Container:ProcessPlayerWinning(targetPlayerid, wonPrizeID, wonPrizeModel);

        mysql_format(handle, string, 300, "\
            DELETE FROM `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
            WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME"` = '%e' \
            AND `"CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID"` = '%d'",
            player_name, container_id
        );
    }
    else
    {
        mysql_format(handle, string, 300, "\
            UPDATE `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
            SET `"CONTAINERS_AUCTION_PLAYERS_COLUMN_WON_PRIZE_ID"` = '%d' \
            WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME"` = '%e' \
            AND `"CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID"` = '%d'",
            wonPrizeID, player_name, container_id
        );
    }
    mysql_tquery(handle, string);

    return true;
}

stock Container:FetchRandomPrizeFromDB(const player_name[], container_id)
{
    new string[600];
    mysql_format(handle, string, 600, "\
        SELECT `"CONTAINERS_ITEMS_COLUMN_ID"`, `"CONTAINERS_ITEMS_COLUMN_OBJECT_MODEL"` FROM `"CONTAINERS_ITEMS_TABLE_NAME"` \
        WHERE `"CONTAINERS_ITEMS_COLUMN_CONTAINER_TYPE"` = \
        (SELECT `"CONTAINERS_COLUMN_TYPE"` FROM `"CONTAINERS_TABLE_NAME"` WHERE `"CONTAINERS_COLUMN_ID"` = '%d') \
        ORDER BY RAND() / `"CONTAINERS_ITEMS_COLUMN_CHANCE"` DESC LIMIT 1",
        container_id
    );

    mysql_tquery(handle, string, ""CONTAINER_PRIZE_FETCHED_CALLBACK_NAME"", "sd", player_name, container_id);
}

stock Container:ClearAllActualAuctionInDB()
{
    mysql_tquery(handle, "\
        UPDATE `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
        SET `"CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION"` = '0' \
        WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION"` = '1'"
    );
}

stock Container:DelAllStaleAuctDataFromDB()
{
    mysql_tquery(handle, "\
        DELETE FROM `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
        WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION"` = '0' \
        AND `"CONTAINERS_AUCTION_PLAYERS_COLUMN_WON_PRIZE_ID"` IS NULL"
    );
}

stock Container:DeletePlayerDataFromDB(playerid)
{
    new _playerName[MAX_PLAYER_NAME];
    Player:GetPlayerNameByID(playerid, _playerName);

    new string[256];
    mysql_format(handle, string, 256, "\
        DELETE FROM `"CONTAINERS_AUCTION_PLAYERS_TABLE_NAME"` \
        WHERE `"CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME"` = '%e' \
        AND `"CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION"` = '0'", 
        _playerName
    );

    mysql_tquery(handle, string);
}

#define CONTAINER_GET_PRIZE_OBJECT_MODEL_CALLBACK_NAME "Cont_GetPrizeObjectModelByID"
forward bool:Container:GetPrizeObjectModelByID(playerid);
public bool:Container:GetPrizeObjectModelByID(playerid)
{
    if(cache_num_rows() == 0)
    {
        return false;
    }

    new wonPrizeModel;
    cache_get_value_index_int(0, 0, wonPrizeModel);
    Container:ShowWonTDForPlayer(playerid, wonPrizeModel);

    return true;
}

stock Container:FetchPrizeObjectModelByID(playerid, prize_id)
{
    new string[256];
    mysql_format(handle, string, 256, "\
        SELECT `"CONTAINERS_ITEMS_COLUMN_OBJECT_MODEL"` \
        FROM `"CONTAINERS_ITEMS_TABLE_NAME"` \
        WHERE `"CONTAINERS_ITEMS_COLUMN_ID"` = %d", 
        prize_id
    );
    mysql_tquery(handle, string, ""CONTAINER_GET_PRIZE_OBJECT_MODEL_CALLBACK_NAME"", "i", playerid);
}

