#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// used in interface
#define FIRST_CARD      0
#define SECOND_CARD     1
#define TABLE_CARDS     5
#define BACK            (-1)
#define PLAYER          0
#define START_BALANCE   10000
#define DRAW            (-1)
#define OUT             0
#define FOLD            1
#define CHECK           2
#define CALL            3
#define BET             4
#define ALL_IN          5

// used in macro or constants
#define SUITS           4
#define COMBINATIONS    11
#define ACE             12
#define KING            11
#define QUEEN           10
#define JACK            9
#define TEN             8

// used in realization
#define DECK_SIZE       52
#define VALUES          13
#define PLAYER_CARDS    2
#define PLAYED          (-1)
#define NOT_COMBINATION 0
#define HIGH_CARD       1
#define PAIR            2
#define TWO_PAIRS       3
#define THREE_OF_A_KIND 4
#define STRAIGHT        5
#define FLUSH           6
#define FULL_HOUSE      7
#define FOUR_OF_A_KIND  8
#define STRAIGHT_FLUSH  9
#define ROYAL_FLUSH     10
#define SF_CMB_LEN      4
#define TRUE            1
#define FALSE           0
#define BET_MULTIPLIER  0.05

// used in main
#define PLAYERS         5
#define FLOP_CARDS      3
#define TURN_CARDS      4
#define RIVER_CARDS     5
#define TURN_CARD       3
#define RIVER_CARD      4
#define PREFLOP_CARDS   0

#define IS_PAIR(crd1, crd2) ((crd1) == (crd2))
#define IS_THREE_OF_A_KIND(crd1, crd2, crd3) (IS_PAIR(crd1, crd2) && IS_PAIR(crd1, crd3))
#define IS_STRAIGHT(crd1, crd2) ((crd1 + 1) == (crd2))
#define OUT_OF_BOUNDS(crd1, crd2) ((crd1) >= (crd2))
#define IS_FOUR_OF_A_KIND(crd1, crd2, crd3, crd4) (IS_THREE_OF_A_KIND(crd1, crd2, crd3) && IS_PAIR(crd3, crd4))
#define IS_ACE(crd) ((crd) == ACE)
#define IS_KING(crd) ((crd) == KING)
#define IS_QUEEN(crd) ((crd) == QUEEN)
#define IS_JACK(crd) ((crd) == JACK)
#define IS_TEN(crd) ((crd) == TEN)
#define IS_STRAIGHT_FLUSH(crd1, crd2) ((crd1) == (crd2) && crd1)
#define IS_ROYAL_FLUSH(a, k, q, j, ten) (IS_ACE(a) && IS_KING(k) && IS_QUEEN(q) && IS_JACK(j) && IS_TEN(ten))
#define STRAIGHT_CONTINUES(res, size, crd1, crd2) (((res) + 1) < (size) && ((crd1) == (crd2) || ((crd1) + 1) == (crd2)))
#define FLUSH_CONTINUES(res, size, crd1, crd2) (((res) + 1) < (size) && (crd1) == (crd2))
#define IS_BET(first, second) (((first) == (second)) || ((first) >= TEN && (second) >= TEN))
#define IS_CALL(v1, v2, s1, s2, balance, bet) (((v1 > TEN || v2 > TEN) || (s1 == s2)) && (balance / (bet + 1) > 2))

typedef struct Card {
    char suit, value;
}Card;

typedef struct Player {
    Card layout[PLAYER_CARDS];
    char bestCardValue, isPlaying, isAllIn;
    int balance, bet;
}Player;

typedef struct Table {
    Card layout[TABLE_CARDS];
    int atStake, call;
}Table;

const char *values[VALUES] =             {"two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack",
                                          "queen", "king", "ace"};
const char *suits[SUITS] =               {"clubs", "diamonds", "hearts", "spades"};
const char *combinations[COMBINATIONS] = {"ERROR", "HIGH CARD", "PAIR", "TWO PAIRS", "THREE OF A KIND", "STRAIGHT",
                                          "FLUSH", "FULL HOUSE", "FOUR OF A KIND", "STRAIGHT FLUSH", "ROYAL FLUSH"};


