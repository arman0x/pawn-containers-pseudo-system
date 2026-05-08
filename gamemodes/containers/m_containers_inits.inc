#define Container:%0(%1)                                        Cont_%0(%1)

#define CONTAINERS_DEBUG_MODE

//
#define MAX_CONTAINERS                                          10
#define INVALID_CONTAINER_ID                                    -1
#define MAX_VISIBLE_CONTAINER_PARTICIPANTS                      5
#define CONTAINERS_INDEX_OFFSET                                 1
#define CONTAINER_MINIMAL_BID_MULTIPLIER                        1.1
#define CONTAINER_MAXIMAL_BID_MULTIPLIER                        2.0
#define CONTAINER_START_BIDDING_TIMER_INTERVAL_IN_MS            60000
#define CONTAINER_START_BIDDING_TIMER_RANDOM_INTERVAL_IN_MS     5000
#define CONTAINER_START_BIDDING_SHUTDOWN_TIMER_INTERVAL_IN_MS   60000
#define DIALOG_CONTAINER_BID                                    1
#define CONTAINER_SELECT_TEXTDRAW_HEX_COLOR                     0xAFAFAFAA
#define CONTAINER_INTERACTION_Y_COORD_OFFSET                    4.0
//        
#define CONTAINER_SET_BID_TEXTDRAW_ID                                     6
#define CONTAINER_CLOSE_MENU_TEXTDRAW_ID                                  7
#define CONTAINER_PREVIEW_MODEL_TEXTDRAW_ID                               2
#define CONTAINER_CLOSE_WON_MENU_TEXTDRAW_ID                              3
//     
#define CONTAINERS_TABLE_NAME                                   "containers"
#define CONTAINERS_COLUMN_ID                                    "id"
#define CONTAINERS_COLUMN_TYPE                                  "type"
#define CONTAINERS_COLUMN_START_PRICE                           "start_price"
#define CONTAINERS_COLUMN_OBJ_MODEL                             "object_model"
#define CONTAINERS_COLUMN_OBJ_COORD_X                           "object_coord_x"
#define CONTAINERS_COLUMN_OBJ_COORD_Y                           "object_coord_y"
#define CONTAINERS_COLUMN_OBJ_COORD_Z                           "object_coord_z"

#define CONTAINERS_AUCTION_PLAYERS_TABLE_NAME                   "container_auction_players"
#define CONTAINERS_AUCTION_PLAYERS_COLUMN_ID                    "id"
#define CONTAINERS_AUCTION_PLAYERS_COLUMN_PLAYER_NAME           "player_name"
#define CONTAINERS_AUCTION_PLAYERS_COLUMN_CONTAINER_ID          "container_id"
#define CONTAINERS_AUCTION_PLAYERS_COLUMN_TOTAL_BID             "total_bid"
#define CONTAINERS_AUCTION_PLAYERS_COLUMN_IS_ACTUAL_AUCTION     "is_actual_auction"
#define CONTAINERS_AUCTION_PLAYERS_COLUMN_WON_PRIZE_ID          "won_prize_id"

#define CONTAINERS_ITEMS_TABLE_NAME                             "container_items"
#define CONTAINERS_ITEMS_COLUMN_ID                              "id"
#define CONTAINERS_ITEMS_COLUMN_CHANCE                          "chance"
#define CONTAINERS_ITEMS_COLUMN_CONTAINER_TYPE                  "container_type"
#define CONTAINERS_ITEMS_COLUMN_OBJECT_MODEL                    "object_model"
//

//
enum E_CONTAINER_CLIENT_MESSAGE
{
    CC_SYSTEM_ERROR = -1,

    // Local
    CC_ER_BID_NT_STRTD_OR_INV_IDX,
    CC_BID_CONFIRMED,
    CC_BID_INCORRECT_SUM,
    CC_BID_INSUFFICIENT_FUNDS,
    CC_BID_ALREADY_PLACED,
    CC_BID_NO_FREE_SLOT,
    CC_BID_WON,

    // Global
    CC_CONTAINERS_BIDDING_STARTED,
    CC_CONTAINERS_BIDDING_ENDED
}

enum E_CONTAINER_FLAGS
{
    CF_SYSTEM_ERROR = -1,

    CF_ALREADY_PLACED_BID,
    CF_NO_FREE_BID_SLOT,
    CF_INCORRECT_BID_SUM,
    CF_INSUFFICIENT_FUNDS,
    CF_SUCCESS_BID
}

enum E_CONTAINER_DATA
{
    CONTAINER_ID,
    CONTAINER_TYPE,
    CONTAINER_START_PRICE,
    CONTAINER_TOTAL_BID,
    CONTAINER_PLACED_BID_PLAYERS[MAX_PLAYERS],

    CONTAINER_OBJ_MODEL,
    Float:CONTAINER_OBJ_COORD_X,
    Float:CONTAINER_OBJ_COORD_Y,
    Float:CONTAINER_OBJ_COORD_Z
}

//
new ContainerData[MAX_CONTAINERS][E_CONTAINER_DATA];
new bool:ContainersBiddingStatus = false;
new playerOpenedMenuContainerIndex[MAX_PLAYERS];
new pendingPrizeFetches;

//
new Iterator:CreatedContainerData<MAX_CONTAINERS>;
new Iterator:ContainerBiddersIter[MAX_CONTAINERS]<MAX_PLAYERS>;

//
new Text:ContainerMenuTD[8];
new Text:ContainerMenuNickNamesTD[MAX_CONTAINERS][MAX_VISIBLE_CONTAINER_PARTICIPANTS];
new Text:ContainerWonTD[4];