////will be replaced with array of pictures
//                       // 2       3       4       5       6       7       8       9       10     jack    queen    king      ace
//Card deck[DECK_SIZE] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12},  //clubs
//                        {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12},  //diamonds
//                        {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10}, {2, 11}, {2, 12},  //hearts
//                        {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}}; // spades

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////REALIZATION//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// realization
// updated
int getCard(Card deck[]) {
    int crd;

    do {
        crd = (rand() % DECK_SIZE);
    } while (deck[crd].suit == PLAYED);
    return crd;
}

// auxiliary
char cmp(char a, char b) {
    if (a > b)
        return a;
    return b;
}

// realization
// updated
void dealCard(Card *crd, Card deck[], int ind) {
    int random = getCard(deck);

    crd[ind] = deck[random];
    deck[random].suit = PLAYED;
    deck[random].value = PLAYED;
}

// interface
void printInfo(Card crd[], int size) {
    for (int i = 0; i < size; i++) {
        if (crd[i].suit != BACK) {
            printf("%s of %s\n", values[crd[i].value], suits[crd[i].suit]);
        } else {
            printf("Card is not redeemed\n");
        }
    }
}

// auxiliary
int findMaxIndex(const int a[], int size) {
    int max = 0;

    for (int i = 1; i < size; i++) {
        if (a[i] > a[max]) {
            max = i;
        }
    }
    return max;
}

// realization
char cmbPair(Card cards[], int *left, int size) {
    while (*left < size - 1) {
        if (IS_PAIR(cards[*left].value, cards[++(*left)].value)) {
            return (char)(cards[(*left)++].value + 1);
        }
    }
    return NOT_COMBINATION;
}

// realization
char cmbTwoPairs(Card cards[], int size) {
    int l = 0;
    char pairOne = cmbPair(cards, &l, size), pairTwo = cmbPair(cards, &l, size);

    if (pairOne && pairTwo) {
        return cmp(pairOne, pairTwo);
    }
    return NOT_COMBINATION;
}

// realization
char cmbThreeOfAKind(Card cards[], int *left, int size) {
    while (*left < size - 2) {
        if (IS_THREE_OF_A_KIND(cards[*left].value, cards[++(*left)].value, cards[++(*left)].value)) {
            return (char) (cards[(*left)++].value + 1);
        }
    }
    return NOT_COMBINATION;
}

// realization
char cmbStraight(Card cards[], int size) {
    int res, comb_size = SF_CMB_LEN;

    for (int i = 0; i < size - 4; i++) {
        res = i + SF_CMB_LEN;
        for (int j = 0; j < comb_size; j++) {
            if (OUT_OF_BOUNDS(i + j + 1, size)) {
                return NOT_COMBINATION;
            }
            if (!IS_STRAIGHT(cards[i + j].value, cards[i + j + 1].value)) {
                if (IS_PAIR(cards[i + j].value, cards[i + j + 1].value)) {
                    comb_size++;
                } else {
                    res = FALSE;
                }
            }
        }
        if (res) {
            while (STRAIGHT_CONTINUES(res, size, cards[res].value, cards[res + 1].value))
                res++;
            return (char) (cards[res].value + 1);
        }
    }
    return NOT_COMBINATION;
}

// realization
char cmbFlush(Card cards[], int *left, int size) {
    int res;

    for (*left = 0; *left < size - 4; (*left)++) {
        res = *left + SF_CMB_LEN;
        for (int j = 0; j < SF_CMB_LEN; j++) {
            if (cards[*left + j].suit != cards[*left + j + 1].suit) {
                res = FALSE;
                break;
            }
        }
        if (res) {
            while (FLUSH_CONTINUES(res, size, cards[res].suit, cards[res + 1].suit))
                res++;
            return (char) (cards[res].value + 1);
        }
    }
    return NOT_COMBINATION;
}

// realization
char cmbFullHouse(Card cards[], int size) {
    int l0 = 0, l1 = 0;
    char pairFirst, threeOfAKindFirst, pairSecond, threeOfAKindSecond;

    pairFirst = cmbPair(cards, &l0, size);
    threeOfAKindFirst = cmbThreeOfAKind(cards, &l0, size);
    threeOfAKindSecond = cmbThreeOfAKind(cards, &l1, size);
    pairSecond = cmbPair(cards, &l1, size);

    if (pairFirst && threeOfAKindFirst) {
        return (char) (cards[l0 - 1].value + 1);
    }
    if (pairSecond && threeOfAKindSecond) {
        return (char) (cards[l1 - 1].value + 1);
    }
    return NOT_COMBINATION;
}

// realization
char cmbFourOfAKind(Card cards[], int size) {
    for (int i = 0; i < size - 3; i++) {
        if (IS_FOUR_OF_A_KIND(cards[i].value, cards[i + 1].value, cards[i + 2].value, cards[i + 3].value)) {
            return (char) (cards[i + 3].value + 1);
        }
    }
    return NOT_COMBINATION;
}

// realization
char cmbStraightFlush(Card cards[], int size) {
    int l = 0;
    char flush, straight;

    flush = cmbFlush(cards, &l, size);
    straight = cmbStraight(cards, size);

    if (IS_STRAIGHT_FLUSH(straight, flush)) {
        return (char) (flush);
    }
    return NOT_COMBINATION;
}

// realization
char cmbRoyalFlush(Card cards[], int size) {
    if (cmbStraightFlush(cards, size)) {
        for (int i = size - 3; i < size; i++) {
            if (IS_ROYAL_FLUSH(cards[i].value, cards[i - 1].value, cards[i - 2].value,
                               cards[i - 3].value, cards[i - 4].value)) {
                return ACE + 1;
            }
        }
    }
    return NOT_COMBINATION;
}

// auxiliary
Card *sortValue(Card arr[], int first, int last) {
    if (first < last) {
        int left = first, right = last;
        char middle = arr[(left + right) / 2].value;

        do {
            while (arr[left].value < middle) {
                left++;
            }
            while (arr[right].value > middle) {
                right--;
            }
            if (left <= right) {
                Card tmp = arr[left];
                arr[left] = arr[right];
                arr[right] = tmp;
                left++;
                right--;
            }
        } while (left <= right);
        sortValue(arr, first, right);
        sortValue(arr, left, last);
    }
    return arr;
}

// auxiliary
void sortNum(int arr[], int first, int last) {
    if (first < last) {
        int left = first, right = last, middle = arr[(left + right) / 2];

        do {
            while (arr[left] < middle) {
                left++;
            }
            while (arr[right] > middle) {
                right--;
            }
            if (left <= right) {
                int tmp = arr[left];
                arr[left] = arr[right];
                arr[right] = tmp;
                left++;
                right--;
            }
        } while (left <= right);
        sortNum(arr, first, right);
        sortNum(arr, left, last);
    }
}

// auxiliary
int *cardToInt(Card crd[], int size) {
    int *res = (int *) malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        res[i] = crd[i].suit * VALUES + crd[i].value;
    }
    return res;
}

// auxiliary
Card *intToCard(const int crdCodes[], int size) {
    Card *res = (Card *) malloc(size * sizeof(Card));

    for (int i = 0; i < size; i++) {
        res[i].suit = (char) (crdCodes[i] / VALUES);
        res[i].value = (char) (crdCodes[i] % VALUES);
    }
    return res;
}

// auxiliary
Card *sortCards(Card crd[], int size) {
    int *crdCodes = cardToInt(crd, size);

    sortNum(crdCodes, 0, size - 1);
    crd = intToCard(crdCodes, size);
    return crd;
}

// realization
int defineComb(Card cards[], Player *player, int size) {
    int l = 0;
    char isCombination;

    cards = sortCards(cards, size);
    isCombination = cmbRoyalFlush(cards, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return ROYAL_FLUSH;
    }
    isCombination = cmbStraightFlush(cards, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return STRAIGHT_FLUSH;
    }
    cards = sortValue(cards, 0, size - 1);
    isCombination = cmbFourOfAKind(cards, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return FOUR_OF_A_KIND;
    }
    isCombination = cmbFullHouse(cards, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return FULL_HOUSE;
    }
    isCombination = cmbFlush(sortCards(cards, size), &l, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return FLUSH;
    }
    l = 0;
    cards = sortValue(cards, 0, size - 1);
    isCombination = cmbStraight(cards, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return STRAIGHT;
    }
    isCombination = cmbThreeOfAKind(cards, &l, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return THREE_OF_A_KIND;
    }
    l = 0;
    isCombination = cmbTwoPairs(cards, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return TWO_PAIRS;
    }
    isCombination = cmbPair(cards, &l, size);
    if (isCombination) {
        player->bestCardValue = isCombination;
        return PAIR;
    }
    isCombination = HIGH_CARD;
    player->bestCardValue = isCombination;
    return HIGH_CARD;
}

// realization
int findCombinations(Card tblCard[], Player *plr, int tblCardsSize) {
    int combination, size = tblCardsSize + PLAYER_CARDS;
    Card *crdAll = (Card *) malloc((size) * sizeof(Card));

    for (int i = 0; i < tblCardsSize; i++) {
        crdAll[i] = tblCard[i];
    }
    for (int i = 0; i < PLAYER_CARDS; i++) {
        crdAll[tblCardsSize + i] = plr->layout[i];
    }
    combination = defineComb(crdAll, plr, size);
    free(crdAll);
    return combination;
}

// interface
void layoutInfo(Player *player, Table tbl, int tblCards) {
    printf("\nCards:\n");
    printInfo(player->layout, PLAYER_CARDS);
    printf("\nCards on the table:\n");
    printInfo(tbl.layout, TABLE_CARDS);
    printf("\nCombination: %s\n", combinations[findCombinations(tbl.layout, player, tblCards)]);
}



// realization
int *fillCombinationCodes(Player plr[], Table tbl, int size) {
    int *cmb = (int *) malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        if (plr[i].isPlaying) {
            cmb[i] = findCombinations(tbl.layout, &(plr[i]), RIVER_CARDS);
        } else{
            // error maybe
            cmb[i] = -1;
        }
    }
    return cmb;
}

// realization
int checkEqualCombs(int arr[], int ind, int size) {
    int equal = FALSE;

    for (int i = 0; i < size; i++) {
        if (arr[i] == arr[ind] && i != ind) {
            equal = TRUE;
        } else if (arr[i] != arr[ind]) {
            arr[i] = FALSE;
        }
    }
    return equal;
}

// realization
void findHigherBestCard(int *arr, Player *plrs, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= plrs[i].bestCardValue;
    }
}

// realization
void findHigherCards(int *arr, Player *plrs, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= plrs[i].layout[FIRST_CARD].value * plrs[i].layout[SECOND_CARD].value;
    }
}

// realization
void splitBank(Player plrs[], Table tbl, int size) {
    for (int i = 0; i < size; i++) {
        if (plrs[i].isPlaying) {
            plrs[i].balance += tbl.atStake / 2;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////INTERFACE//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// interface
void initializePlayers(Player plrs[]) {
    for (char i = 0; i < PLAYERS; i++) {
        plrs[i].layout[FIRST_CARD].suit = BACK;
        plrs[i].layout[FIRST_CARD].value = BACK;
        plrs[i].layout[SECOND_CARD].suit = BACK;
        plrs[i].layout[SECOND_CARD].value = BACK;
        plrs[i].bet = 0;
        if (plrs[i].balance) {
            plrs[i].isPlaying = TRUE;
        } else {
            plrs[i].isPlaying = FALSE;
        }
        plrs[i].isAllIn = FALSE;
        plrs[i].bestCardValue = BACK;
    }
}

// interface
// updated
void cardsDealing(Player toDeal[], Card deck[], int size) {
    for (int i = 0; i < size; i++) {
        dealCard(toDeal[i].layout, deck,  FIRST_CARD);
        dealCard(toDeal[i].layout, deck, SECOND_CARD);
    }
}

// interface
void clearTable(Table *tbl) {
    for (int i = 0; i < TABLE_CARDS; i++) {
        tbl->layout[i].suit = BACK;
        tbl->layout[i].value = BACK;
    }
    tbl->atStake = 0;
    tbl->call = 0;
}

// interface
// updated
void flop(Table *tbl, Card deck[]) {
    for (int i = 0; i < FLOP_CARDS; i++) {
        dealCard(tbl->layout, deck, i);
    }
}

// interface
// updated
void turn(Table *tbl, Card deck[]) {
    dealCard(tbl->layout, deck, TURN_CARD);
}

// interface
// updated
void river(Table *tbl, Card deck[]) {
    dealCard(tbl->layout, deck, RIVER_CARD);
}

// interface
int findWinner(Player players[], Table tbl, int size) {
    int *result, max, isEqualCombinations = FALSE;

    result = fillCombinationCodes(players, tbl, size);
    max = findMaxIndex(result, size);
    isEqualCombinations = checkEqualCombs(result, max, size);
    if (isEqualCombinations) {
        findHigherBestCard(result, players, size);
    } else {
        return max;
    }

    max = findMaxIndex(result, size);
    isEqualCombinations = checkEqualCombs(result, max, size);
    if (isEqualCombinations) {
        findHigherCards(result, players, size);
    } else {
        return max;
    }

    max = findMaxIndex(result, size);
    isEqualCombinations = checkEqualCombs(result, max, size);
    if (isEqualCombinations) {
        return DRAW;
    }
    return max;
}

// interface
void printWinner(Player players[], Table tbl, int size) {
    int winnerInd = findWinner(players, tbl, size);

    if (winnerInd != DRAW) {
        printf("\nWINNER: player number %d\n", winnerInd + 1);
        printf("\nCombination: %s\n", combinations[findCombinations(tbl.layout, &(players[winnerInd]), 5)]);
        players[winnerInd].balance += tbl.atStake;
    } else {
        printf("\nDRAW\n");
        splitBank(players, tbl, size);
    }
}

// interface
int preflopStrategy(Player *plr, Table *tbl) {
    if (IS_BET(plr->layout[FIRST_CARD].value, plr->layout[SECOND_CARD].value)) {
        return BET;
    }
    if (IS_CALL(plr->layout[FIRST_CARD].value, plr->layout[SECOND_CARD].value, plr->layout[FIRST_CARD].suit,
                plr->layout[SECOND_CARD].suit, plr->balance, plr->bet)) {
        return CALL;
    }
    if (!(tbl->call - plr->bet)){
        return CHECK;
    }
    return FOLD;
}

// realization
int riverStrategy(Player *plr, Table *tbl, Card *cards){
    Player fakePlayer;
    int cmb = defineComb(cards, &fakePlayer, TABLE_CARDS + PLAYER_CARDS);

    if (cmb > PAIR){
        return BET;
    }
    if (cmb - 1){
        return CALL;
    }
    if (!(tbl->call - plr->bet)){
        return CHECK;
    }
    return FOLD;
}

// realization
// updated
int middleStrategy(Player *plr, Table *tbl, Card *cards, Card deck[], int tableCards){
    int current = 0, possible = 0;
    Player fakePlayer;

    // - 1 to make HIGH_CARD 0
    if (defineComb(cards, &fakePlayer, TABLE_CARDS + PLAYER_CARDS) - 1) {
        current++;
    }
    for (int k = 0; k < TABLE_CARDS - tableCards; k++) {
        for (int i = 0; i < DECK_SIZE; i++) {
            if (deck[i].suit != BACK) {
                cards[tableCards + PLAYER_CARDS + k] = deck[i];
                if (defineComb(cards, &fakePlayer, tableCards + PLAYER_CARDS + 1 + k) >= STRAIGHT)
                    possible++;
            }
        }
    }

    if (current && possible){
        return BET;
    }
    if (current || possible){
        return CALL;
    }
    if (!(tbl->call - plr->bet)){
        return CHECK;
    }
    return FOLD;
}

// realization
int chooseStrategy(Player *plr, Table *tbl, Card deck[], int tableCards) {
    int strategy;
    Card cards[TABLE_CARDS + PLAYER_CARDS];

    if (!plr->isPlaying) {
        return OUT;
    }

    if (plr->isAllIn) {
        return ALL_IN;
    }

    if (tableCards == PREFLOP_CARDS) {
        return preflopStrategy(plr, tbl);
    }

    for (int i = 0; i < tableCards; i++) {
        cards[i] = tbl->layout[i];
    }
    for (int i = 0; i < PLAYER_CARDS; i++) {
        cards[tableCards + i] = plr->layout[i];
    }

    if (tableCards == TABLE_CARDS) {
        strategy = riverStrategy(plr, tbl, cards);
        return strategy;
    }

    strategy = middleStrategy(plr, tbl, cards, deck, tableCards);
    return strategy;
}

//interface
// changed
int callAfterBets(Player *plr, Table *tbl, Card deck[], int cardsNum) {
    int strategy = chooseStrategy(plr, tbl, deck, cardsNum);

    if (strategy == BET) {
        strategy = CALL;
    }

    return strategy;
}

// realization
int bet(Player *plr, Table *tbl) {
    int bet = (int) ((rand() % plr->balance) * BET_MULTIPLIER) + tbl->call - plr->bet;
    tbl->call = bet + plr->bet;
    plr->bet += bet;
    plr->balance -= bet;
    tbl->atStake += bet;

    return bet;
}

// realization
void fold(Player *plr) {
    plr->isPlaying = FALSE;
}

// realization
void call(Player *plr, Table *tbl) {
    int bet = tbl->call - plr->bet;

    plr->balance -= bet;
    tbl->atStake += bet;
    plr->bet += bet;
}

// realization
void allIn(Player *plr, Table *tbl) {
    plr->isAllIn = TRUE;
    plr->bet += plr->balance;
    plr->balance = 0;
    if (tbl->call < plr->bet){
        tbl->call = plr->bet;
    }
}

// interface
// updated
void computersTurn(Player plrs[], Table *tbl, Card deck[], int plrNum, int cardsNum, int (*strat)(Player*, Table*, Card*, int)) {
    int strategy, tmp;

    for (int i = 1; i < plrNum; i++) {
        strategy = strat(&(plrs[i]), tbl, deck, cardsNum);
        switch (strategy) {
            case OUT:
                printf("OUT STRATEGY Player %d is out\n", i + 1);
                break;
            case FOLD:
                fold(&(plrs[i]));
                printf("FOLD STRATEGY Player %d folds\n", i + 1);
                break;
            case CALL:
                if (plrs[i].balance + plrs[i].bet < tbl->call) {
                    fold(&(plrs[i]));
                    printf("CALL STRATEGY Player %d folds\n", i + 1);
                }

                tmp = tbl->call - plrs[i].bet;
                if (tmp) {
                    call(&(plrs[i]), tbl);
                    printf("CALL STRATEGY Player %d calls %d\n", i + 1, tmp);
                } else {
                    printf("CALL STRATEGY Player %d checks\n", i + 1);
                }
                break;
            case CHECK:
                if (tbl->call - plrs[i].bet) {
                    fold(&(plrs[i]));
                    printf("CHECK STRATEGY Player %d folds\n", i + 1);
                } else {
                    printf("CHECK STRATEGY Player %d checks\n", i + 1);
                }
                break;
            case BET:
                if (plrs[i].balance + plrs[i].bet <= tbl->call) {
                    allIn(&(plrs[i]), tbl);
                    printf("BET STRATEGY Player %d is all in\n", i + 1);
                } else if (plrs[i].bet + plrs[i].balance != tbl->call) {
                    tmp = bet(&(plrs[i]), tbl);
                    printf("BET STRATEGY Player %d bets %d\n", i + 1, tmp);
                } else {
                    printf("BET STRATEGY Player %d checks\n", i + 1);
                }

                break;
            case ALL_IN:
                printf("ALL IN STRATEGY Player %d is all in\n", i + 1);
                break;
            default:
                break;
        }
    }
}

// interface
int makeBet(Player *plr, Table *tbl, int bet) {
    int res = FALSE;

    if (bet > plr->balance) {
        printf("\nNot enough balance for this stake\n");
    } else if (bet < tbl->call - plr->bet) {
        if (bet == plr->balance) {
            if (plr->balance > tbl->call) {
                tbl->call = plr->bet + plr->balance;
            }
            plr->bet += plr->balance;
            tbl->atStake += plr->balance;
            plr->balance = 0;
            res = TRUE;
            plr->isAllIn = TRUE;
            printf("All in\n");
        } else {
            printf("Bet is too low to call\n");
        }
    } else {
        if (bet + plr->bet > tbl->call) {
            tbl->call = bet + plr->bet;
        }
        plr->balance -= bet;
        plr->bet += bet;
        tbl->atStake += bet;
        res = TRUE;
    }
    return res;
}

// interface
void makePlayerBet(Player *player, Table *tbl) {
    int stake, sign;

    do {
        printf("Bank: %d\n", tbl->atStake);
        printf("Your balance: %d\n", player->balance);
        printf("Your bet: %d\n", player->bet);
        printf("To continue playing bet at least %d\n", tbl->call - player->bet);
        printf("Your stake:");
        scanf_s("%d", &stake);
        if (player->isAllIn) {
            sign = TRUE;
        } else {
            sign = makeBet(player, tbl, stake);
        }
    } while (!sign);
}

// interface
void playersTurn(Player *player, Table *tbl, int tblCrdNum) {
    if (player->balance && player->isPlaying) {
        layoutInfo(player, *tbl, tblCrdNum);
        makePlayerBet(player, tbl);
    }
}

void initBalance(Player plrs[]){
    for (int i = 0; i < PLAYERS; i++){
        plrs[i].balance = START_BALANCE;
    }
}

int main() {
    Player players[PLAYERS];
    Table table;
    Card deck[DECK_SIZE];
    int sign = TRUE;

    srand(time(NULL) / 2);

    initBalance(players);

    while (players[PLAYER].balance && sign) {

        for (int i = 0; i < DECK_SIZE; i++) {
            deck[i].value =(char) (i % VALUES);
            deck[i].suit = (char) (i / VALUES);
        }

        for (int i = 0; i < DECK_SIZE; i++){
            printf("%d %d\n", deck[i].value, deck[i].suit);
        }
        // Initialization
        clearTable(&table);
        initializePlayers(players);

        //Preflop
        cardsDealing(players, deck, PLAYERS);



        players[1].layout[0].suit = 1;
        players[1].layout[0].value = 3;
        players[1].layout[1].suit = 3;
        players[1].layout[1].value = 11;

        players[2].layout[0].suit = 1;
        players[2].layout[0].value = 10;
        players[2].layout[1].suit = 0;
        players[2].layout[1].value = 6;

        players[3].layout[0].suit = 0;
        players[3].layout[0].value = 1;
        players[3].layout[1].suit = 3;
        players[3].layout[1].value = 4;

        players[4].layout[0].suit = 2;
        players[4].layout[0].value = 10;
        players[4].layout[1].suit = 3;
        players[4].layout[1].value = 12;

        printf("\nPREFLOP\n");
        computersTurn(players, &table, deck, PLAYERS, PREFLOP_CARDS, chooseStrategy);
        playersTurn(&players[PLAYER], &table, PREFLOP_CARDS);
        computersTurn(players, &table, deck, PLAYERS, PREFLOP_CARDS, callAfterBets);

        //Flop
        flop(&table, deck);
        table.layout[0].suit = 2;
        table.layout[0].value = 11;
        table.layout[1].suit = 3;
        table.layout[1].value = 1;
        table.layout[2].suit = 3;
        table.layout[2].value = 0;

        printf("\nFLOP\n");
        computersTurn(players, &table, deck, PLAYERS, FLOP_CARDS, chooseStrategy);
        playersTurn(&players[PLAYER], &table, FLOP_CARDS);
        computersTurn(players, &table, deck, PLAYERS, FLOP_CARDS, callAfterBets);

        //Turn
        turn(&table, deck);
        table.layout[3].suit = 2;
        table.layout[3].value = 8;
        printf("\nTURN\n");
        computersTurn(players, &table, deck, PLAYERS, TURN_CARDS, chooseStrategy);
        playersTurn(&players[PLAYER], &table, TURN_CARDS);
        computersTurn(players, &table, deck, PLAYERS, TURN_CARDS, callAfterBets);

        //River
        river(&table, deck);
        table.layout[4].suit =  3;
        table.layout[4].value = 2;
        printf("\nRIVER\n");
        computersTurn(players, &table, deck, PLAYERS, RIVER_CARDS, chooseStrategy);
        playersTurn(&players[PLAYER], &table, RIVER_CARDS);
        computersTurn(players, &table, deck, PLAYERS, RIVER_CARDS, callAfterBets);

        //Result
        for (int i = 0; i < PLAYERS; i++) {
            printf("\nPlayer %d\n", i + 1);
            if (!players[i].isPlaying) {
                printf("Is out\n");
            }
            printInfo(players[i].layout, PLAYER_CARDS);
        }
        printf("\nTable:\n");
        printInfo(table.layout, TABLE_CARDS);

        printWinner(players, table, PLAYERS);
        printf("\nPress 0 to quit\n");
        scanf_s("%d", &sign);
    }
    return 0;
}